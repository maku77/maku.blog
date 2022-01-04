---
title: "WebGL入門 (2) シェーダーを使用する"
url: "/p/8s4uhzv"
date: "2019-08-17"
tags: ["3D", "WebGL"]
weight: 1002
---

{{% private %}}
- 参考コード: https://github.com/yukoba/WebGLBook/blob/master/lib/cuon-utils.js
{{% /private %}}

<center>
<canvas id="canvas-002" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>

上記の canvas 要素は、WebGL により描画しています。
canvas の中央に 1 つの点を表示しています。


シェーダーを使うまでの手順
----

WebGL は OpenGL ES 2.0、WebGL 2.0 は OpenGL ES 3.0 をベースに策定されているため、プリミティブをレンダリングするには、どちらも GLSL ES 言語で記述したシェーダーコード（頂点シェーダー、フラグメントシェーダー）を用意する必要があります。

シェーダーを使用して描画するまでの流れは下記のようになります。

1. シェーダーオブジェクトの準備（頂点シェーダーとフラグメントシェーダー）
    1. シェーダーオブジェクトを作成する ([gl.CreateShader](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/createShader))
    2. シェーダーコードを設定する ([gl.ShaderSource](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/shaderSource))
    3. シェーダーコードをコンパイルする ([gl.CompileShader](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/compileShader))
    4. 必要があればコンパイル結果のチェック ([gl.getShaderParameter](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/getShaderParameter))
    5. 必要があればコンパイルエラーの内容を出力 ([gl.getShaderInfoLog](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/getShaderInfoLog))
2. プログラムオブジェクトの準備
    1. プラグラムオブジェクトを作成する ([gl.CreateProgram](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/createProgram))
    2. 2 つのシェーダーオブジェクトをアタッチする ([gl.AttachShader](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/attachShader))
    3. 2 つのシェーダーオブジェクトをリンクする ([gl.LinkProgram](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/linkProgram))
    4. 必要があればリンク結果のチェック ([gl.getProgramParameter](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/getProgramParameter))
    5. 必要があればリンクエラーの内容を出力 ([el.getProgramInfoLog](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/getProgramInfoLog))
3. レンダリング前に使用するプログラムオブジェクトを選択する ([gl.UseProgram](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/useProgram))
4. 頂点情報などを用意して描画 ([gl.drawArrays](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/drawArrays))


ユーティリティ関数の作成
----

### シェーダーオブジェクトの準備 (createShader)

指定したシェーダーコードをコンパイルしてシェーダーオブジェクトを作成する `createShader` 関数を作成します。

{{< code lang="js" >}}
/**
 * シェーダーコードをコンパイルしてシェーダーオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl  WebGL コンテキスト
 * @param type  gl.VERTEX_SHADER あるいは gl.FRAGMENT_SHADER
 * @param source  シェーダーのソースコード
 */
