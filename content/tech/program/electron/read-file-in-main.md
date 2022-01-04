---
title: "Electron: レンダラーからの要求でメインプロセスでファイルを読み込んで返す"
linkTitle: "レンダラーからの要求でメインプロセスでファイルを読み込んで返す"
url: "/p/aq2airz"
date: "2020-06-29"
tags: ["Electron"]
---

Electron アプリのメインプロセスで使用できる __`ipcMain.handle`__ を使うと、レンダラーから呼び出せる関数のようなものを定義できます。
正確には、特定の名前のチャネル (channel) へのメッセージをハンドルするコールバック関数を設定します。
次の例では、メインプロセスで `read-textfile` メッセージを受診したときに、指定されたファイルを読み込んでその内容を返すように実装しています。

{{< code lang="typescript" title="main.js（メインプロセス）" >}}
import { app, ipcMain, BrowserWindow } from 'electron';
import * as fs from 'fs';

// ...

// 指定されたテキストファイルを読みこんで、その内容を返します。
ipcMain.handle('read-textfile', async (event, filename) => {
  const buf = await fs.promises.readFile(filename);
  return buf.toString();
});
{{< /code >}}

一方、レンダラー側では __`ipcRenderer.invoke`__ を使って、あたかも関数呼び出しのようにメインプロセス側の処理を呼び出すことができます。
次の例では、画面上のボタンを押した時にメインプロセスに `read-textfile` メッセージを送り、その戻り値を表示しています。

{{< code lang="typescript" title="renderer.js" >}}
import { ipcRenderer } from 'electron';

const btn = document.querySelector('#btn')!;
const output = document.querySelector('#output')!;

btn.addEventListener('click', () => {
  ipcRenderer.invoke('read-textfile', './hoge.txt')
    .then((text) => {
      output.innerHTML = text;
    })
    .catch((err) => {
      alert(err);
    });
});
{{< /code >}}

HTML コンテンツの内容は次のような感じになります。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>Hello Electron!</title>
    <!-- https://electronjs.org/docs/tutorial/security#csp-meta-tag -->
    <meta http-equiv="Content-Security-Policy" content="script-src 'self' 'unsafe-inline';" />
  </head>
  <body>
    <button id="btn">ファイル読み込み</button>
    <div id="output"></div>
    <script>require('./build/renderer')</script>
  </body>
</html>
{{< /code >}}

