---
title: "D3.js の基本"
url: "p/v38nmhw/"
date: "2023-08-08"
tags: ["D3.js"]
draft: true
---

{{< maku-common/d3 id="mysvg" w="400" h="200">}}
const svg = d3.select("#mysvg")
const width = svg.attr("width")
const height = svg.attr("height")

const dataset = [10, 30, 60, 40, 90, 20, 50, 70, 10, 40]

const scaleX = d3.scaleLinear()
  .domain([0, dataset.length])
  .range([0, width])
const scaleY = d3.scaleLinear()
  .domain([0, 100])
  .range([0, height])

d3.select("#mysvg")
  .style("background", "#ffe")
  .style("max-width", "100%")
  .selectAll("rect")
  .data(dataset)
  .join("rect")
  .attr("x", (_d, i) => scaleX(i))
  .attr("y", (d) => height - scaleY(d))
  .attr("width", width / dataset.length)
  .attr("height", scaleY)
  .attr("fill", "steelblue")
{{< /maku-common/d3 >}}

