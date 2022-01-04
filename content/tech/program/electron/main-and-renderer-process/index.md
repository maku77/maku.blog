---
title: "Electron のメインプロセスとレンダラープロセスの関係"
url: "/p/mkisgqb"
date: "2020-07-02"
tags: ["Electron"]
---

メインプロセスとレンダラープロセス
----

Electron アプリを作成するときは、メインプロセスとレンダラープロセスを意識して使い分ける必要があります。

{{< image w="600" src="img-001.png" >}}

レンダラープロセス側で呼び出し可能な Electron モジュール（他の Node モジュールも含む）は制限されていて、実行できる JavaScript ライブラリは、Web ブラウザ上で実行可能な JavaScript に毛が生えたものくらいものと考えておくのがよいです。

__`ipcRenderer`__ モジュールは例外的にレンダラープロセスから使ってもよいとされているモジュールのひとつで、これ経由でメインプロセスに対して要求（メッセージ）を送ることができます。
逆にメインプロセスからのメッセージをハンドルすることもできます。


通知先のレンダラープロセスの指定
----

メインプロセスに対して、レンダラープロセスは複数存在することがあるので、メインプロセスからメッセージを送るときは、どのレンダラープロセスへのメッセージなのかを意識する必要があります。

### レンダラープロセスからのメッセージに応答する

レンダラープロセスからのイベントを __`ipcMain.on()`__ でハンドルする場合、コールバック関数の第1パラメータとして渡される __`event: Electron.IpcMainEvent`__ オブジェクトから送信元のレンダラープロセス (`sender: Electron.WebContents`) を参照することができます。

下記のメインプロセスは、`my-add` イベントとしてレンダラープロセスから 2 つの数値を受け取り、__`IpcMainEvent.reply()`__ を使って送信元のレンダラープロセスに応答メッセージ（数値を足した結果）を返しています。

{{< code lang="typescript" title="main.ts（メインプロセス）" >}}
// レンダラープロセスからメッセージを受信して、応答メッセージを返す
ipcMain.on('my-add', (evt: Electron.IpcMainEvent, num1: number, num2: number) => {
  evt.reply('my-add-reply', num1 + num2);
  //evt.sender.send('my-add-reply', num1 + num2);
});
{{< /code >}}

レンダラープロセス側では、__`ipcRenderer.send()`__ でメインプロセスにメッセージを送り、その応答を __`ipcRenderer.on()`__ でハンドルするように実装します。

{{< code lang="typescript" title="renderer.ts（レンダラープロセス）" >}}
import { ipcRenderer } from 'electron';

// メインプロセスへメッセージを送信
ipcRenderer.send('my-add', 100, 200);

// メインプロセスからメッセージを受信
ipcRenderer.on('my-add-reply', (evt: Electron.Event, result: number) => {
    alert(result);  //=> 300
});
{{< /code >}}

### ある BrowserWindow のレンダラープロセスにメッセージを送る

`BrowserWindow` のインスタンスがあれば、そのウィンドウのレンダラープロセスに対してメッセージを送ることができます。

{{< code lang="typescript" title="main.ts（メインプロセス）" >}}
const win = new BrowserWindow(options);

win.webContents.once('did-finish-load', () => {
  win.webContents.send('message-from-main', 'Hello!');
});
win.loadFile('public/index.html');
{{< /code >}}

メインプロセス側から能動的にメッセージを送る場合は、レンダラープロセス側のコンテンツが準備できてから送らないといけないことに注意してください。
上記の例では、レンダラープロセスから最初に [did-finish-load イベント](https://www.electronjs.org/docs/api/web-contents#event-did-finish-load) を受診したときに、メッセージを送信するようにしています。

別の方法として、`win.loadFile()` が返す `Promise` が resolve 状態になるのを待つという方法があります。
タイミングは `did-finish-load` イベントが発行されるタイミングと同じになります。
このあたりは、[loadFile() の API ドキュメント](https://www.electronjs.org/docs/api/web-contents#contentsloadfilefilepath-options) に記述されています。

{{< code lang="typescript" title="main.ts（別の方法）" >}}
win.loadFile('public/index.html').then(() => {
  win.webContents.send('message-from-main', 'Hello!');
});
{{< /code >}}

レンダラープロセス側は、単純に `ipcRenderer.on()` で受診すれば OK です。

{{< code lang="typescript" title="renderer.ts（レンダラープロセス）" >}}
import { ipcRenderer } from 'electron';

ipcRenderer.on('message-from-main', (evt: Electron.Event, msg: string) => {
  alert('Message from main: ' + msg);
});
{{< /code >}}

