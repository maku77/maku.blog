---
title: "Next.js で Bootstrap と React Bootstrap を使う"
linkTitle: "Bootstrap を使う"
url: "/p/k8mxakw"
date: "2021-05-10"
tags: ["Bootstrap", "Next.js"]
---

何をするか？
----

Next.js アプリから、CSS フレームワークの Bootstrap を使えるようにする方法を説明します。

1. [Bootstrap](https://getbootstrap.com/) を導入
    - 各コンポーネントの実装で、Bootstrap の CSS クラスを参照できるようにします
2. [React Bootstrap](https://react-bootstrap.github.io/) を導入
    - Bootstrap を React コンポーネントの形で使えるようにします


Bootstrap を導入する
----

### bootstrap モジュールをインストールする方法

Bootstrap は `npm install` で簡単にインストールできます。

{{< code >}}
$ npm install bootstrap@next --save
{{< /code >}}

あとは、次のような __`pages/_app.tsx`__ ファイルを作成して、グローバル CSS としてインポートすれば OK です。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import 'bootstrap/dist/css/bootstrap.min.css'

export default function MyApp({ Component, pageProps }: AppProps) {
  return <Component {...pageProps} />
}
{{< /code >}}

これで、各ページコンポーネントから Bootstrap の CSS クラスを参照できるようになります。

### CDN 上のファイルを参照する方法

CDN 上で公開されている Bootstrap ファイルを参照することもできます。
__`pages/_app.tsx`__ ファイルを作成して、次のように Bootstrap の CSS ファイルを読み込みます。
これで、各ページの `head` 要素内に `link` 要素が追加されます。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import Head from 'next/head'

export default function MyApp({ Component, pageProps }: AppProps) {
  return <>
    <Head>
      <!-- Bootstrap CSS -->
      <link
        href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css"
        rel="stylesheet"
        integrity="sha384-giJF6kkoqNQ00vy+HMDP7azOuL0xtbfIcaT9wjKHr8RbDVddVHyTfAAsrekwKmP1"
        crossOrigin="anonymous">
      </link>
    </Head>
    <Component {...pageProps} />
  </>
}
{{< /code >}}

最新の CDN アドレスは、[Bootstrap のサイト](https://getbootstrap.jp/docs/5.0/getting-started/introduction/) で確認してください。
一点注意なのは、JSX では `crossorigin` 属性は __`crossOrigin`__ のように `O` を大文字にしないといけないというところです。
Bootstrap の高度なコンポーネントを使うときは、JavaScript ファイルも必要なことがあります。
その場合は、`Component` 要素の後ろあたりに `script` 要素を追加すれば OK です。

### 動作確認

適当なページコンポーネントで、Bootstrap の CSS クラスを参照できれば成功です。
例えば、以下のように [btn クラスを適用したボタン](https://getbootstrap.jp/docs/5.0/components/buttons/) を表示すれば、うまく適用できているか分かります。

{{< image w="333" border="true" src="img-001.png" >}}

{{< code lang="tsx" title="pages/index.tsx" >}}
const Home: React.FC = () => {
  return <>
    <button type="button" className="btn btn-primary">Primary</button>{' '}
    <button type="button" className="btn btn-secondary">Secondary</button>{' '}
    <button type="button" className="btn btn-danger">Danger</button>
  </>
}

export default Home
{{< /code >}}


React Bootstrap を導入する
----

### react-bootstrap のインストール

[React Bootstrap](https://react-bootstrap.github.io/) を導入すると、Bootstrap の CSS が適用された React コンポーネントを使えるようになります。
Bootstrap 本体の方は、前述の方法で導入しておく必要があります。

{{< code title="React Bootstrap のインストール" >}}
$ npm install react-bootstrap --save
{{< /code >}}

### 動作確認

次の例では、React Bootstrap の [Button コンポーネント](https://react-bootstrap.github.io/components/buttons/) を使用しています。
Bootstrap の CSS クラスをそのまま使うよりは、若干シンプルに記述できるようになっています。

{{< image w="333" border="true" src="img-001.png" >}}

{{< code lang="tsx" title="pages/index.tsx" >}}
import Button from 'react-bootstrap/Button';

const Home: React.FC = () => {
  return <>
    <Button variant="primary">Primary</Button>{' '}
    <Button variant="secondary">Secondary</Button>{' '}
    <Button variant="danger">Danger</Button>
  </>
}

export default Home
{{< /code >}}

