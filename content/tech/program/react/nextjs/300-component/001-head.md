---
title: "Next.js で各ページの head 要素をカスタマイズする (next/head)"
url: "/p/rdr4fq2"
date: "2021-05-04"
tags: ["Next.js"]
weight: 1
---

Next.js で各ページの `head` 要素をカスタマイズしたいときは、Next.js の [Head コンポーネント](https://nextjs.org/docs/api-reference/next/head) を使用して、必要な値のみを設定していきます。
典型的なのは、次のような `title` 要素の設定です。

{{< code lang="tsx" >}}
// import Head from 'next/head'

<Head>
  <title>Page Title</title>
</Head>
{{< /code >}}

ちなみに、`head` 要素ではなく、`html` 要素の属性値などを設定したいときは、`pages/_document.tsx` ファイルを作成します。

