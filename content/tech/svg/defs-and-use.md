---
title: "SVG の defs 要素で図形を定義して use で使い回す"
url: "p/wkrp4mf/"
date: "2023-10-15"
tags: ["SVG"]
---

defs と use の基本
----

SVG の [__`defs`__](https://developer.mozilla.org/ja/docs/Web/SVG/Element/defs) 要素内で定義した描画要素（`rect` や `path`、`g` など）は、[__`use`__](https://developer.mozilla.org/ja/docs/Web/SVG/Element/use) 要素を使って繰り返し参照することができます。
`defs` 以下に定義した図形は、`use` 要素で参照するまで描画されません。

次の例では、`defs` 要素内に `path` 要素を 1 つ定義し、それを 5 つの `use` 要素から参照しています。

<center>
<svg style="width: 300px; max-width: 100%; border: thin solid gray;" viewBox="0 0 200 60">
  <defs>
    <path id="tri" d="M0,-10 L9,5 -9,5 Z" fill="magenta" />
  </defs>
  <use href="#tri" transform="translate(20 30)" />
  <use href="#tri" transform="translate(50 30)" />
  <use href="#tri" transform="translate(85 30) scale(1.5)" />
  <use href="#tri" transform="translate(129 30) scale(2)" />
  <use href="#tri" transform="translate(170 30) rotate(90) scale(2)" />
</svg>
<div>図: defs と use による図形の使い回し</div>
</center>

```xml
<svg style="width: 300px;" viewBox="0 0 200 60">
  <defs>
    <path id="tri" d="M0,-10 L9,5 -9,5 Z" fill="magenta" />
  </defs>
  <use href="#tri" transform="translate(20 30)" />
  <use href="#tri" transform="translate(50 30)" />
  <use href="#tri" transform="translate(85 30) scale(1.5)" />
  <use href="#tri" transform="translate(129 30) scale(2)" />
  <use href="#tri" transform="translate(170 30) rotate(90) scale(2)" />
</svg>
```

`use` 要素には __`transform`__ 属性を指定できるので、上記のように、表示位置 (`translate`) や拡大率 (`scale`)、回転角度 (`rotate`) などの指定が可能です。


defs 以下の要素以外も参照可能
----

`use` で参照する描画要素は、通常は `defs` 要素内に定義したものを使いますが、通常の描画要素として配置した要素も参照することができます（オススメはしません）。

次の例では、`circle` 要素を使って丸を描画し、それを 2 つの `use` 要素でコピーしています。

<center>
<svg style="width: 200px; max-width: 100%; border: thin solid gray;" viewBox="0 0 115 55">
  <circle id="myCircle" cx="20" cy="20" r="10" fill="blue" />
  <use href="#myCircle" transform="translate(30)" />
  <use href="#myCircle" transform="translate(60) scale(1.5)" />
</svg>
<div>図: 描画済みの要素を use でコピーする</div>
</center>

```xml
<svg style="width: 200px;" viewBox="0 0 115 55">
  <circle id="myCircle" cx="20" cy="20" r="10" fill="blue" />
  <use href="#myCircle" transform="translate(30)" />
  <use href="#myCircle" transform="translate(60) scale(1.5)" />
</svg>
```

この場合、`use` での描画位置は、元の `circle` の座標値 (`cx`, `cy`) を起点にすることに注意してください。
つまり、1 つ目の `use` による丸の描画位置は、(20+30, 20+0) になります。
2 つ目の `use` による丸の拡大率 (`scale(1.5)`) は、もとの `circle` 要素の座標値 (`cx`, `cy`) にも効いてきます。

このように、座標値の設定された描画要素を `use` で参照する場合、描画位置の調整が煩雑になるため、通常は `defs` 要素内に定義された描画要素（描画位置は原点 (0,0) を基準）を参照することが推奨されます。

<center>
<svg style="width: 200px; max-width: 100%; border: thin solid gray;" viewBox="0 0 110 40">
  <defs>
    <circle id="myCircle2" r="10" fill="blue" />
  </defs>
  <use href="#myCircle2" transform="translate(20 20)" />
  <use href="#myCircle2" transform="translate(50 20)" />
  <use href="#myCircle2" transform="translate(85 20) scale(1.5)" />
</svg>
<div>図: 図形は defs 内で原点を基準に定義するのがよい</div>
</center>

```xml
<svg style="width: 200px;" viewBox="0 0 110 40">
  <defs>
    <circle id="myCircle2" r="10" fill="blue" />
  </defs>
  <use href="#myCircle2" transform="translate(20 20)" />
  <use href="#myCircle2" transform="translate(50 20)" />
  <use href="#myCircle2" transform="translate(85 20) scale(1.5)" />
</svg>
```


use 要素の x, y 属性と transform 属性の併用は避ける
----

`use` で描画する要素の座標は `transform` 属性で制御できますが、もっと単純に __`x`__、__`y`__ 属性で指定することもできます。

<center>
<svg style="width: 200px; max-width: 100%; border: thin solid gray;" viewBox="0 0 120 30">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" x="30" y="15" />
  <use href="#myRect" x="60" y="15" />
  <use href="#myRect" x="90" y="15" />
</svg>
<div>図: use 要素の x, y 属性による位置指定</div>
</center>

```xml
<svg style="width: 200px;" viewBox="0 0 120 30">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" x="30" y="15" />
  <use href="#myRect" x="60" y="15" />
  <use href="#myRect" x="90" y="15" />
</svg>
```

この `x`、`y` 属性による位置指定は、単純に座標値のみを指定すればよいケースではとてもシンプルです。
ただし、`transform` 属性を組み合わせて図形を変形させる場合は、座標値の制御が難しくなります。

下記は、20° 傾けた矩形を水平に並べようとして失敗した例です。

<center>
<svg style="width: 200px; max-width: 100%; border: thin solid gray;" viewBox="0 0 110 60">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" x="30" y="10" transform="rotate(20)" />
  <use href="#myRect" x="60" y="10" transform="rotate(20)" />
  <use href="#myRect" x="90" y="10" transform="rotate(20)" />
</svg>
<div>図: x, y 属性と transform 属性を組み合わせて使った場合</div>
</center>

```xml
<svg style="width: 200px;" viewBox="0 0 110 60">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" x="30" y="10" transform="rotate(20)" />
  <use href="#myRect" x="60" y="10" transform="rotate(20)" />
  <use href="#myRect" x="90" y="10" transform="rotate(20)" />
</svg>
```

図形の変形を伴う場合は、次のように `transform` 属性だけで簡潔させると直感的な配置が可能です。

<center>
<svg style="width: 200px; max-width: 100%; border: thin solid gray;" viewBox="0 0 120 40">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" transform="translate(30 20) rotate(20)" />
  <use href="#myRect" transform="translate(60 20) rotate(20)" />
  <use href="#myRect" transform="translate(90 20) rotate(20)" />
</svg>
<div>図: transform 属性だけで位置と回転角度を指定した場合</div>
</center>

```xml
<svg style="width: 200px;" viewBox="0 0 120 40">
  <defs>
    <rect id="myRect" x="-10" y="-5" width="20" height="10"
        fill="none" stroke="green" />
  </defs>
  <use href="#myRect" transform="translate(30 20) rotate(20)" />
  <use href="#myRect" transform="translate(60 20) rotate(20)" />
  <use href="#myRect" transform="translate(90 20) rotate(20)" />
</svg>
```

