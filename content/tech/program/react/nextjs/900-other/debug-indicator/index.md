---
title: "Next.js で開発環境で実行しているときに Web サイト上に dev 表示する (TargetEnvIndicator)"
url: "/p/d4p4fr4"
date: "2021-10-29"
tags: ["Next.js"]
---

何をするか？
----

{{< image border="true" src="img-001.png" >}}

Next.js (React) などで Web サイトの開発を行うとき、その開発フェーズに応じて、開発環境 (dev)、ステージング環境 (stg)、本番環境 (prod) などを分けてリリースしていくことが多いと思います。
このとき、ブラウザ上でどのフェーズのサイトを表示しているのかが分かるように、画面上に __dev 環境__ のようなインジケーターを表示すると便利です（上図）。

ここでは、環境変数 __`NEXT_PUBLIC_TARGET_ENV`__ の値が __`prod`__ 以外のときに、上記のような表示をすることにします。

- 参考: [Next.js で環境変数を扱う (.env, NEXT_PUBLIC, NODE_ENV)](/p/gbpeyov)


実装例
----

次のコンポーネント `TargetEnvIndicator` は、画面右下に環境名（例: `dev 環境`）を表示します。

{{< code lang="tsx" title="components/common/TargetEnvIndicator.tsx" >}}
import { FC } from 'react'

/**
 * ビルド時のターゲット環境が `prod` 以外のときに、画面右下に環境名を表示します。
 */
export const TargetEnvIndicator: FC = () => {
  const target = process.env.NEXT_PUBLIC_TARGET_ENV ?? 'dev'

  // 本番環境 (prod) の場合は何も表示しない
  if (target === 'prod') return null

  return (
    <div
      style={{
        position: 'fixed',
        right: 10,
        bottom: 10,
        padding: '0.2em 0.6em',
        borderRadius: '0.2em',
        fontSize: '16pt',
        background: '#f008',
        color: '#fffd',
        zIndex: 1000,
      }}
    >
      {target} 環境
    </div>
  )
}
{{< /code >}}

ターゲット環境は環境変数 `NEXT_PUBLIC_TARGET_ENV` で指定しますが、この環境変数が設定されていない場合は、デフォルトで「dev 環境」と表示します。

| NEXT_PUBLIC_TARGET_ENV の値 | 表示内容 |
| ---- | ---- |
| `prod` | 何も表示しない |
| `prod` 以外 | `<その値> 環境` |
| 未設定 | `dev 環境` |

上記のコンポーネントは、アプリ全体に適用されるように、例えば、カスタム App コンポーネントの中で次のように記述しておきます。

{{< code lang="tsx" title="pages/_app.tsx" hl_lines="2 17" >}}
// ...
import { TargetEnvIndicator } from '../components/common/TargetEnvIndicator'

export default function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  // ...
  return (
    <>
      <Head>
        <title>My App</title>
        <meta name="viewport"
          content="minimum-scale=1, initial-scale=1, width=device-width"
        />
      </Head>
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <Component {...pageProps} />
        <TargetEnvIndicator />
      </ThemeProvider>
    </>
  )
}
{{< /code >}}

あとは、ビルド時 (`next build`) や開発時 (`next dev`) に、環境変数 `NEXT_PUBLIC_TARGET_ENV` を設定しておけば、その名前がブラウザ画面右下に表示されるようになります（`prod` の場合は何も表示しません）。

