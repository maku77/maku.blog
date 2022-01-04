---
title: "WebGL入門 (3) バッファーオブジェクトで頂点座標をシェーダーへ送る (VBO)"
url: "/p/ogk9ogw"
date: "2019-09-11"
tags: ["3D", "WebGL"]
weight: 1003
---

<center>
<canvas id="canvas-003" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>


はじめに
----

WebGL (OpenGL) でアプリケーション側 (CPU) のコードから、シェーダー側 (GPU) へ図形の頂点情報を送るには、**バッファーオブジェクト (Buffer Object)** の仕組みを使用します。
WebGL 側に確保したバッファーオブジェクトに、頂点座標の配列データをまとめて書き込んでおいて、最後に描画開始要求を送ることで、CPU と GPU 間の通信回数を抑えることができます。
頂点座標を格納するために確保したバッファーオブジェクトのことを、特に **VBO (Vertext Buffer Object)** と呼んだりします。

WebGL でバッファーオブジェクトを使って図形を描画するまでの流れは以下のようになります。

1. WebGL 側にバッファーオブジェクトを作成する
    - `gl.createBuffer()`
2. バッファーオブジェクトをターゲットとバインドする
    - `gl.bindBuffer()`
3. バッファーオブジェクトに頂点情報を転送する
    - `gl.bufferData()`
4. 頂点シェーダーの attribute 変数でそのデータを1つずつ取り出せるようにする
    - `gl.getAttribLocation()`
    - `gl.vertexAttribPointer()`
    - `gl.enableVertexAttribArray()`
5. 描画要求を送る
    - `gl.drawArrays()`

以下、単純な三角形を描画するまでの手順を示します。


バッファーオブジェクトを作成する
----

まず、WebGL 側に頂点情報を格納するためのバッファーオブジェクトを生成します。
これは、[gl.createBuffer()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/createBuffer) をパラメータなしで呼ぶだけなので簡単です。

{{< code lang="js" >}}
const vertexBuffer = gl.createBuffer();
if (!vertexBuffer) {
  throw Error('Failed to create the buffer object.');
}
{{< /code >}}


バッファーオブジェクトをターゲットにバインドする
----

バッファーオブジェクトを作成したら、[gl.bindBuffer()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/bindBuffer) を使って、そのバッファーオブジェクトをターゲットとバインドします。
ターゲットというのは、そのバッファーオブジェクトをどのような用途で使用するかを示すためのもので、頂点属性のために確保したのであれば、**`gl.ARRAY_BUFFER`** というターゲットとバインドします。
頂点属性というのは、「頂点座標」、「頂点カラー」、「テクスチャ座標」などのことで、ここでは頂点座標だけを扱います。

{{< code lang="js" >}}
gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
{{< /code >}}

{{< note >}}
バインドのイメージとしては、<code>gl.ARRAY_BUFFER</code> というターゲットを、<code>vertexBuffer</code> が示すバッファーオブジェクトを指すように設定すると考えるのがよいです。
というのも、この次に実行する <code>gl.bufferData()</code> は、<code>gl.ARRAY_BUFFER</code> をパラメータに指定することで、バッファーオブジェクトを操作することになるからです。
{{< /note >}}

ターゲットには他にも `gl.ELEMENT_ARRAY_BUFFER` などがあり、これは、頂点配列の中のどのインデックスのデータを使うかを示すインデックス配列です。
何らかのオブジェクトを描画する場合、通常は、1 つの頂点が複数の面を構成するために繰り返し使用されます。
そのような場合に、頂点データを使いまわすことでメモリ効率を上げることができます。
`gl.ELEMENT_ARRAY_BUFFER` を使った描画に関しては、別の記事で説明します。


バッファーオブジェクトに頂点情報を転送する
----

