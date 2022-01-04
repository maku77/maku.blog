---
title: "Next.js ですべてのページにグローバルな CSS を適用する (pages/_app.ts)"
linkTitle: "すべてのページにグローバルな CSS を適用する (pages/_app.ts)"
url: "/p/s9iry9g"
date: "2021-05-06"
tags: ["Next.js"]
weight: 1
---

Next.js で、すべてのページコンポーネント (`pages/*.tsx`) に共通のスタイルシート（いわゆるグローバル CSS）を適用したい場合は、__`pages/_app.tsx(js)`__ というファイルを作成して、そこで CSS ファイルをインポートします。

例えば、グローバル CSS として次のようなファイルを用意したとします。

{{< accordion title="styles/global.css" >}}
{{< code lang="css" >}}
* {
  box-sizing: border-box;
}

html {
    margin: 0;
    padding: 0;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, Segoe UI, Roboto, Oxygen,
    Ubuntu, Cantarell, Fira Sans, Droid Sans, Helvetica Neue, sans-serif;
  max-width: 50rem;
  padding: 0.5rem;
  margin: 0 auto;
  font-size: 100%;
}

a {
  color: inherit;
  text-decoration: none;
}
{{< /code >}}
{{< /accordion >}}

この CSS ファイルを `pages/_app.tsx` からインポートすれば、すべてのコンポーネントにスタイルが反映されます。
このファイルが存在しない場合は作成してください。

{{< code lang="tsx" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import '../styles/global.css'

export default function MyApp({ Component, pageProps }: AppProps) {
  return <Component {...pageProps} />
}
{{< /code >}}

{{% note title="pages/_app.tsx って何者？" %}}
Next.js は `pages/_app.tsx` を特殊なファイルとして扱い、ここで `default export` しているコンポーネントを、すべてのページコンポーネントの親コンポーネントとして扱います。
具体的には、上記コード中の `Component` 部分が各ページのコンポーネントに置き換えられた形で動作します。
結果的に、ここでインポートした CSS が、全ページから参照できることになります。
ちなみに、ここで複数の CSS ファイルをインポートしている場合は、Next.js が Web サイトのビルド時に 1 つの CSS ファイルにマージしてくれます。
{{% /note %}}

Bootstrap や Tailwind CSS などの CSS フレームワークを使いたい場合も、この `pages/_app.tsx` ファイルでインポートすれば OK です。

{{% reference %}}
- [Next.js で Bootstrap (React Bootstrap) を使う](/p/k8mxakw)
{{% /reference %}}

