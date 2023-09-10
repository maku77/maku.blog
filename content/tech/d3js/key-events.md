---
title: "D3.js でキーボードイベントを扱う (keydown)"
url: "p/6emds9m/"
date: "2023-09-11"
tags: ["D3.js"]
---

D3.js でキーボードからの入力イベントを扱うコードのサンプルです。

{{< maku-common/d3 id="svg-z5a3fzu" w="200" h="100" title="クリックしてから上下左右キーで動かす" >}}
const svg = d3.select("#svg-z5a3fzu");  // D3 Selection オブジェクトを取得

svg
  .on("keydown", handleKeyDown)  // keydown イベントのハンドラーを設定
  .attr("tabindex", 0)  // フォーカスを当てられるようにする
  // .node().focus()  // 自動でフォーカスする場合

// カーソルキーで動かせる丸を表示
const circle = svg.append("circle")
  .attr("cx", 100)
  .attr("cy", 50)
  .attr("r", 10)
  .attr("fill", "dodgerblue")

function handleKeyDown(event) {
  switch (event.code) {
    case "ArrowLeft":
      circle.attr("cx", +circle.attr("cx") - 5)
      event.preventDefault();
      break;
    case "ArrowRight":
      circle.attr("cx", +circle.attr("cx") + 5)
      event.preventDefault();
      break;
    case "ArrowUp":
      circle.attr("cy", +circle.attr("cy") - 5)
      event.preventDefault();
      break;
    case "ArrowDown":
      circle.attr("cy", +circle.attr("cy") + 5)
      event.preventDefault();
      break;
  }
}
{{< /maku-common/d3 >}}

次の例では、`svg` 要素でキーボードイベントをハンドルしています。
`svg` 要素がフォーカスされた状態でカーソルキー（上下左右）を入力すると、丸印 (`circle`) が上下左右に動きます。

```html
<svg id="svg-z5a3fzu" width="200" height="100"></svg>
<script>
const svg = d3.select("#svg-z5a3fzu");  // D3 Selection オブジェクトを取得

svg
  .on("keydown", handleKeyDown)  // keydown イベントのハンドラーを設定
  .attr("tabindex", 0)  // フォーカスを当てられるようにする
  // .node().focus()  // 自動でフォーカスする場合

// カーソルキーで動かせる丸を表示
const circle = svg.append("circle")
  .attr("cx", 100)
  .attr("cy", 50)
  .attr("r", 10)
  .attr("fill", "dodgerblue")

function handleKeyDown(event) {
  switch (event.code) {
    case "ArrowLeft":
      circle.attr("cx", +circle.attr("cx") - 5)
      event.preventDefault();
      break;
    case "ArrowRight":
      circle.attr("cx", +circle.attr("cx") + 5)
      event.preventDefault();
      break;
    case "ArrowUp":
      circle.attr("cy", +circle.attr("cy") - 5)
      event.preventDefault();
      break;
    case "ArrowDown":
      circle.attr("cy", +circle.attr("cy") + 5)
      event.preventDefault();
      break;
  }
}
</script>
```

ある要素でキーボードイベントをハンドルするには、__`tabindex`__ 属性を設定してその要素をフォーカスできるようにしておく必要があります。
また、上下左右キーはデフォルトでブラウザ画面のスクロール動作に割り当てられているので、この振る舞いを __`event.preventDefault()`__ で抑制しています。

ブラウザ上でキーボードイベントを適切に処理するには、こういった細かいケアが必要になってくるため、基本的には [マウスイベント](/p/ffz4v3w/) だけで動作するアプリケーション仕様を考えるのがよさそうです。

