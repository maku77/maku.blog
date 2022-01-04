---
title: "Next.js の public 以下のファイルのパスを正しく扱う"
url: "/p/xjjbwes"
date: "2021-07-30"
tags: ["Next.js"]
---

Next.js アプリでは、`/public` ディレクトリ以下に配置した静的リソースファイル（画像ファイルなど）は、次のような感じで URL のドメイン直下に配置されたファイルとして参照できるようになります。

{{< code lang="tsx" >}}
// import Image from 'next/image'

<Image
  src="/me.png"
  alt="Picture of the author"
  width={500}
  height={500}
/>
{{< /code >}}

仮に、作成した Web サイトを GitHub Pages の「プロジェクトサイト」として公開する場合は、`https://username.github.io/reponame/` のように一階層深い URL パスがアプリのルートになりますが、`next.config.js` で次のようにベースパスを設定しておけば、Next.js の `Image` コンポーネントは正しいパス (`/reponame/me.png`) に補正して画像ファイルを参照してくれます。

{{< code lang="js" title="next.config.js" >}}
const urlPrefix = '/reponame'

module.exports = {
  // ...
  assetPrefix: urlPrefix,
  basePath: urlPrefix,
  trailingSlash: true,
}
{{< /code >}}

- 参考: [Next.js アプリを GitHub Actions でビルドして GitHub Pages で公開する](/p/au8ju6g)

一方で、Next.js が提供する `Image` コンポーネントなどを使わず、`img` 要素をそのまま使った場合、このような URL プレフィックスの付加は自動では行われず、画像が参照できなくなるという問題が発生します。

{{< code lang="tsx" title="画像が参照できなくなる" >}}
<img src="/me.png" alt="Picture of the author" />
{{< /code >}}

Vercel 以外のサーバーで Web サイトをホスティングする場合、`Image` コンポーネント (next/image) は使えないことが多いので、この問題には意外とよく遭遇します。
次のユーティリティ関数 `url()` は、`public` ディレクトリ以下のファイルを参照するときに、正しい URL に補正するための関数です。

{{< code lang="ts" title="src/utils/config.ts" >}}
import getConfig from 'next/config'

/**
 * public ディレクトリ以下に配置したファイルを参照するための URL を取得します。
 *
 * 例: url('/img/sample.png') => /reponame/img/sample.png
 *
 * next.config.js の publicRuntimeConfig.urlPrefix プロパティで指定した文字列が、
 * filename 引数で指定された文字列のプレフィックスとして付加されます。
 * 例えば Web サイトを GitHub Pages のプロジェクトページなどで公開する場合、
 *
 *   https://username.github.io/reponame/
 *
 * のように、URL のドメイン直下がアプリのルートにならないため、
 * この URL 補正のために必要です。
 *
 * @see https://maku.blog/p/xjjbwes
 */
export function url(filename: string): string {
  const { publicRuntimeConfig } = getConfig() as {
    publicRuntimeConfig: { urlPrefix: string }
  }
  return publicRuntimeConfig.urlPrefix + filename
}
{{< /code >}}

上記のコードでは、`next.config.js` ファイルの __`publicRuntimeConfig.urlPrefix`__ プロパティで設定した URL プレフィックスを参照しているので、`next.config.js` ファイルに正しい値を設定しておく必要があります。

- 参考: [next.config.js: Runtime Configuration | Next.js](https://nextjs.org/docs/api-reference/next.config.js/runtime-configuration)

例えば次のように記述しておくと、`next build` によるビルド時に `URL_PREFIX` 環境変数で指定されていた値が、URL プレフィックスとして使用されるようになります。

{{< code lang="js" title="next.config.js" >}}
const urlPrefix = process.env.URL_PREFIX ? '/' + process.env.URL_PREFIX : ''

module.exports = {
  assetPrefix: urlPrefix,
  basePath: urlPrefix,
  trailingSlash: true,
  publicRuntimeConfig: { urlPrefix },  // ★コレ
}
{{< /code >}}

これで、コンポーネント内では次のように記述しておくだけで、public リソースファイルの参照が壊れてしまう心配がなくなります。

{{< code lang="tsx" >}}
// import { url } from '../utils/config'

<link rel="icon" href={url('/favicon.ico')} sizes="any" />
<img src={url('/img/sample.png')} alt="Sample image" />
{{< /code >}}

