---
title: "D3.js でレイヤー用の g 要素を作成するユーティリティ関数 (layer)"
url: "p/298nhnq/"
date: "2023-11-03"
tags: ["D3.js"]
---

便利な layer 関数を実装する
----

SVG の `g` 要素は、描画要素の表示順序を制御するためのレイヤーとして使われることがあります（参考: [SVG の g 要素の使い方](/p/rx9hdei/)）。
D3.js でレイヤー構造を作るのは簡単で、次のように任意のセレクションオブジェクトの `append()` メソッドで `g` 要素を追加するだけです。

```js
function draw(data) {
  const svg = d3.select("#mysvg");
  const layer1 = svg.append("g");  // 奥に表示するレイヤー
  const layer2 = svg.append("g");  // 手前に表示するレイヤー

  // layer1 への描画
  layer1.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("cx", () => Math.random() * 170 + 15)
      .attr("cy", () => Math.random() * 40 + 15)
      .attr("r", 10)
      .attr("fill", "#00f9")

  // layer2 への描画（省略）
}
```

注意しなければいけないのは、上記のように直接 `append()` メソッドを呼び出す場合は、このコードが 1 度だけ実行されるようにしておくことです。
[D3.js の `data` メソッドによるデータ結合の仕組み](/p/bp2btie/) を使っていると、データ配列に変更があったときに、`enter()` や `append()` を組み合わせたコードを繰り返し呼び出すことになります。
このとき、上記のように単純に `append()` している部分まで呼び出してしまうと、`g` 要素がどんどん増えていってしまいます。

```js
draw(d3.range(3));  // 1 度目の描画でレイヤー用の g 要素を作成する
draw(d3.range(3));  // データ変更後の再描画で余計な g 要素が作成されてしまう
```

次のユーティリティ関数 __`layer()`__ は、このようなミスを防ぐために、レイヤー用の `g` 要素がまだ追加されていない場合のみ `append()` を実行するようにしています。

```js
/**
 * レイヤーを新たに作成または既存のレイヤーを取得します。
 * @see {@link https://maku.blog/p/298nhnq/}
 */
function layer(selection, id) {
  const g = selection.select("#" + id);
  return !g.empty() ? g : selection.append("g").attr("id", id);
}
```

前述の `draw()` 関数を次のように書き換えると、安心して繰り返し呼び出せるようになります。

```js
function draw(data) {
  const svg = d3.select("#mysvg");

  // レイヤー用のセレクションオブジェクトを取得
  const layer1 = layer(svg, "layer1");
  const layer2 = layer(svg, "layer2");

  // ...
}
```


layer 関数の使用例
----

下記は、このユーティリティ関数 (`layer`) の具体的な使用例です。

{{< maku-common/d3 id="svg-af2nv6q" w="200" h="70" title="layer() で作成したレイヤーに描画" >}}
draw(d3.range(3));

function draw(data) {
  const svg = d3.select("#svg-af2nv6q");
  const layer1 = layer(svg, "layer-rx5sfkn");
  const layer2 = layer(svg, "layer-v73ikwo");

  layer1.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("cx", () => Math.random() * 170 + 15)
      .attr("cy", () => Math.random() * 40 + 15)
      .attr("r", 10)
      .attr("fill", "#00f9")

  layer2.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("cx", () => Math.random() * 170 + 15)
      .attr("cy", () => Math.random() * 40 + 15)
      .attr("r", 10)
      .attr("fill", "#f009")
}

function layer(selection, id) {
  const g = selection.select("#" + id);
  return !g.empty() ? g : selection.append("g").attr("id", id);
}
{{< /maku-common/d3 >}}

```html
<svg id="svg-af2nv6q" width="200" height="70"></svg>
<script>
draw(d3.range(3)); // 1回目の描画（内部でレイヤーが生成される）
draw(d3.range(3)); // 2回目の描画（再描画時はレイヤーが再利用される）

function draw(data) {
  const svg = d3.select("#svg-af2nv6q");
  const layer1 = layer(svg, "layer-rx5sfkn");
  const layer2 = layer(svg, "layer-v73ikwo");

  layer1.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("cx", () => Math.random() * 170 + 15)
      .attr("cy", () => Math.random() * 40 + 15)
      .attr("r", 10)
      .attr("fill", "#00f9")

  layer2.selectAll("circle")
    .data(data)
    .join("circle")
      .attr("cx", () => Math.random() * 170 + 15)
      .attr("cy", () => Math.random() * 40 + 15)
      .attr("r", 10)
      .attr("fill", "#f009")
}

/**
 * レイヤーを新たに作成または既存のレイヤーを取得します。
 * @see {@link https://maku.blog/p/298nhnq/}
 */
function layer(selection, id) {
  const g = selection.select("#" + id);
  return !g.empty() ? g : selection.append("g").attr("id", id);
}
</script>
```

さらに、次のように全レイヤーのセレクションオブジェクトをまとめて返す関数 (`getLayers`) を作っておくのもいいですね。
これを使えば、`draw()` 関数をよりシンプルに記述できるようになるだけでなく、レイヤーの表示順序（＝作成順序）に関する知識を関数内に閉じることができます。

```js
function getLayers() {
  const svg = d3.select("#svg-af2nv6q");
  return {
    layer1: layer(svg, "layer-rx5sfkn"),
    layer2: layer(svg, "layer-v73ikwo"),
  };
}

function draw(data) {
  const { layer1, layer2 } = getLayers();

  // ... 以下同様 ...
}
```

