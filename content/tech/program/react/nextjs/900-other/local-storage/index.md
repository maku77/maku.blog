---
title: "Next.js で useState とローカルストレージ (localStorage) を連動させる"
url: "/p/cwdyhec"
date: "2021-12-01"
tags: ["Next.js"]
---

何をするか？
----

React (Next.js) の `useState` フックは、Web ページの状態を保持するものですが、ページのリロードや、ブラウザの再起動を行うと、その状態はリセットされてしまいます。

一方、Web ブラウザに搭載されている [localStorage](https://developer.mozilla.org/ja/docs/Web/API/Window/localStorage) や [sessionStorage](https://developer.mozilla.org/ja/docs/Web/API/Window/sessionStorage) を使用すると、キー＆バリュー（両方とも文字列のみ）の形でデータを保存することができます。

ここでは、これらを一緒に使うことで、`useState` で管理している状態をローカルストレージに保存・復帰できるようにしてみます。


使い方のイメージ
----

例えば、Web サイト上でダークモードの On/Off を行うスイッチがあるとして、その状態をローカルストレージに保存できるようにしたいとします。

{{< image border="true" src="img-001.png" title="ダークモード切り替えのイメージ" >}}

ダークモードの状態は `useDarkMode` のようなカスタムフックを作成して、次のように扱えると便利です。

{{< code lang="tsx" title="src/pages/sample.tsx" >}}
import { NextPage } from 'next'
import { useDarkMode } from '../hooks/useDarkMode'

const SamplePage: NextPage = () => {
  // 一見 useState と同様だが localStorage と連動している
  const [isDark, setDark] = useDarkMode(false)

  return (
    <div
      style={{
        width: '100vw',
        height: '100vh',
        color: isDark ? 'white' : 'black',
        background: isDark ? 'black' : 'white',
      }}
    >
      <label>
        <input
          type="checkbox"
          checked={isDark}
          onChange={(e) => setDark(e.target.checked)}
        />
        {isDark ? 'DARKモードです' : 'LIGHTモードです'}
      </label>
    </div>
  )
}

export default SamplePage
{{< /code >}}


useDarkMode の実装
----

`useState` による状態をローカルストレージと連動させるには、状態の初期化時に `localStorage.getItem` で値をロード、状態の変更時に `localStorage.setItem` で値をセーブ、とすればよさそうです。
ただし、`localStorage` オブジェクトは、クライアントサイド JS でしか参照できないため、Next.js などのサーバーサイドレンダリング時に値を参照しようとするとエラーになってしまいます。
`localStorage` の参照タイミングをうまく制御しながら、`useState` フックと連携させなければいけません。

### useEffect を使う方法

`useEffect`（副作用フック）で設定したコールバック関数は、クライアントサイドでのレンダリング時にしか呼び出されないことが保証されているので、次のように `localStorage.getItem` を呼び出してやれば、状態の初期化はうまくいきます。

{{< code lang="tsx" title="src/hooks/useDarkMode.ts" >}}
import { useCallback, useEffect, useState } from 'react'

/**
 * ダークモード設定を保存するローカルストレージのキー名。
 * ダークモードなら `true` という文字列を格納する。
 */
const STORAGE_KEY_DARK_MODE = 'myapp.example.com/darkMode'

/**
 * ダークモード設定状態を扱うためのフック。
 */
export function useDarkMode(
  defaultValue: boolean
): [isDark: boolean, setDark: (dark: boolean) => void] {
  const [isDarkInternal, setDarkInternal] = useState(defaultValue)

  // クライアントでの初期レンダリング直後にローカルストレージの設定を反映
  useEffect(() => {
    const dark = localStorage.getItem(STORAGE_KEY_DARK_MODE) === 'true'
    if (dark !== defaultValue) {
      setDarkInternal(dark)
    }
  }, [setDarkInternal])

  // 外部からのセッター呼び出し時にローカルストレージに値を保存する
  const setDark = useCallback(
    (isDark: boolean) => {
      localStorage.setItem(STORAGE_KEY_DARK_MODE, isDark ? 'true' : 'false')
      setDarkInternal(isDark)
    },
    [setDarkInternal]
  )

  return [isDarkInternal, setDark]
}
{{< /code >}}

### 初期表示時のフラッシュ問題

`useEffect` は初回レンダリング後に呼び出されるので、Web ブラウザをリロードしたときに、デフォルトの状態（上記の例の場合は `isDark = false`）で描画されてしまうことを防ぐことができません。
ユーザーがダークモードに設定していたとしても、ブラウザのリロードを行うと、瞬間的にライトモードの（SSR 生成された）ページが見えてしまいます。

この問題の解決方法はいろいろ考えられますが、いずれも若干トリッキーな実装が必要になるみたいです。
参考になりそうなサイトから、ポイントだけ抜粋しておきます。

- [Adding dark mode with Next.js, styled-components, and useDarkMode](https://brianlovin.com/writing/adding-dark-mode-with-next-js)
  - SSR 時には全体を `visibility: 'hidden'` で描画しておき、初回レンダリング後（`useEffect` コールバック後）に通常表示に切り替える方法。実はデフォルトモードで描画してるんだけど、見えないようにしてるということ。

    ```tsx
    const [mounted, setMounted] = React.useState(false)
    React.useEffect(() => {
      setMounted(true)
    }, [])

    // prevents ssr flash for mismatched dark mode
    const body = <ThemeProvider theme={theme}>{children}</ThemeProvider>
    if (!mounted) return <div style={{ visibility: 'hidden' }}>{body}</div>
    return body
    ```

- [donavon/use-dark-mode: A custom React Hook to help you implement a "dark mode" component.](https://github.com/donavon/use-dark-mode#that-flash)
  - `_document.tsx` で特殊な [noflash.js](https://github.com/donavon/use-dark-mode/blob/develop/noflash.js.txt)をロードするようにしておき、その中で次のように全体に反映される CSS クラスを設定する方法。この JS ファイルはクライアントサイドでの初期レンダリング時に必ず実行されるので、確実に`localStorage` の値を CSS クラスに反映できる。

    ```ts
    (function() {
      // ...
      document.body.classList.add(darkMode ? classNameDark : classNameLight)
      // ...
    })()
    ```

いやぁ。なかなか大変ですね。。。

とはいえ、現在のテーマ設定値に関しては、少なくとも [React のコンテキスト](https://ja.reactjs.org/docs/context.html) として保持するようにしておけば、ページ遷移時に画面がフラッシュするようなことは防げます。
というわけで、ここでサンプルコードとして使った`useDarkMode` の例はあまりよくなかったかもです。。。

