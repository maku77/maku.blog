---
title: "D3.js で矢印を表示する (defs, marker, marker-end)"
url: "p/gtg33no/"
date: "2023-09-29"
tags: ["D3.js"]
---

D3.js で `line` 要素や `path` 要素の先端に矢印を表示するには、__`marker`__ 要素で矢印形状を定義して、それを `line` 要素などの __`marker-end`__ 属性（あるいは __`marker-start`__ 属性）にセットします。

次の例では、2 つの `circle` 要素とそれらを結ぶ `line` 要素を配置し、`line` 要素の終端に矢印を表示しています。

{{< maku-common/d3 id="svg-h2cqngx" w="120" h="70" title="矢印の表示" >}}
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 100, y: 20 }

// 線 (line) を作成
function createLine(parent, x1, y1, x2, y2) {
  return parent.append("line")
    .attr("x1", x1)
    .attr("y1", y1)
    .attr("x2", x2)
    .attr("y2", y2)
    .attr("stroke", "blue")
}

// 円 (circle) を作成
function createCircle(parent, x, y) {
  return parent.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 5)
    .attr("fill", "orange")
}

// マーカー (marker) を作成
function createMarker(parent) {
  const marker = parent.append("defs")
    .append("marker")
    .attr("id", "MyArrow")
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 3)  // 始点、終点の丸記号と重ならないように調整
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
  marker.append("path")
    .attr("d", "M0,0 L2,1 0,2 1,1 Z")
    .attr("fill", "blue")

  return marker
}

// SVG の各描画要素を作成
const svg = d3.select("#svg-h2cqngx")
const line = createLine(svg, p1.x, p1.y, p2.x, p2.y)
const marker = createMarker(svg)
createCircle(svg, p1.x, p1.y)
createCircle(svg, p2.x, p2.y)

// マーカーを線に割り当てる
line.attr("marker-end", "url(#MyArrow)")
{{< /maku-common/d3 >}}

{{< code lang="html" title="上記 SVG を出力するためのコード" >}}
<svg id="svg-h2cqngx" width="120" height="70"></svg>
<script>
// 始点と終点の座標
const p1 = { x: 20, y: 50 }
const p2 = { x: 100, y: 20 }

// 線 (line) を作成
function createLine(parent, x1, y1, x2, y2) {
  return parent.append("line")
    .attr("x1", x1)
    .attr("y1", y1)
    .attr("x2", x2)
    .attr("y2", y2)
    .attr("stroke", "blue")
}

// 円 (circle) を作成
function createCircle(parent, x, y) {
  return parent.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 5)
    .attr("fill", "orange")
}

// マーカー (marker) を作成
function createMarker(parent) {
  const marker = parent.append("defs")
    .append("marker")
    .attr("id", "MyArrow")
    .attr("viewBox", "0 0 2 2")
    .attr("refX", 3)  // 始点、終点の丸記号と重ならないように調整
    .attr("refY", 1)
    .attr("markerWidth", 10)
    .attr("markerHeight", 10)
    .attr("orient", "auto")
  marker.append("path")
    .attr("d", "M0,0 L2,1 0,2 1,1 Z")
    .attr("fill", "blue")

  return marker
}

// SVG の各描画要素を作成
const svg = d3.select("#svg-h2cqngx")
const line = createLine(svg, p1.x, p1.y, p2.x, p2.y)
const marker = createMarker(svg)
createCircle(svg, p1.x, p1.y)
createCircle(svg, p2.x, p2.y)

// マーカーを線に割り当てる
line.attr("marker-end", "url(#MyArrow)")
</script>
{{< /code >}}

この仕組みは、D3.js というよりは SVG の仕組みなので、矢印形状の詳しい定義方法は下記記事を参考にしてください。

- 参考: [SVG の marker 要素で矢印（マーカー）を描画する](/p/hoiytvm/)

