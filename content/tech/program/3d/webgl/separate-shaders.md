---
title: "WebGL で シェーダーコードを分離する方法"
url: "/p/itkdix9"
date: "2019-08-18"
tags: ["3D", "WebGL"]
---

解決したいこと
----

WebGL で使用する頂点シェーダーとフラグメントシェーダーの GLSL ES コードは単純な文字列データであればよいので、下記のように JavaScript のコードに埋め込んでしまう方法が最初に思いつきます。

{{< code lang="js" >}}
const VSHADER_CODE = `
  void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_PointSize = 10.0;
  }`;

const FSHADER_CODE = `
  void main() {
    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
  }`;
{{< /code >}}

しかし、これだとアプリ側のコードとシェーダーのコードが混ざってしまうので美しくありません。
ここでは、シェーダーコードを分離するいくつかの方法を示します。


script 要素内にシェーダーコードを埋め込む方法
----

下記のような感じで、頂点シェーダーとフラグメントシェーダー用の script 要素を用意して、そこにコードを埋め込んでしまう方法です。

{{< code lang="html" title="HTML 抜粋" >}}
<script id="vshader" type="x-shader/x-vertex">
  void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_PointSize = 10.0;
  }
</script>

<script id="fshader" type="x-shader/x-fragment">
  void main() {
    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
  }
</script>
{{< /code >}}

それぞれのシェーダーコードは、JavaScript コードで下記のようにして取り出すことができます。

{{< code lang="js" >}}
const vsCode = document.getElementById('vshader').textContent;
const fsCode = document.getElementById('fshader').textContent;
{{< /code >}}


import でシェーダーファイルを読み込む方法
----

ECMAScript 2015 (ES6) の [import 構文](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Statements/import) を使用すると、別ファイルとして保存した JavaScript ファイルを読み込むことができます。

{{< code lang="js" >}}
import { VS_CODE, FS_CODE } from 'shaders.js';
{{< /code >}}

例えば、下記のように個別の JS ファイルとしてシェーダーコードを保存しておければ、少なくともアプリ側の JS コードにシェーダーコードが埋もれてしまうことは防ぐことができます。

{{< code lang="js" title="shaders.js" >}}
// Vertex Shader
export const VS_CODE = `
void main() {
  gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
  gl_PointSize = 10.0;
}
`;

// Fragment Shader
export const FS_CODE = `
void main() {
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
`;
{{< /code >}}

`import` を使用するスクリプトの `<script>` タグには、**`type="module"`** を指定しておく必要があります。

{{< code lang="html" >}}
<script type="module">
  import { VS_CODE, FS_CODE } from './shaders.js';

  // ...
  const shader = gl.createShader(gl.VERTEX_SHADER);
  gl.shaderSource(shader, VS_CODE);
  gl.compileShader(shader);
  // ...
</script>
{{< /code >}}

メインスクリプトを JavaScript ファイルに分離するのであれば次ような感じ。

{{< code lang="html" >}}
<script type="module" src="./main.js"></script>
{{< /code >}}

