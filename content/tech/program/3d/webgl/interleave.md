---
title: "WebGL入門 (4) 頂点属性をインターリーブしてバッファオブジェクトに格納する"
url: "/p/wqjqbpi"
date: "2019-09-13"
tags: ["3D", "WebGL"]
weight: 1004
---

<center>
<canvas id="canvas-004" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>

インターリーブとは
----

インターリーブ (interleave) とは、一般的なコンピュータ用語で、**データを交互に配置していくこと**を示します。
例えば、3D モデルの各頂点の「XY座標」「RGBカラー」をインターリーブして配列に詰めると、3 つの頂点の情報は、

{{< shout "X Y R G B X Y Z G B X Y R G B ..." >}}

のようにレイアウトされることになります。

WebGL (OpenGL) では、このようにインターリーブされた頂点属性データを扱えるようになっています。
インターリーブする主な頂点属性は、頂点座標 (XYZ)、テクスチャ座標 (UV)、頂点カラー (RGBA) などです。

ここでは、頂点座標 (XY) と頂点カラー (RGB) をインターリーブした配列を JavaScript 側の `Float32Array` として作成し、それを WebGL 側のバッファオブジェクトに詰める方法を示します。


シェーダー側の実装
----

まずは、頂点シェーダーとフラグメントシェーダーを用意します。

{{< code lang="c" title="頂点シェーダー" >}}
attribute vec4 a_Position;  // 入力（XY座標）
attribute vec4 a_Color;     // 入力（RGAカラー）
varying vec4 v_Color;       // 出力（RGAカラー）

void main() {
  gl_Position = a_Position;
  v_Color = a_Color;
}
{{< /code >}}

頂点シェーダーでは、バッファオブジェクトから XY 座標を取り出すための attribute 変数 `a_Position` と、RGB カラーを取り出すための `a_Color` を定義します。
カラー値は最終的にはフラグメントシェーダー側で必要になるので、varying 変数の `v_Color` を介して値をフラグメントシェーダーへ渡します。

{{< code lang="c" title="フラグメントシェーダー" >}}
precision mediump float;  // float のデフォルト精度（中）
varying vec4 v_Color;     // 頂点シェーダーからの入力

void main() {
  gl_FragColor = v_Color;
}
{{< /code >}}

フラグメントシェーダー側では、まず float の精度を設定しておかなければならないので、ここでは中精度 (`mediump`) に設定しています。
実際に使用している変数の型は `vec4` ですが、これは `float` を 4 つセットにしたものなので、`float` の精度だけ設定しておけば OK です。

頂点シェーダー側と同じ名前で定義した varying 変数 `v_Color` からは、頂点シェーダー側で代入されたカラー値を受け取ることができます。
実際には、各頂点の中間に位置するフラグメントでは、補間されたカラー値が取り出されます（なのでこのサンプルではグラデーションになって見えます）。

あとは、この値を `gl_FragColor` に代入すれば、それぞれのフラグメントに異なる色の付いた図形が描画されます。

{{< note >}}
varying 変数は、頂点シェーダーで設定した値と、フラグメントシェーダー側で取り出される値が変わってくるので varying「変わる」という名前が付けられたそうですが、新しいシェーダー言語仕様では in/out に名前が変わっています。確かに最初に varying というキーワードを見た人にはほとんど意味が伝わらないと思いますので、不評だったんでしょうね。
{{< /note >}}


アプリ側の実装
----

アプリ (CPU) 側では、まず、インターリーブされた配列データを用意します（ここではハードコーディングしていますが、複雑なモデルの場合は glTF 形式などの外部ファイルを使用します）。

{{< code lang="js" >}}
const vertices = new Float32Array([
   0.0,  0.5, 1.0, 0.0, 0.0,  // 1つ目の頂点座標 + RGB
  -0.5, -0.5, 0.0, 1.0, 0.0,  // 2つ目の頂点座標 + RGB
   0.5, -0.5, 0.0, 0.0, 1.0   // 3つ目の頂点座標 + RGB
]);

const ELEM_BYTES = vertices.BYTES_PER_ELEMENT;  // 4
{{< /code >}}

