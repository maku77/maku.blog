---
title: "SVG の marker 要素で矢印（マーカー）を描画する"
url: "p/hoiytvm/"
date: "2023-09-25"
tags: ["SVG"]
---

SVG の __`marker`__ 要素を使うと、`line` 要素などの線の先端に表示するマーカー形状を定義することができます。
多くの場合、矢印を表す三角形を使いますが、丸や四角など、任意の形状のマーカーを定義できます。

marker による矢印描画の基本
----

### 実装例

次の例では、`line` 要素の先端に `marker` で定義した三角形を割り当てています。
マーカー形状を分かりやすくするために、半透明の赤色で表示しています（通常は `line` の色と合わせます）。

<center>
<svg width="480" height="140" viewBox="0 0 240 70"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="arrow" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="16" markerHeight="16" orient="auto-start-reverse">
      <path d="M0,0 L10,5 L0,10 Z" fill="red" opacity="0.5" />
    </marker>
  </defs>
  <line x1="20" y1="20" x2="100" y2="20" stroke="green" stroke-width="1"
      marker-end="url(#arrow)" />
  <line x1="80" y1="50" x2="160" y2="20" stroke="red" stroke-width="1"
      marker-start="url(#arrow)" marker-end="url(#arrow)" />
  <line x1="140" y1="50" x2="220" y2="50" stroke="blue" stroke-width="1"
      marker-start="url(#arrow)" />
</svg>
<div>図: marker 要素による矢印の描画</div>
</center>

{{< code lang="html" title="svg のコード" >}}
<svg width="480" height="140" viewBox="0 0 240 70"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <!-- marker で矢印の形を定義する -->
  <defs>
    <marker id="arrow" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="16" markerHeight="16" orient="auto-start-reverse">
      <path d="M0,0 L10,5 L0,10 Z" fill="red" opacity="0.5" />
    </marker>
  </defs>
  <!-- marker を割り当てる -->
  <line x1="20" y1="20" x2="100" y2="20" stroke="green" stroke-width="1"
      marker-end="url(#arrow)" />
  <line x1="80" y1="50" x2="160" y2="20" stroke="red" stroke-width="1"
      marker-start="url(#arrow)" marker-end="url(#arrow)" />
  <line x1="140" y1="50" x2="220" y2="50" stroke="blue" stroke-width="1"
      marker-start="url(#arrow)" />
</svg>
{{< /code >}}

### 解説

#### marker で矢印の形を定義する

```html
<defs>
  <marker id="arrow" viewBox="0 0 10 10" refX="5" refY="5"
      markerWidth="16" markerHeight="16" orient="auto-start-reverse">
    <path d="M0,0 L10,5 L0,10 Z" fill="red" opacity="0.5" />
  </marker>
</defs>
```

`marker` 要素は __`defs`__ 要素の子要素として定義し、他の要素（`line` など）から参照できるようにしておきます。
それぞれの属性は次のような意味を持っています。

- __`id="arrow"`__
  - この `marker` 要素を参照するときに指定する一意の ID です。
- __`viewBox="0 0 10 10"`__
  - `marker` の形状を定義するときのユーザー座標系です。この例の場合は、10x10 のサイズのキャンバスがあるものとして `path` をレイアウトします。
- __`refX="5" refY="5"`__
  - 線の先端を `viewBox` 内のどの座標にアタッチするかを指定します。この例の場合は、`viewBox` のちょうど中央 (5, 5) に線の先端がくるように配置されます。
- __`markerWidth="16" markerHeight="16"`__
  - この `marker` の表示サイズです。実際の表示サイズは、`viewBox` ではなく、こちらの `markerWidth` と `markerHeight` で決まります。デフォルトは `3` なので、とても小さいです。
- __`orient="auto-start-reverse"`__
  - これを指定しておくと、`marker` を `line` の始点にアタッチ (`marker-start`) したときに、矢印を逆向きに表示してくれます。常に終点方向を向く矢印を表示したいときは、__`orient="auto"`__ を指定します。何も指定しないと、マーカーは定義通りの右向きのままで表示されます。

