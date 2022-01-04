---
title: "Next.js で全ページ共通のレイアウトを定義する（Layout コンポーネント）"
url: "/p/dxamw8i"
date: "2021-05-04"
tags: ["Next.js"]
weight: 2
---

Next.js で Web サイトに見た目の統一感を持たせるには、`Layout` コンポーネントを作成して、全てのページのベースレイアウトとして使用するようにします。
Next.js の仕組みというより、React コンポーネントを作成するときの慣例のようなもので、コンポーネントに `Layout` という名前を付けるのも多くの人がその名前を使っているだけです。

Layout コンポーネントを定義する
----

`Layout` コンポーネントを定義するために、次のようなファイルをプロジェクト内に作成します。
__`{children}`__ の部分には、`Layout` 要素以下に配置した子要素が展開されることになります。

{{< code lang="tsx" title="components/Layout.tsx" >}}
import { ReactNode } from 'react'

type Props = {
  children: ReactNode;
}

export function Layout({ children, ...props }: Props) {
  return <div {...props}>{children}</div>
}
{{< /code >}}

この `Layout` コンポーネントを使用するには、各ページのコンポーネント実装において、ルート要素として配置します。

{{< code lang="tsx" title="pages/about.tsx" >}}
import Head from 'next/head'
import Link from 'next/link'
import { Layout } from '../components/Layout'

export default () => (
  <Layout>
    <Head>
      <title>About me</title>
    </Head>
    <h1>About me</h1>
    <Link href="/">
      <a>Back to home</a>
    </Link>
  </Layout>
)
{{< /code >}}


Layout コンポーネントに CSS を適用する
----

### styled-jsx で直接スタイルを埋め込む方法

[styled-jsx の仕組み](/p/pbp2dpy) を使って、コンポーネントの定義内に直接 CSS を埋め込んでしまう方法です。

{{< code lang="tsx" title="components/Layout.tsx" >}}
import { ReactNode } from 'react'

type Props = {
  children: ReactNode;
}

export function Layout({ children, ...props }: Props) {
  return <>
    <style jsx>{`
      .container {
        max-width: 36rem;
        padding: 0 1rem;
        margin: 3rem auto 6rem;
        background: #eee;
      }
    `}</style>
    <div className="container" {...props}>
      {children}
    </div>
  </>
}
{{< /code >}}

### CSS Modules の仕組みで別ファイルにスタイル定義する方法

[CSS Modules の仕組み](/p/seq2cmw) で、`Layout` コンポーネント用の CSS ファイルを作成してインポートする方法です。

{{< code lang="css" title="components/Layout.module.css" >}}
.container {
  max-width: 36rem;
  padding: 0 1rem;
  margin: 3rem auto 6rem;
  background: #eee;
}
{{< /code >}}

{{< code lang="tsx" title="components/Layout.tsx" >}}
import { ReactNode } from 'react'
import styles from './Layout.module.css'

type Props = {
  children: ReactNode;
}

export function Layout({ children, ...props }: Props) {
  return (
    <div className={styles.container} {...props}>
      {children}
    </div>
  )
}
{{< /code >}}

### グローバルな CSS でスタイル定義する方法

サイト全体に CSS ファイルを適用して、その中で定義した CSS クラスを `Layout` コンポーネント内で参照するという方法もあります。

- 参考: [すべてのページにグローバルな CSS を適用する (pages/_app.ts)](/p/s9iry9g)

