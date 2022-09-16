---
title: "Apollo Client の fetchMore を自動で呼び出して GitHub GraphQL の100件制限を乗り越える (useAutoFetchMore)"
url: "p/w7igunc/"
date: "2021-09-16"
tags: ["Apollo", "GitHub", "GraphQL", "React"]
---

何をするか？
----

GitHub の GraphQL API で Issue 情報などを取得しようとすると、[リソース制限](https://docs.github.com/ja/graphql/overview/resource-limitations) のため一度に 100 件までの情報しか取得できません。
Apollo Client が提供する `useQuery` や `useLazyQuery` などの React フック関数を使用すると、戻り値で返される `fetchMore` 関数を使って追加読み込み（ページネーション処理）を行うことができますが、この関数の使用例として提示されているものは、ユーザーによるボタンクリックなどを必要とするものばかりです。
ここでは、`useQuery` 実行後に自動で `fetchMore` を繰り返し呼び出して、100 件を超える情報を取得する方法の例を示します。

fetchMore のための設定
----

前提として、Apollo Client の `fetchMore` 関数の基本的な使い方は理解しているものとします（下記記事などを参考にしてください）。

- [Apollo Client の Pagenation 機能を使って GraphQL API を呼び出す](/p/cu6eox7)

今回サンプルコードで使う GraphQL クエリには、次のような `search` コネクションが含まれていることを想定しています。
ページネーションの対象となるのは、この `search` コネクション部分です。

{{< code lang="graphql" >}}
query QueryIssues($cursor: String) {
  search(type: ISSUE, first: 100, after: $cursor, query: "...") {
    ...
  }
}
{{< /code >}}

そのため、`ApolloClient` に設定するキャッシュのフィールドポリシーとして、`search` フィールドの値が `fetchMore` 時にマージされるように設定しておきます。
`cache` オブジェクトの生成時に呼び出している `relayStylePagination` 関数あたりがポイントです。

{{< code lang="tsx" title="GitHubApolloProvider.tsx" >}}
import * as React from 'react'
import {
  ApolloClient,
  ApolloLink,
  ApolloProvider,
  createHttpLink,
  InMemoryCache,
} from '@apollo/client'
import { relayStylePagination } from '@apollo/client/utilities'
import { setContext } from '@apollo/client/link/context'
import { Auth } from '@/utils/auth'

const httpLink = createHttpLink({
  uri: 'https://api.github.com/graphql',
})

const authLink = setContext((_, { headers }) => {
  // Get the authentication token from local storage if it exists
  const token = Auth.getToken()

  // Return the headers to the context so httpLink can read them
  return {
    headers: {
      ...headers,
      authorization: token ? `Bearer ${token}` : '',
    } as ApolloLink,
  }
})

// GraphQL cache with field policies
const cache = new InMemoryCache({
  typePolicies: {
    Query: {
      fields: {
        search: relayStylePagination(['type', 'query']), // ★
      },
    },
  },
})

// Create a GraphQL client
const apolloClient = new ApolloClient({
  link: authLink.concat(httpLink),
  cache,
})

export const GitHubApolloProvider: React.FC = (prop) => {
  return <ApolloProvider client={apolloClient}>{prop.children}</ApolloProvider>
}
{{< /code >}}

あとは、上記ファイルで export されている `GitHubApolloProvider` コンポーネントをトップレベルのコンポーネントとして配置すれば、それ以下の階層で `useQuery` フックが適切に動作するようになります。
例えば、Next.js を使っている場合なら、カスタム App コンポーネントで次のような感じで配置すればよいでしょう。

{{< code lang="tsx" title="pages/_app.tsx" >}}
function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  return (
    <>
      <Head>
        <title>My App</title>
        <meta name="description" content="すんごいアプリ" />
      </Head>
      <GitHubApolloProvider>
        <Component {...pageProps}></Component>
      </GitHubApolloProvider>
    </>
  )
}

export default MyApp
{{< /code >}}


fetchMore を自動呼出しするためのフック関数 (useAutoFetchMore) を作る
----

`fetchMore` を自動で繰り返し呼び出す仕組みですが、ここでは `useEffect` を使って、`pageInfo.hasNextPage` の値などが変化したときに `fetchMore` を呼び出すようにしてみます。
この実装をコンポーネントのコードに入れてもいいのですが、ある程度汎用的に使える処理なので、__`useAutoFetchMore`__ フック関数として定義します。

{{< code lang="ts" title="src/hooks/useAutoFetchMore.ts" >}}
import { useEffect, useRef } from 'react'
import { ApolloError } from '@apollo/client'

/**
 * ApolloClient (useQuery) の fetchMore を自動で繰り返し呼び出すためのフック関数です。
 *
 * 各パラメーターには、useQuery() の戻り値をそのまま設定します。
 * ページネーションは Relay スタイル（pageInfo や edges）で提供されていることを前提とし、
 * fetchMore の呼び出しごとにクエリ変数 (variables) の cursor の値が更新されていきます。
 * 手違いによる大量呼び出しを防ぐため、最大呼び出し回数 (maxAutoFetch) を設定できます。
 */
