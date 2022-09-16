---
title: "Apollo Client の useQuery 呼び出し部分をカスタムフックで分離する"
url: "p/kmj7sdv/"
date: "2020-11-30"
lastmod: "2021-07-20"
tags: ["Apollo", "GitHub", "GraphQL", "React"]
---

{{% private %}}
- https://www.apollographql.com/docs/react/development-testing/static-typing/
{{% /private %}}

Apollo Client で GraphQL クエリを実行するときは、カスタムフックとして `useQuery` 関数の呼び出し部分を抽出すると、コンポーネント側のコードをシンプルにすることができます。


分離前のコード
----

次のサンプルコードでは、GraphQL クエリで GitHub のログインユーザー情報を取得して表示する `Viewer` コンポーネントを実装しています。
GraphQL のクエリ呼び出し部分や、取得したデータを `ViewerData` オブジェクトに詰める部分などが混在しており、あまり整理されているとは言えません。

{{< code lang="jsx" title="components/Viewer.tsx" >}}
import { FC } from 'react'
import { gql, useQuery } from '@apollo/client'
import { LoadingComponent } from './LoadingComponent'
import styles from './Viewer.scss'

const QUERY_VIEWER = gql`
  query QueryViewer {
    viewer {
      login
      url
      avatarUrl
    }
  }
`

type ViewerData = {
  /** ログインID */
  login: string
  /** ホームページのURL */
  url: string
  /** アバター画像のURL */
  avatarUrl: string
}

/** 「ユーザー情報」を表示するコンポーネント */
export const Viewer: FC = () => {
  const {loading, error, data} = useQuery(QUERY_VIEWER)
  if (loading) return <LoadingComponent />
  const viewer: ViewerData = data.viewer

  return (
     <div className={styles.container}>
       <a href={viewer.url}><img src={viewer.avatarUrl}/></a>
     </div>
  )
}
{{< /code >}}

こんなときは、カスタムフックを作成して、GraphQL クエリを実行する部分や、その結果を加工する部分などを分離するとコンポーネント側のコードがシンプルになります。


分離後のコード
----

下記の `useViewer` カスタムフックは、Apollo Client API の `useQuery` 呼び出し部分や、その戻り値のデータパース処理を実装しています。

{{% note title="カスタムフックとは" %}}
関数内部で `useXxx` 系のフック関数を呼び出すものをカスタムフックと呼びます。
カスタムフックは通常のフック関数と同様の制約を引き継ぐため、関数コンポーネント内では一定の順序で呼び出さないといけません。
{{% /note %}}

{{< code lang="typescript" title="hooks/useViewer.ts（カスタムフック）" >}}
import { gql, useQuery } from '@apollo/client'

export const QUERY_VIEWER = gql`
  query QueryViewer {
    viewer {
      login
      url
      avatarUrl
    }
  }
`

/** useViewer カスタムフックの戻り値の型 */
export type ViewerData = {
  /** ログインID */
  login: string
  /** ユーザーのホームページアドレス */
  url: string
  /** アバター画像のURL */
  avatarUrl: string
}

/** useQuery の戻り値 QueryResult の data プロパティの型 */
type Data = {
  viewer: ViewerData
}

/**
 * サインイン中の GitHub ユーザー情報を取得します。
 * ロード中やエラー時は undefined を返します。
 */
export function useViewer(): ViewerData | undefined {
  const { data } = useQuery<Data>(QUERY_VIEWER)
  return data?.viewer
}
{{< /code >}}

ポイントは、`useQuery` の型パラメータとして `Data` 型を指定しているところです。
これにより、`useQuery` の戻り値の `data` プロパティの型が `Data` 型になります。

{{< note title="QueryResult インタフェース" >}}
`QueryResult` インタフェースは、Apollo Client が定義している `useQuery` の戻り値の型です。
`QueryResult` オブジェクトの `data` プロパティは、デフォルトでは `any` 型ですが、`QueryResult<ViewerData>` のように型パラメータを指定すると、`data` プロパティを `ViewerData` 型として参照できるようになります。
{{< /note >}}

この `Data` 型は、下記の GraphQL クエリに対応するプロパティを保持するよう定義する必要があります（この場合、`Data` 型は `viewer` プロパティを持つよう定義します）。

{{< code lang="graphql" >}}
query QueryViewer {
  viewer {
    login
    url
    avatarUrl
  }
}
{{< /code >}}

実際に `useViewer` カスタムフックを使う側が参照したいのは、この `Data` 型のデータではなく、そこに含まれている `viewer` （`ViewerData` オブジェクト）の方なので、`res.data.viewer` オブジェクトをリターンするようにしています。

下記は、このカスタムフックの使用例です。

{{< code lang="jsx" title="components/Viewer.tsx（リファクタ後）" >}}
import { FC } from 'react'
import { LoadingComponent } from './LoadingComponent'
import { useViewer } from '../hooks/useViewer'
import styles from './Viewer.scss'


/** 「ユーザー情報」を表示するコンポーネント */
export const Viewer: FC = () => {
  const viewer = useViewer()
  if (!viewer) return <LoadingComponent />

  return (
    <div className={styles.container}>
      <a href={viewer.url}><img src={viewer.avatarUrl} /></a>
    </div>
  )
}
{{< /code >}}

とてもシンプルですね！
`Viewer` コンポーネントから GraphQL クエリの呼び出し部分などが取り除かれ、UI 表現に集中して実装できるようになりました。

`useViewer` フックは、データ取得前は `undefined` を返し、データ取得後に `ViewerData` オブジェクトを返すようにしているので、戻り値が `undefined` の場合は Loading 表示などを行えば OK です。

エラー情報やローディング状態を独立した変数で返して欲しいのであれば、カスタムフックの戻り値を次のように一段ラップすればよいです。

{{< code lang="ts" >}}
export type UseViewerOutput = {
  loading: boolean
  error?: ApolloError
  viewer?: ViewerData
}

export function useViewer(): UseViewerOutput {
  const { loading, error, data } = useQuery<Data>(QUERY_VIEWER)
  return { loading, error, viewer: data?.viewer }
}
{{< /code >}}

（ただ、基本的にローディング中かどうかは `viewer == undefined` で判定できるので `loading` は必要ないはず）


あわせて読みたい記事
----

- [Apollo CLI の codegen で GraphQL クエリレスポンスの TypeScript 型を自動生成する](/p/bnrrqpn)

