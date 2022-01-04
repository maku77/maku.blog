---
title: "Android開発: TextureView に関するメモ"
linkTitle: "TextureView に関するメモ"
url: "/p/2udtaq5"
date: "2014-07-24"
tags: ["Android"]
---

- 少ない数の `Canvas#drawRect()` や `Canvas#drawText()` をするなら、`SurfaceView` よりも描画が速い。ある程度たくさん描画すると、`SurfaceView` に負ける。さらにたくさん描画すると同じくらいになる。
- ベンチマークを Xperia Tablet Z で取ると次のような感じ。
    - Rect(x100),Text(x100): SurfaceView 60fps, TextureView 80fps
    - Rect(x500),Text(x500): SurfaceView 32fps, TextureView 27fps
    - Rect(x1000),Text(x1000); SurfaceView 15fps, TextureView 15fps
- `lockCanvas()` ～ `unlockCanvasAndPost()` の中では毎回全体を描き直す必要がある。前回の描画情報が保持されることが保証されないため（`TextureView#lockCanvas()` の Javadoc を参照）。
* `SurfaceTextureListener#onSurfaceTextureDestroyed()` は通常 `return true` するように実装する。
* 画面回転すると強制終了したりする（`SurfaceView` のときは大丈夫だった）。

