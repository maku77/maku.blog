---
title: "Electron: 処理が終わるまでボタンを無効状態 (disabled) にする"
linkTitle: "処理が終わるまでボタンを無効 (disabled) にする"
url: "/p/xkxbnza"
date: "2020-06-29"
tags: ["Electron"]
---

button 要素の disabled プロパティ
----

JavaScript で `button` 要素の __`disabled`__ プロパティを true に設定すると、ボタンを無効状態 (disabled) にすることができます。
TypeScript を使っている場合は、`document.querySelector()` の戻り値を __`HTMLButtonElement`__ にキャストすることで `disabled` プロパティを参照できるようになります。

{{< code lang="typescript" title="button 要素を disabled にする" >}}
const btn = document.querySelector('#btn') as HTMLButtonElement;
btn.disabled = true;
{{< /code >}}

ボタンを押したときに重い処理（データ取得など）を実行する場合、処理中にボタンを無効化することで、ユーザーによるボタンの連打を防ぐことができます。


より実践的なサンプル
----

下記は Electron アプリの実装サンプル（レンダラープロセスの抜粋）です。
ボタンを押したときにボタンを無効化すると同時に Web からデータ取得します。
そして、データ取得が完了するか、エラーが発生したときにボタンを有効化します。

ここでは、HTTP GET リクエストを送るために `superagent` モジュールを使用しています。

{{< code title="superagent のインストール" >}}
$ npm install --save superagent
$ npm install --save @types/superagent
{{< /code >}}

{{< code lang="typescript" title="renderer.ts" >}}
import * as superagent from 'superagent';

const btn = document.querySelector('#btn') as HTMLButtonElement;

btn.addEventListener('click', async () => {
  btn.disabled = true;  // データ取得前にボタンを無効化
  try {
    const res = await superagent.get('https://example.com/');
    console.log(res.text);
  } catch (err) {
    alert(err);
  } finally {
    btn.disabled = false;  // データ取得後にボタンを有効化
  }
});
{{< /code >}}

HTML ファイルの方にはボタンだけ配置しておけば OK です。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>Hello Electron!</title>
    <!-- https://electronjs.org/docs/tutorial/security#csp-meta-tag -->
    <meta http-equiv="Content-Security-Policy"
          content="script-src 'self' 'unsafe-inline';" />
  </head>
  <body>
    <button id="btn">データ取得</button>
    <script>require('./build/renderer')</script>
  </body>
</html>
{{< /code >}}

