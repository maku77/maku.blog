---
title: "D3.js でマウスイベントを扱う (click, mousemove, mouseover, mouseout)"
url: "p/ffz4v3w/"
date: "2023-09-09"
tags: ["D3.js"]
---

D3.js でマウスイベントを扱うコードのサンプルです。


click イベント
----

{{< maku-common/d3 id="svg-vwuifrt" w="200" h="100" title="click イベントをハンドルする" >}}
const svg = d3.select("#svg-vwuifrt");  // D3 Selection オブジェクトを取得

svg.on("click", (event) => {
  const [x, y] = d3.pointer(event)
  svg.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 10)
    .attr("fill", "deeppink")
    .style("fill-opacity", 0.5)
})
{{< /maku-common/d3 >}}

D3.js でマウスのクリックイベントをハンドルするには、D3 Selection オブジェクトに __`click`__ イベントハンドラを追加します。
上記の svg 要素内でクリックすると、その座標にピンク色の円が表示されます。

```html
<svg id="svg-vwuifrt" width="200" height="100" />
<script>
const svg = d3.select("#svg-vwuifrt");  // D3 Selection オブジェクトを取得

svg.on("click", (event) => {
  const [x, y] = d3.pointer(event)
  svg.append("circle")
    .attr("cx", x)
    .attr("cy", y)
    .attr("r", 10)
    .attr("fill", "deeppink")
    .style("fill-opacity", 0.5)
})
</script>
```

`event.clientX` や `event.clientY` でマウスカーソルの座標値を取得してしまうと、ブラウザのクライアント領域全体の座標値になってしまうので、__`d3.point()`__ で `svg` 要素内の座標値に変換するところがポイントです。


mousemove イベント
----

{{< maku-common/d3 id="svg-mousemove" w="200" h="100" title="mousemove イベントをハンドルする" >}}
const svg = d3.select("#svg-mousemove");

// 座標値を表示するための text 要素を追加しておく
const text = svg.append("text")
  .attr("x", "50%")
  .attr("y", "50%")
  .attr("text-anchor", "middle")  // 水平方向のアンカーポイントを中央に
  .attr("dominant-baseline", "middle")  // 垂直方向のベースラインを中央に
  .text("x=0, y=0")

// カーソル位置を表示する circle 要素を追加しておく
const circle = svg.append("circle")
  .attr("r", 6)
  .attr("fill", "red")

svg.on("mousemove", (event) => {
  const [x, y] = d3.pointer(event)
  text.text(`x=${Math.floor(x)}, y=${Math.floor(y)}`)
  circle.attr("cx", x).attr("cy", y)
})
{{< /maku-common/d3 >}}

D3.js でマウスカーソルを動かしたときのイベントをハンドルするには、D3 Selection オブジェクトに __`mousemove`__ イベントハンドラを追加します。
上記の svg 要素内でマウスカーソルを動かすと、現在の座標値を表示します。

```html
<svg id="svg-mousemove" width="200" height="100" />
<script>
const svg = d3.select("#svg-mousemove");

// 座標値を表示するための text 要素を追加しておく
const text = svg.append("text")
  .attr("x", "50%")
  .attr("y", "50%")
  .attr("text-anchor", "middle")  // 水平方向のアンカーポイントを中央に
  .attr("dominant-baseline", "middle")  // 垂直方向のベースラインを中央に
  .text("x=0, y=0")

// カーソル位置を表示する circle 要素を追加しておく
const circle = svg.append("circle")
  .attr("r", 6)
  .attr("fill", "red")

// mousemove イベントをハンドル
svg.on("mousemove", (event) => {
  const [x, y] = d3.pointer(event)
  text.text(`x=${Math.floor(x)}, y=${Math.floor(y)}`)
  circle.attr("cx", x).attr("cy", y)
})
</script>
```


mouseover / mouseout イベント
----

{{< maku-common/d3 id="svg-mouseover" w="200" h="100" title="mousemove イベントをハンドルする" >}}
const svg = d3.select("#svg-mouseover");

// 矩形を追加
const rect = svg.append("rect")
  .attr("x", 35)
  .attr("y", 25)
  .attr("width", 50)
  .attr("height", 50)
  .attr("stroke", "gray") // 枠線の色
  .attr("fill", "transparent") // 塗りつぶしの色
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

// 円を追加
const circle = svg.append("circle")
  .attr("cx", 140)
  .attr("cy", 50)
  .attr("r", 25)
  .attr("stroke", "gray") // 枠線の色
  .attr("fill", "transparent") // 塗りつぶしの色
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

// 要素にカーソルが入ったとき
function handleMouseOver() {
  d3.select(this)
    .attr("fill", "cyan")
}

// 要素からカーソルが出たとき
function handleMouseOut(event) {
  d3.select(this)
    .attr("fill", "transparent")
}
{{< /maku-common/d3 >}}

マウスカーソルが、特定の要素内に入った／出たタイミングを知るには、__`mouseover`__ / __`mouseout`__ イベントをハンドルします。
上の `svg` 要素内の矩形と円の中にマウスカーソルが入ると、背景色が変化します。

```html
<svg id="svg-mouseover" width="200" height="100" />
<script>
const svg = d3.select("#svg-mouseover");

// 矩形を追加
const rect = svg.append("rect")
  .attr("x", 35)
  .attr("y", 25)
  .attr("width", 50)
  .attr("height", 50)
  .attr("stroke", "gray") // 枠線の色
  .attr("fill", "transparent") // 塗りつぶしの色
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

// 円を追加
const circle = svg.append("circle")
  .attr("cx", 140)
  .attr("cy", 50)
  .attr("r", 25)
  .attr("stroke", "gray") // 枠線の色
  .attr("fill", "transparent") // 塗りつぶしの色
  .on("mouseover", handleMouseOver)
  .on("mouseout", handleMouseOut)

// 要素にカーソルが入ったとき
function handleMouseOver() {
  d3.select(this)
    .attr("fill", "cyan")
}

// 要素からカーソルが出たとき
function handleMouseOut(event) {
  d3.select(this)
    .attr("fill", "transparent")
}
</script>
```

塗りつぶしを行わない場合は、`fill` 属性に `transparent` をセットすることに注意してください。
`none` でも同様の表示になりますが、`none` だと表示自体されていないものとみなされるため、マウスカーソルのイベントをハンドルできません。

