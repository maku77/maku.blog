---
title: "Next.js でローカル開発時 (next dev) のみ有効なデバッグページを作成する (getStaticProps)"
url: "/p/3vbr2bm"
date: "2022-05-09"
tags: ["Next.js"]
---

何をするか？
----

Next.js アプリを作っていると、開発中にだけ表示したいデバッグ用のページを作りたくなることがよくあります。
ここでは、ローカルサーバー (`next dev`) での開発中のみ有効になるデバッグページの作り方を説明します。


デバッグページの作成
----

Next.js の各ページのビルド時には、必要に応じてデータフェッチなどを行うための `getStaticProps` 関数が呼び出されます（参考: [Next.js のプリレンダリング機能を使用する (getStaticProps)](/p/iv4agnt/)）。

この関数の戻り値として、[notFound プロパティ](https://nextjs.org/docs/api-reference/data-fetching/get-static-props#notfound) を true にしたオブジェクトを返すと、そのページがないものとして扱うことができます（404 Not Found になる）。

```ts
export const getStaticProps: GetStaticProps = () => {
  // ...
  return {
    // Return the default 404 page with a status code of 404.
    notFound: true
  }
}
```

この仕組みを利用して次のように実装すれば、ローカルサーバーでの開発中のみ表示可能なデバッグページを作ることができます。
ローカルサーバー (`next dev`) で実行されているかどうかは、環境変数 `process.env.NODE_ENV` の値で判断できます（参考: [Next.js で環境変数を扱う (.env, NEXT_PUBLIC, NODE_ENV)](/p/gbpeyov/)）。

{{< code lang="ts" title="pages/debug/info.tsx" >}}
import { GetStaticProps, NextPage } from 'next'

type EmptyProps = { [key: string]: never }

/**
 * ページビルド時の前処理。
 * 開発サーバー (next dev) での実行時のみこのページが存在するようにします。
 */
export const getStaticProps: GetStaticProps<EmptyProps> = () => {
  const isLocalDev = process.env.NODE_ENV !== 'production'
  return { notFound: !isLocalDev, props: {} }
}

const DebugInfoPage: NextPage = () => {
  // ...

  return (
    <>
      <h1>Debug page</h1>
      <p>This page is displayed only during development.</p>
    </>
  )
}

export default DebugInfoPage
{{< /code >}}

