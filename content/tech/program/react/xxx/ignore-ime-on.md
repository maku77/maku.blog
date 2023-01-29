---
title: "React で入力フォームの onChange イベントを IME ON のときに無視する"
url: "p/cv6fpx7/"
date: "2023-01-29"
tags: ["React"]
---

何をするか？
----

React のテキスト入力フォーム（`input` コンポーネントや mui の `TextField` など）で `onChange` イベントハンドラーを設定すると、入力テキストが変化したときに任意の処理を行うことができます。
ただ、__`onChange` イベントは IME での日本語変換中にも呼び出されてしまう__ ので、おそらく、想定しているよりも多く呼び出されてしまいます。
インクリメンタルサーチなどで、入力のたびに API 呼び出しをしているようなケースでは、この振る舞いは抑制しなければいけません。

ここでは、IME が ON になっているとき（つまり日本語変換中）に、`onChange` イベントを無視する実装例を紹介します。
このあたりの実装は、ブラウザごとの微妙な振る舞いの差（特に `Esc` キーで IME 入力をキャンセルした場合など）を考えると、結構複雑だったりします。


実装してみる
----

React の `input` コンポーネントの __`onCompositionStart`__ と __`onCompositionEnd`__ イベントハンドラーを設定すると、IME を ON/OFF したタイミングを知ることができます。
そのタイミングで、`isImeOn` のようなフラグを制御すれば、必要に応じて `onChange` イベントを無視できるようになります。

```tsx
// import { FC, useRef, useState } from 'react'

const SearchBox: FC = () => {
  // 現在 IME ON（変換中）かどうかのフラグ
  const isImeOn = useRef(false)

  // 以前の入力テキスト（ブラウザごとの onChange の振る舞いの差異への対策）
  const [prevText, setPrevText] = useState('')

  // 入力テキストを処理する
  const handleChange = (text: string) => {
    if (prevText === text) return
    if (text === '') {
      // Chrome ではテキストクリア時に onCompositionEnd が呼ばれないことがある
      isImeOn.current = false
    } else if (isImeOn.current) {
      return // IME 変換中は何もしない
    }
    setPrevText(text)

    // ここで最新の入力値にもとづいて検索処理などを行う
    console.log(text)
  }

  return (
    <input
      id="search"
      type="search"
      onChange={(e) => handleChange(e.target.value)}
      onCompositionStart={() => {
        isImeOn.current = true // IME 入力中フラグを ON
      }}
      onCompositionEnd={(e) => {
        isImeOn.current = false // IME 入力中フラグを OFF
        handleChange((e.target as HTMLInputElement).value) // 入力確定したとき
      }}
    />
  )
}
```

ポイントは、`onCompositionEnd` ハンドラーの中で `handleChange` を明示的に呼び出しているところでしょうか。
これを入れておかないと、IME での変換を確定したときに、入力されたテキストを処理することができません（`onChange` は呼び出されるけど、IME 変換中とみなして処理をスキップしてしまうため）。

`input` コンポーネントの代わりに、mui の `TextField` コンポーネントを使う場合も、同様の props 設定で OK です。

```tsx
<TextField
  id="search"
  type="search"
  fullWidth
  label="Search"
  variant="filled"
  onChange={(e) => handleChange(e.target.value)}
  onCompositionStart={() => {
    isImeOn.current = true // IME 入力中フラグを ON
  }}
  onCompositionEnd={(e) => {
    isImeOn.current = false // IME 入力中フラグを OFF
    handleChange((e.target as HTMLInputElement).value) // 入力確定したとき
  }}
/>
```


コンポーネント化する
----

実際に上記のようなコードを導入する場合は、IME まわりの処理を隠蔽する形でコンポーネント化してしまった方がよいでしょう。
次の `SearchTextField` コンポーネントは、`props` として `onSearch` ハンドラーを受け取ります。
このハンドラーは、適切なタイミングで呼び出される `onChange` ハンドラーとして使用することができます。

{{% accordion title="components/SearchTextField.tsx" %}}
```tsx
import { FC, useRef, useState } from 'react'
import { TextField } from '@mui/material'

interface Props {
  onSearch?: (text: string) => void
}

/**
 * インクリメンタルサーチ用に `TextField` を拡張したコンポーネントです。
 *
 * ユーザーの入力に応じて検索 API を呼び出す場合は、`onSearch` ハンドラーを使用します。
 * `onSearch` は、`TextField` の `onChange` よりも適切なタイミングで呼び出されます。
 * 例えば、`onChange` は IME 変換中にも呼び出されてしまいますが、
 * `onSearch` はユーザーが入力を確定したときにしか呼び出されないようになっています。
 */
export const SearchTextField: FC<Props> = ({ onSearch }) => {
  // 現在 IME ON（変換中）かどうかのフラグ
  const isImeOn = useRef(false)

  // 以前の入力テキスト（ブラウザによって onChange の振る舞いが微妙に異なるための対策）
  const [prevText, setPrevText] = useState('')

  // 入力テキストを処理する
  const handleChange = (text: string) => {
    if (prevText === text) return
    if (text === '') {
      // Chrome ではテキストクリア時に onCompositionEnd が呼ばれないことがある
      isImeOn.current = false
    } else if (isImeOn.current) {
      return // IME 変換中は何もしない
    }
    setPrevText(text)

    // ここで任意のコールバック関数を呼び出す
    onSearch?.(text)
  }

  return (
    <TextField
      id="search"
      type="search"
      fullWidth
      label="Search"
      variant="filled"
      onChange={(e) => handleChange(e.target.value)}
      onCompositionStart={() => {
        isImeOn.current = true // IME 入力中フラグを ON
      }}
      onCompositionEnd={(e) => {
        isImeOn.current = false // IME 入力中フラグを OFF
        handleChange((e.target as HTMLInputElement).value) // 入力確定したとき
      }}
    />
  )
}
```
{{% /accordion %}}

このコンポーネントを使用すると、前述の実装を次のように簡潔に記述できます。

```tsx
// import { FC } from 'react'
// import { SearchTextField } from '../components/SearchTextField'

const SearchBox: FC = () => {
  return <SearchTextField onSearch={(text) => console.log(text)} />
}
```

