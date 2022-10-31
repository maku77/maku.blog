---
title: "Next.js でローカル開発時 (next dev) のみ有効なデバッグページを作成する (getStaticProps)"
url: "/p/3vbr2bm"
date: "2022-05-09"
tags: ["Next.js"]
changes:
  - 2022-10-17: getStaticProps の実装を共通化する方法
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

{{< code lang="ts" title="src/pages/debug/info.tsx" >}}
import { GetStaticProps, NextPage } from 'next'

type EmptyProps = { [key: string]: never }

/**
 * デバッグページのビルド時の前処理。
 * 開発サーバー (next dev) での実行時のみ、このページが存在するようにします。
 * 本番サーバー (next start) での実行時は、404 Not Found になります。
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


（おまけ）モジュール化しておく
----

複数のデバッグページを作成する場合は、前述の `getStaticProps` 実装を使いまわせるようにしておくと便利です。

{{< code lang="ts" title="src/utils/debug.ts" >}}
import { GetStaticProps } from 'next'

type EmptyProps = { [key: string]: never }

/**
 * デバッグページ用の `getStaticProps` 実装です。
 * 開発サーバー (next dev) での実行時のみ、このページが存在するようにします。
 * 本番サーバー (next start) での実行時は、404 Not Found になります。
 */
export const getDebugPageStaticProps: GetStaticProps<EmptyProps> = () => {
  const isLocalDev = process.env.NODE_ENV !== 'production'
  return { notFound: !isLocalDev, props: {} }
}
{{< /code >}}

あとは、開発中（デバッグ中）のみ表示したいページで、次のように設定します。

{{< code lang="jsx" title="src/pages/hello.tsx" hl_lines="4-5" >}}
import { NextPage } from 'next'
import { getDebugPageStaticProps } from '../utils/debug'

// ローカル開発時 (next dev) のみページを表示するようにします
export const getStaticProps = getDebugPageStaticProps

const HelloPage: NextPage = () => {
  return <p>This page is only displayed during development.</p>
}

export default HelloPage
{{< /code >}}

本番環境用にビルドして実行 (`yarn build && yarn start`) したときに、上記ページへのアクセスが 404 になることを確認してください。

