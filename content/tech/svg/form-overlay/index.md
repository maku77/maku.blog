---
title: "SVG 要素の上に別の HTML 要素を重ねて表示する"
url: "p/fmxdrcp/"
date: "2023-10-04"
tags: ["SVG"]
---

多数のデータを `svg` 要素を使用してプロットする場合、ブラウザの画面全体に `svg` 要素を広げて表示したいケースがあります。
このような場合、別の HTML 要素（ページタイトルや操作パネルなど）は、`svg` 要素の上に重ねて表示する必要があります。
ここでは、その方法として、`svg` 要素を背景として表示する方法と、他の要素を相対的に配置する方法の 2 つを紹介します。


svg 要素を背景として表示する方法 (position: fixed)
----

<center>
  <iframe width="260" height="80" src="./demo.html" style="resize: both; overflow: auto;"></iframe>
  <div>デモ1（<a target="_blank" href="./demo.html">別ページで開く</a>）</div>
</center>

この例では、`svg` 要素を __`position: fixed`__ スタイルを用いて、画面全体に固定表示しています。
通常、`position: fixed` スタイルは、ウェブサイトのヘッダーを画面上部に固定する用途などで使用されますが、これを `svg` 要素に適用することで、背景画像のような効果を得ることができます。
`svg` 以外の要素は、まるで `svg` 要素が存在しないかのようにレイアウトされます。
`position: fixed` スタイルが適用された `svg` 要素は、デフォルトでは他の要素よりも前面に表示されてしまうため、これを背面に配置するために __`z-index: -1`__ を指定しています。

{{< code lang="html" title="HTML" >}}
<svg>
  <circle cx="40" cy="40" r="25" fill="royalblue" />
  <circle cx="100" cy="40" r="25" fill="indigo" />
  <circle cx="160" cy="40" r="25" fill="darkslateblue" />
  <circle cx="220" cy="40" r="25" fill="darkmagenta" />
</svg>
<p>
  この p 要素の後ろに、svg 要素がページ全体に表示されています。
</p>
{{< /code >}}

{{< code lang="css" title="CSS" hl_lines="7 13" >}}
* {
  margin: 0;
  box-sizing: border-box;
}

svg {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background: midnightblue;
  z-index: -1;  /* 他の要素より奥に表示 */
}
{{< /code >}}


他の要素を親要素の相対座標で配置する方法 (position: absolute;)
----

<center>
  <iframe width="360" height="135" src="./demo2.html" style="resize: both; overflow: auto;"></iframe>
  <div>デモ2（<a target="_blank" href="./demo2.html">別ページで開く</a>）</div>
</center>

__`position: absolute`__ スタイルを指定すると、親要素の位置からの相対座標で表示できるので、これを利用して `svg` 要素の上に他の要素を重ねて表示することができます。
子要素に `position: absolute` を指定する場合は、親要素にも何らかの `position` 指定（下記例では `position: relative`）が必要なことに注意してください。

- 参考: [CSS の position プロパティによるレイアウト方法を理解する - まくまく HTML/CSS ノート](https://maku77.github.io/p/3dtq7o5/)


{{< code lang="html" title="HTML" >}}
<p>これは svg の外側に配置する要素。</p>

<div id="container">
  <svg>
    <circle cx="40" cy="40" r="25" fill="royalblue" />
    <circle cx="100" cy="40" r="25" fill="indigo" />
    <circle cx="160" cy="40" r="25" fill="darkslateblue" />
    <circle cx="220" cy="40" r="25" fill="darkmagenta" />
  </svg>
  <div id="overlay">
    <p>この裏に svg 要素が表示されています。</p>
  </div>
</div>

<p>これも svg の外側に配置する要素。</p>
{{< /code >}}

{{< code lang="css" title="CSS" hl_lines="2 15" >}}
#container {
  position: relative;  /* 子要素で absolute 指定するため */
  width: calc(100vw - 3rem);
  height: 80px;
  margin: auto;
}

svg {
  width: 100%;
  height: 100%;
  background: midnightblue;
}

#overlay {
  position: absolute;  /* 親要素を基準にして配置する */
  top: 0;
  left: 0;
  color: #ddd;
}
{{< /code >}}


（おまけ）svg 内に foreignObject を埋め込む方法
----

`svg` 要素内に __`foreignObject`__ 要素を配置することで、HTML の `p` 要素などを配置できます。

{{< code lang="html" hl_lines="7-11" >}}
<svg style="background: darkblue;">
  <circle cx="40" cy="40" r="25" fill="royalblue" />
  <circle cx="100" cy="40" r="25" fill="indigo" />
  <circle cx="160" cy="40" r="25" fill="darkslateblue" />
  <circle cx="220" cy="40" r="25" fill="darkmagenta" />

  <foreignObject x="0" y="0" width="100%" height="100%">
    <p xmlns="http://www.w3.org/1999/xhtml">
      この p 要素は svg 要素内に配置されています。
    </p>
  </foreignObject>
</svg>
{{< /code >}}

ただし、`foreignObject` 内に配置された HTML 要素は、外部の HTML 要素とは異なるレンダリングコンテキストで扱われるため、異なる見た目になる可能性があることに注意してください。


（おまけ）かっこいい透過コントロールパネル
----

`svg` 要素の上にコントロールパネルなどを表示する場合、アルファ値を設定した背景色でパネル表示するとかっこいい感じになります。

<center>
  <iframe width="270" height="150" src="./demo3.html" style="resize: both; overflow: auto; border: none;"></iframe>
  <div>デモ3（<a target="_blank" href="./demo3.html">別ページで開く</a>）</div>
</center>

