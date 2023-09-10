---
title: "D3.js でボタンなどの入力要素を扱う"
url: "p/s5zrgp3/"
date: "2023-09-10"
tags: ["D3.js"]
---

D3.js は主に SVG の描画に使われるライブラリですが、一般的な DOM 要素のイベントをハンドルするためにも使用できます。
ここでは、ボタンやテキスト入力要素を D3.js で扱う例を示します。

次の例では、ボタン (`button`) をクリックしたときに、その独自属性 (`data-color`) にセットされた値を取り出しています。
この仕組みを利用すれば、ボタンを押したときに SVG の表示内容を切り替えるといったことが可能です。

<center>
<div id="buttonContainer" style="border: thin solid lightgray; width: 15em; max-width: 100%; padding: 0.5em; display: inline-block; border-radius: 0.3rem;">
  <button data-color="g">Green</button>
  <button data-color="y">Yellow</button>
  <button data-color="r">Red</button>
  <p id="message">ボタンを押してみて</p>
</div>
</center>

<script type="module">
import * as d3 from "https://cdn.jsdelivr.net/npm/d3@7/+esm";

d3.selectAll("#buttonContainer button")
  .on("click", function() {
    const color = d3.select(this).attr("data-color")
    d3.select("#message").text(`選択したカラーコード = ${color}`)
  })
</script>

```html
<div id="buttonContainer">
  <button data-color="g">Green</button>
  <button data-color="y">Yellow</button>
  <button data-color="r">Red</button>
  <p id="message">ボタンを押してみて</p>
</div>

<script type="module">
import * as d3 from "https://cdn.jsdelivr.net/npm/d3@7/+esm";

d3.selectAll("#buttonContainer button")
  .on("click", function() {
    const color = d3.select(this).attr("data-color")
    d3.select("#message").text(`選択したカラーコード = ${color}`)
  })
</script>
```

上記の例では、`click` ハンドラーの中で、`d3.select()` を使って D3 セレクションオブジェクトを生成していますが、次のように従来の DOM API だけを使って記述することも可能です。

```js
.on("click", (event) => {
  const color = event.target.dataset["color"]  // button の data-color の値を取得
  document.querySelector("#message").innerText = `選択したカラーコード = ${color}`
})
```

`d3.select()` を使っちゃった方が全体的にシンプルなコードになりますね。
実行効率は多少落ちるのかもしれませんが。

