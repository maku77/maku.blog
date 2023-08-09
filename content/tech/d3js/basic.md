---
title: "D3.js の基本"
url: "p/v38nmhw/"
date: "2023-08-08"
tags: ["D3.js"]
draft: true
---

{{< maku-common/d3 id="svg" >}}
const data = [13, 45, 6]
const node = d3.select("#svg")
  .selectAll("rect")
  .data(data)
  .enter()
  .append("rect")
  .attr("x", function(d, i) { return i * 20; })
  .attr("y", 0)
  .attr("width", 20)
  .attr("height", function(d) { return d[1]; });
{{< /maku-common/d3 >}}

