---
title: "D3.js のデータ結合を理解する (2) データ配列の要素ごとに表示／非表示を切り替える"
url: "p/nxe97gx/"
date: "2023-10-09"
tags: ["D3.js"]
weight: 3
---

この記事は、D3.js のデータ結合まわりのメソッド (`data`/`enter`/`exit`/`join`) の基本を理解していることを前提としています。

- 参考: [D3.js のデータ結合を理解する (1) data/enter/exit/join の基本](/p/bp2btie/)

D3.js でデータ配列の内容をプロットするとき、各要素の値を元にその要素を表示するか表示しないかを制御したいことがあります。
このようなケースでは、D3.js の仕組みで処理しようと考えがちですが、単純にデータ配列の要素をフィルタしてから、`selection.data()` でデータ結合してやれば OK です。

次の例では、データ配列 (`data`) の `enabled` プロパティを見て、描画要素の表示／非表示を制御しています。
とはいっても、データ配列の要素を `filter()` メソッドで先にフィルタしているだけです。

{{< maku-common/d3 id="svg-6y3q" w="120" h="40" title="表示の on/off 制御" >}}
const data = [
  { pos: 1, color: "#f00", enabled: true },
  { pos: 2, color: "#0f0", enabled: false },
  { pos: 3, color: "#00f", enabled: true },
]

// 先にデータ配列をフィルタしておく
const filteredData = data.filter((d) => d.enabled)

d3.select("#svg-6y3q")
  .selectAll("circle")
  .data(filteredData, (d) => d.pos)
  .join("circle")
    .attr("cx", (d) => d.pos * 30)
    .attr("cy", 20)
    .attr("r", 10)
    .attr("fill", (d) => d.color)
{{< /maku-common/d3 >}}

{{< code lang="js" hl_lines="7-8" >}}
const data = [
  { pos: 1, color: "#f00", enabled: true },
  { pos: 2, color: "#0f0", enabled: false },
  { pos: 3, color: "#00f", enabled: true },
]

// 先にデータ配列をフィルタしておく
const filteredData = data.filter((d) => d.enabled)

d3.select("#svg-6y3q")
  .selectAll("circle")
  .data(filteredData, (d) => d.pos)
  .join("circle")
    .attr("cx", (d) => d.pos * 30)
    .attr("cy", 20)
    .attr("r", 10)
    .attr("fill", (d) => d.color)
{{< /code >}}

次の例では、この方法がうまく動作することを示すために、1 秒ごとにデータ配列の `enable` フラグをランダムに入れ替えています。
`enabled=true` になったデータはアニメーションしながら表示され、`enabled=false` になったデータはアニメーションしながら非表示になります。

{{< maku-common/d3 id="svg-wwhf" w="120" h="40" >}}
const data = [
  { pos: 1, color: "#f00", enabled: true },
  { pos: 2, color: "#0f0", enabled: true },
  { pos: 3, color: "#00f", enabled: true },
]

// enabled プロパティの true/false をランダムにセットする
function updateData() {
  data.forEach((d) => {
    d.enabled = Math.random() < 0.5;
  })
}

function render(filteredData) {
  d3.select("#svg-wwhf")
    .selectAll("circle")
    .data(filteredData, (d) => d.pos)
    .join(
      (enter) => enter.append("circle")
        .attr("cx", (d) => d.pos * 30)
        .attr("cy", 20)
        .attr("fill", (d) => d.color)
        .transition()
          .duration(500)
          .attr("r", 10),
      (update) => update,
      (exit) => exit.transition()
        .duration(500)
        .attr("r", 0)
        .remove()
    )
}

setInterval(() => {
  updateData()
  const filteredData = data.filter((d) => d.enabled)
  render(filteredData)
}, 1000)
{{< /maku-common/d3 >}}

```js
const data = [
  { pos: 1, color: "#f00", enabled: true },
  { pos: 2, color: "#0f0", enabled: true },
  { pos: 3, color: "#00f", enabled: true },
]

// enabled プロパティの true/false をランダムにセットする
function updateData() {
  data.forEach((d) => {
    d.enabled = Math.random() < 0.5;
  })
}

function render(filteredData) {
  d3.select("#svg-wwhf")
    .selectAll("circle")
    .data(filteredData, (d) => d.pos)
    .join(
      (enter) => enter.append("circle")
        .attr("cx", (d) => d.pos * 30)
        .attr("cy", 20)
        .attr("fill", (d) => d.color)
        .transition()
          .duration(500)
          .attr("r", 10),
      (update) => update,
      (exit) => exit.transition()
        .duration(500)
        .attr("r", 0)
        .remove()
    )
}

setInterval(() => {
  updateData()
  const filteredData = data.filter((d) => d.enabled)
  render(filteredData)
}, 1000)
```

