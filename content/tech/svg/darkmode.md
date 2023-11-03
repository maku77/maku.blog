---
title: "SVG 画像をダークモードに対応させる"
url: "p/shr62rs/"
date: "2023-11-01"
tags: ["SVG"]
---

何をするか
----

SVG 内に `style` 要素を埋め込んで、ブラウザのダークモードの設定 (ON/OFF) に連動して各描画要素の色を変更する方法を説明します。

<div style="text-align: center">
  <svg id="div-tds6j4y" style="width: 300px; max-width: 100%; border: thin solid gray;" viewBox="0 0 93 43">
    <rect id="background" x="0" y="0" width="100%" height="100%" fill="#eee" />
    <circle cx="20" cy="15" r="10" fill="blue" />
    <path d="M45,5 l11.5,20 -23,0 Z" fill="red" />
    <rect x="62" y="5" width="20" height="20" fill="green" />
    <text x="5" y="38" font-size="8pt">DarkMode:</text>
    <text id="text-on" x="65" y="38" font-size="8pt">ON</text>
    <text id="text-off" x="65" y="38" font-size="8pt">OFF</text>
    <style>
      @media(prefers-color-scheme: light) {
        #div-tds6j4y {
          & #text-on { display: none; }
        }
      }
      @media(prefers-color-scheme: dark) {
        #div-tds6j4y {
          & #text-off { display: none; }
          & #background { fill: darkblue; }
          & circle { fill: lightblue; }
          & path { fill: pink; }
          & rect { fill: #bbffbb; }
          & text { fill: white; }
        }
      }
    </style>
  </svg>
  <div>図: ダークモードに対応した SVG</div>
</div>

上記の SVG 要素では、ブラウザのダークモード設定に応じて、各描画要素の色 (`fill`) を切り替えるようにしています。
さらに、現在のダークモード設定を示す `ON`/`OFF` というテキストの表示・非表示も切り替えています。

（ちなみに、この Web サイト『まくろぐ』全体は、ダークモード設定によって配色が変わるようにはなってません）


CSS のメディアクエリでダークモード用スタイルを定義する
----

CSS のメディアクエリ (__`@media`__) を使うと、ブラウザのダークモード設定に応じたスタイル定義を行うことができます。

{{< code lang="css" title="CSS のメディアクエリの使い方" >}}
/* ここに共通のスタイル定義 */

@media(prefers-color-scheme: light) {
  /* ここにライトモード時のスタイル定義 */
}

@media(prefers-color-scheme: dark) {
  /* ここにダークモード時のスタイル定義 */
}
{{< /code >}}

SVG には `style` 要素を埋め込むことができるので、そこでメディアクエリを使えば、ダークモード設定に連動する SVG を作成することができます。

{{< code lang="xml" title="SVG に style 設定を埋め込む例" >}}
<svg width="100" viewBox="0 0 40 30">
  <circle cx="20" cy="15" r="10" fill="blue" />
  <style>
    @media(prefers-color-scheme: dark) {
      /* ブラウザのダークモードが ON のときは赤色にする */
      circle { fill: red; }
    }
  </style>
</svg>
{{< /code >}}


ダークモードに対応した SVG 要素の例
----

最初に表示した SVG は、次のようなコードで構成されています。
`svg` 要素内に、メディアクエリを含んだ `style` 要素が埋め込まれているところがポイントです。

{{< code lang="xml" hl_lines="9-21" >}}
<svg style="width: 300px; max-width: 100%; border: thin solid gray;" viewBox="0 0 93 43">
  <rect id="background" x="0" y="0" width="100%" height="100%" fill="#eee" />
  <circle cx="20" cy="15" r="10" fill="blue" />
  <path d="M45,5 l11.5,20 -23,0 Z" fill="red" />
  <rect x="62" y="5" width="20" height="20" fill="green" />
  <text x="5" y="38" font-size="8pt">DarkMode:</text>
  <text id="text-on" x="65" y="38" font-size="8pt">ON</text>
  <text id="text-off" x="65" y="38" font-size="8pt">OFF</text>
  <style>
    @media(prefers-color-scheme: light) {
      #text-on { display: none; }
    }
    @media(prefers-color-scheme: dark) {
      #text-off { display: none; }
      #background { fill: darkblue; }
      circle { fill: lightblue; }
      path { fill: pink; }
      rect { fill: #bbffbb; }
      text { fill: white; }
    }
  </style>
</svg>
{{< /code >}}


（おまけ）JavaScript でダークモードの変更を検出する方法
----

ブラウザ上で動く JavaScript 内で現在のダークモード設定を参照したり、変更を検出するには次のようにします。

{{< code lang="js" title="JavaScript 内でダークモード設定を扱う" >}}
function handleDarkMode(e) {
  if (e.matches) {
    alert("ダークモードは ON です");
  } else {
    alert("ダークモードは OFF です");
  }
}

const mediaQuery = window.matchMedia("(prefers-color-scheme: dark)");
mediaQuery.addListener(handleDarkMode); // ダークモードの変更を検出
handleDarkMode(mediaQuery); // ページロード時に初期状態を表示
{{< /code >}}

