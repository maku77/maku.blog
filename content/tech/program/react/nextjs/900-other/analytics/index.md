---
title: "Next.js アプリに Google Analytics を設定する (next/script)"
url: "p/zycmw6f/"
date: "2023-03-21"
tags: ["Google Analytics", "Next.js"]
---

何をするか？
----

Google Analytics で Web サイトのアクセス解析を行うには、次のような感じのコードを各ページに埋め込む必要があります。

{{< code lang="html" title="Google Analytics 用の埋め込みコード" >}}
<!-- Google tag (gtag.js) -->
<script async src="https://www.googletagmanager.com/gtag/js?id=G-ABCDE12345"></script>
<script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());
  gtag('config', 'G-ABCDE12345');
</script>
{{< /code >}}

ここでは、Next.js が提供する __`Script`__ コンポーネント (__`next/script`__) を使用して、Next.js アプリに同等のコードを埋め込む方法を説明します。
HTML 標準の `script` ではなく、Next.js の `Script` コンポーネントを使用することで、Next.js アプリのレンダリング処理に合わせて実行タイミングを最適化できます。


事前準備（Google Analitics でプロパティとデータストリームを追加する）
----

事前準備として、[Google Analytics](https://analytics.google.com/) のサイトで、サイドバーの __管理__ メニューから対象アプリ用に __プロパティ__ を追加しておいてください。
さらに、そのプロパティに対して __データストリーム__ を追加すると、Web サイトや Android アプリの利用状況を監視できるようになります。

{{< image border="true" src="img-001.png" title="プロパティにデータストリームを追加する" >}}

データストリームを追加すると、次のように __測定 ID__ が発行されます。
これが、前述の JavaScript コードで指定する ID になります。

{{< image border="true" src="img-002.png" title="測定 ID が発行された" >}}


Next.js 用の Analytics コンポーネントを作成する
----

下記の `Analytics.tsx` は、Google Analytics 用のコードを埋め込むためのコンポーネントの実装例です。
Google Analytics の測定 ID は、ホスティングサーバー（例えば Vercel）の環境変数 __`NEXT_PUBLIC_ANALYTICS_ID`__ で設定することを想定しています（自サイト専用のコンポーネントにするなら、ハードコーディングしちゃっても大丈夫です）。

{{< code lang="tsx" title="components/common/Analytics.tsx" >}}
import { FC } from 'react'
import Script from 'next/script'

// サーバーの環境変数で Google Analytics の測定 ID を指定します
const ANALYTICS_ID = process.env.NEXT_PUBLIC_ANALYTICS_ID
// あるいはハードコーディングでも可
// const ANALYTICS_ID = 'G-ABCDE12345'

/** Google Analytics によるアクセス解析を行うためのコンポーネント */
export const Analytics: FC = () => {
  if (process.env.NODE_ENV !== 'production') {
    // 開発サーバー上での実行 (next dev) では何も出力しない
    return <></>
  }

  if (!ANALYTICS_ID) {
    console.warn('NEXT_PUBLIC_ANALYTICS_ID not defined')
    return <></>
  }

  return (
    <>
      <Script
        src="https://www.googletagmanager.com/gtag/js?id=${ANALYTICS_ID}"
        strategy="afterInteractive"
      />
      <Script id="google-analytics" strategy="afterInteractive">
        {`
          window.dataLayer = window.dataLayer || [];
          function gtag(){dataLayer.push(arguments);}
          gtag('js', new Date());
          gtag('config', '${ANALYTICS_ID}');
        `}
      </Script>
    </>
  )
}
{{< /code >}}

`Script` コンポーネントの `strategy` プロパティでは、読み込みタイミングを細かく制御できます（参考: [Handling Scripts | Next.js](https://nextjs.org/docs/basic-features/script#strategy)）。
`afterInteractive` を指定すると、ページのハイドレーション処理が行われたタイミング（つまりユーザー操作が可能になったタイミング）で実行されます。
デフォルト値は `afterInteractive` なので、実はサンプルコード内の `strategy` プロパティの指定は省略することができます。

JavaScript コードをインラインで埋め込んでいる方の `Script` タグには、`id` プロパティを指定しておく必要があります。
この `id` は、Next.js が内部での最適化に使用します。
サンプルコードでは `google-analytics` と指定していますが、一意の文字列であれば何でも構いません。


Analytics コンポーネントを設置する
----

`Analytics` コンポーネントは、各ページで実行されるように __`pages/_app.tsx`__ から呼び出すようにします。

{{< code lang="tsx" title="pages/_app.tsx" hl_lines="3 13" >}}
import type { AppProps } from 'next/app'
import Head from 'next/head'
import { Analytics } from '../components/common/Analytics'

export default function MyApp({ Component, pageProps }: AppProps) {
  return (
    <>
      <Head>
        <meta charSet="utf-8" />
        <meta name="viewport" content="initial-scale=1, width=device-width" />
        <title>Awesome website</title>
      </Head>
      <Analytics />
      <Component {...pageProps} />
    </>
  )
}
{{< /code >}}

Next.js の `Script` 要素は `Head` 要素の外に記述しないと動作しないようなので注意してください。
これで、Web サイトのアクセス情報を Google Analytics のページで確認できるようになります。

٩(๑❛ᴗ❛๑)۶ わーぃ

