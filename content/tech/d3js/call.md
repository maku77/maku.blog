---
title: "D3.js の call メソッドを使ってメソッドチェーン処理をスマートに記述する"
url: "p/994xjma/"
date: "2023-10-08"
tags: ["D3.js"]
---

call メソッドの基本
----

D3 セレクションオブジェクトの __`call()`__ メソッドを使うと、任意の関数に自身のセレクションオブジェクト (`this`) を渡す形で呼び出すことができます。
下記コードの (A) と (B) は同じ振る舞いになります。

{{< code lang="html" hl_lines="7 10" >}}
<script>
function setMessage(selection) {
  selection.text('Hello');
}

// (A) 普通の関数呼びだし
// setMessage(d3.select('#msg'));

// (B) call による関数呼び出し
d3.select('#msg').call(setMessage);
</script>

<div id='msg'></div>
{{< /code >}}

`call()` メソッド呼び出し時の第 2 引数以降は、第 1 引数で指定した関数にパラメーターとして渡されます。

```js
function setMessage(selection, message) {
  selection.text(message);
}

d3.select('#msg')
  .call(setMessage, 'Hello');
```


call メソッドの使いどころ
---

`selection.call()` メソッドの戻り値は、呼び出し元のレシーバーである `selection` オブジェクト自身 (`this`) になります。
これは、つまり、`selection` オブジェクトに対するメソッドのチェーンを続けられるということです。

例えば、次のように、`svg` 要素の D3 セレクションオブジェクトに対して複数の処理を行うコードを考えてみます。

```js
const data = [50, 100, 150];
const svg = d3.select('#mysvg');

// data 配列に基づいて circle 要素を追加
svg
  .selectAll('circle')
  .data(data)
  .join('circle')
  .attr('cx', (d) => d)
  .attr('cy', 20)
  .attr('r', 10)
  .attr('stroke', 'blue')
  .attr('fill', 'cyan');

// data 配列に基づいて rect 要素を追加
svg
  .selectAll('rect')
  .data(data)
  .join('rect')
  .attr('x', (d) => d - 10)
  .attr('y', 40)
  .attr('width', 20)
  .attr('height', 20)
  .attr('stroke', 'orange')
  .attr('fill', 'yellow');
```

`circle` 要素を追加する部分と、`rect` 要素を追加する部分は、ひとまとまりの処理になっているので次のように関数化できそうです（TypeScript の型付きで記述しています）。

```js
type Selection = d3.Selection<d3.BaseType, any, HTMLElement, any>;

function drawCircles(selection: Selection, data: number[]) {
  selection
    .selectAll('circle')
    .data(data)
    .join('circle')
    .attr('cx', (d) => d)
    .attr('cy', 20)
    .attr('r', 10)
    .attr('stroke', 'blue')
    .attr('fill', 'cyan');
}

function drawRects(selection: Selection, data: number[]) {
  selection
    .selectAll('rect')
    .data(data)
    .join('rect')
    .attr('x', (d) => d - 10)
    .attr('y', 40)
    .attr('width', 20)
    .attr('height', 20)
    .attr('stroke', 'orange')
    .attr('fill', 'yellow');
}
```

これで、前述のコードは次のように `call` メソッドを使ってメソッドチェーンする形で記述できるようになります。

```js
const data = [50, 100, 150];

d3.select('#mysvg')
  .call(drawCircles, data)
  .call(drawRects, data);
```

スッキリ、すっきり ٩(๑❛ᴗ❛๑)۶


call メソッドに渡す関数を生成する D3 ユーティリティ
----

D3.js のユーティリティ関数には、D3 セレクションオブジェクトの `call` メソッドと組み合わせて使用することを想定したものがあるので紹介しておきます。

### d3.axisBottom() ... 軸の描画

__`d3.axisBottom()`__ 関数や __`d3.axisLeft()`__ 関数は、グラフの軸を描画するための関数を生成します。
生成した関数を `selection.call()` メソッドに渡すことで、実際に軸を描画するための描画要素が生成されます。

{{< code lang="js" hl_lines="8" >}}
const xScale = d3.scaleLinear()
  .domain([0, 100])
  .range([0, width - marginLeft - marginRight]);

svg.append('g')
  .attr('class', 'x-axis')
  .attr('transform', `translate(${marginLeft} ${height - marginBottom})`)
  .call(d3.axisBottom(xScale));
{{< /code >}}

### d3.zoom() ... ズームとパン操作

__`d3.zoom()`__ 関数は、マウス操作によるズーム操作やパン操作を可能にするための関数を生成します。

{{< code lang="js" hl_lines="5" >}}
const zoom = d3.zoom()
  .scaleExtent([1, 4])
  .on("zoom", zoomed);

svg.call(zoom);
{{< /code >}}

### d3.drag() ... 描画要素のドラッグ

__`d3.drag()`__ 関数は、描画要素をドラッグ可能にするための関数を生成します。

{{< code lang="js" hl_lines="6" >}}
const drag = d3.drag()
  .on("start", dragStarted)
  .on("drag", dragged)
  .on("end", dragEnded);

svg.call(drag);
{{< /code >}}

### d3.brush() ... 選択範囲の描画

__`d3.brush()`__ 関数は、データの選択や範囲の選択を行うための関数を生成します。

{{< code lang="js" hl_lines="7" >}}
const brush = d3.brushX()
  .extent([[0, 0], [width, height]])
  .on('end', brushed);

svg.append('g')
  .attr('class', 'brush')
  .call(brush);
{{< /code >}}