次に、アプリケーション側 (CPU側）で用意した座標の配列を、[gl.bufferData()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/bufferData) で GPU 側のバッファーオブジェクトに転送します。

座標配列は、JavaScript の配列として用意するのですが、汎用の `Array` 型ではなく Typed Array のひとつである [Float32Array](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/Float32Array) を使用します。
頂点データ（座標や色）などの配列を扱う場合、すべての要素が同一の型であることを前提にして作成された Typed Array を使用した方が効率がよいからです。

- [WebGL で使う Typed Array](/p/twmzpfj)

{{< code lang="js" >}}
const vertices = new Float32Array([
  0.0, 0.5,    // 1つ目の頂点座標
  -0.5, -0.5,  // 2つ目の頂点座標
  0.5, -0.5    // 3つ目の頂点座標
]);

gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
{{< /code >}}

第1引数には、バッファーオブジェクトではなく、ターゲット名（ここでは `gl.ARRAY_BUFFER`）を指定するところがちょっとわかりにくいところですね。
第2引数には転送する頂点データを格納した Typed Array オブジェクトを指定します。

第3引数は、この頂点データをどのように使用するかのパラメータで、WebGL が効率的に描画を行うためのヒント情報として使用されます（実際にどう活用されるかは WebGL の実装によります）。
ヒント情報としては下記のような値を指定できます（WebGL2 ではさらに増えています）。

- `gl.STATIC_DRAW`: バッファーの内容はアプリから一度だけ設定され、描画するためにたくさん参照されます。
- `gl.STREAM_DRAW`: バッファーの内容はアプリから一度だけ設定され、描画するためにいくらか参照されます。
- `gl.DYNAMIC_DRAW`: バッファーの内容はアプリから何度も設定され、描画するためにたくさん参照されます。


頂点シェーダーの attribute 変数でデータを1つずつ取り出せるようにする
----

バッファーオブジェクトに頂点座標の配列をセットしたら、頂点シェーダーからその座標値を1つずつ取り出せるようにする必要があります。
頂点シェーダーは、バッファーオブジェクトに設定した頂点 1 つごとに 1 回呼び出されることになります。

ここでは、頂点シェーダーを下記のように実装し、`a_Position` という名前の attribute 変数経由で座標値を 1 つずつ取り出すように実装します（`a_` プレフィックスは attribute を表します）。
`gl_Position` に代入した値が、その頂点の最終的な座標になります。

{{< code lang="c" title="Vertex shader" >}}
attribute vec4 a_Position;

void main() {
  gl_Position = a_Position;
}
{{< /code >}}

ピクセル毎に呼び出されるフラグメントシェーダーは、下記のように単色（ここでは緑）で描画するようにしておきます。

{{< code lang="c" title="Fragment shader" >}}
void main() {
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // RGBA
}
{{< /code >}}

アプリ側のコードでは、頂点シェーダーが attribute 変数経由でバッファーオブジェクト内の頂点データを取り出せるように設定します。
まず、[gl.getAttribLocation()](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/getAttribLocation) で、attribute 変数にアクセスするためのハンドルを取得します。

{{< code lang="js" >}}
const a_Position = gl.getAttribLocation(gl.program, 'a_Position');
{{< /code >}}

ここで、第1引数にプログラムオブジェクトが必要になりますが、ここでは、`gl.program` というプロパティにあらかじめ格納されているとします。

次に、[gl.vertexAttribPointer()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/vertexAttribPointer) を使い、**現在 `gl.ARRAY_BUFFER` にバインドされているバッファーオブジェクト**に、頂点情報がどのようなレイアウトで格納されているかを設定します。つまり、シェーダー側に、attribute 変数経由でどのような単位でデータを取り出していけばよいかを知らせます。バッファーオブジェクトに設定した頂点情報は、構造化されていない一次元の配列（バイナリ）なので、このような情報が必要になります。

{{< code lang="js" >}}
gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);
{{< /code >}}

第2引数 (size) では、頂点あたりの次元数（頂点毎に消費する配列要素数）を指定します。
ここでは、X座標とY座標のペアを格納しているので、2 と指定します。
有効な値は 1～4 です。

第3引数 (type) では、各要素の型を指定します。
ここでは、`Float32Array` で頂点座標をセットしているので、対応する WebGL の型として `gl.FLOAT` を指定しています。

第4引数 (normalized) では、整数データを浮動小数点数に型変換するときに値の範囲を -1.0～1.0 に正規化するかを指定します。ただし、セットするデータがもともと浮動小数点数の場合は正規化は行われないので、ここは `false` にしておけば OK です。

