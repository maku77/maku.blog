---
title: "Next.js でコンポーネント内に直接 CSS を記述する (styled-jsx)"
linkTitle: "コンポーネント内に直接 CSS を記述する (styled-jsx)"
url: "/p/pbp2dpy"
date: "2021-05-04"
tags: ["Next.js"]
weight: 4
---

Next.js は CSS-in-JS ライブラリの一種である [styled-jsx](https://www.npmjs.com/package/styled-jsx) をデフォルトでサポートしています（`styled-jsx` も `Next.js` と同様に Vercel がメンテナンスしています）。
React コンポーネントの定義の中に、次のように CSS を直接記述することができます。

{{< code lang="tsx" >}}
<style jsx>{`
  ...
`}</style>
{{< /code >}}

下記は実際に styled-jsx を使って `p` 要素をスタイル設定した例です。

{{< code lang="tsx" title="pages/sample.tsx" >}}
export default () => <>
  <p>only this paragraph will get the style</p>
  <style jsx>{`
    p {
      color: red;
    }
  `}</style>
</>
{{< /code >}}

ここで設定したスタイルのスコープは、この React コンポーネントに閉じたものになるため、他の React コンポーネントに影響を与えてしまう心配がありません。
そのため、シンプルなタグ名やクラス名でスタイル設定を行うことができます。

Next.js は、外部ファイルとして作成した `.css` や `scss` ファイルを読み込む [CSS Modules の機能](/p/seq2cmw) もデフォルトでサポートしています（その場合は `*.module.css` というファイル名にするというルールがあります）。