export function useAutoFetchMore(
  loading: boolean,
  error: ApolloError | undefined,
  pageInfo: { hasNextPage: boolean; endCursor: string | null } | undefined,
  fetchMore: any, // eslint-disable-line
  maxAutoFetch = 2
): void {
  const hasNextPage = pageInfo?.hasNextPage ?? false
  const endCursor = pageInfo?.endCursor

  // fetchMore を自動で呼び出した回数を保持しておく（呼び出しすぎ防止）
  const count = useRef(0)

  // しかるべきタイミングで fetchMore を自動呼出しする
  useEffect(() => {
    if (error || loading || count.current >= maxAutoFetch) return
    if (hasNextPage) {
      count.current += 1
      // eslint-disable-next-line @typescript-eslint/no-unsafe-call
      fetchMore({ variables: { cursor: endCursor } })
    }
  }, [loading, error, hasNextPage, endCursor, fetchMore, maxAutoFetch])
}
{{< /code >}}


useAutoFetchMore フックの使用例
----

次のサンプルコンポーネントは、上記で作成した `useAutoFetchMore` フックを使用して、GitHub の `apollographql/apollo-client` リポジトリの Issue 情報を連続取得します。
一度に取得する件数は `first: 100` のように最大 100 件に設定することができますが、ここでは連続して取得していることが分かるように `first: 5` として 5 件ずつ取得するようにしています。

{{< code lang="tsx" title="src/pages/sample.tsx" >}}
import { gql, useQuery } from '@apollo/client'
import { FC } from 'react'
import { useAutoFetchMore } from '@/hooks/useAutoFetchMore'

const QUERY_ISSUES = gql`
  query QueryIssues($cursor: String) {
    search(
      type: ISSUE
      query: "repo:apollographql/apollo-client is:issue is:open"
      first: 5
      after: $cursor
    ) {
      edges {
        node { ... on Issue { id number title } }
      }
      pageInfo { endCursor hasNextPage }
    }
  }
`

// 本来は useQuery の型パラメーターをちゃんと指定して ESLint 警告を取り除くべき
/* eslint-disable */
const SamplePage: FC = () => {
  const { loading, error, data, fetchMore } = useQuery(QUERY_ISSUES)
  useAutoFetchMore(loading, error, data?.search.pageInfo, fetchMore)

  if (error) return <p>{error.message}</p>
  if (loading) return <p>Loading ...</p>

  const { search } = data
  return (
    <ol>
      {search.edges.map(({ node: issue }) => (
        <li key={issue.id}>
          <b>#{issue.number}</b> {issue.title}
        </li>
      ))}
    </ol>
  )
}
/* eslint-enable */

export default SamplePage
{{< /code >}}

ポイントは、下記のフック呼び出し部分です。
`useQuery` フックに続けて `useAutoFetchMore` フックを呼び出しておくことで、内部で `fetchMore` が自動的に繰り返し呼ばれるようになります。

{{< code lang="ts" >}}
const { loading, error, data, fetchMore } = useQuery(QUERY_ISSUES)
useAutoFetchMore(loading, error, data?.search.pageInfo, fetchMore)
{{< /code >}}

内部で `fetchMore` が呼び出されるごとにコンポーネントの再描画が行われるため、段階的に表示量が増えていく振る舞いになります。


応用（useLazyQuery)
----

Apollo Client において、任意のタイミング（ボタンクリックなど）で GraphQL クエリを発行したいときは、`useQuery` の代わりに __`useLazyQuery`__ フックを使用します。

- 参考: [Apollo Client でクリック時に GraphQL クエリを実行する](/p/m7ju6gr)

今回実装した `useAutoFetchMore` フックは、そのまま `useLazyQuery` にも適用することができます（クエリが実行されるまでは、内部的に `hasNextPage == false` と同じ振る舞いになるようにしているので）。
次のサンプルコードでは、ユーザーが `Get Issues` ボタンをクリックしたときに `fetchMore` の自動実行を開始するようにしています。

{{< code hl_lines="3 4" lang="tsx" title="src/pages/test.tsx（抜粋）" >}}
/* eslint-disable */
const TestPage: FC = () => {
  const [doQuery, { called, loading, error, data, fetchMore }] =
    useLazyQuery(QUERY_ISSUES)
  useAutoFetchMore(loading, error, data?.search.pageInfo, fetchMore)

  if (error) return <p>{error.message}</p>
  if (loading) return <p>Loading ...</p>

  const searchEdges = called ? data.search.edges : []
  return (
    <>
      <button onClick={() => doQuery()}>Get Issues</button>
      <ol>
        {searchEdges.map(({ node: issue }) => (
          <li key={issue.id}>
            <b>#{issue.number}</b> {issue.title}
          </li>
        ))}
      </ol>
    </>
  )
}
/* eslint-enable */
{{< /code >}}