`ELEM_BYTES` は、配列の 1 要素当たりのバイト数で、ここでは float 型の値なので 4 になります。
この値は後ほど、WebGL の API を呼び出すときに使います。

次に、頂点情報を格納するためのバッファオブジェクトを WebGL 側に生成し、上記の配列データを転送します。
座標値とカラー値をインターリーブしているので、バッファオブジェクトは 1 つだけで済みます。

{{< code lang="js" >}}
const vertexBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
{{< /code >}}

**次の処理が重要**で、インターリーブされた座標値とカラー値を、異なる attribute 変数（`a_Position` と `a_Color`）で取り出せるように設定します。

{{< code lang="js" >}}
// 座標値 (XY) を取り出すための attribute 変数を設定
const a_Position = gl.getAttribLocation(gl.program, 'a_Position');
gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, ELEM_BYTES * 5, 0);
gl.enableVertexAttribArray(a_Position);

// カラー値 (RGB) を取り出すための attribute 変数を設定
const a_Color = gl.getAttribLocation(gl.program, 'a_Color');
gl.vertexAttribPointer(a_Color, 3, gl.FLOAT, false, ELEM_BYTES * 5, ELEM_BYTES * 2);
gl.enableVertexAttribArray(a_Color);
{{< /code >}}

インターリーブされた情報を扱うには、`gl.vertexAttributePointer()` の第5引数 (stride) と第6引数 (offset) をうまく設定する必要があります。

第5引数 (stride) では、1 頂点あたりの使用バイト数を指定します。
ここでは、1 頂点につき 5 要素 (X,Y,R,G,B) があり、それぞれ float の 4 バイト (= ELEM_BYTES) を使用するので、20 (`ELEM_BYTES * 5`) を指定しています。
配列の要素数 (5) ではなく、バイト数で指定しないといけないことに注意してください。

最後の第6引数 (offset) は、取り出すべき最初の要素が、バッファー内のどこから始まるかを示すバイトオフセット値です。
座標値の場合は先頭から始まっているので 0 を指定し、カラー値の場合は、その前に配置された 2 つの座標値分だけオフセットして取り出したいので、`ELEM_BYTES * 2` を指定しています。

ここで指定されたオフセット位置から、stride パラメータのバイト数ずつずらしながら情報が取り出されていくことになります。

{{< note >}}
座標値は 2 要素 (XY)、カラー値は 3 要素 (RGB) で格納しているのに、シェーダー側では共に <code>vec4</code> 型で取り出していることに気が付いたかもしれません。
4 要素に満たないデータを <code>vec4</code> で取り出そうとすると、3 要素目に 0.0、4 要素目に 1.0 というデフォルト値が格納されます。
このデフォルト値は、<code>vec4</code> 変数を座標値として扱う場合も、RGBA 値として扱う場合も都合がよい値になっています。
{{< /note >}}

最後に、`gl.drawArrays()` で描画命令を WebGL に送ってやれば、バッファーオブジェクトに渡した頂点情報を元に描画が行われます。

{{< code lang="js" >}}
gl.clearColor(0, 0, 0.5, 1.0);
gl.clear(gl.COLOR_BUFFER_BIT);
gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
{{< /code >}}


<script id="vs-004" type="x-shader/x-vertex">
attribute vec4 a_Position;  // 入力（XY座標）
attribute vec4 a_Color;     // 入力（RGAカラー）
varying vec4 v_Color;       // 出力（RGAカラー）

void main() {
  gl_Position = a_Position;
  v_Color = a_Color;
}
</script>

<script id="fs-004" type="x-shader/x-fragment">
precision mediump float;
varying vec4 v_Color;

void main() {
  gl_FragColor = v_Color;
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function() {
  const gl = initGL('canvas-004', 'vs-004', 'fs-004');

  const vertices = new Float32Array([
    0.0, 0.5, 1.0, 0.0, 0.0,    // 1つ目の頂点座標 + RGB
    -0.5, -0.5, 0.0, 1.0, 0.0,  // 2つ目の頂点座標 + RGB
    0.5, -0.5, 0.0, 0.0, 1.0    // 3つ目の頂点座標 + RGB
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
  gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
});
</script>

