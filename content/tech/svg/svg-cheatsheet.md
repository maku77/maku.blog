---
title: "SVG チートシート"
url: "p/kq6gwfn/"
date: "2023-09-09"
tags: ["cheatsheet", "SVG"]
---

<style>
  svg { box-sizing: content-box; }
</style>

`svg` 要素内に配置できる描画要素のまとめです。
座標値のデフォルトの単位はピクセル (`px`) ですが、`svg` 要素に [`viewBox`](/p/563uibg/) が設定されている場合は `viewBox` 内の座標系で座標値を指定します。


text 要素（テキスト）
----

{{< code lang="html" title="text 要素の例" >}}
<text x="20" y="55" font-size="20" fill="red">
  表示するテキスト
</text>
{{< /code >}}

<center>
  <svg width="200" height="100" style="border: 3px double gray">
    <text x="20" y="55" font-size="20" fill="red">
      表示するテキスト
    </text>
  </svg>
</center>

| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `x` | テキストアンカーポイントの X 座標 | `100`, `50%` |
| `y` | テキストアンカーポイントの Y 座標 | `100`, `50%` |
| [`text-anchor`](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor) | 水平方向のアンカーポイント | * `start` ... テキスト先頭（ltr 言語なら左端）<br>* `middle` ... 中央<br/>* `end` ... テキスト末尾（ltr 言語なら右端） |
| [`dominant-baseline`](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline) | 垂直方向のベースライン | * `auto` ... 通常は下端<br/>* `middle` ... 中央<br/>* `hanging` ... 通常は上端 |
| `font-size` | フォントサイズ | `30` |
| `font-weight` | 文字の太さ | `600` |
| `fill` | テキスト内部の色 | `red`, <code style="white-space:nowrap">rgb(255, 0, 0)</code>,<code style="white-space:nowrap">rgba(255, 0, 0, 0.7)</code> `#ff0033` |
| `fill-opacity` | テキスト内部の色の透明度。`fill` 属性で `rgba` を使って透明度を指定することも可能。 | `0`（完全透過）〜 `1.0`（非透過） |
| `stroke` | テキスト周囲の線の色 | `red` |
| `stroke-width` | テキスト周囲の線の太さ | `2` |
| `stroke-opacity` | テキスト周囲の線の透明度。`stroke` 属性で `rgba` を使って透明度を指定することも可能。 | `0`（完全透過）〜 `1.0`（非透過） |
| [`text-decoration`](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-decoration) | テキストの装飾 | * `none` ... デフォルト<br/>* `underline` ... 下線<br/>* `line-through` ... 取消線 |
| `opacity` | 要素全体の透明度 | `0`（完全透過）〜 `1.0`（非透過） |

### text 要素のメモ

