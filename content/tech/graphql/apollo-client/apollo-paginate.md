---
title: "Apollo Client の Pagenation 機能を使って GraphQL API を呼び出す"
url: "p/cu6eox7/"
date: "2020-10-02"
lastmod: "2021-09-15"
tags: ["Apollo", "GitHub", "GraphQL", "React"]
---

Apollo Client の Pagination 機能
----

GraphQL API では柔軟なクエリ発行が可能ですが、多数の要素を取得する場合は、[Pagenation 処理](https://graphql.org/learn/pagination/) により何度かに分けて API 呼び出しを行う必要があります。
例えば、GitHub の GraphQL API では一度のクエリで取得可能な要素数は 100 件までであり、それを超える情報を取得する場合に Pagination 処理が必要です。

Apollo Client には、GraphQL の Pagination 処理を簡単に扱うための仕組み（__`fetchMore`__ 関数）が用意されています。

- 参考: [Pagination - Client (React) - Apollo GraphQL Docs](https://www.apollographql.com/docs/react/data/pagination/)

と言っても、そこまで簡単ではないので、ここでは GitHub の GraphQL API における Pagination 処理の具体的な実装例を紹介します。


Pagination の実装例（フィールドポリシーを使う方法）
----

次のサンプルコードは、GitHub の `myorg/myrepo` リポジトリの Issue リストを表示する `IssueList` コンポーネントの実装例です。
Issue の数が 100 件を超える場合は、「さらに読み込む」ボタンを表示し、このボタンが押されたときに Pagination 処理（`fetchMore` 関数）で次のデータを取得するようにしています。

Apollo クライアントの `useQuery` 関数が返す `fetchMore` 関数を呼び出すと、再度 GraphQL クエリを実行することができます。
このとき、オプションで `variables` パラメータの値（クエリ変数）を変更できるので、Issue の読み出し開始位置を示す `after` の値を進めていくことで、100 件を超えるデータを順番に読み出すことができます。

{{< code lang="tsx" title="IssueList.tsx" >}}
import { gql, useQuery } from '@apollo/client'
import { FC } from 'react'

// GraphQL クエリ
const QUERY_ISSUES = gql`
  query QueryIssues($cursor: String) {
    search(first: 100, after: $cursor, type: ISSUE,
        query: "repo:myorg/myrepo is:issue is:open") {
      edges {
        node {
          ... on Issue { id number title }
        }
      }
      pageInfo { hasNextPage endCursor }
    }
  }
`

// "さらに読み込む" ボタン
function createFetchMoreButton(pageInfo, fetchMore): JSX.Element | null {
  if (!pageInfo.hasNextPage) {
    return null
  }

  return (
    <button onClick={() => {
      fetchMore({
        variables: { cursor: pageInfo.endCursor }
      })
    }}>さらに読み込む</button>
  )
}

export const IssueList: FC = () => {
  const { loading, error, data, fetchMore } = useQuery(QUERY_ISSUES)
  if (error) return <p>{error.message}</p>
  if (loading) return <p>Loading ...</p>
  const { search } = data
  const { pageInfo } = search

  return <>
    <ul>
      {search.edges.map(({ node }) => (
        <li key={node.id}>
          {node.number}: {node.title}
        </li>
      ))}
    </ul>
    {createFetchMoreButton(pageInfo, fetchMore)}
  </>
}
{{< /code >}}

実はこれだけでは、不十分で、`ApolloClient` インスタンスを生成するときにキャッシュの __フィールドポリシー__ というものを設定しておく必要があります。
`ApolloClient`（の `InMemoryCache`）は、デフォルトの動作として、クエリ結果を別々のオブジェクトとしてキャッシュしようとするので、`fetchMore` で複数回に分けて取得したデータをマージして返して欲しい場合は、`InMemoryCache` オブジェクトを次のようにカスタマイズしなければいけません。

{{< code lang="tsx" title="GitHubApolloProvider.tsx" >}}
import { ApolloClient, ApolloProvider, InMemoryCache } from '@apollo/client'
import { relayStylePagination } from '@apollo/client/utilities'

// ...

const cache = new InMemoryCache({
  typePolicies: {
    Query: {
      fields: {
        search: relayStylePagination(['type', 'query']),
      },
    },
  },
})

// Creates a GraphQL client
const apolloClient = new ApolloClient({
  link: authLink.concat(httpLink),
  cache
})

export const GitHubApolloProvider: React.FC = (prop) => {
  return <ApolloProvider client={apolloClient}>{prop.children}</ApolloProvider>
}
{{< /code >}}

上記の例では、クエリ結果の `search` フィールドをページネーションによって繰り返し取得した場合に、結果のリストをマージして返すように指定しています。
__`relayStylePagination`__ 関数は Apollo が提供しているユーティリティ関数で、Relay-style connections によるページネーション処理を行う場合に使用することができます。
Relay-style というのは、上記の GitHub GraphQL レスポンスのように、`pageInfo` や `edges` というフィールドがある場合に採用されているページネーションスタイルだと考えればよいです。

- 参考: [GraphQL Cursor Connections Specification](https://relay.dev/graphql/connections.htm)

`relayStylePagination` 関数には `keyArgs` と呼ばれる引数を渡すことができ、クエリ時にどのパラメーターが異なっていたら別のキャッシュとして管理するかを指定します。
[search クエリ (Connection)](https://docs.github.com/en/graphql/reference/queries#searchresultitemconnection) は、`type` 引数あるいは `query` 引数に違う値が指定された場合は、全然違う情報を取得することになるので結果をマージされては困ります。
そこで、上記のように `['type', 'query']` と指定するわけです。

{{< note title="edges と nodes" >}}
Relay スタイル (Cursor Connections) では、ページネーション用に `pagesInfo` や `edges` などのオブジェクトを使用します。
一方で、GitHub の GraphQL スキーマでは、`edges` 以下の各 `node` にアクセスするためのショートカットとして `nodes` が定義されています。
多くのケースではこの `nodes` オブジェクトを参照することでクライアントコードが簡潔になるのですが、Apollo client が提供する `relayStylePagination()` 関数を使用する場合は、`edges` を参照するような GraphQL クエリを発行しないとうまく動作しないようですので注意してください。
{{< /note >}}

ページネーションをどのように実装しているかは、GraphQL のサーバーによって異なるので、それに応じてフィールドポリシーの設定方法も変える必要があります。
フィールドポリシーの詳しい設定方法は、下記の Apollo Client ドキュメントを参照してください。

- 参考: [Core pagination API - Client (React) - Apollo GraphQL Docs](https://www.apollographql.com/docs/react/pagination/core-api/)


Pagination の実装例（fetchMore の updateQuery オプションを使う方法）
----

__下記の updateQuery を使ったページネーション処理は、2021-09-15 時点で deprecated になっています。前述のキャッシュの「フィールドポリシー」を使った方法を使ってください。__

このコードを実行すると、次のように警告が出ます。

> The updateQuery callback for fetchMore is deprecated, and will be removed in the next major version of Apollo Client.

このあたりの変更は地味につらい。。。

{{< code lang="jsx" title="IssueList.tsx" >}}
import { FC } from 'react'
import { gql, useQuery } from '@apollo/client'

// GraphQL クエリ
const QUERY_ISSUES = gql`
  query QueryIssues($cursor: String) {
    search(first: 100, after: $cursor, type: ISSUE,
        query: "repo:myorg/myrepo is:issue is:open") {
      edges {
        node {
          ... on Issue { id number title }
        }
      }
      pageInfo { hasNextPage endCursor }
    }
  }
`

// "さらに読み込む" ボタン
function createFetchMoreButton(pageInfo, fetchMore) {
  if (!pageInfo.hasNextPage) {
    return null
  }

  return (
    <button onClick={() => {
      fetchMore({
        variables: {cursor: pageInfo.endCursor},
        updateQuery: (prevResult, {fetchMoreResult}) => {
          if (!fetchMoreResult) return prevResult
          return fetchMoreResult
        }
      })
    }}>さらに読み込む</button>
  )
}

export const IssueList: FC = () => {
  const {loading, error, data, fetchMore} = useQuery(QUERY_ISSUES)
  if (error) return <p>{error.message}</p>
  if (loading) return <p>Loading ...</p>
  const {search} = data
  const {pageInfo} = search

  return <>
    <ul>
      {search.edges.map(({ node }) => (
        <li key={node.id}>{node.number}: {node.title}</li>
      ))}
    </ul>
    {createFetchMoreButton(pageInfo, fetchMore)}
  </>
}
{{< /code >}}

ポイントは、Apollo Client の `useQuery` フックが返す __`fetchMore`__ 関数の使い方です。
上記の例では、「さらに読み込む」ボタンが押された時に、この `fetchMore` 関数を呼び出しています。

{{< code lang="typescript" >}}
fetchMore({
  variables: {cursor: pageInfo.endCursor},
  updateQuery: (prevResult, {fetchMoreResult}) => {
    if (!fetchMoreResult) return prevResult;
    return fetchMoreResult;
  }
});
{{< /code >}}

`fetchMore` 関数を呼び出すと、`variables` パラメータで指定した変数値を使って再度 GraphQL クエリが実行され、`updateQuery` パラメータで指定した関数が返す値で再描画が行われます。
`fetchMoreResult` には、新しいクエリでサーバーから返された値が格納されているため、これをそのまま返すことで、次のページの情報を描画することができます。

もし、前回取得したデータとマージして表示したいのであれば、次のような感じで、戻り値をうまいこと加工してやります。
この例では、`search.nodes` のフィールドをマージしています。

{{< code lang="typescript" >}}
fetchMore({
  variables: {cursor: pageInfo.endCursor},
  updateQuery: (prevResult, {fetchMoreResult}) => {
    if (!fetchMoreResult) {
      return prevResult;
    }
    return {
      search: {
        __typename: fetchMoreResult.search.__typename,
        issueCount: fetchMoreResult.search.issueCount,
        nodes: [...prevResult.search.nodes, ...fetchMoreResult.search.nodes],
        pageInfo: fetchMoreResult.search.pageInfo,
      }
    };
  }
});
{{< /code >}}

`search` フィールドの値を漏れなく列挙するのが面倒な場合は、`Object.assign()` による Shallow マージの仕組みを使って、次のように記述することもできます。

{{< code lang="typescript" >}}
return {
  search: Object.assign({}, fetchMoreResult.search, {
    nodes: [...prevResult.search.nodes, ...fetchMoreResult.search.nodes]
  })
};
{{< /code >}}

