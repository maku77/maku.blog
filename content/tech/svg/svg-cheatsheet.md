---
title: "SVG チートシート"
url: "p/kq6gwfn/"
date: "2023-09-09"
tags: ["cheatsheet", "SVG"]
---

`svg` 要素内に配置できる描画要素のまとめです。
座標値のデフォルトの単位はピクセル (`px`) ですが、`svg` 要素に [`viewBox`](/p/563uibg/) が設定されている場合は `viewBox` 内の座標系で座標値を指定します。


text 要素（テキスト）
----

| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `x` | テキストアンカーポイントの X 座標 | `100`, `50%` |
| `y` | テキストアンカーポイントの Y 座標 | `100`, `50%` |
| [`text-anchor`](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor) | 水平方向のアンカーポイント | * `start` ... テキスト先頭（ltr 言語なら左端）<br>* `middle` ... 中央<br/>* `end` ... テキスト末尾（ltr 言語なら右端） |
| [`dominant-baseline`](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline) | 垂直方向のベースライン | * `auto` ... 通常は下端<br/>* `middle` ... 中央<br/>* `hanging` ... 通常は上端 |
| `font-size` | フォントサイズ | `30` |
| `font-weight` | 文字の太さ | `600` |
| `fill` | テキスト内部の色 | `red`<br/>`rgb(39, 250, 102)`<br/>`#ff0033` |
| `stroke` | テキスト周囲の線の色 | `red` |
| `stroke-width` | テキスト周囲の線の太さ | `2` |

{{< code lang="html" title="text 要素の例" >}}
<text x="100" y="100" font-size="30" fill="blue">
  表示するテキスト
</text>
{{< /code >}}

- `text` 要素のテクニック
  - viewBox 内で水平方向に中央寄せする ... `x="50%" text-anchor="middle"`
  - viewBox 内で垂直方向に中央寄せする ... `y="50%" dominant-baseline="middle"`

{{% private %}}
- [MDN - text element](https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text)
- [SVG- text element cheatsheet](https://codepen.io/tripti1410/pen/WBmMOB)
{{% /private %}}


line 要素（直線）
----

| 属性 | 説明 |
| ---- | ---- |
| `x1` | 線の始点 X 座標 |
| `y1` | 線の始点 Y 座標 |
| `x2` | 線の終点 X 座標 |
| `y2` | 線の終点 Y 座標 |
| `stroke` | 線の色 |
| `stroke-width` | 線の太さ |

{{< code lang="html" title="line 要素の例" >}}
<line x1="0" y1="0" x2="50" y2="30" stroke="red" stroke-width="3" />
{{< /code >}}


rect 要素（矩形）
----

| 属性 | 説明 | 設定例 |
| ---- | ---- | ---- |
| `x` | 矩形左上の X 座標 | `100` |
| `y` | 矩形左上の Y 座標 | `200` |
| `width` | 矩形の幅 | `100` |
| `height` | 矩形の高さ | `200` |
| `fill` | 塗りつぶし色。塗りつぶしを行わない場合、`none` あるいは `transparent` を指定できます。`none` の場合は場合はそこに何も存在しないかのように扱われます。`mouseover` などのイベントをハンドルする場合は、`transparent` の方を指定する必要があります。 | `none`, `transparent`, `green`, `rgb(0, 255, 0)`, `#00ff00`, `#0f0` |
| `stroke` | 枠線の色 | |
| `stroke-width` | 枠線の太さ。デフォルトは `1`。`0` が設定されると見えなくなります。 | `1`, `1.5`, `2` |
| `rx` | 角を丸くするときの半径（水平方向）。省略した場合は `ry` と同じ値が使われます。通常は `rx` だけ指定すれば OK です。 | |
| `ry` | 角を丸くするときの半径（垂直方向）。省略した場合は `rx` と同じ値が使われます。通常は `rx` だけ指定すれば OK です。 | |

{{< code lang="html" title="rect 要素の例" >}}
<rect x="10" y="20" width="100" height="50"
      fill="red" stroke="blue" stroke-width="2" />
{{< /code >}}


circle 要素（円）
----

| 属性 | 説明 |
| ---- | ---- |
| `cx` | 円の中央の X 座標 |
| `cy` | 円の中央の Y 座標 |
| `r` | 円の半径 |
| `fill` | 円の塗りつぶし色 |
| `stroke` | 円周の線の色 |
| `stroke-width` | 円周の線の太さ |

{{< code lang="html" title="circle 要素の例" >}}
<circle cx="50" cy="50" r="40" fill="cyan" />
{{< /code >}}


ellipse 要素（楕円）
----

| 属性 | 説明 |
| ---- | ---- |
| `cx` | 楕円の中央の X 座標 |
| `cy` | 楕円の中央の Y 座標 |
| `rx` | 楕円の水平方向の半径 |
| `ry` | 楕円の垂直方向の半径 |
| `fill` | 塗りつぶし色 |
| `stroke` | 円周の線の色 |
| `stroke-width` | 円周の線の太さ |

{{< code lang="html" title="circle 要素の例" >}}
<circle cx="50" cy="50" r="40" fill="cyan" />
{{< /code >}}


その他
----

- 透過度は CSS の `opacity` プロパティで指定できます。
  - `opacity: 0.0;` ... 完全に透過（見えない）
  - `opacity: 0.5;` ... 50% 透過
  - `opacity: 1.0;` ... 非透過 (default)

