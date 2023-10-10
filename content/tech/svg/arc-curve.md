---
title: "SVG の path 要素による円弧の描画例（A コマンド）"
url: "p/rpp2z3w/"
date: "2023-10-11"
tags: ["SVG"]
---

A コマンドのパラメーター
----

SVG の __`path`__ 要素の __`d`__ 属性に __`A`__/__`a`__ コマンドを指定すると、様々な形状の円弧（楕円の一部）を描画することができます（参考: [path 要素のチートシート](/p/kq6gwfn/#path)）。
パラメーターが多くて分かりにくいのでまとめておきます。

<center>
  <svg width="160" viewBox="-20 -65 140 85" style="border: thin solid gray">
    <path d="M0,0 A50 50 0 0 1 100 0" stroke="black" fill="none" />
  </svg>
  <p>図: path 要素による円弧の描画</p>
</center>

{{< code lang="xml" title="具体例" >}}
<svg width="200" viewBox="-20 -65 140 85">
  <path d="M0,0 A50 50 0 0 1 100 0" stroke="black" fill="none" />
</svg>
{{< /code >}}

`A`/`a` コマンドは、円弧の始点として「現在の座標」を使うので、通常は上記のように、先行する `M` コマンドなどで始点座標を移動しておきします。
円弧の線だけを表示するには、`stroke` に線の色を指定し、`fill` に `none`（塗りつぶしなし）を指定しておく必要があります。

`A` コマンドには次の 7 つの数値パラメーターを指定します。
`a` コマンドの場合は、最後の終点座標 (`x`, `y`) の指定が、相対位置 (`dx`, `dy`) の指定に変わります。

{{< code lang="html" title="A コマンドのパラメーター" >}}
<rx> <ry> <x-axis-rotation> <large-arc-flag> <sweep-flag> <x> <y>
{{< /code >}}

| パラメーター | 説明 |
| ---- | ---- |
| `rx` | 円弧の X 軸半径 |
| `ry` | 円弧の Y 軸半径 |
| `x-axis-rotation` | X 軸からの傾き (radian) |
| `large-arch-flag` | `1`: 大きな弧の部分を描画<br/>`0`: 小さな弧の部分を描画 |
| `sweep-flag` | `1`: 時計回り (clockwise) になる弧の部分を描画<br/>`0`: 反時計回り (counterclockwise) になる弧の部分を描画 |
| `x` (`dx`) | 円弧の終点 X 座標（`A` コマンドなら絶対座標、`a` コマンドなら相対座標） |
| `y` (`dy`) | 円弧の終点 Y 座標（`A` コマンドなら絶対座標、`a` コマンドなら相対座標） |

下記のギャラリーを見ると、具体的な指定方法を理解できると思います。


ギャラリー
----

### X軸半径とY軸半径

<table>
<tr><td>
  <svg width="200" viewBox="-20 -40 200 60" style="border: 3px double gray;">
    <path d="M0,0 A50 25 0 0 1 100 0" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(100 0)">
      <circle r="6" fill="cyan" />
      <text x="10" y="5">(100, 0)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A<b style="color:red">50 25</b> 0 0 0 100 0"</code>
  <p>rx=50, ry=25</p>
</td></tr>
<tr><td>
  <svg width="200" viewBox="-20 -65 200 85" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 0 1 100 0" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(100 0)">
      <circle r="6" fill="cyan" />
      <text x="10" y="5">(100, 0)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A<b style="color:red">50 50</b> 0 0 0 100 0"</code>
  <p>rx=50, ry=50</p>
</td></tr>
<tr><td>
  <svg width="200" viewBox="-20 -90 200 110" style="border: 3px double gray;">
    <path d="M0,0 A50 75 0 0 1 100 0" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(100 0)">
      <circle r="6" fill="cyan" />
      <text x="10" y="5">(100, 0)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A<b style="color:red">50 75</b> 0 0 0 100 0"</code>
  <p>rx=50, ry=75</p>
</td></tr>
</table>

### 時計回りと反時計回り

<table>
<tr><td>
  <svg width="200" viewBox="-20 -60 200 80" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 0 1 100 0" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(100 0)">
      <circle r="6" fill="cyan" />
      <text x="10" y="5">(100, 0)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 0 <b style="color:red">1</b> 100 0"</code>
  <p>sweep-flag=1（時計回りで終点に向かう）</p>
</td></tr>
<tr><td>
  <svg width="200" viewBox="-20 -20 200 80" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 0 0 100 0" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(100 0)">
      <circle r="6" fill="cyan" />
      <text x="10" y="5">(100, 0)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 0 <b style="color:red">0</b> 100 0"</code>
  <p>sweep-flag=0（反時計回りで終点に向かう）</p>
</td></tr>
</table>


### 大きい弧と小さい弧

<table>
<tr><td>
  <svg width="150" viewBox="-60 -70 170 180" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 1 1 50 50" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(50 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="25">(50, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 <b style="color:red">1 1</b> 50 50"</code>
  <p>大きい弧 (large-arch-flag=1)</p>
  <p>時計回り (sweep-flag=1)</p>
</td></tr>
<tr><td>
  <svg width="150" viewBox="-60 -70 170 180" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 0 0 50 50" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(50 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="25">(50, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 <b style="color:red">0 0</b> 50 50"</code>
  <p>小さい弧 (large-arch-flag=0)</p>
  <p>反時計回り (sweep-flag=0)</p>
</td></tr>
<tr><td>
  <svg width="150" viewBox="-60 -70 170 180" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 1 0 50 50" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="10" y="5">(0, 0)</text>
    </g>
    <g transform="translate(50 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="-15">(50, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 <b style="color:red">1 0</b> 50 50"</code>
  <p>大きい弧 (large-arch-flag=1)</p>
  <p>反時計回り (sweep-flag=0)</p>
</td></tr>
<tr><td>
  <svg width="150" viewBox="-60 -70 170 180" style="border: 3px double gray;">
    <path d="M0,0 A50 50 0 0 1 50 50" stroke="black" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="-10" y="-15">(0, 0)</text>
    </g>
    <g transform="translate(50 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="25">(50, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A50 50 0 <b style="color:red">0 1</b> 50 50"</code>
  <p>小さい弧 (large-arch-flag=0)</p>
  <p>時計回り (sweep-flag=1)</p>
</td></tr>
</table>

### X軸からの傾き

<table>
<tr><td>
  <svg width="150" viewBox="-85 -40 170 130" style="border: 3px double gray;">
    <path d="M0,0 A60 25 0 1 1 0 50" stroke="black" fill="none" />
    <path d="M0,0 A60 25 0 0 0 0 50" stroke="lightgray" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="-10" y="5" text-anchor="end">(0, 0)</text>
    </g>
    <g transform="translate(0 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="5" text-anchor="end">(0, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A60 25 <b style="color:red">0</b> 1 0 0 50"</code>
  <p>x-axis-rotation=0</p>
</td></tr>
<tr><td>
  <svg width="150" viewBox="-85 -40 170 130" style="border: 3px double gray;">
    <path d="M0,0 A60 25 -30 1 1 0 50" stroke="black" fill="none" />
    <path d="M0,0 A60 25 -30 0 0 0 50" stroke="lightgray" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="-10" y="5" text-anchor="end">(0, 0)</text>
    </g>
    <g transform="translate(0 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="5" text-anchor="end">(0, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A60 25 <b style="color:red">-30</b> 1 0 0 50"</code>
  <p>x-axis-rotation=-30</p>
</td></tr>
<tr><td>
  <svg width="150" viewBox="-85 -40 170 130" style="border: 3px double gray;">
    <path d="M0,0 A60 25 30 1 1 0 50" stroke="black" fill="none" />
    <path d="M0,0 A60 25 30 0 0 0 50" stroke="lightgray" fill="none" />
    <g transform="translate(0 0)">
      <circle r="6" fill="blue" />
      <text x="-10" y="5" text-anchor="end">(0, 0)</text>
    </g>
    <g transform="translate(0 50)">
      <circle r="6" fill="cyan" />
      <text x="-10" y="5" text-anchor="end">(0, 50)</text>
    </g>
  </svg>
</td><td>
  <code>d="M0,0 A60 25 <b style="color:red">30</b> 1 0 0 50"</code>
  <p>x-axis-rotation=30</p>
</td></tr>
</table>