`marker` の子要素の `path` 要素では、次のような __右向き__ の矢印形状を定義しています。
右向きの形状を定義することは、歴史的な経緯により SVG の仕様で決まっているのですが、0 度という角度が通常は右方向を表すからと考えればしっくり来ます。

<center>
<svg width="50" height="50" viewBox="0 0 10 10"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
    <path d="M0,0 L10,5 L0,10 Z" fill="red" opacity="0.5" />
</svg>
</center>

#### marker を割り当てる

```html
<line x1="20" y1="20" x2="100" y2="20" stroke="green" stroke-width="1"
    marker-end="url(#arrow)" />
<line x1="80" y1="50" x2="160" y2="20" stroke="red" stroke-width="1"
    marker-start="url(#arrow)" marker-end="url(#arrow)" />
<line x1="140" y1="50" x2="220" y2="50" stroke="blue" stroke-width="1"
    marker-start="url(#arrow)" />
```

`marker` 要素で定義した矢印の先端は、線を描画する要素（`line`、`path`、`polyline`、`polygon` 要素）の次のような属性でアタッチします。

- __`marker-start="url(#arrow)"`__ ... 線の始点にマーカー (`id="arrow"`) を表示します。
- __`marker-end="url(#arrow)"`__ ... 線の終点にマーカー (`id="arrow"`) を表示します。
- __`marker-mid="url(#arrow)"`__ ... `path` や `polyline`、`polygon` 要素で複数の直線や曲線を繋げた場合に、各線の接続部分に表示します。

ちなみに、`marker` は CSS でも割り当て可能です。
多数の要素に同一の `marker` を割り当てたいときはこの方法が便利です。

```html
<style>
  line {
    marker: url(#arrow);
  }
</style>
```


（応用）線の接続部分にマーカーを表示する (marker-mid)
----

SVG で複数の直線や曲線を繋げた複雑な線を描画するときは、`polyline` や `path` 要素を使います。
このとき、各線の接続点にマーカーを表示するには、__`marker-mid`__ 属性を使ってマーカーを割り当てます。

### 例: polyline 要素の接続点にマーカーを表示する

<center>
<svg width="240" height="100" viewBox="0 0 120 50"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="dot" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="8" markerHeight="8">
      <circle cx="5" cy="5" r="4" fill="white" stroke="black" />
    </marker>
  </defs>
  <polyline fill="none" stroke="black" marker-mid="url(#dot)"
      points="10,40 30,10 50,30 70,20 90,40 110,10" />
</svg>
<div>図: polyline の各線の接続点にマーカーを表示</div>
</center>

```html
<svg width="240" height="100" viewBox="0 0 120 50"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="dot" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="8" markerHeight="8">
      <circle cx="5" cy="5" r="4" fill="white" stroke="black" />
    </marker>
  </defs>
  <polyline fill="none" stroke="black" marker-mid="url(#dot)"
      points="10,40 30,10 50,30 70,20 90,40 110,10" />
</svg>
```

### 例: path 要素の接続点にマーカーを表示する

<center>
<svg width="200" height="100" viewBox="0 0 100 50"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="direction" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="8" markerHeight="8" orient="auto">
      <polygon points="0,0 10,5 0,10 2,5" fill="black" />
    </marker>
  </defs>
  <path fill="none" stroke="black" marker-mid="url(#direction)"
      d="M10,40 Q50,40 50,25 T90,10" />
</svg>
<div>図: path の各線の接続点にマーカーを表示</div>
</center>

```html
<svg width="200" height="100" viewBox="0 0 100 50"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="direction" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="8" markerHeight="8" orient="auto">
      <polygon points="0,0 10,5 0,10 2,5" fill="black" />
    </marker>
  </defs>
  <path fill="none" stroke="black" marker-mid="url(#direction)"
      d="M10,40 Q50,40 50,25 T90,10" />
</svg>
```

