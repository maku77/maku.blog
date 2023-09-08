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


