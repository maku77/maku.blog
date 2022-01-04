---
title: "Electron アプリ内のリンクをOSのデフォルトブラウザで開く"
url: "/p/7yhwcr7"
date: "2020-07-25"
tags: ["Electron"]
---

BrowserWindow 内で表示した HTML のリンクをクリックすると、デフォルトではそのウィンドウ内でリンク先のページへ遷移します。
リンククリック時に発生する [will-navigate イベント](https://www.electronjs.org/docs/api/web-contents#event-will-navigate) と [new-window イベント](https://www.electronjs.org/docs/api/web-contents#event-new-window) をハンドルすることで、OS のデフォルトブラウザでリンクを開くことができます。
リンククリック時には通常 `will-navigate` イベントが発生するのですが、`<a target="_blank" href="...">` のように別ウィンドウで開くようなリンクをクリックした場合は `new-window` イベントが発生するので、両方のイベントをハンドルする必要があります。

{{< code lang="typescript" >}}
// リンククリック時に OS のデフォルトブラウザで開く
const handleUrlOpen = (event: Event, url: string) => {
  event.preventDefault();
  shell.openExternal(url);
};

// リンククリック時のイベントハンドラを登録
win.webContents.on('will-navigate', handleUrlOpen);
win.webContents.on('new-window', handleUrlOpen);
{{< /code >}}

下記はメインプロセス全体のコードです。

{{< code lang="typescript" title="main.ts" >}}
import { app, BrowserWindow, shell } from 'electron';

class MainWindow {
  private options: Electron.BrowserWindowConstructorOptions = {
    width: 800,
    height: 400,
    webPreferences: {
      nodeIntegration: true
    }
  }

  // リンククリック時に OS のデフォルトブラウザで開く
  private handleUrlOpen = (event: Event, url: string) => {
    event.preventDefault();
    shell.openExternal(url);
  };

  constructor() {
    const win = new BrowserWindow(this.options);

    // リンククリック時のイベントハンドラを登録
    win.webContents.on('will-navigate', this.handleUrlOpen);
    win.webContents.on('new-window', this.handleUrlOpen);

    win.loadFile('public/index.html');
  }
}

// Electron の初期化が完了したらウィンドウを作成
app.whenReady().then(() => new MainWindow())
{{< /code >}}

