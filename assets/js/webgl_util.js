/**
 * canvas 要素に描画するための WebGL コンテキストを作成します。
 */
function createWebGLContext(canvasId) {
  const canvas = document.getElementById(canvasId);
  const gl2 = canvas.getContext('webgl2');
  if (gl2) {
    return gl2;
  }
  console.warn('Failed to obtain WebGL 2.0 context. Use 1.0 instead.');

  const gl1 = canvas.getContext('webgl');
  if (!gl1) {
    throw new Error('Failed to obtain WebGL 1.0 context');
  }
  return gl1;
}

/**
 * シェーダーコードをコンパイルしてシェーダーオブジェクトを作成します。
 *
 * @param gl GLコンテキスト
 * @param type 作成するシェーダタイプ (gl.VERTEX_SHADER or gl.FRAGMENT_SHADER)
 * @param source シェーダーのソースコード
 */
function createShader(gl, type, source) {
  const shader = gl.createShader(type);
  if (shader == null) {
    throw new Error('Failed to create a shader');
  }

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  // コンパイルエラーの確認
  const compiled = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
  if (!compiled) {
    const log = gl.getShaderInfoLog(shader);
    gl.deleteShader(shader);
    throw new Error('Failed to compile shader\n' + log);
  }

  return shader;
}

/**
 * シェーダーオブジェクトをリンクしてプログラムオブジェクトを作成します。
 *
 * @param gl  WebGL コンテキスト
 * @param vshader  頂点シェーダーオブジェクト
 * @param fshader  フラグメントシェーダーオブジェクト
 */
function createProgram(gl, vshader, fshader) {
  var program = gl.createProgram();
  if (!program) {
    throw new Error('Failed to create a program object');
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
    gl.deleteProgram(program);
    throw new Error('Failed to link a program\n' + log);
  }
  return program;
}

export function initGL(canvasId, vshaderId, fshaderId) {
  const gl = createWebGLContext(canvasId);
  const vsCode = document.getElementById(vshaderId).textContent;
  const fsCode = document.getElementById(fshaderId).textContent;
  const vs = createShader(gl, gl.VERTEX_SHADER, vsCode);
  const fs = createShader(gl, gl.FRAGMENT_SHADER, fsCode);
  const program = createProgram(gl, vs, fs);
  gl.useProgram(program);

  // プログラムオブジェクトは後から attribute 変数の取得などに
  // 使用するので、プロパティとして参照できるようにしておく。
  gl.program = program;

  return gl;
}