第5引数 (stride) は、配列データ内に頂点座標、テクスチャ座標、頂点カラーといった複数種の属性を交互に詰めている（インターリーブされている）場合の、1 頂点あたりの使用バイト数を指定します。
最後の第6引数 (offset) は、最初の要素が、バッファー内のどこから始まるかを示すバイトオフセット値です。
ここでは、頂点座標だけを格納しているので、第5引数と第6引数は両方とも 0 を指定しておけば OK です。
インターリーブされたデータを扱う方法は下記の記事を参考にしてください。

- 参考: [頂点属性をインターリーブしてバッファオブジェクトに格納する](/p/wqjqbpi)

最後に、[gl.enableVertexAttribArray()](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/enableVertexAttribArray) を呼び出して、attribute 変数に割り当てたバッファーオブジェクトを有効にします。

{{< code lang="js" >}}
gl.enableVertexAttribArray(a_Position);
{{< /code >}}

ここまで設定が終わると、頂点シェーダーの attribute 変数経由で、バッファオブジェクト内の頂点データを 1 つずつ取り出せるようになります。

{{< note >}}
<code>gl.enableVertexAttribArray()</code> を実行した時点で、その attribute 変数はバッファオブジェクトからの入力専用になるため、<code>gl.vertexAttrib[1234]f()</code> などの関数で個々の頂点データを attribute 変数に直接渡すことはできなくなります。
<code>gl.disableVertexAttribArray()</code> すれば再び渡せるようになります。
{{< /note >}}


描画要求を送る
----

バッファーオブジェクトへの頂点データの格納と、attribute 変数の設定を完了したら、あとは [gl.drawArrays()](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/drawArrays) を呼び出すことで、実際に頂点データを使った描画が行われます。

{{< code lang="js" >}}
gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
{{< /code >}}

第1引数 (mode) では描画するプリミティブのタイプ（ここでは一つの三角形なので、`gl.TRIANGLE` でも `gl.TRIANGLE_STRIP` でも OK）を指定します。

第2引数 (first) では、頂点配列の開始インデックスを指定します。

第3引数 (count) では、使用する頂点の数を指定します。
ここでは、3 つの頂点を使うので、3 を指定しています。
頂点シェーダーは、3 回実行されることになります。


全体のソースコード
----

{{< code lang="html" >}}
<script id="vs" type="x-shader/x-vertex">
attribute vec4 a_Position;
void main() {
  gl_Position = a_Position;
}
</script>

<script id="fs" type="x-shader/x-fragment">
void main() {
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // RGBA
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function () {
  const gl = initGL('canvas', 'vs', 'fs');

  const vertices = new Float32Array([
    0.0, 0.5,    // 1つ目の頂点座標
    -0.5, -0.5,  // 2つ目の頂点座標
    0.5, -0.5    // 3つ目の頂点座標
  ]);

  const vertexBuffer = gl.createBuffer();
  if (!vertexBuffer) {
    throw Error('Failed to create the buffer object.');
  }
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

  const a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(a_Position);

  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
});
</script>
{{< /code >}}

上記でインポートしている `webgl_util.js` は、プログラムオブジェクトを作成するユーティリティ関数 `initGL()` を定義しています。
プログラムオブジェクトの作成に関しては、[こちら](/p/8s4uhzv)を参照してください。


<script id="vs-003" type="x-shader/x-vertex">
attribute vec4 a_Position;
void main() {
  gl_Position = a_Position;
}
</script>

<script id="fs-003" type="x-shader/x-fragment">
void main() {
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // RGBA
}
</script>

<script type="module">
import { initGL } from '/assets/js/webgl_util.js';

window.addEventListener('load', function () {
  const gl = initGL('canvas-003', 'vs-003', 'fs-003');

  const vertices = new Float32Array([
    0.0, 0.5,    // 1つ目の頂点座標
    -0.5, -0.5,  // 2つ目の頂点座標
    0.5, -0.5    // 3つ目の頂点座標
  ]);

  const vertexBuffer = gl.createBuffer();
  if (!vertexBuffer) {
    throw Error('Failed to create the buffer object.');
  }
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

  const a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(a_Position);

  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
});
</script>

