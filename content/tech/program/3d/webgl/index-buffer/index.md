---
title: "WebGL入門 (6) インデックスバッファを使って頂点を使い回す (drawElements)"
url: "/p/wbvv3ha"
date: "2019-09-19"
tags: ["3D", "WebGL"]
weight: 1006
---

インデックスバッファを用いた描画の概要
----

`gl.drawArrays()` による描画には、[3 つの三角形の描画モード](/p/9sxkmma)があり、連なった三角形を描画するときには、`gl.TRIANGLE_STRIP` あるいは `gl.TRIANGLE_FAN` の描画モードを使用すると、頂点情報を使い回しながら効率的に描画することができます。

ただし、図形が少し複雑になってくると、一度の `gl.drawArrays()` 呼び出しではうまく描画できなくなってきます。
例えば次のような 3 つの三角形（トライフォース）を描画することを考えてみます。

{{< image src="index-buffer-001.png" >}}

面 A、B、C は独立した三角形に見えるので、それぞれに 3 つの頂点（合計 9 頂点）を用意して `gl.drawArrays()` の `gl.TRIANGLES` モードでレンダリングすればよさそうですが、頂点 1、2、4 に関しては座標が同じなので、本来であれば上記のように 6 つの頂点情報を用意するだけで足りそうです。
このような場合は、インデックスバッファと [gl.drawElements()](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/drawElements) を使用すると、効率的な描画を行えます。

頂点バッファオブジェクト (VBO) に座標情報を入れておくのは `gl.drawArrays()` を使った場合と同様ですが、もう一つ別のバッファオブジェクトとして、**インデックスバッファオブジェクト (IBO: Index Buffer Object)** を作成します。
IBO には、VBO 内のどの頂点情報を使って図形描画を行うかを示す、頂点インデックスの情報を格納します。

* <b>頂点バッファオブジェクト (VBO)</b>
    - 頂点情報（座標、色など）を重複しないように格納する。
    - 上記の例では、頂点 0～5 の 6 つの頂点情報を格納する。
* <b>インデックスバッファオブジェクト (IBO)</b>
    - 図形描画に VBO 内のどの頂点情報を使うかを示すインデックス配列。
    - 上記の例では、面Aは 0,1,2、面Bは 1,3,4、面Cは 2,4,5 の頂点を使用するという情報。一次元で、0,1,2,1,3,4,2,4,5 と格納すればよい。


実装
----

<center>
<canvas id="canvas-006" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>

ここでは、各頂点に異なる色をつけたトライフォースを描画してみます。

### 頂点バッファオブジェクト (VBO) を作成する

{{< code lang="js" >}}
const vertices = new Float32Array([
   0.0 , 0.5,   1.0, 1.0, 0.0,  // v0 (XYRGB) 黄
  -0.25, 0.0,   0.0, 1.0, 0.0,  // v1 (XYRGB) 緑
   0.25, 0.0,   1.0, 0.0, 0.0,  // v2 (XYRGB) 赤
  -0.5, -0.5,   1.0, 0.0, 1.0,  // v3 (XYRGB) 紫
   0.0, -0.5,   0.0, 1.0, 1.0,  // v4 (XYRGB) シアン
   0.5, -0.5,   1.0, 1.0, 1.0   // v5 (XYRGB) 白
]);
const ELEM_BYTES = vertices.BYTES_PER_ELEMENT;  // = 4

// 頂点バッファオブジェクト (VBO) の作成
const vertexBuffer = gl.createBuffer();
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
{{< /code >}}

頂点バッファオブジェクトの作成方法は、`gl.drawArrays()` で描画する場合と同様です。
ここでは、ひとつのバッファオブジェクト内にインターリーブする形で頂点座標と頂点カラーを格納し、それぞれ `a_Position`、`a_Color` という attribute 変数で 1 つずつ取り出せるように設定しています。


### インデックスバッファオブジェクト (IBO) を作成する

{{< code lang="js" >}}
const indices = new Uint8Array([
  0, 1, 2,  // 面 A を構成する頂点のインデックス
  1, 3, 4,  // 面 B を構成する頂点のインデックス
  2, 4, 5   // 面 C を構成する頂点のインデックス
]);

// インデックスバッファオブジェクト (IBO) の作成
const indexBuffer = gl.createBuffer();
gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);
{{< /code >}}

インデックス情報を格納するためのバッファオブジェクトも、VBO と同様に `gl.createBuffer()` で作成します。
ただし、`gl.bindBuffer()` と `gl.bufferData()` のターゲットには **`gl.ELEMENT_ARRAY_BUFFER`** を指定します（`gl.ARRAY_BUFFER` ではない）。

OpenGL ES の仕様では、インデックスバッファの各要素は、`gl.UNSIGNED_BYTE` 型、あるいは `gl.UNSIGNED_SHORT` 型の値でなければいけないため、JavaScript 側の頂点インデックス配列も **`Uint8Array`** 型（`gl.UNSIGNED_BYTE` に対応）で作成しています。