- `viewBox` 内で水平方向に中央寄せする ... `x="50%" text-anchor="middle"`
- `viewBox` 内で垂直方向に中央寄せする ... `y="50%" dominant-baseline="middle"`
- 部分テキストの装飾 ... `text` 要素の子要素として [`tspan`](https://developer.mozilla.org/en-US/docs/Web/SVG/Element/tspan) を配置すると、部分テキストごとにスタイルや位置を設定できます。

{{% private %}}
- [MDN - text element](https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text)
- [SVG- text element cheatsheet](https://codepen.io/tripti1410/pen/WBmMOB)
{{% /private %}}


line 要素（直線）
----

{{< code lang="html" title="line 要素の例" >}}
<line x1="30" y1="20" x2="170" y2="20" stroke="red" stroke-width="10" />
<line x1="30" y1="40" x2="170" y2="40" stroke="red" stroke-width="10" stroke-linecap="square" />
<line x1="30" y1="60" x2="170" y2="60" stroke="red" stroke-width="10" stroke-linecap="round" />
<line x1="30" y1="80" x2="170" y2="80" stroke="blue" stroke-width="3" stroke-dasharray="5,5" />
<line x1="30" y1="100" x2="170" y2="100" stroke="blue" stroke-width="3" stroke-dasharray="5,10,5" />
{{< /code >}}

<center>
  <svg width="200" height="120" style="border: 3px double gray">
    <line x1="30" y1="20" x2="170" y2="20" stroke="red" stroke-width="10" />
    <line x1="30" y1="40" x2="170" y2="40" stroke="red" stroke-width="10" stroke-linecap="square" />
    <line x1="30" y1="60" x2="170" y2="60" stroke="red" stroke-width="10" stroke-linecap="round" />
    <line x1="30" y1="80" x2="170" y2="80" stroke="blue" stroke-width="3" stroke-dasharray="5,5" />
    <line x1="30" y1="100" x2="170" y2="100" stroke="blue" stroke-width="3" stroke-dasharray="5,10,5" />
  </svg>
</center>

| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `x1` | 線の始点 X 座標 | `30`, `10%` |
| `y1` | 線の始点 Y 座標 | `70`, `20%` |
| `x2` | 線の終点 X 座標 | `170`, `100%` |
| `y2` | 線の終点 Y 座標 | `30`, `60%` |
| `stroke` | 線の色 | `red`, `rgba(255, 255, 0, 0.5)` |
| `stroke-width` | 線の太さ | `3` |
| `stroke-linecap` | 両端の形 | * `butt` ... デフォルト<br/>* `square` ... 四角（`stroke-width` の半分だけ長くなる）<br/>* `round` ... 丸 |
| `stroke-dasharray` | 破線にする | * `5,5` ... 5 の線、5 の空白を繰り返す<br/>* `5,10,5` ... 5 の線、10 の空白、5 の線、5 の空白、10 の線、5 の空白を繰り返す |
| `stroke-opacity` | 線の透明度（`line` 要素の場合 `opacity` と同じ？） | `0`（完全透過）〜 `1.0`（非透過） |
| `opacity` | 要素全体の透明度 | `0`（完全透過）〜 `1.0`（非透過） |


rect 要素（矩形）
----

{{< code lang="html" title="rect 要素の例" >}}
<rect x="40" y="20" width="100" height="50" rx="10"
    fill="cyan" stroke="blue" stroke-width="4" />
{{< /code >}}

<center>
  <svg width="200" height="100" style="border: 3px double gray">
    <rect x="40" y="20" width="100" height="50" rx="10"
        fill="cyan" stroke="blue" stroke-width="4" />
  </svg>
</center>


| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `x` | 矩形左上の X 座標 | `100` |
| `y` | 矩形左上の Y 座標 | `200` |
| `width` | 矩形の幅 | `100` |
| `height` | 矩形の高さ | `200` |
| `fill` | 塗りつぶし色。塗りつぶしを行わない場合、`none` あるいは `transparent` を指定できます。`none` の場合は場合はそこに何も存在しないかのように扱われます。`mouseover` などのイベントをハンドルする場合は、`transparent` の方を指定する必要があります。 | `none`, `transparent`, `green`, <code style="white-space:nowrap">rgb(0, 255, 0)</code>, `#00ff00`, `#0f0` |
| `fill-opacity` | 塗りつぶしの透明度。`fill` 属性で `rgba` を使って透過度を指定することも可能。 | `0`（完全透過）〜 `1.0`（非透過） |
| `stroke` | 枠線の色 | |
| `stroke-width` | 枠線の太さ。デフォルトは `1`。`0` が設定されると見えなくなります。 | `1`, `1.5`, `2` |
| `stroke-opacity` | 線の透明度。`stroke` 属性で `rgba` を使って透明度を指定することも可能。線の半分の領域は塗りつぶし色の上に重なることに注意。 | `0`（完全透過）〜 `1.0`（非透過） |
| `rx` | 角を丸くするときの半径（水平方向）。省略した場合は `ry` と同じ値が使われます。通常は `rx` だけ指定すれば OK です。 | `5` |
| `ry` | 角を丸くするときの半径（垂直方向）。省略した場合は `rx` と同じ値が使われます。通常は `rx` だけ指定すれば OK です。 | `5` |
| `opacity` | 要素全体の透明度 | `0`（完全透過）〜 `1.0`（非透過） |


circle 要素（円）
----

{{< code lang="html" title="circle 要素の例" >}}
<circle cx="80" cy="50" r="30" fill="lightgreen" stroke="green" />
{{< /code >}}

<center>
  <svg width="200" height="100" style="border: 3px double gray">
    <circle cx="80" cy="50" r="30" fill="lightgreen" stroke="green" />
  </svg>
</center>

| 属性 | 説明 |
| ---- | ---- |
| `cx` | 円の中央の X 座標 |
| `cy` | 円の中央の Y 座標 |
| `r` | 円の半径 |
| `fill` | 円の塗りつぶし色 |
| `stroke` | 円周の線の色 |
| `stroke-width` | 円周の線の太さ |


ellipse 要素（楕円）
----

{{< code lang="html" title="ellipse 要素の例" >}}
<ellipse cx="100" cy="50" rx="50" ry="15" fill="yellow" stroke="orange" />
{{< /code >}}

<center>
  <svg width="200" height="100" style="border: 3px double gray">
    <ellipse cx="100" cy="50" rx="50" ry="15" fill="yellow" stroke="orange" />
  </svg>
</center>

| 属性 | 説明 |
| ---- | ---- |
| `cx` | 楕円の中央の X 座標 |
| `cy` | 楕円の中央の Y 座標 |
| `rx` | 楕円の水平方向の半径 |
| `ry` | 楕円の垂直方向の半径 |
| `fill` | 塗りつぶし色 |
| `stroke` | 円周の線の色 |
| `stroke-width` | 円周の線の太さ |


plyline 要素（連続線） {#polyline}
----

{{< code lang="html" title="polyline 要素の例" >}}
<polyline points="20,10 40,60 60,40 80,90"
    fill="aqua" />
<polyline points="90,10 110,60 130,40 150,90"
    fill="none" stroke="blue" stroke-width="2" stroke-linecap="round" />
<polyline points="160,10 180,60 200,40 220,90"
    fill="aqua" stroke="blue" stroke-width="2" stroke-linecap="round" />
{{< /code >}}

<center>
  <svg width="240" height="100" style="border: 3px double gray">
    <polyline points="20,10 40,60 60,40 80,90"
        fill="aqua" />
    <polyline points="90,10 110,60 130,40 150,90"
        fill="none" stroke="blue" stroke-width="2" stroke-linecap="round" />
    <polyline points="160,10 180,60 200,40 220,90"
        fill="aqua" stroke="blue" stroke-width="2" stroke-linecap="round" />
  </svg>
</center>

| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `points` | 点のリスト。`x,y` のペアをスペース区切りで並べます | <code style="white-space:nowrap">10,10 80,60 25,15</code> |
| `fill` | 塗りつぶし色。始点と終点を結ぶラインと、他のラインで囲まれる領域が塗りつぶされます。線だけを引きたい場合は、明示的に `none` を指定する必要があります | `red`, `none` |
| `stroke` | 線の色 | `black` |
| `stroke-width` | 線の太さ | `4` |
| `stroke-linecap` | 両端の形 | * `butt` ... デフォルト<br/>* `square` ... 四角<br/>* `round` ... 丸 |
| `stroke-linejoin` | 線の接続部の形 | * `miter` ... デフォルト<br/>* `bevel` ... 斜めに削る<br/>* `round` ... 丸 |

始点と終点を自動的に接続するには、`polyline` の代わりに [`polygon`](#polygon) 要素を使用します。

{{% private %}}
- https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
{{% /private %}}


polygon 要素（多角形） {#polygon}
----

{{< code lang="html" title="polygon 要素の例" >}}
<polygon points="20,10 40,60 60,40 80,90"
    fill="aqua" />
<polygon points="90,10 110,60 130,40 150,90"
    fill="none" stroke="blue" stroke-width="2" stroke-linejoin="round" />
<polygon points="160,10 180,60 200,40 220,90"
    fill="aqua" stroke="blue" stroke-width="2" stroke-linejoin="round" />
{{< /code >}}

<center>
  <svg width="240" height="100" style="border: 3px double gray">
    <polygon points="20,10 40,60 60,40 80,90"
        fill="aqua" />
    <polygon points="90,10 110,60 130,40 150,90"
        fill="none" stroke="blue" stroke-width="2" stroke-linejoin="round" />
    <polygon points="160,10 180,60 200,40 220,90"
        fill="aqua" stroke="blue" stroke-width="2" stroke-linejoin="round" />
  </svg>
</center>

`polygon` は [`polyline`](#polyline) 要素と異なり、始点と終点を結ぶ線が自動的に引かれて閉じたパスが形成されます（`stroke` 属性で線の色を指定したときに違いが分かります）。
指定可能な属性は `polyline` 要素と同様です。

{{% private %}}
- https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polygon
{{% /private %}}


path 要素（パス）
----

{{< code lang="html" title="path 要素の例" >}}
<path d="M20,30 Q40,5 50,30 T90,30"
    fill="none" stroke="blue" stroke-width="5"/>
{{< /code >}}

<center>
  <svg width="150" height="60" style="border: 3px double gray">
    <path d="M20,30 Q40,5 50,30 T90,30" fill="none" stroke="blue" stroke-width="5"/>
  </svg>
</center>

__`path`__ 要素を使うと、複数の直線や曲線を組み合わせた複雑な図形を描くことができます。
`path` 要素の __`d`__ 属性に一連のコマンドをスペース区切りで並べることで形状を定義します。

| `d` 属性のコマンド | 説明 |
| ---- | ---- |
| `M5,10` | Move to の略。指定した座標 (5,10) に移動します。描画は行いません。 |
| `m5,10` | 上記の相対座標バージョン。現在の座標から (+5,+10) の座標へ移動します。描画は行いません。 |
| `L10,20` | Line to の略。現在座標から指定した座標 (10,20) まで直線を描画します。`L` は省略可能です。 |
| `l10,20` | 上記の相対座標バージョン。現在の座標から (+10,+20) の座標まで直線を描画します。 |
| `H75` | 現在の座標から x=75 の位置までの水平線を描画します。 |
| `h75` | 上記の相対座標バージョン。現在の座標から x 軸方向へ +75 の位置までの水平線を描画します。 |
| `V90` | 現在の座標から y=90 の位置までの垂直線を描画します。 |
| `v90` | 上記の相対座標バージョン。現在の座標から y 軸方向へ +90 の位置までの垂直線を描画します。 |
| `Z` or `z` | 現在座標から始点に戻る直線を描画します。 |
| <code style="white-space:nowrap">C20,20 40,20 50,10</code> | 3次ベジェ曲線（`C` の代わりに `c` を使うとすべて相対座標になる）。<ul><li>`20,20` ... 制御点1</li><li>`40,20` ... 制御点2</li><li>`50,10` ... 終点</li></ul> |
| <code style="white-space:nowrap">S50,50 80,80</code> | 3次ベジェ曲線の省略版（`S` の代わりに `s` を使うとすべて相対座標になる）。<ul><li>`50,50` ... 制御点2</li><li>`80,80` ... 終点</li></ul>`C` コマンドに続けてこの `S` コマンドを使用すると、先行する `C` コマンドの制御点 2 の反対側に制御点 1 を設置したものとして扱ってくれます。これは、2 つの曲線を滑らかに接続する効果があります。 |
| `Q40,5 50,30` | 2次ベジェ曲線（`Q` の代わりに `q` を使うとすべて相対座標になる）。<ul><li>`40,5` ... 制御点</li><li>`50,30` ... 終点</li></ul> |
| `T80,40` | 2次ベジェ曲線の省略版（`T` の代わりに `t` を使うとすべて相対座標になる）。<ul><li>`80,40` ... 終点</li></ul>`Q` コマンドに続けてこの `T` コマンドを使用すると、先行する `Q` コマンドの制御点の反対側に制御点を設置したものとして扱ってくれます。 |

カンマとスペースは置き換えが可能です。
例えば、`C20,20 40,20 50,10` は `C 20 20,40 20,50 10` とも記述できます。
`C`/`S`/`Q`/`T` コマンドを使ったベジェ曲線はちょっと分かりにくいので、具体例を別記事としてまとめてあります。

- 参考: [SVG の path 要素によるベジェ曲線の描画例](/p/ka6xumg/)

{{% private %}}
- https://developer.mozilla.org/ja/docs/Web/SVG/Tutorial/Paths
{{% /private %}}

