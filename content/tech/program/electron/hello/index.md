---
title: "Electron で Hello World (1) 最小構成で作る"
url: "/p/ter4eq2"
date: "2020-06-08"
tags: ["Electron"]
weight: 101
---

Electron とは
----

{{< image w="256" src="electron-logo.svg" >}}

[Electron | Build cross-platform desktop apps with JavaScript, HTML, and CSS.](https://www.electronjs.org/)

Electron は GitHub が開発した、デスクトップアプリを開発するためのプラットフォームで、2020 年現在も活発な開発が続けられています。
Node.js や HTML5 技術を利用しており、 __様々な OS (Windows, macOS, Linux) で動作するデスクトップアプリ__ を作成することができます。

フロントエンド（UI 表示部分）に Chromium (HTML/CSS) を使用し、バックエンド（OSとの連携部分）に Node.js を使用するという構成になっています。
Electron で作成されている有名なアプリケーションに Visual Studio Code や Slack などがあります。
これらのアプリケーションの完成度を見れば、大規模なアプリケーション開発にも使用できるプラットフォームだということが分かります。

__Node.js さえ入っていれば、簡単にデスクトップアプリの開発を始められる__ ので、下記の Hello World だけでも試してみてください。


Electron で Hello World アプリを作成する
----

### package.json の作成

Electron アプリは Node.js アプリとして作成するので、プロジェクトのルートディレクトリに __`package.json`__ を作成します。
__`npm init`__ コマンドなどでベースとなる `package.json` ファイルをサクッと生成し、

{{< code >}}
$ npm init -y
{{< /code >}}

次のような内容を含むように修正します。

{{< code lang="json" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "main": "main.js",
  "scripts": {
    "start": "electron ."
  }
}
{{< /code >}}

`main` プロパティでは、パッケージングされた Electron アプリのエントリポイントを指定します。
開発中は `npm start` コマンドでアプリ起動することになりますが、上記のように `scripts.start` プロパティを指定しておくと、`electron` コマンド経由で `main.js` を起動してくれるようになります。


### electron モジュールのインストール

Electron のパッケージは、Node.js の `npm install` コマンドでインストールできます。
次のように `-D (--save-dev)` オプションを付けて実行することで、プロジェクトの `package.json` ファイルに開発用の依存情報として記録します。

{{< code >}}
$ npm install electron --save-dev
{{< /code >}}

### メインウィンドウ用の HTML (index.html) を作成

アプリ起動時に表示するコンテンツとして、`index.html` ファイルを作成します。
Hello World なので、簡単な内容で構いません。

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
    <h1>Hello Electron!</h1>
    <ul>
      <li>Node ver: <script>document.write(process.versions.node)</script>
      <li>Chrome ver: <script>document.write(process.versions.chrome)</script>
      <li>Electron ver: <script>document.write(process.versions.electron)</script>.
    </ul>
  </body>
</html>
{{< /code >}}

ここでのポイントは、 __Web ページ内の JavaScript なのに、Node.js の `process` モジュールを使用できている__ というところです。
秘密は下記で説明する `nodeIntegration: true` という設定にあります。

### エントリポイント (main.js) の作成

プログラムのエントリポイントとなる `main.js` では、メインウィンドウ (`electron.BrowserWindow`) を生成し、そこに上記の HTML ファイル (`index.html`) を読み込みます。

{{< code lang="js" title="main.js" >}}
const { app, BrowserWindow } = require('electron')

function createWindow () {
  // 新規ウィンドウを作成する
  const win = new BrowserWindow({
    width: 400,
    height: 300,
    webPreferences: {
      nodeIntegration: true  // Node 機能の使用を許可
    }
  })

  // ウィンドウ内に HTML ファイルを表示
  win.loadFile('index.html')

  // 起動と同時に Chrome の DevTools を開く
  // win.webContents.openDevTools()
}

// Electron の初期化が完了したらウィンドウを作成
app.whenReady().then(createWindow);
{{< /code >}}

上記の例では、`nodeIntegration` プロパティを `true` にセットしていますが、これはレンダラープロセスから Node.js の機能を呼び出すことを許可するためのものです。
ここではローカルの `index.html` を実行するので大丈夫ですが、インターネット上のファイルを読み込む場合は XSS 攻撃を防ぐために `false` にしておく必要があります。

`main.js` ファイルの作成が終わったら、次のように Electron アプリを起動できます。

{{< code >}}
$ npm start
{{< /code >}}

{{< image w="400" border="true" src="img-001.png" >}}


macOS の Dock に対応する
----

多くの macOS アプリは、ウィンドウを閉じても Dock にアプリのアイコンが残り、そこから再度ウィンドウを表示できるという仕様になっています。
このような振る舞いを実現するには、`main.js` ファイルに次のようなコードを追加します。

{{< code lang="js" title="main.js（追記）">}}
// Quit when all windows are closed.
app.on('window-all-closed', () => {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
});

app.on('activate', () => {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow()
  }
});
{{< /code >}}

次のステップ → [Electron で Hello World (2) TypeScript で開発できるようにする](/p/78whxix)