接続点のマーカー (`marker-mid`) は、あくまで 2 つの線の接続部分に表示されることに注意してください。
1 つのベジェ曲線だけで構成された線には、マーカーは表示されません。
上記の例では、2 つのベジェ曲線の接続部分にマーカーが表示されています。

- 参考: [SVG の path 要素によるベジェ曲線の描画例](/p/ka6xumg/)


（応用）stroke-width に応じて marker は拡大される
----

`marker` の表示サイズは、`markerWidth` および `markerHeight` 属性で設定しますが、この `marker` をアタッチする `line` 要素の `strokeWidth` を `1` よりも大きくすると、その太さに比例して実際の表示サイズが拡大されます。
これは、__`markerUnits`__ 属性のデフォルト値が __`"strokeWidth"`__（`stroke-width` の値を基準単位とする）に設定されているからです。

<center>
<svg width="210" height="120" viewBox="0 0 105 60"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="MyArrow" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="5" markerHeight="5" orient="auto-start-reverse">
      <path d="M0,0 L10,5 L0,10 Z" />
    </marker>
  </defs>
  <line x1="20" y1="10" x2="80" y2="10" stroke="green" stroke-width="1"
      marker-end="url(#MyArrow)" />
  <line x1="20" y1="25" x2="80" y2="25" stroke="red" stroke-width="2"
      marker-end="url(#MyArrow)" />
  <line x1="20" y1="45" x2="80" y2="45" stroke="blue" stroke-width="3"
      marker-end="url(#MyArrow)" />
</svg>
<div>図: 矢印の先端が stroke-width に比例して大きくなる</div>
</center>

```html
<svg width="200" height="120" viewBox="0 0 100 60"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="MyArrow" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="5" markerHeight="5" orient="auto-start-reverse">
      <path d="M0,0 L10,5 L0,10 Z" />
    </marker>
  </defs>
  <line x1="20" y1="10" x2="80" y2="10" stroke="green" stroke-width="1"
      marker-end="url(#MyArrow)" />
  <line x1="20" y1="25" x2="80" y2="25" stroke="red" stroke-width="2"
      marker-end="url(#MyArrow)" />
  <line x1="20" y1="45" x2="80" y2="45" stroke="blue" stroke-width="3"
      marker-end="url(#MyArrow)" />
</svg>
```

この振る舞いを抑制するには、`markerUnits` 属性に __`"userSpaceOnUse"`__ を指定します。

{{< code lang="html" hl_lines="3" >}}
<marker id="MyArrow" viewBox="0 0 10 10" refX="5" refY="5"
    markerWidth="5" markerHeight="5" orient="auto-start-reverse"
    markerUnits="userSpaceOnUse">
{{< /code >}}

<center>
<svg width="210" height="100" viewBox="0 0 105 50"
    xmlns="http://www.w3.org/2000/svg" style="border: thin solid gray">
  <defs>
    <marker id="MyArrow-2" viewBox="0 0 10 10" refX="5" refY="5"
        markerWidth="5" markerHeight="5" orient="auto-start-reverse"
        markerUnits="userSpaceOnUse">
      <path d="M0,0 L10,5 L0,10 Z" />
    </marker>
  </defs>
  <line x1="20" y1="10" x2="80" y2="10" stroke="green" stroke-width="1"
      marker-end="url(#MyArrow-2)" />
  <line x1="20" y1="25" x2="80" y2="25" stroke="red" stroke-width="2"
      marker-end="url(#MyArrow-2)" />
  <line x1="20" y1="40" x2="80" y2="40" stroke="blue" stroke-width="3"
      marker-end="url(#MyArrow-2)" />
</svg>
<div>図: stroke-width に依存しないようにした矢印の先端サイズ</div>
</center>

基本的にはマーカーサイズは `stroke-width` に連動させておいた方が見た目はよさそうです。

{{% private %}}
- https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/markerUnits
{{% /private %}}

