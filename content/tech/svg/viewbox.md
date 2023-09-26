---
title: "SVG の viewBox の使い方とレスポンシブ対応"
url: "p/563uibg/"
date: "2023-09-22"
tags: ["SVG"]
---

svg 要素の viewBox 属性について
----

`svg` 要素内の各描画要素に設定する座標値のデフォルト単位はピクセル (`px`) ですが、上位の `svg` 要素に __`viewBox`__ 属性が設定されている場合は、その論理的な座標系（__ユーザー座標系__）の座標値を指定することになります。
例えば、次のように `viewBox` を設定すると、画面上での `svg` 要素の表示サイズは 800px x 600px となりますが、`svg` 要素内に配置する各描画要素 (`rect` など）は、40x30 という仮想的なサイズの中で座標指定することになります。

{{< code lang="html" title="viewBox の指定" >}}
<svg width="800" height="600" viewBox="0 0 40 30">
{{< /code >}}

`viewBox` 属性を指定してある場合は、`width` 属性と `height` 属性の一方を省略して記述できるようになります。
その場合は、`viewBox` のアスペクト比を考慮した `width` あるいは `height` が自動的に設定されます。
よって、上記の `svg` タグは次のように省略して記述することが可能です。

{{< code lang="html" title="height の省略" >}}
<svg width="800" viewBox="0 0 40 30">
{{< /code >}}

下記の例では 1 つの `rect` 要素を配置していますが、これは `svg` 要素の左上 4 分の 1 を占める矩形を描画します。

```html
<svg width="200" viewBox="0 0 40 20">
  <rect x="0" y="0" width="20" height="10" fill="steelblue" />
</svg>
```

<center>
  <svg width="200" viewBox="0 0 40 20" style="border: thin solid gray">
    <rect x="0" y="0" width="20" height="10" fill="steelblue" />
  </svg>
  <div>表示例</div>
</center>


svg 要素のレスポンシブ対応
----

`svg` 要素も `img` 要素と同様に、CSS で __`max-width: 100%;`__ と指定しておけば、横幅が足りなくなった場合に縮小して表示することができます。

<center>
  <svg width="800" viewBox="0 0 20 10" style="max-width: 100%; border: thin solid gray;">
    <rect x="1" y="1" width="8" height="8" fill="pink" />
    <circle cx="15" cy="5" r="4" fill="lightblue" />
  </svg>
  <div>max-width: 100%; を設定した svg 要素</div>
</center>

{{< code lang="html" title="svg 要素のレスポンシブ表示用の設定" hl_lines="1" >}}
<svg id="mysvg" width="800" viewBox="0 0 20 10" style="max-width: 100%;">
  <rect x="1" y="1" width="8" height="8" fill="pink" />
  <circle cx="15" cy="5" r="4" fill="lightblue" />
</svg>
{{< /code >}}

ちなみに、`max-width: 100%;` の指定により縮小された `svg` 要素の実際のサイズを取得するには、以下のように `svg` 要素の __`clientWidth`__ プロパティや、__`getBoundingClientRect()`__ メソッドを使用します。

```js
// ウィンドウサイズ変更時に svg 要素の実際のサイズを取得
window.addEventListener("resize", function(event) {
  const svgElem = document.getElementById("mysvg");
  const width = svgElem.clientWidth;
  console.log(`width=${width}`);

  // 次のようにすると、ボーダー領域を含めたサイズを取得できます
  // const width = svgElem.getBoundingClientRect().width;

  // 次のようにすると、常に 800（属性で指定した固定値）が返されることに注意
  // const width = svgElem.getAttribute("width");
});
```

