---
title: "D3.js で矢印を表示する (defs, marker, marker-end)"
url: "p/gtg33no/"
date: "2023-09-29"
tags: ["D3.js"]
---


marker 要素で矢印の形を定義する
----

D3.js で `line` 要素や `path` 要素の先端に矢印を表示するには、__`marker`__ 要素で矢印形状を定義して、それを `line` 要素などの __`marker-end`__ 属性（あるいは __`marker-start`__ 属性）にセットします。

次の例では、2 つの `circle` 要素とそれらを結ぶ `line` 要素を配置し、`line` 要素の終端に矢印を表示しています。

{{< mm/d3 id="svg-h2cqngx" w="120" h="70" title="矢印の表示" >}}
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 100, y: 20 }
const MARKER_ID = "marker-1"

// マーカー (marker) を定義する
function createMarker(selection) {
  selection.append("defs")
    .append("marker")
    .attr("id", MARKER_ID)
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 3)  // 始点、終点の丸記号と重ならないように調整
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
    .append("path")
      .attr("d", "M0,0 L2,1 0,2 1,1 Z")
      .attr("fill", "blue")
}

// 線 (line) を描画する
function drawLine(selection, x1, y1, x2, y2) {
  selection.append("line")
    .attr("x1", x1)
    .attr("y1", y1)
    .attr("x2", x2)
    .attr("y2", y2)
    .attr("stroke", "blue")
    .attr("marker-end", `url(#${MARKER_ID})`)  // マーカーを線に割り当てる
}

// 円 (circle) を描画する
function drawCircle(selection, x, y) {
  selection.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 5)
    .attr("fill", "orange")
}

// SVG の各描画要素を作成する
d3.select("#svg-h2cqngx")
  .call(createMarker)
  .call(drawLine, p1.x, p1.y, p2.x, p2.y)
  .call(drawCircle, p1.x, p1.y)
  .call(drawCircle, p2.x, p2.y)
{{< /mm/d3 >}}

{{< code lang="html" title="上記 SVG を出力するためのコード" >}}
<svg id="svg-h2cqngx" width="120" height="70"></svg>
<script>
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 100, y: 20 }

const MARKER_ID = "MyMarker"

// マーカー (marker) を定義する
function createMarker(selection) {
  selection.append("defs")
    .append("marker")
    .attr("id", MARKER_ID)
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 3)  // 始点、終点の丸記号と重ならないように調整
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
    .append("path")
      .attr("d", "M0,0 L2,1 0,2 1,1 Z")
      .attr("fill", "blue")
}

// 線 (line) を描画する
function drawLine(selection, x1, y1, x2, y2) {
  selection.append("line")
    .attr("x1", x1)
    .attr("y1", y1)
    .attr("x2", x2)
    .attr("y2", y2)
    .attr("stroke", "blue")
    .attr("marker-end", `url(#${MARKER_ID})`)  // マーカーを線に割り当てる
}

// 円 (circle) を描画する
function drawCircle(selection, x, y) {
  selection.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 5)
    .attr("fill", "orange")
}

// SVG の各描画要素を作成する
d3.select("#svg-h2cqngx")
  .call(createMarker)
  .call(drawLine, p1.x, p1.y, p2.x, p2.y)
  .call(drawCircle, p1.x, p1.y)
  .call(drawCircle, p2.x, p2.y)
</script>
{{< /code >}}

この仕組みは、D3.js というよりは SVG の仕組みなので、矢印形状の詳しい定義方法は下記記事を参考にしてください。

- 参考: [SVG の marker 要素で矢印（マーカー）を描画する](/p/hoiytvm/)


（応用）始点と終点の円に重ならないように線の長さを調整する
----

前述の例では、円の中心座標を結ぶように `line` 要素の始点と終点の座標を指定しました。
そして、矢印のマーカーが円に重ならないようにするため、`marker` 要素の `refX`/`refY` 属性でマーカー自体の表示位置を調整しました。
ただし、この方法だと、円のサイズが変化する場合に対応できません（円のサイズが大きくなったときに、円と矢印が重なってしまいます）。

次の例では、円の半径に応じて、`line` 要素の始点と終点の座標を調整するようにしています。

{{< mm/d3 id="svg-2du6dtt" w="120" h="70" title="円のサイズに応じて線の長さを調整" >}}
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 95, y: 25 }
const MARKER_ID = "marker-2"

// マーカー (marker) を定義する
function createMarker(selection) {
  selection.append("defs")
    .append("marker")
    .attr("id", MARKER_ID)
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 2)
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
    .append("path")
      .attr("d", "M0,0 L2,1 0,2 1,1 Z")
      .attr("fill", "blue")
}

// 線 (line) を描画する
// * startR ... 始点ノードの円の半径
// * endR ... 終点ノードの円の半径
function drawLine(selection, x1, y1, x2, y2, startR, endR) {
  if (x1 == x2 && y1 == y2) return

  // 始点と終点の円に重ならないように長さを調整
  const dist = Math.hypot(x1 - x2, y1 - y2)
  const cos = (x2 - x1) / dist
  const sin = (y2 - y1) / dist
  const dx1 = startR * cos
  const dy1 = startR * sin
  const dx2 = endR * cos
  const dy2 = endR * sin

  selection.append("line")
    .attr("x1", x1 + dx1)
    .attr("y1", y1 + dy1)
    .attr("x2", x2 - dx2)
    .attr("y2", y2 - dy2)
    .attr("stroke", "blue")
    .attr("marker-end", `url(#${MARKER_ID})`)
}

