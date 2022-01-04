---
title: "Next.js でハッシュフラグメントを扱う（useHash カスタムフック）"
url: "/p/k2ahpw5"
date: "2021-06-28"
tags: ["Next.js"]
---

ハッシュフラグメントとは
----

URL のハッシュフラグメントというのは、下記のような URL の末尾の `#` 以降の部分を指します。

{{< code >}}
https://examle.com/sample#AAA
{{< /code >}}

似たようなものにクエリパラメーター（`?key=val` のみたいなの）もありますが、ハッシュフラグメントは HTTP リクエスト時に、その文字列（上記の例では `AAA`）がサーバーに送られないという違いがあります。
つまり、ハッシュフラグメントの値は、クライアントサイドで使用することが意図されています。


useHash フックの実装
----

下記の `useHash` 関数は、Next.js でハッシュフラグメントを簡単に扱えるようにするカスタムフックの例です。
`useState` フックと同じ感覚で使えるように、ハッシュフラグメントの現在値と、設定用の関数をペアで返します。

{{< code lang="ts" title="src/libs/useHash.ts" >}}
import { useCallback } from 'react'
import { useRouter } from 'next/router'

/**
 * URL のハッシュフラグメント部分を扱うためのフックです。
 *
 * 次のようにすると、`hash` 変数に URL の `#` 以降の値が格納されます。
 * URL の `#` 以降の値を変更したいときは、`setHash` 関数を使用します。
 *
 * ```
 * const [hash, setHash] = useHash()
 * ```
 */
export function useHash(): [string, (newHash: string) => void] {
  const router = useRouter()
  const hash = extractHash(router.asPath)
  const setHash = useCallback((newHash: string) => {
    // ブラウザの履歴に残すなら、ここを router.push に変えれば OK
    router.replace({ hash: newHash }, undefined, { shallow: true })
  }, [])
  return [hash, setHash]
}

// URL の # 以降の文字列を取り出すユーティリティ
function extractHash(url: string): string {
  return url.split('#')[1] ?? ''
}
{{< /code >}}

やっていることは簡単で、`router.asPath` から抽出したハッシュフラグメントの値を返しているだけです。
レンダリングのたびに値を抽出して返そうとしますが、まぁこれくらいはよいかなと(^^;


useHash フックの使用例
----

下記は `useHash` フックの使用例です。

{{< image border="true" w="317" src="img-001.png" >}}

テキストボックスに何か入力すると、Web ブラウザの URL のハッシュフラグメント部分がそれに合わせて変化し、さらにページ内の表示（Current hash の値）も連動して変化します。

{{< code lang="tsx" title="src/pages/sample.tsx" >}}
import { FC } from 'react'
import { useHash } from '../libs/useHash'

const SamplePage: FC = () => {
  const [hash, setHash] = useHash()

  return (
    <>
      <p>Current hash = {hash}</p>
      <input onChange={(e) => setHash(e.target.value)} />
    </>
  )
}

export default SamplePage
{{< /code >}}

と、ここまでやってみて気付きましたが、__Next.js アプリとハッシュフラグメントは相性がよくない__ ですね (^^;
Next.js はプリレンダリングの仕組みがあり、当然そのタイミングではハッシュフラグメントの値は取得できないので、上記のコードではプリレンダリング時には `hash` の値は必ず空っぽになってしまいます（もちろん、初回のクライアントサイドでの描画時にはすぐにハッシュフラグメントの値を取得できるようになりますが）。
なので、初期表示では `hash` の値が空っぽの状態のページがロードされるということを意識してコーディングをしておく必要があります。
このあたりの事情は [router.query で URL のクエリパラメーターを扱う](/p/r7fou3a) ときも同様です。


（おまけ）ハッシュフラグメントの変更と React コンポーネントの再レンダリングの関係
----

URL 末尾のハッシュフラグメントを変更する方法はいくつもあるので、それぞれの方法で変更した場合に、React コンポーネントが再レンダリングされるのかについてまとめておきます。

### 再レンダリングされないケース

- ユーザーが Web ブラウザのアドレスバーで、直接ハッシュフラグメントだけ変更した場合（例: `https://example.com/#1` → `https://example.com/#2`）
- コンポーネントのコードで `location.hash = 'Hello'` とした場合
- `<a href="#1">Hello</a>` のようなリンクをクリックした場合

このように、URL のハッシュフラグメント部分が直接的に変更された場合は、現在表示されているページコンポーネントの再レンダリングは実行されません。
このケースでは、ハッシュフラグメントの変化は、__`window.addEventListener('hashchange', handler)`__ で監視できます。
一方で、`next/router` の [router.event.on('hashChangeComplete', handler)](https://nextjs.org/docs/api-reference/next/router#routerevents) では変更監視できません。

### 再レンダリングされるケース

- `next/router` で `Router.push({ hash: 'Hello' })` した場合
- `next/link` の `<Link href="#Hello">Hello</Link>` のようなリンクをクリックした場合

これらのケースでは、現在表示されているページコンポーネントに再レンダリングのトリガーがかかります。
ハッシュフラグメントの変化は、`next/router` の `router.event.on('hashChangeComplete', handler)` で監視できます。
一方で、`window.addEventListener('hashchange', handler)` では変更監視できません。

ようするに、URL 末尾のハッシュフラグメント部分の変更と React コンポーネントのレンダリングを同期させたいときは、Next.js のアプリ内ルーティングを扱うための `next/router` や `next/link` を使ってハッシュフラグメント部分を変更しなければいけないということです。
その際に `hashchange` イベントが発生しないのは、Next.js のルーティングで内部的に `pushState` が使われていることが原因のようです。

仮に、Web ブラウザーのアドレスバーでハッシュフラグメント部分を直接変更した場合にも再レンダリングを発火させたいときは、例えば次のような感じで実装すれば全て連動させることができると思います。

- ハッシュフラグメントだけ変更するときは、`location.hash` を変更するか `<a href="#xxx">` のリンクをクリックさせる。
- 変更監視は `window.addEventListener('hashchange')` で行う。
- 変更を検出したら、`useState()` フックで生成したセッター `setXxx` を呼び出して、React コンポーネントの再レンダリングをかける。

ちょっと面倒だし、わかりにくいですね。。。
[URL のクエリパラメーター (router.query) による処理](/p/r7fou3a) で間に合うのであれば、素直にそちらを使った方がよさそうです。

{{< private >}}
下記はそれに対応したカスタムフックの実装例です。

ちゃんとハッシュフラグメントの変化だけを監視するのであれば、次のような感じで `hashchange` イベントをハンドルすべきかもしれません。

{{< code lang="ts" >}}
useEffect(() => {
  window.addEventListener('hashchange', onHashChange)
  return () => { window.removeEventListener('hashchange', onHashChange) }
}, [])
{{< /code >}}

{{< /private >}}

