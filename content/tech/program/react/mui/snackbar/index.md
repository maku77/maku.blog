---
title: "MUI のスナックバーを簡単に表示できるようにする (@mui/material/Snackbar)"
url: "p/jbv7gox/"
date: "2022-07-25"
tags: ["Material-UI"]
---

何をするか？
----

React 用の UI ライブラリである mui（旧: Material-UI）が提供している [Snackbar コンポーネント](https://mui.com/material-ui/react-snackbar/) を使うと、次のようなスナックバーを表示することができます。

{{< image src="img-001.png" >}}

ただ、MUI はあくまで UI ライブラリなので、表示の制御は React コンポーネントのステート管理の仕組み（`useState` など）を使って自力で行わなければいけません。
ただメッセージを表示したいだけなのに、ステート管理とか面倒です。
ここでは、MUI のスナックバーを簡単に表示できるようなフック関数 (`useSnackbar`) を作成してみます。
使い方のイメージは次のような感じです。

{{< code lang="tsx" title="pages/sample.tsx" >}}
import { NextPage } from 'next'
import { Button } from '@mui/material'
import { useSnackbar } from '../libs/snackbar/Snackbar'

const SamplePage: NextPage = () => {
  const { showSnackbar } = useSnackbar()
  return (
    <Button onClick={() => showSnackbar('This is an error message!', 'error')}>
      Show
    </Button>
  )
}

export default SamplePage
{{< /code >}}

任意の React コンポーネント（あるいは Next.js のページ）から、上記のように `useSnackbar` フック関数を呼び出して、返された `showSnackbar` 関数を呼び出すだけです。
なお、以下の実装では React のコンテキストの仕組み（グローバル変数的なもの）をふんだんに利用しているので、そのあたりは下記記事を参考にすると理解しやすいと思います。

- 参考: [React Context で複数のコンポーネント間でデータを共有する](/p/qoz8fow/)


スナックバー用の UI コンポーネントの作成
----

まずはスナックバーを表示するための UI コンポーネントを作成しておきます。
コンポーネント名は MUI の `Snackbar` と被らないように、`GlobalSnackbar` としました（`AppSnackbar` でもいいかも）。
実装はほぼ [MUI の Snackbar ドキュメント](https://mui.com/material-ui/react-snackbar/) からの流用ですが、props で `onClose` ハンドラの登録や、色の指定を行えるようにしています。

{{< code lang="tsx" title="libs/snackbar/GlobalSnackbar.tsx" >}}
import * as React from 'react'
import { Snackbar } from '@mui/material'
import MuiAlert, { AlertColor, AlertProps } from '@mui/material/Alert'

/** スナックバーの表示をカスタマイズ */
const Alert = React.forwardRef<HTMLDivElement, AlertProps>(function Alert(props, ref) {
  return <MuiAlert elevation={6} ref={ref} variant="filled" {...props} />
})

/** GlobalSnackbar コンポーネントに渡す props の型情報 */
type Props = {
  /** スナックバーを表示するか */
  open: boolean

  /** スナックバーに表示するメッセージ */
  message: string

  /** スナックバーの色 (error | warning | info | success) */
  severity?: AlertColor

  /** スナックバーを閉じるべきタイミングで呼び出されます */
  onClose?: () => void
}

/** スナックバーを表示するコンポーネント */
export const GlobalSnackbar: React.FC<Props> = ({
  open, message, severity = 'info', onClose
}) => {
  return (
    <Snackbar open={open} onClose={onClose} autoHideDuration={6000}
      anchorOrigin={{ vertical: 'bottom', horizontal: 'center' }}
    >
      <Alert severity={severity}>{message}</Alert>
    </Snackbar>
  )
}
{{< /code >}}

スナックバーは 6 秒放置で自動クローズするようにしていますが、このあたりは props でカスタマイズできるようにしておいてもいいかもしれません。
表示位置は `anchorOrigin` prop で柔軟に制御できますが、ここはアプリ内で統一されていた方がよいので、props に切り出す必要はありません。


コンテキストおよびフック関数の実装
----

今回のキモとなるコンテキストオブジェクトの定義です。
`SnackbarContext` というコンテキストオブジェクトを定義して、スナックバーに表示すべき内容をグローバルに管理します。
コンテキストオブジェクトには、`showSnackbar` という関数も持たせるようにして、どの階層のコンポーネントからでもスナックバーを表示できるようにしています。

{{< code lang="tsx" title="libs/snackbar/Snackbar.tsx" >}}
import * as React from 'react'
import { AlertColor } from '@mui/material'
import { GlobalSnackbar } from './GlobalSnacbar'

/** SnackbarContext コンテキストオブジェクトの型 */
export type SnackbarContextType = {
  /** Snackbar に表示する文字列。空文字列のときは Snackbar を表示しないことを意味します */
  message: string

  /** Snackbar の色 */
  severity: AlertColor // 'error' | 'warning' | 'info' | 'success'

  /** Snackbar を表示したいときに呼び出します */
  showSnackbar: (message: string, severity: AlertColor) => void
}

/** スナックバーの表示状態を管理するコンテキストオブジェクト */
export const SnackbarContext = React.createContext<SnackbarContextType>({
  message: '', // デフォルト値
  severity: 'error', // デフォルト値
  // eslint-disable-next-line @typescript-eslint/no-unused-vars, @typescript-eslint/no-empty-function
  showSnackbar: (_message: string, _severity: AlertColor) => {}, // ダミー関数
})

/**
 * SnackbarContext コンテキストオブジェクトを提供するコンポーネント。
 *
 * このコンポーネント以下に配置した子コンポーネントであれば、
 * useSnackbar フック関数を呼び出すことができます。
 */
export const SnackbarContextProvider: React.FC<{
  children: React.ReactNode
}> = ({ children }) => {
  const context: SnackbarContextType = React.useContext(SnackbarContext)
  const [message, setMessage] = React.useState(context.message)
  const [severity, setSeverity] = React.useState(context.severity)

  // コンテクストオブジェクトに自分自身の値を変更する関数をセットする
  const newContext: SnackbarContextType = React.useMemo(
    () => ({
      message,
      severity,
      showSnackbar: (message: string, severity: AlertColor) => {
        setMessage(message)
        setSeverity(severity)
      },
    }),
    [message, severity, setMessage, setSeverity]
  )

  // スナックバーを閉じるためのハンドラー関数
  const handleClose = React.useCallback(() => {
    setMessage('')
  }, [setMessage])

  return (
    <SnackbarContext.Provider value={newContext}>
      {children}
      <GlobalSnackbar
        open={newContext.message !== ''}
        message={newContext.message}
        severity={newContext.severity}
        onClose={handleClose}
      />
    </SnackbarContext.Provider>
  )
}

/** SnackbarContext を簡単に使うためのユーティリティ関数 */
export function useSnackbar(): SnackbarContextType {
  return React.useContext(SnackbarContext)
}
{{< /code >}}

ここでは、コンテキストオブジェクトの Provider コンポーネント (`SnackbarContext.Provider`) の下に、スナックバーの UI コンポーネント (`GlobalSnackbar`) も配置しちゃってます。
ちょっと行儀悪いかもしれませんが、このコンポーネントを使う側で楽をしたかったので。


アプリの上位にコンテキストオブジェクトの Provider を配置
----

作成した `SnackbarContextProvider` は、その子コンポーネントに対してスナックバー用のコンテキストオブジェクト (`SnackbarContext`) を提供します。
具体的には、スナックバーの表示状態や、表示トリガーとなる関数です。
この Provider クラスは、コンポーネント階層の上の方に配置しておきます。
Next.js であれば、`pages/_app.tsx` あたりでしょうか。

{{< code lang="tsx" title="pages/_app.tsx" >}}
// ...（省略）...
export default function MyApp({
  Component,
  emotionCache = clientSideEmotionCache,
  pageProps,
}: Props): JSX.Element {
  return (
    <CacheProvider value={emotionCache}>
      <Head>
        <title>Next.js App with MUI</title>
        <meta name="viewport" content="minimum-scale=1, initial-scale=1, width=device-width" />
      </Head>
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <SnackbarContextProvider>
          <Component {...pageProps} />
        </SnackbarContextProvider>
      </ThemeProvider>
    </CacheProvider>
  )
}
{{< /code >}}

これで、任意のコンポーネントから `showSnackbar` 関数を呼び出せるようになります。

```tsx
// ...（省略）...
const SamplePage: NextPage = () => {
  const { showSnackbar } = useSnackbar()
  return <Button onClick={() => showSnackbar('Success!', 'success')}>Show</Button>
}
```
