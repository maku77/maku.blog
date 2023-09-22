---
title: "SVG の viewBox の使い方"
url: "p/563uibg/"
date: "2023-09-22"
tags: ["SVG"]
---

`svg` 要素内の各描画要素に設定する座標値のデフォルト単位はピクセル (`px`) ですが、上位の `svg` 要素に __`viewBox`__ 属性が設定されている場合は、その論理的な座標系内での座標値を指定することになります。
例えば、次のように `viewBox` を設定すると、画面上での `svg` 要素の表示サイズは 800x600 となりますが、`svg` 要素内に配置する各描画要素 (`rect` など）は、40x30 という __仮想的なサイズ__ の中で座標指定することになります。

```html
<svg viewBox="0 0 40 30" width="800" height="600">
```

下記の例では 1 つの `rect` 要素を配置していますが、これは `svg` 要素の左上 4 分の 1 を占める矩形を描画します。

```html
<svg viewBox="0 0 40 20" width="200" height="100" style="border: thin solid gray">
  <rect x="0" y="0" width="20" height="10" fill="steelblue" />
</svg>
```

<center>
  <svg viewBox="0 0 40 20" width="200" height="100" style="border: thin solid gray">
    <rect x="0" y="0" width="20" height="10" fill="steelblue" />
  </svg>
  <div>表示例</div>
</center>

