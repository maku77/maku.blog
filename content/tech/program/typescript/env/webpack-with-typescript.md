---
title: "webpack と TypeScript を組み合わせて使用する"
url: "/p/ucnv6en"
date: "2020-07-05"
tags: ["TypeScript", "webpack"]
---

はじめに
----

[TypeScript](https://www.typescriptlang.org/) は JavaScript コードに型付けすることができる優れたトランスパイラですが、変換後の `.js` ファイル群をまとめる（バンドルする）機能は備えていません。
また、モダンな Web サイトを構築するときは、CSS Modules や Sass/Less/Stylus といった仕組みを使用するのが常套手段となっています。

そのため、Web サイト用の `.js` ファイルを TypeScript を使って作成する場合、[webpack](https://webpack.js.org/) などのバンドルツールを組み合わせて使用する必要があります。

- TypeScript ... `.ts` ファイルから `.js` ファイルへの変換
- webpack ... Web サイト用の各種リソースをバンドルする

バンドルツールには様々なものがありますが、大きなシェアを占めているのは webpack なので（2020年現在）、ここでは TypeScript と webpack を組み合わせて使用する方法を説明します。

{{< note title="webpack は必要なくなる？" >}}
[ES Module](https://ja.javascript.info/modules-intro) の仕組みにより、Web ブラウザからモジュール化された `.js` ファイルをインポートすることが可能になりつつあります。
しかし、Web サイトの最終的なデプロイ時には、各種リソースを最適化（minify など）する必要があるため、まだまだ webpack などのバンドルツールが必要です。
{{< /note >}}


関連パッケージのインストール
----

### TypeScript のインストール

プロジェクト用のディレクトリと package.json を作成し、TypeScript をインストールします。

{{< code >}}
$ mkdir myapp && cd $_
$ npm init -y
$ npm install --save-dev typescript
{{< /code >}}

### webpack のインストール

webpack 関連のパッケージをインストールします。

{{< code >}}
$ npm install --save-dev webpack webpack-cli ts-loader html-webpack-plugin
{{< /code >}}

- `webpack` ... webpack 本体
- `webpack-cli` ... webpack のコマンドラインツール
- `ts-loader` ... webpack 経由で `.ts` ファイルをトランスパイルする
- `html-webpack-plugin` ... `.html` ファイルを `dist` ディレクトリに出力する


実装
----

ここでは、下記のようなディレクトリ構成をゴールとします。
Web サーバーにデプロイすべきファイル群（HTML ファイルや JS ファイル）は、`dist` ディレクトリ以下に出力されることを想定しています。

### ディレクトリ構成

{{< code >}}
myapp/
  +-- package.json       # Node プロジェクトの設定
  +-- tsconfig.json      # TypeScript の設定
  +-- webpack.config.js  # webpack の設定
  +-- dist/              # 出力先
  +-- src/
        +-- index.html
        +-- index.ts
{{< /code >}}

### TypeScript の設定 (tsconfig.json)

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "ES2015",
    "module": "commonjs",
    "lib": ["esnext", "dom"],
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true,

    // 出力先などは webpack 側で指定するので本質的には必要なし
    "sourceMap": true,
    "outDir": "./dist",
    "sourceRoot": "./src",
  }
}
{{< /code >}}

トランスパイル時の出力先などは webpack 側で制御するので、`outDir` などの指定は本質的には必要ありませんが、間違えて `tsc` コマンドで直接変換してしまった場合に、変な場所に `.js` ファイルが出力されないように念のため指定しておきます。

### webpack の設定 (webpack.config.js)

webpack によるバンドル後の結果を `dist` ディレクトリに出力することや、TypeScript 関連のコード（`.ts` ファイル）を認識させるための設定を行います。

{{< code lang="javascript" title="webpack.config.js" >}}
const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  // 開発用の設定
  mode: 'development',

  // エントリポイントとなるコード
  entry: './src/index.ts',

  // バンドル後の js ファイルの出力先
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'index.js'
  },

  // ソースマップファイルの出力設定
  devtool: 'source-map',

  module: {
    rules: [
      // TypeScript ファイルの処理方法
      {
        test: /\.ts$/,
        use: "ts-loader",
        include: path.resolve(__dirname, 'src'),
        exclude: /node_modules/
      }
    ]
  },

  plugins: [
    // HTML ファイルの出力設定
    new HtmlWebpackPlugin({
      template: './src/index.html'
    })
  ]
};
{{< /code >}}

### index.html の作成

Web サイトのトップページとなる `index.html` を作成します。
このファイルに `script` 要素を記述しておく必要はありません。
`HtmlWebpackPlugin` が `src/index.html` を `dist/index.html` にコピーするときに、`script` 要素を自動挿入してくれます。

{{< code lang="html" title="src/index.html" >}}
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>MyApp</title>
    <meta name="viewport" content="width=device-width, initial-scale=1"></head>
  <body>
    <div id="root"></div>
  </body>
</html>
{{< /code >}}

### index.ts の作成

トップページから読み込まれる `index.js` ファイルの TypeScript 版を作成します。
ここでは、`div` 要素のテキストを `Hello` に変更しています。

{{< code lang="typescript" title="src/index.ts" >}}
const root = document.getElementById('root') as HTMLDivElement;
root.innerText = 'Hello';
{{< /code >}}


Web サイトのビルド
----

webpack と TypeScript の設定ができたら、次のようにビルドを実行します。

{{< code >}}
$ npx webpack
{{< /code >}}

`dist` ディレクトリ以下に、`index.html` や `index.js` ファイルが出力されるので、出力された `index.html` ファイルを開いて、Hello と表示されることを確認します。

ビルド方法は、`package.json` でスクリプト定義しておきましょう。

{{< code lang="json" title="package.json" >}}
{
  "name": "myapp",
  "version": "1.0.0",
  "scripts": {
    "build": "webpack"
  },
  "devDependencies": {
    "html-webpack-plugin": "^4.3.0",
    "ts-loader": "^7.0.5",
    "typescript": "^3.9.6",
    "webpack": "^4.43.0",
    "webpack-cli": "^3.3.12"
  }
}
{{< /code >}}

これで、`npm run build` でビルドできるようになります。

