---
title: "WebGL入門 (1) WebGL コンテキストの取得"
url: "/p/46jrh68"
date: "2019-08-16"
tags: ["3D", "WebGL"]
weight: 1001
---

<center>
<canvas id="canvas-001" width="300" height="200">
このブラウザは canvas タグに対応していません。
</canvas>
</center>

上記の canvas 要素は、WebGL により描画しています。
サーフェスをクリアしているだけなので、何も表示されていないのは正しいです。

WebGL のコンテキストを取得する
----

WebGL は、Web ブラウザ上で OpenGL の機能を使用する仕組みです。
HTML の `canvas` 要素をレンダリング用のサーフェストとして使用します。
`canvas` 要素から WebGL のコンテキスト [WebGLRenderingContext](https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext) を取得したら、後は OpenGL ES を用いたアプリと同じ感覚で API を呼び出していけます。

下記は、WebGL でサーフェスをクリアするだけの最低限のコードです。
`gl.clearColor()` でクリア色を設定しています。

{{< code lang="html" title="HTML" >}}
<canvas id="canvas" width="300" height="200"></canvas>
{{< /code >}}

{{< code lang="js" title="JavaScript" >}}
function main() {
  const canvas = document.getElementById('canvas');
  const gl = canvas.getContext('webgl');
  if (!gl) {
    console.error('Failed to obtain WebGL context');
    return;
  }

  gl.clearColor(0, 0, 0.5, 1.0);  // RGBA（暗い青）
  gl.clear(gl.COLOR_BUFFER_BIT);
}

window.addEventListener('load', main);
{{< /code >}}


WebGL 2.0 を使用する
----

WebGL 2.0 の API を使用したい場合は、`canvas.getContext()` のパラメータを `webgl` から `webgl2` に変更し、[WebGL2RenderingContext](https://developer.mozilla.org/en-US/docs/Web/API/WebGL2RenderingContext) を取得します。

{{< code lang="js" >}}
const gl = canvas.getContext('webgl');    //=> WebGLRenderingContext
const gl2 = canvas.getContext('webgl2');  //=> WebGL2RenderingContext
{{< /code >}}

WebGL は OpenGL ES 2.0 をベースにしており、WebGL2 は OpenGL ES 3.0 をベースにしています。
メジャーなブラウザは WebGL 2 に対応しているので、特に理由がなければ WebGL 2 の方を使っていきたいですね。

下記のようにすれば、WebGL 2.0 が使える場合はそちらを、使えない場合は WebGL 1.0 のコンテキストを取得できます。

{{< code lang="js" >}}
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
{{< /code >}}


<script>
function main() {
  const canvas = document.getElementById('canvas-001');
  const gl = canvas.getContext('webgl');
  if (!gl) {
    console.error('Failed to obtain WebGL context');
    return;
  }

  gl.clearColor(0, 0, 0.5, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);
}

window.addEventListener('load', main);
</script>

