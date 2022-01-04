---
title: "Next.js のページコンポーネントが Client と Server どちらで実行されているか調べる (isServer, isClient, NoSsr)"
url: "/p/m7is4dn"
date: "2021-08-13"
tags: ["Next.js", "Material-UI"]
---

Next.js のページコンポネント (`src/pages/*.tsx`) の関数は、静的な HTML ファイルを生成するためにビルド時にも実行されます（参考: [Next.js のプリレンダリング機能を使用する](/p/iv4agnt)）。
従来の React による SPA アプリはクライアントサイド JavaScript でしか実行されないので、同じような感覚で実装していると振る舞いの違いでハマることがあります。
例えば、ページコンポーネントから次のように `window` オブジェクトを参照しようとすると、Next.js によるプリレンダリング時にエラーになります。
これは、`window` オブジェクトは、Web ブラウザ上で JavaScript を実行しているときにしか存在しないからです。

{{< code lang="ts" title="src/pages/hello.tsx" hl_lines="4" >}}
import { FC } from 'react'

const HelloPage: FC = () => {
  console.log(window.location) // ReferenceError: window is not defined
  return <h1>Hello</h1>
}
{{< /code >}}

逆に言うと、実行時に `window` オブジェクトが存在しているかどうかを調べることによって、ページコンポーネント内のコードが、どのタイミングで実行されているかを判別できます。

{{< code lang="ts" hl_lines="1" >}}
const isClient = () => typeof window !== 'undefined'
// const isServer = () => typeof window === 'undefined'

const HelloPage: FC = () => {
  if (isClient()) {
    console.log('これはクライアントサイド JS として実行されているよ！')
    console.log(window.location)
    alert('だから alert も使えるよ！')
  }
  return <h1>Hello</h1>
}
{{< /code >}}

ちなみに、React フックの `useEffect` で設定したコールバック関数は、DOM 要素のレンダリング後に実行されるものなので、クライアントサイドでのみ実行されることが保証されています。

{{< code lang="ts" >}}
const HelloPage: FC = () => {
  useEffect(() => {
    // ここであれば window オブジェクトは確実に参照できる
    console.log(window.location)
  }, [])

  return <h1>Hello</h1>
}
{{< /code >}}

JSX コードで記述した部分も、Next.js のサーバーサイドビルド時と、クライアントサイド JS の両方で実行されます。
クライアントサイドでのみレンダリングしたい部分は、上記で定義した `isClient` 関数を使って次のように記述します。

{{< code lang="tsx" hl_lines="5" >}}
const HelloPage: FC = () => {
  return (
    <>
      <p>Rendered in server and client</p>
      {isClient() && <p>Rendered in client</p>}
    </>
  )
}
{{< /code >}}

例えば、URL のハッシュフラグメント (`window.location.hash`) の情報を利用してデータ生成を行うような場合は、クライアントサイド JS でしか処理できない（基本的にユーザーインタラクションによってクライアントサイドでのみ変化する）ので、上記のような判定が必要になったりします。

UI ライブラリとして Material-UI を使用している場合は、同様のことを実現する `NoSsr` というコンポーネントが用意されているので、こちらを使用すると簡単です。

{{< code lang="tsx" hl_lines="2 8" >}}
import { FC } from 'react'
import { NoSsr } from '@material-ui/core'

const HelloPage: FC = () => {
  return (
    <>
      <p>Rendered in server and client</p>
      <NoSsr><p>Rendered in client</p></NoSsr>
    </>
  )
}
{{< /code >}}

[NoSsr コンポーネントの実装](https://github.com/mui-org/material-ui/blob/e85ce447ae4da2b814fd68803a6196f3f7c9f4db/packages/material-ui-unstyled/src/NoSsr/NoSsr.js#L14) も単純で、初回の DOM 要素レンダリング後にのみ子要素 (`children`) を return するようになっています。
`useEffect` を使えば、同様のコンポーネントを実装するのは簡単だと思います。