function createShader(gl, type, source) {
  const shader = gl.createShader(type);
  if (shader == null) {
    console.error('Failed to create a shader');
    return null;
  }

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  // コンパイル結果を検査する
  const compiled = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
  if (!compiled) {
    var log = gl.getShaderInfoLog(shader);
    console.error('Failed to compile a shader\n' + log);
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}
{{< /code >}}

この関数を使って、頂点シェーダーとフラグメントシェーダーをコンパイルします。

{{< code lang="js" >}}
const vshader = createShader(gl, gl.VERTEX_SHADER, vshaderCode);
const fshader = createShader(gl, gl.FRAGMENT_SHADER, fshaderCode);
{{< /code >}}


### プログラムオブジェクトの準備 (createProgram)

作成した 2 つのシェーダーを実際に使用するには、プログラムオブジェクトを作成してリンクする必要があります。
シェーダーオブジェクトからプログラムオブジェクトを作成する `createProgram` 関数を作成します。

{{< code lang="js" >}}
/**
 * シェーダーオブジェクトをリンクしてプログラムオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl  WebGL コンテキスト
 * @param vshader  頂点シェーダーオブジェクト
 * @param fshader  フラグメントシェーダーオブジェクト
 */
function createProgram(gl, vshader, fshader) {
  var program = gl.createProgram();
  if (!program) {
    return null;
  }

  gl.attachShader(program, vshader);
  gl.deleteShader(vshader);
  gl.attachShader(program, fshader);
  gl.deleteShader(fshader);
  gl.linkProgram(program);

  // リンクエラーの確認
  var linked = gl.getProgramParameter(program, gl.LINK_STATUS);
  if (!linked) {
    var log = gl.getProgramInfoLog(program);
    console.error('Failed to link a program\n' + log);
    gl.deleteProgram(program);
    return null;
  }
  return program;
}
{{< /code >}}

上記コードで `gl.attachShader` の直後に `gl.deleteShader` を呼び出していますが、このタイミングでは削除フラグが立つだけです。シェーダーオブジェクトは、プログラムにアタッチされている状態では削除されない仕様なので、これは正しく動作します。

これで、下記のようにしてプログラムオブジェクトを作成することができます。

{{< code lang="js" >}}
const vshader = createShader(gl, gl.VERTEX_SHADER, vshaderCode);
const fshader = createShader(gl, gl.FRAGMENT_SHADER, fshaderCode);
const program = createProgram(gl, vshader, fshader);
{{< /code >}}

これは定型処理になるので、直接シェーダーコードを渡してプログラムオブジェクトを作成してしまうユーティリティ関数を用意しておくのもよいですね。

{{< code lang="js" >}}
/**
 * 頂点シェーダーとフラグメントシェーダーのソースコードから
 * プログラムオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl  WebGL コンテキスト
 * @param vshaderCode  頂点シェーダーのソースコード
 * @param fshaderCode  フラグメントシェーダーのソースコード
 */
function createProgramFromCode(gl, vshaderCode, fshaderCode) {
  const vshader = createShader(gl, gl.VERTEX_SHADER, vshaderCode);
  if (!vshader) {
    return null;
  }

  const fshader = createShader(gl, gl.FRAGMENT_SHADER, fshaderCode);
  if (!fshader) {
    gl.deleteShader(vshader);
    return null;
  }

  return createProgram(gl, vshader, fshader);
}
{{< /code >}}

これで、下記のようにしてシェーダーのコードから直接プログラムオブジェクトを作成できます。

{{< code lang="js" >}}
const program = createProgramFromCode(gl, vshaderCode, fshaderCode);
{{< /code >}}


main プログラムの作成
----

シェーダーオブジェクトやプログラムオブジェクトを作成するユーティリティ関数の作成が終わったので、後はメインプログラムを作成します。
頂点シェーダーとフラグメントシェーダーのソースコードは、下記のように、直接 JavaScript でハードコーディングします。

{{< code lang="js" >}}
const VSHADER_CODE = `
  void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);  // 画面中央
    gl_PointSize = 10.0;  // 頂点サイズ
  }`;

const FSHADER_CODE = `
  void main() {
    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // RGBA（緑色）
  }`;
{{< /code >}}

ここでは、頂点を 1 つだけ表示するために、座標は画面中央 (0, 0, 0) に決め打ち、色も緑色で決め打ちにしています。

後は、[gl.drawArrays](https://developer.mozilla.org/ja/docs/Web/API/WebGLRenderingContext/drawArrays) 関数を使って、頂点を 1 つ描画する命令を発行すれば WebGL によるレンダリングが行われます。

{{< code lang="js" >}}
function render(gl) {
  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.POINTS, 0, 1);
}

function main() {
  const canvas = document.getElementById('canvas');
  const gl = canvas.getContext('webgl2');
  if (!gl) {
    console.error('Failed to obtain WebGL 2.0 context');
    return;
  }

  // プログラムオブジェクトの設定
  const program = createProgramFromCode(gl, VSHADER_CODE, FSHADER_CODE);
  gl.useProgram(program);

  render(gl);
}

window.addEventListener('load', main);
{{< /code >}}


<script>
/**
 * シェーダーコードをコンパイルしてシェーダーオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl GLコンテキスト
 * @param type 作成するシェーダタイプ (gl.VERTEX_SHADER or gl.FRAGMENT_SHADER)
 * @param source シェーダーのソースコード
 */
function createShader(gl, type, source) {
  const shader = gl.createShader(type);
  if (shader == null) {
    console.error('Failed to create a shader');
    return null;
  }

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  // コンパイルエラーの確認
  const compiled = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
  if (!compiled) {
    const log = gl.getShaderInfoLog(shader);
    console.error('Failed to compile shader\n' + log);
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}

/**
 * シェーダーオブジェクトをリンクしてプログラムオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl  WebGL コンテキスト
 * @param vshader  頂点シェーダーオブジェクト
 * @param fshader  フラグメントシェーダーオブジェクト
 */
function createProgram(gl, vshader, fshader) {
  var program = gl.createProgram();
  if (!program) {
    return null;
  }

  gl.attachShader(program, vshader);
  gl.deleteShader(vshader);
  gl.attachShader(program, fshader);
  gl.deleteShader(fshader);
  gl.linkProgram(program);

  // リンクエラーの確認
  var linked = gl.getProgramParameter(program, gl.LINK_STATUS);
  if (!linked) {
    var log = gl.getProgramInfoLog(program);
    console.error('Failed to link a program\n' + log);
    gl.deleteProgram(program);
    return null;
  }
  return program;
}

/**
 * 頂点シェーダーとフラグメントシェーダーのソースコードから
 * プログラムオブジェクトを作成します。
 * 作成に失敗した場合は null を返します。
 *
 * @param gl  WebGL コンテキスト
 * @param vshaderCode  頂点シェーダーのソースコード
 * @param fshaderCode  フラグメントシェーダーのソースコード
 */
function createProgramFromCode(gl, vshaderCode, fshaderCode) {
  const vshader = createShader(gl, gl.VERTEX_SHADER, vshaderCode);
  if (!vshader) {
    return null;
  }

  const fshader = createShader(gl, gl.FRAGMENT_SHADER, fshaderCode);
  if (!fshader) {
    gl.deleteShader(vshader);
    return null;
  }

  return createProgram(gl, vshader, fshader);
}

const VSHADER_CODE = `
  void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_PointSize = 10.0;
  }`;

const FSHADER_CODE = `
  void main() {
    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  // RGBA（緑色）
  }`;

function render(gl) {
  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.drawArrays(gl.POINTS, 0, 1);
}

function main() {
  const canvas = document.getElementById('canvas-002');
  const gl = canvas.getContext('webgl2');
  if (!gl) {
    console.error('Failed to obtain WebGL 2.0 context');
    return;
  }

  // プログラムオブジェクトの設定
  const program = createProgramFromCode(gl, VSHADER_CODE, FSHADER_CODE);
  gl.useProgram(program);

  render(gl);
}

window.addEventListener('load', main);
</script>

