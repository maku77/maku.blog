---
title: "WebGL入門 (5) 三角形の 3 つの描画モード"
url: "/p/9sxkmma"
date: "2019-09-13"
tags: ["3D", "WebGL"]
weight: 1005
---

{{< image w="550" src="triangle-mode-001.png" >}}

[gl.drawArrays()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/drawArrays) や [gl.drawElements](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/drawElements) で図形を描画するとき、第1引数で頂点をどのように使用するかの描画モードを指定します。
OpenGL ES では、四角形を描画することはできないので、面を表現するときには、三角形の組み合わせで描画することになります。
三角形の描画モードには、下記の 3 種類があります。

- `gl.TRIANGLE` - 各三角形は頂点を共有しない
- `gl.TRIANGLE_STRIP` - ジグザグに頂点が配置されていると想定
- `gl.TRIANGLE_FAN` - 扇形に頂点が配置されていると想定

`gl.TRIANGLE` は、隣接する面の描画においても、独立した頂点データが必要です。
よって、基本的には `gl.TRIANGLE_STRIP` か `gl.TRIANGLE_FAN` を使うのがよいのですが、多くの環境では **`gl.TRIANGLE_STRIP` の描画効率がよい**ようです。

下記は、`gl.drawArrays()` に `gl.TRIANGLE_STRIP` を指定して 4 つの三角形を描画した例です。

<center>
<canvas id="canvas-005" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>

コードでは、下記のように頂点座標と頂点カラーを定義しています。

{{< code lang="js" >}}
const vertices = new Float32Array([
  -1.0,  0.5, 1.0, 0.0, 0.0,  // v0 (XYRGB) 赤
  -0.6, -0.5, 0.0, 1.0, 0.0,  // v1 (XYRGB) 緑
   0.2,  0.5, 0.0, 0.0, 1.0,  // v2 (XYRGB) 青
   0.2, -0.5, 1.0, 0.0, 0.0,  // v3 (XYRGB) 赤
   0.6,  0.5, 0.0, 1.0, 0.0,  // v4 (XYRGB) 緑
   1.0, -0.5, 0.0, 0.0, 1.0   // v5 (XYRGB) 青
]);

// ...

gl.drawArrays(gl.TRIANGLE_STRIP, 0, 6);  // 頂点を 6 つ使用する
{{< /code >}}


<script id="vs-005" type="x-shader/x-vertex">
attribute vec4 a_Position;  // 入力（XY座標）
attribute vec4 a_Color;     // 入力（RGAカラー）
varying vec4 v_Color;       // 出力（RGAカラー）

void main() {
  gl_Position = a_Position;
  v_Color = a_Color;
}
</script>

<script id="fs-005" type="x-shader/x-fragment">
precision mediump float;
varying vec4 v_Color;

void main() {
  gl_FragColor = v_Color;
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function() {
  const gl = initGL('canvas-005', 'vs-005', 'fs-005');

  const vertices = new Float32Array([
    -1.0,  0.5, 1.0, 0.0, 0.0,  // v0 (XYRGB) 赤
    -0.6, -0.5, 0.0, 1.0, 0.0,  // v1 (XYRGB) 緑
     0.2,  0.5, 0.0, 0.0, 1.0,  // v2 (XYRGB) 青
     0.2, -0.5, 1.0, 0.0, 0.0,  // v3 (XYRGB) 赤
     0.6,  0.5, 0.0, 1.0, 0.0,  // v4 (XYRGB) 緑
     1.0, -0.5, 0.0, 0.0, 1.0   // v5 (XYRGB) 青
  ]);
  const ELEM_BYTES = vertices.BYTES_PER_ELEMENT;  // = 4

  const vertexBuffer = gl.createBuffer();
  if (!vertexBuffer) {
    throw Error('Failed to create the buffer object.');
  }
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

  // 頂点座標の attribute 変数を設定
  const a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, ELEM_BYTES * 5, 0);
  gl.enableVertexAttribArray(a_Position);

  // 頂点カラーの attribute 変数を設定
  const a_Color = gl.getAttribLocation(gl.program, 'a_Color');
  gl.vertexAttribPointer(a_Color, 3, gl.FLOAT, false, ELEM_BYTES * 5, ELEM_BYTES * 2);
  gl.enableVertexAttribArray(a_Color);

  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.TRIANGLE_STRIP, 0, 6);
});
</script>

