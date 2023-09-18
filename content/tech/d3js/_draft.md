---
title: "DRAFT - D3.js 未分類メモ"
url: "p/ebkp6r4/"
date: "2023-09-08"
tags: ["D3.js"]
draft: true
---

{{< maku-common/d3 id="mysvg3" w="100" h="70" >}}
const svg = d3.select("#mysvg3")
const width = svg.attr("width")
const height = svg.attr("height")
const data = [10, 30, 60, 40, 90, 20, 50, 70, 10, 40]
const scaleX = d3.scaleLinear().domain([0, data.length]).range([0, width])
const scaleY = d3.scaleLinear().domain([0, 100]).range([0, height])
svg
  .style("background", "#ffe")
  .style("max-width", "100%")
  .selectAll("rect")
  .data(data)
  .join("rect")
  .attr("x", (_d, i) => scaleX(i))
  .attr("y", (d) => height - scaleY(d))
  .attr("width", width / data.length)
  .attr("height", scaleY)
  .attr("fill", "steelblue")
{{< /maku-common/d3 >}}


ノードの設定
----

ノードにかかる力をシミュレートするには、__`d3.forceSimulation()`__ 関数でシミュレーションオブジェクトを生成します。
この関数に、ノードの配列を渡すと、自動的に

- `index` ... 0 始まりのインデックス
- `x` ... X 座標
- `y` ... Y 座標
- `x` ... X 軸の移動速度
- `y` ... Y 軸の移動速度

{{% private %}}
// 摩擦係数。大きくすると止まりやすくなる。デフォルトは 0.04?
simulation.velocityDecay(0.01)

// 全ノードが中心に集まるようにする
simulation.force("center", d3.forceCenter(width / 2, height / 2))

// 全ノードが指定した X 座標、Y 座標に集まるようにする
simulation.force("x", d3.forceX(width / 2))
simulation.force("y", d3.forceY(height / 2))
{{% /private %}}


- Simulation オブジェクトからノードオブジェクトを取り出す
  ```js
  const node = simulation.nodes()[0]
  ```

