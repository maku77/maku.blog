---
title: "Electron で Hello World (2) TypeScript で開発できるようにする"
url: "/p/78whxix"
date: "2020-06-28"
tags: ["Electron", "TypeScript"]
weight: 102
---

概要
----

前回の記事（[Electron で Hello World (1) 最小構成で作る](/p/ter4eq2)）では、プレーンな JavaScript を使用して簡単な Electron アプリを作成しましたが、本格的な開発を始めるときは TypeScript の環境を導入しておいた方がよいでしょう。

ここでは、上記のプレーンな JavaScript による Hello World ができているとして、TypeScript の環境を追加でセットアップします。
次のように、`src` ディレクトリ内に `.ts` ファイルを作成し、トランスパイルされた `.js` ファイルを `build` ディレクトリに出力するように設定することにします。

{{< code >}}
myapp/
  +-- build/         # トランスパイル後の .js ファイルを格納
  +-- src/           # .ts ファイルを格納
  +-- public/        # そのままパッケージングするもの
  |     +-- index.html
  +-- package.json   # Node.js 用設定ファイル
  +-- tsconfig.json  # TypeScript 用設定ファイル
{{< /code >}}


ビルド設定
----

TypeScript をインストールします。

{{< code >}}
$ npm install typescript --save-dev
{{< /code >}}

Node.js ライブラリ用の型宣言ファイル (`node.d.ts`) をインストールします。

{{< code >}}
$ npm install @types/node --save-dev
{{< /code >}}

{{< note >}}
Electron の型定義については、Electron パッケージにデフォルトで含まれているので、型定義ファイルを別途インストールする必要はありません。
{{< /note >}}

TypeScript の設定ファイル (`tsconfig.json`) を作成します。
次のように `tsc --init` コマンドでテンプレートファイルを生成し、必要に応じて内容を修正するのが手っ取り早いです。

{{< code >}}
$ npx tsc --init
{{< /code >}}

次のような感じの内容になるように `tsconfig.json` を修正します。
プロジェクトのディレクトリ構成に合わせて `outDir` と `rootDir` を正しく指定するところがポイントでしょうか。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "es6",
    "module": "commonjs",
    "lib": ["esnext","dom"],
    "outDir": "build",
    "rootDir": "src",
    "sourceMap": true,
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true
  }
}
{{< /code >}}

{{< note title="skipLibCheck プロパティ" >}}
上記 `tsconfig.json` ファイル内の `skipLibCheck: true` は、型定義ファイル内の型チェックを行わないようにする指定です。
執筆時点では [Electron が Node.js の新しい型定義に対応していない](https://github.com/electron/electron/issues/21612) ため、このプロパティをセットしておかないと、`tsc` によるトランスパイル時にエラーになります。
{{< /code >}}

`package.json` ファイルを修正し、`build` スクリプトや `start` スクリプトで `tsc` によるトランスパイルを実行するようにしておきます。
エントリポイントとなるファイルが `main.js` から `build/main.js` に変わったので、そのパスの変更も忘れずに。

{{< code lang="json" title="package.json" >}}
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
    "electron": "^9.0.2",
    "typescript": "^3.9.5"
  }
}
{{< /code >}}

TypeScript で開発するための基本的なビルド設定はこれで完了です。


実装
----

`tsconfig.json` の `rootDir` プロパティで指定した通り、TypeScript のソースコードは、`src` ディレクトリ以下に作成するようにします。
モジュールのインポートには、ECMAScript 2015 (ES6) の `import ~ from` 構文を使うようにし、必要に応じて関数や変数をタイプアノテーションで型付けします。

{{< code lang="typescript" title="src/main.ts" >}}
import { app, BrowserWindow } from 'electron';

function createWindow () {
  const options: Electron.BrowserWindowConstructorOptions = {
    width: 400,
    height: 300,
    webPreferences: {
      nodeIntegration: true  // Node 機能の使用を許可
    }
  }
  const win = new BrowserWindow(options);
  win.loadFile('public/index.html');
}

// Electron の初期化が完了したらウィンドウを作成
app.whenReady().then(createWindow);
{{< /code >}}


実行
----

あとは、次のように実行すれば、TypeScript のトランスパイルが実行され、Electron アプリが起動します（`npm start` は `npm run start` の省略記法です）。

{{< code >}}
$ npm start
{{< /code >}}

TypeScript のトランスパイルのみを実行したい場合は次のようにします。

{{< code >}}
$ npm run build
{{< /code >}}

次のステップ → [Electron で Hello World (3) React を使えるようにする](/p/6pybmv6)

