---
title: "Next.js で Sass (scss/sass) を有効にする"
linkTitle: "Sass (scss/sass) を有効にする"
url: "/p/2n2doyb"
date: "2021-05-06"
tags: ["Next.js"]
weight: 3
---

Next.js は Sass を使ったスタイルシート記述 (`.scss` / `.sass` ファイル）をサポートしています。
ほぼゼロコンフィグ（設定なし）で使用できますが、NPM の sass モジュールだけはマニュアルでインストールする必要があります。

{{< code title="sass のインストール" >}}
### yarn の場合
$ yarn add sass --dev

### npm の場合
$ npm install sass --save-dev
{{< /code >}}

今まで `.css` 拡張子で作成していたファイルを、`.scss` に変更すれば、SCSS フォーマットでスタイル記述できます。

- 置換前: <code>styles/global<strong>.css</strong></code>
- 置換後: <code>styles/global<strong>.scss</strong></code>

あとは、`.css` ファイルをインポートしている部分を、`.scss` に置換すれば導入完了です。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import '../styles/global.scss'

export default function MyApp({ Component, pageProps }: AppProps) {
  return <Component {...pageProps} />
}
{{< /code >}}

もちろん、[CSS Modules の仕組み](/p/seq2cmw) を使っている場合にも Sass を使うことができます。
その場合は、`.module.css` という拡張子を `.module.scss` に置き換えます。