### gl.drawElements() で描画する

{{< code lang="js" >}}
// キャンバスのクリア
gl.clearColor(0, 0, 0.5, 1.0);
gl.clear(gl.COLOR_BUFFER_BIT);

// インデックスバッファオブジェクト (IBO) のインデックスに従って描画
gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_BYTE, 0);
{{< /code >}}

VBO と IBO の作成が終わったら、後は **`gl.drawElements`** を使って描画を行います。
それぞれの面（三角形）には独立した頂点インデックスが 3 つずつ与えられているため、第１引数 (mode) の描画モードとしては `gl.TRIANGLES` を指定すれば OK です。
第2引数 (count) には頂点インデックスの数（IBO の要素数）、第3引数 (type) には頂点インデックスの型（ここでは `gl.UNSIGNED_BYTE`）、第4引数 (offset) には使用するデータのバイトオフセット（IBO の先頭から参照するなら 0 でよい）を指定します。


全体のコード
----

{{< code lang="html" >}}
<script id="vs-006" type="x-shader/x-vertex">
attribute vec4 a_Position;  // 入力（XY座標）
attribute vec4 a_Color;     // 入力（RGBAカラー）
varying vec4 v_Color;       // 出力（RGBAカラー）

void main() {
  gl_Position = a_Position;
  v_Color = a_Color;
}
</script>

<script id="fs-006" type="x-shader/x-fragment">
precision mediump float;
varying vec4 v_Color;

void main() {
  gl_FragColor = v_Color;
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function() {
  const gl = initGL('canvas-006', 'vs-006', 'fs-006');

  const vertices = new Float32Array([
     0.0 , 0.5,   1.0, 1.0, 0.0,  // v0 (XYRGB) 黄
    -0.25, 0.0,   0.0, 1.0, 0.0,  // v1 (XYRGB) 緑
     0.25, 0.0,   1.0, 0.0, 0.0,  // v2 (XYRGB) 赤
    -0.5, -0.5,   1.0, 0.0, 1.0,  // v3 (XYRGB) 紫
     0.0, -0.5,   0.0, 1.0, 1.0,  // v4 (XYRGB) シアン
     0.5, -0.5,   1.0, 1.0, 1.0   // v5 (XYRGB) 白
  ]);
  const ELEM_BYTES = vertices.BYTES_PER_ELEMENT;  // = 4

  const indices = new Uint8Array([
    0, 1, 2,  // 面 A を構成する頂点のインデックス
    1, 3, 4,  // 面 B を構成する頂点のインデックス
    2, 4, 5   // 面 C を構成する頂点のインデックス
  ]);

  // 頂点バッファオブジェクト (VBO) の作成
  const vertexBuffer = gl.createBuffer();
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

  // インデックスバッファオブジェクト (IBO) の作成
  const indexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);

  // キャンバスのクリア
  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  // インデックスバッファオブジェクト (IBO) のインデックスに従って描画
  gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_BYTE, 0);
});
</script>
{{< /code >}}


<script id="vs-006" type="x-shader/x-vertex">
attribute vec4 a_Position;  // 入力（XY座標）
attribute vec4 a_Color;     // 入力（RGAカラー）
varying vec4 v_Color;       // 出力（RGAカラー）

void main() {
  gl_Position = a_Position;
  v_Color = a_Color;
}
</script>

<script id="fs-006" type="x-shader/x-fragment">
precision mediump float;
varying vec4 v_Color;

void main() {
  gl_FragColor = v_Color;
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function() {
  const gl = initGL('canvas-006', 'vs-006', 'fs-006');

  const vertices = new Float32Array([
     0.0 , 0.5,   1.0, 1.0, 0.0,  // v0 (XYRGB) 黄
    -0.25, 0.0,   0.0, 1.0, 0.0,  // v1 (XYRGB) 緑
     0.25, 0.0,   1.0, 0.0, 0.0,  // v2 (XYRGB) 赤
    -0.5, -0.5,   1.0, 0.0, 1.0,  // v3 (XYRGB) 紫
     0.0, -0.5,   0.0, 1.0, 1.0,  // v4 (XYRGB) シアン
     0.5, -0.5,   1.0, 1.0, 1.0   // v5 (XYRGB) 白
  ]);
  const ELEM_BYTES = vertices.BYTES_PER_ELEMENT;  // = 4

  const indices = new Uint8Array([
    0, 1, 2,  // 面 A を構成する頂点のインデックス
    1, 3, 4,  // 面 B を構成する頂点のインデックス
    2, 4, 5   // 面 C を構成する頂点のインデックス
  ]);

  // 頂点バッファオブジェクト (VBO) の作成
  const vertexBuffer = gl.createBuffer();
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

  // インデックスバッファオブジェクト (IBO) の作成
  const indexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);

  // キャンバスのクリア
  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  // インデックスバッファオブジェクト (IBO) のインデックスに従って描画
  gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_BYTE, 0);
});
</script>


