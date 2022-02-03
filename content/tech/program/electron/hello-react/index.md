---
title: "Electron で Hello World (3) React を使えるようにする"
url: "/p/6pybmv6"
date: "2020-07-03"
tags: ["Electron", "React"]
weight: 103
---

{{< image border="true" w="500" src="img-001.png" >}}

概要
----

ここでは、Electron アプリの開発に React を導入する手順を示します。
React を導入すると、HTML をフラットな形でゴリゴリ記述していくのではなく、独自コンポーネント（例: `<MyButton>` コンポーネント）を定義して、まとまりのある単位でコンテンツを構築していくことができます。

下記の手順により、Electron + TypeScript による開発環境が構築できていることを前提とします。

- [Electron で Hello World (1) 最小構成で作る](/p/ter4eq2)
- [Electron で Hello World (2) TypeScript で開発できるようにする](/p/78whxix)

この記事の手順が完了すると、Electron + TypeScript + React による開発環境が整います。
一応 webpack などのバンドルツールを使わなくても開発を始められるので、Electron と React の開発環境としての相性はよさそうです。


React のセットアップ
----

### React モジュールのインストール

React モジュールおよび、TypeScript 用の型定義ファイルをインストールします。

{{< code >}}
$ npm install --save react react-dom
$ npm install --save-dev @types/react @types/react-dom
{{< /code >}}

`package.json` の内容は次のような感じになります。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "main": "build/main.js",
  "scripts": {
    "build": "tsc",
    "start": "tsc && electron ."
  },
  "devDependencies": {
    "@types/node": "^14.0.14",
    "@types/react": "^16.9.41",
    "@types/react-dom": "^16.9.8",
    "electron": "^9.0.5",
    "typescript": "^3.9.6"
  },
  "dependencies": {
    "react": "^16.13.1",
    "react-dom": "^16.13.1"
  }
}
{{< /code >}}

### JSX コードの有効化

TypeScript の設定ファイル (`tsconfig.json`) を編集し、 __`.tsx`__ ファイル内に記述した JSX コードを認識できるようにします。
JSX コードはトランスパイルによって、通常の JavaScript コードに変換されます。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "es6",
    "module": "commonjs",
    "lib": ["esnext","dom"],
    "outDir": "build",
    "rootDir": "src",
    "jsx": "react",  // .tsx ファイル内の JSX を認識
    "sourceMap": true,
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true
  }
}
{{< /code >}}

これだけで、Electron + TypeScript の開発環境上で React を使用できるようになります。


実装
----

### React コンポーネント (Hello.tsx)

`src/components` ディレクトリ以下に、独自の React コンポーネントを定義するための `.tsx` ファイルを格納することにします。
ここでは、`Hello` コンポーネントを作成します。
`Hello` 要素は、オプショナルな `name` 属性（プロパティ）を指定できるようにしてみます。
JSX 形式のコードを含むので、拡張子は __`.tsx`__ にすることに注意してください（`.jsx` の TypeScript 版です）。

{{< code lang="typescript" title="src/components/Hello.tsx" >}}
import React from 'react';

// Hello コンポーネントの属性（プロパティ）
export interface HelloProps {
  name?: string;
}

// Hello コンポーネントの定義
export class Hello extends React.Component<HelloProps> {
  public render(): React.ReactNode {
    const name = this.props.name ?? 'Mr. Unknown';
    return (
      <h1>Hello {name} in Electron</h1>
    );
  }
}
{{< /code >}}

### レンダラープロセス (renderer.tsx)

上記で定義した `Hello` コンポーネントを表示するためのレンダラープロセスを実装します。
ここでも JSX コードを使うので、拡張子は `.tsx` を使います。

{{< code lang="tsx" title="src/renderer.tsx" >}}
import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { Hello } from './components/Hello';

ReactDOM.render(
  <Hello />,
  document.querySelector('#root')
);
{{< /code >}}

このコードは、HTML 内の `<div id="root">` 要素の内容を、`Hello` コンポーネントが出力する内容に置き換えます。
パッと見、`React` クラスのインポートは不要に見えますが、JSX コードが `React` クラスを使うコードに変換されるので、インポートしておかないとコンパイルエラーになります。

次の HTML ファイルから、上記のスクリプトを読み込みます。

{{< code lang="html" title="public/index.html" >}}
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
    <div id="root"></div>
    <script>require('../build/renderer.js');</script>
  </body>
</html>
{{< /code >}}

### メインプロセス (main.ts)

最後に、Electron アプリのエントリポイントとなるメインプロセス (`main.ts`) の実装です。
このコードは特に変更はなく、単純に `public/index.html` を読み込んで表示します。

{{< code lang="typescript" title="src/main.ts" >}}
import { app, BrowserWindow } from 'electron';

// メインウィンドウの表示
function createWindow () {
  const options: Electron.BrowserWindowConstructorOptions = {
    width: 500,
    height: 200,
    webPreferences: {
      nodeIntegration: true  // とりあえず import のため
    }
  }
  const win = new BrowserWindow(options);
  win.loadFile('public/index.html');
}

// Electron の初期化が完了したらウィンドウを作成
app.whenReady().then(createWindow);
{{< /code >}}

