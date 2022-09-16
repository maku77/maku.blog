---
title: "Apollo Client でクリック時に GraphQL クエリを実行する"
url: "p/m7ju6gr/"
date: "2020-11-12"
tags: ["Apollo", "GitHub", "GraphQL", "React"]
---

{{< private >}}
- [Apollo Client [React]— How to Query on Click](https://www.apollographql.com/blog/apollo-client-react-how-to-query-on-click-c1d4fecf9b66/)
{{< /private >}}

{{< image src="img-001.gif" border="true" title="useLazyQuery による GraphQL クエリ実行" >}}

はじめに
----

Apollo Client の `useQuery` フックを使用すると、GraphQL を使って取得した情報を表示する React コンポーネントをシンプルに実装することができます。
`useQuery` フックによる GraphQL クエリは、React コンポーネントの表示時に実行されますが、代わりに __`useLazyQuery`__ フックを使用すると、任意のタイミング、例えばボタンを押した時に GraphQL クエリを実行できるようになります。

前提として、Apollo Client の `useQuery` の基本的な使い方は下記の記事などで理解しているものとし、ここでは、`useLazyQuery` フックの使い方を説明します。

- 参考: [Apollo Client で GitHub GraphQL API を使う (Node & React)"](/p/qcp2cnx)


useQuery と useLazyQuery の違い
----

下記の抜粋コードは、`useQuery` 関数と `useLazyQuery` 関数の使い方の違いを表しています。

{{< code lang="typescript" >}}
// const GET_ISSUES = gql`...`;

const {loading, error, data} = useQuery(GET_ISSUES);
const [getIssues, {loading, error, data}] = useLazyQuery(GET_ISSUES);
{{< /code >}}

`useQuery` 関数は呼び出し直後に GraphQL クエリが実行され、その状態や結果が直ちに `loading`、`error`、`data` といった戻り値に格納されます。
一方 `useLazyQuery` 関数の場合は、戻り値の最初の要素として、クエリ実行関数が返されます（上記の例では `getIssues` にしてるけど、変数名は `executeQuery` とか何でも OK）。
GraphQL クエリを実行するには、このクエリ実行関数を呼び出す必要があるので、例えば次のようにボタン要素の `onClick` で呼び出すようにしておきます。
あとは、`useQuery` 関数の使い方と同様です。

{{< code >}}
<button onClick={() => getIssues()}>Issue取得</button>
{{< /code >}}

ちなみに、`useLazyQuery` 関数が返す値（`loading`、`error`、`data` など）は次のように変化します（ここでは呼び出しが正常に終了したものとしているので、`error` の値は `undefined` で変化しません）。

| クエリ実行前 | クエリ実行中 | 結果取得後 |
| ---- | ---- | ---- |
| loading = false<br>error = undefined<br>data = undefined<br>called = false | <b>loadng = true</b><br>error = undefined<br>data = undefined<br><b>called = true</b> | loading = false<br>error = undefined<br><b>data = 結果オブジェクト</b><br><b>called = true</b> |

`called` はまさに `useLazyQuery` 関数用の戻り値で、クエリ実行関数を呼び出した時点で `true` になります。
GraphQL クエリを実行する前と、実行した後で表示を切り替えたい場合に便利です。


useLazyQuery を使った実装例
----

下記は、Apollo Client の `useLazyQuery` 関数の使用例です。
表示された `Issue 取得` ボタンを押すと、GraphQL クエリで GitHub リポジトリ (`apollographql/apollo`) の Issue 情報を取得して表示します。
`ApolloProvider` コンポーネントなどのセットアップは、上位のコンポーネントで行われているものとします。

{{< code lang="jsx" >}}
import * as React from 'react';
import {ApolloError, gql, useLazyQuery} from '@apollo/client';

// 発行する GraphQL クエリ
const GET_ISSUES = gql`
  query {
    search(query: "repo:apollographql/apollo is:issue", type: ISSUE, last: 3) {
      issueCount
      nodes {
        ... on Issue { number title }
      }
    }
  }
`;

// RepoInfo コンポーネントの実装
export const RepoInfo: React.FC = () => {
  const [getIssues, {loading, error, data, called}] = useLazyQuery(GET_ISSUES);
  const issueList = called ? createIssueList(loading, error, data) : null;
  return <div>
    <button onClick={() => getIssues()}>Issue取得</button>
    {issueList}
  </div>;
};

// useLazyQuery の結果から表示内容を構築する
// （data の構造は GraphQL クエリに対応している）
function createIssueList(loading: boolean, error: ApolloError, data: any): React.ReactElement {
  // クエリ実行中の表示
  if (loading) return <p>Loading ...</p>;

  // エラー発生時（レスポンスがないとき）の表示
  if (error) return <p style={{color: 'red'}}>{error.message}</p>;

  // クエリの結果が返ってきたときの表示
  const {issueCount, nodes: issues} = data.search;
  return <>
    <h2>Num of issues: {issueCount}</h2>
    <ul>
      { issues.map(i => <li key={i.number}>{i.number} - {i.title}</li>) }
    </ul>
  </>;
}
{{< /code >}}