// 円 (circle) を描画する
function drawCircle(selection, x, y, r) {
  selection.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", r)
    .attr("stroke", "red")
    .attr("fill", "transparent")
}

// SVG の各描画要素を作成する
d3.select("#svg-2du6dtt")
  .call(createMarker)
  .call(drawLine, p1.x, p1.y, p2.x, p2.y, 5, 10)
  .call(drawCircle, p1.x, p1.y, 5)
  .call(drawCircle, p2.x, p2.y, 10)
{{< /mm/d3 >}}

```js
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 95, y: 25 }
const MARKER_ID = "marker-2"

// マーカー (marker) を定義する
function createMarker(selection) {
  selection.append("defs")
    .append("marker")
    .attr("id", MARKER_ID)
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 2)
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
    .append("path")
      .attr("d", "M0,0 L2,1 0,2 1,1 Z")
      .attr("fill", "blue")
}

// 線 (line) を描画する
// * startR ... 始点ノードの円の半径
// * endR ... 終点ノードの円の半径
function drawLine(selection, x1, y1, x2, y2, startR, endR) {
  if (x1 == x2 && y1 == y2) return

  // 始点と終点の円に重ならないように長さを調整
  const dist = Math.hypot(x1 - x2, y1 - y2)
  const cos = (x2 - x1) / dist
  const sin = (y2 - y1) / dist
  const dx1 = startR * cos
  const dy1 = startR * sin
  const dx2 = endR * cos
  const dy2 = endR * sin

  selection.append("line")
    .attr("x1", x1 + dx1)
    .attr("y1", y1 + dy1)
    .attr("x2", x2 - dx2)
    .attr("y2", y2 - dy2)
    .attr("stroke", "blue")
    .attr("marker-end", `url(#${MARKER_ID})`)
}

// 円 (circle) を描画する
function drawCircle(selection, x, y, r) {
  selection.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", r)
    .attr("stroke", "red")
    .attr("fill", "transparent")
}

// SVG の各描画要素を作成する
d3.select("#svg-2du6dtt")
  .call(createMarker)
  .call(drawLine, p1.x, p1.y, p2.x, p2.y, 5, 10)
  .call(drawCircle, p1.x, p1.y, 5)
  .call(drawCircle, p2.x, p2.y, 10)
```


（応用）marker を静的に定義する
----

前述のコードの `defs` 要素によるマーカーの定義をよく見ると、アタッチ先の座標に影響されない静的な定義であることが分かります。
であれば、クライアントサイド JavaScript で実行される D3.js を使って `defs` 要素を動的に生成する必要はないはずです。

下記のコードは、Svelte コンポーネントから D3.js を使用する例です。
マーカーの定義は、静的な `svg` 要素の定義の中にハードコードされています。

{{< code lang="html" title="src/lib/components/Diagram.svelte（Svelte コンポーネントの実装例）" hl_lines="56-61">}}
<script>
	import * as d3 from 'd3';
	import { onMount } from 'svelte';

	// 始点と終点の座標
	const p1 = { x: 20, y: 50 };
	const p2 = { x: 95, y: 25 };
	const MARKER_ID = 'my-marker';

	// 線 (line) を描画する
	// * startR ... 始点ノードの円の半径
	// * endR ... 終点ノードの円の半径
	function drawLine(selection, x1, y1, x2, y2, startR, endR) {
		if (x1 == x2 && y1 == y2) return;

		// 始点と終点の円に重ならないように長さを調整
		const dist = Math.hypot(x1 - x2, y1 - y2);
		const cos = (x2 - x1) / dist;
		const sin = (y2 - y1) / dist;
		const dx1 = startR * cos;
		const dy1 = startR * sin;
		const dx2 = endR * cos;
		const dy2 = endR * sin;

		selection
			.append('line')
			.attr('x1', x1 + dx1)
			.attr('y1', y1 + dy1)
			.attr('x2', x2 - dx2)
			.attr('y2', y2 - dy2)
			.attr('stroke', 'blue')
			.attr('marker-end', `url(#${MARKER_ID})`);
	}

	// 円 (circle) を描画する
	function drawCircle(selection, x, y, r) {
		selection
			.append('circle')
			.attr('cx', x)
			.attr('cy', y)
			.attr('r', r)
			.attr('stroke', 'red')
			.attr('fill', 'transparent');
	}

	// DOM 要素の参照はマウント後に行う
	onMount(() => {
		d3.select('svg')
			.call(drawLine, p1.x, p1.y, p2.x, p2.y, 5, 10)
			.call(drawCircle, p1.x, p1.y, 5)
			.call(drawCircle, p2.x, p2.y, 10);
	});
</script>

<svg width="200" viewBox="0 0 120 70" style="border: thin solid gray">
	<defs>
		<marker id={MARKER_ID} viewBox="0 0 2 2" refX="2" refY="1"
				markerWidth="10" markerHeight="10" orient="auto">
			<path d="M0,0 L2,1 0,2 1,1 Z" fill="blue" />
		</marker>
	</defs>
</svg>
{{< /code >}}
