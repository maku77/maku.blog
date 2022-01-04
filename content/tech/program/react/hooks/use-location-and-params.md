---
title: "React: ドラフト (useLocation/useParams/useRouteParams)"
url: "/p/pbnx9is"
date: "2021-06-19"
tags: ["React"]
draft: true
weight: 1
---

useLocation フック <!-- りあクト応用編 p.31 -->
----

- `react-router-dom` モジュールが提供する `useLocation` フックを使用すると、React Router が提供する `Location` オブジェクトを取得できます。
- `Location` オブジェクトには現在のアドレス情報が含まれており、`<Link>` コンポーネントおクリックなどで表示内容を変化させたときに、その URL を都度取得するといったことができます。
- React による SPA アプリで Google アナリティクスによる解析を行いたいときは、この `Location` オブジェクトから取得した URL 情報を送ってやることで、ページ遷移に関する解析を行うことができます。


useParams/useRouteParams フック
----

`useParams` や `useRouteParams` フックは、React Router が提供する `match` オブジェクトを扱うためのフックです。
例えば、`/user/:userId` のようなパスでコンポーネントを描画する場合、

{{< code lang="ts" >}}
const { userId } = useParams<{ userId: string }>()
{{< /code >}}

というコードでパラメータ（`:userId` の部分）を取り出すことができます。
`useRouteParams` を使うと、次のようにもう少し詳しい情報として取得することができます。

{{< code lang="ts" >}}
{
  path: '/user/:userId',
  url: '/user/maku',
  isExact: true,
  params: { userId: 'maku' }
}
{{< /code >}}

`useParams` の方は、この中の `params` の部分だけを取り出している感じですね。

