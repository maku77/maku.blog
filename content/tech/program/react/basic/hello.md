---
title: "React + TypeScript の環境を整える (1) 自力で webpack 設定する方法"
url: "/p/m4dmt3a"
date: "2020-07-02"
tags: ["TypeScript", "React"]
weight: 100
---

{{< private >}}
[公式ドキュメント - 静的型チェック – React](https://ja.reactjs.org/docs/static-type-checking.html#typescript)
{{< /private >}}

React とは
----

Web サイトを作成するときに React を採用すると、HTML をフラットな形でゴリゴリ記述していくのではなく、独自コンポーネント（例: `<MyButton>` コンポーネント）を定義してまとまりのある単位でサイトを構築していくことができます。
Web Components という同様な技術の標準化が進んでいますが、しばらくは React のようなコンポーネントライブラリが使われるでしょう。

- [React – ユーザインターフェース構築のための JavaScript ライブラリ](https://ja.reactjs.org/)

ここでは React を使った Web サイト開発用に、下記のようなツールを組み合わた環境を構築します。

- __React__ ... コンポーネントベースで Web サイト構築するためのライブラリ
- __TypeScript__ ... JavaScript を型付けできるようにした言語
- __webpack__ ... Web サイトのリソースをバンドルするためのツール

この環境構築方法を理解すれば、React を利用した Web サイトをどんどん作ることができます。
ちなみに、上記のすべてのツールは [Node.js](https://nodejs.org/ja/) 上で動作するため、Node.js がインストールされていない場合は先にインストールしてください。

TypeScript、React、webpack のインストール
----

チュートリアルなどでは、`create-react-app` を使って React アプリの雛形を生成する方法がよく載っていますが、よくわからないモジュールが勝手にインストールされるのは気持ち悪いので、ここでは自力で各モジュールをインストールしてきます。

{{< note title="create-react-app は使わない方がいい" >}}
TypeScript ベースの React アプリの雛形を生成するには、`create-react-app myapp --template typescript` のように実行します。
このコマンドによって作成された雛形をリファレンスにするのがよいかなと思ったのですが、少なくともバージョン 3.4.1 時点で生成される雛形はかなり怪しいです。
例えば、TypeScript の処理系や型定義ファイルが `devDependencies` ではなく、`dependencies` でインストールされるようになっていたりします。
{{< /note >}}

最終的に Web サーバーにデプロイするファイル群は webpack で生成（バンドル）することを想定しているので、`npm` (or `yarn`) でインストールするモジュールは、 __すべて `devDependencies`（開発用モジュール）としてインストール__ していきます（Web サーバー側で `npm install` を実行することはないということです）。

### プロジェクトの作成

プロジェクト用のディレクトリがなければ作成し、`package.json` ファイルを作成しておきます。

{{< code >}}
$ mkdir myapp && cd myapp
$ npm init -y
{{< /code >}}

### 各モジュールのインストール

TypeScript をインストールします。

{{< code >}}
$ npm install --save-dev typescript
{{< /code >}}

React 関連のモジュール（`react`、`react-dom`）と、その型定義をインストールします。
本体の方は実行時にも使用するので、`--save` オプションでインストールする必要があります。

{{< code >}}
$ npm install --save react react-dom
$ npm install --save-dev @types/react @types/react-dom
{{< /code >}}

webpack 関連のモジュールをインストールします。
コマンドラインツールの `webpack-cli`、TypeScript コードを認識させるための `ts-loader`、HTML ファイルを生成するための `html-webpack-plugin` も一緒にインストールします。

{{< code >}}
$ npm install --save-dev webpack webpack-cli ts-loader html-webpack-plugin
{{< /code >}}

### 確認

ここまでで、`package.json` 内の依存定義 (`devDependencies`) は次のようになっています。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "dependencies": {
    "react": "^16.13.1",
    "react-dom": "^16.13.1"
  },
  "devDependencies": {
    "@types/react": "^16.9.41",
    "@types/react-dom": "^16.9.8",
    "html-webpack-plugin": "^4.3.0",
    "ts-loader": "^7.0.5",
    "typescript": "^3.9.6",
    "webpack": "^4.43.0",
    "webpack-cli": "^3.3.12"
  }
}
{{< /code >}}


設定ファイル
----

### ディレクトリ構成

下記のようなディレクトリ構成を想定して、各種ツールの設定を行っていきます。

{{< code >}}
myapp/
  +-- package.json       # Node プロジェクトの設定
  +-- tsconfig.json      # TypeScript の設定
  +-- webpack.config.js  # webpack の設定
  +-- dist/              # webpack の出力先
  +-- src/               # ソースコード
        +-- index.html
        +-- index.tsx
        +-- ...
{{< /code >}}

ソースコードや各種リソースファイルは `src` ディレクトリ以下に作成し、webpack によるバンドル結果を `dist` ディレクトリに出力します。
最終的に `dist` ディレクトリの中身を Web サーバーにデプロイすることになります。
ルートディレクトリには、設定ファイル以外のファイルをなるべく置かないようにするとスッキリします。

### webpack の設定

Web サイト用のコンテンツは `webpack` コマンドにより生成するため、webpack の設定は重要です。
webpack は、ここで指定されたエントリーポイント（`.js` あるいは `.ts` ファイル）を起点にファイルの依存関係を認識し、それらのファイルをバンドルします。

{{< code lang="js" title="webpack.config.js" >}}
const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  // 開発用の設定
  mode: 'development',

  // エントリポイントとなるコード
  entry: './src/index.tsx',

  // バンドル後の js ファイルの出力先
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'index.js'
  },

  // import 時に読み込むファイルの拡張子
  resolve: {
    extensions: ['.js', '.json', '.ts', '.tsx']
  },

  // ソースマップファイルの出力設定
  devtool: 'source-map',

  module: {
    rules: [
      // TypeScript ファイル (.ts/.tsx) を変換できるようにする
      {
        test: /\.tsx?$/,
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

Web サイト用のクライアントスクリプトを TypeScript で作成する場合は、`.ts` ファイルおよび、`.tsx` ファイルを `ts-loader` で処理するように指定しておく必要があります。

webpack はあくまでバンドルツールなので、HTML ファイルの出力などはオプショナルな振る舞いになります。
上記の例では、`html-webpack-plugin` の機能を使って、`src/index.html` から `dist/index.html` を生成するように設定しています。

### TypeScript の設定

webpack によりデプロイ用のファイル群を生成する場合、TypeScript のトランスパイルも webpack 経由で実行することになります（`tsc` コマンドは直接実行しない）。
TypeScript の設定ファイルでは、基本的な変換ルールだけ定義しておきます。
ほとんど空っぽでも動くはずですが、`.tsx` ファイル内の JSX コードを認識させるための設定は必須です。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "ES2015",
    "module": "commonjs",
    "lib": ["esnext", "dom"],
    "jsx": "react",  // .tsx ファイル内の JSX 記述を認識
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


Hello World の実装
----

基本的な設定が終わったので、React を使って独自の `<Hello>` コンポーネントを作る実装を行います。

### src/index.html

トップページとなる `index.html` ファイルを作成します。
`script` 要素は webpack が勝手に挿入してくれるので、最小限の要素だけ記述しておけば OK です。

{{< code lang="html" title="src/index.html" >}}
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>MyApp</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
    <div id="root"></div>
  </body>
</html>
{{< /code >}}

### src/index.tsx

Web アプリのエントリポイントとして設定したスクリプトを作成します。
ここでは、React コンポーネントとして `Hello` コンポーネントを作成し、HTML のルートに配置した `div` 要素の内容として表示します。

{{< code lang="ts" title="src/index.tsx" >}}
import * as React from 'react';
import * as ReactDom from 'react-dom';

// Hello コンポーネントの属性（プロパティ）を定義
interface HelloProps {
  name?: string;  // オプショナルな name 属性
}

// Hello コンポーネントを定義
class Hello extends React.Component<HelloProps> {
  public render(): React.ReactNode {
    const name = this.props.name ?? 'Unknown';
    return (
      <b>Hello, {name}!</b>
    );
  }
}

// Hello コンポーネントを <div id="root"> に表示
ReactDom.render(
  <Hello name="React" />,
  document.getElementById('root')
);
{{< /code >}}


ビルド
----

次のように `webpack` コマンドを実行すると、`dist` ディレクトリ以下に Web サイト用コンテンツが出力されます。

{{< code >}}
$ npx webpack
{{< /code >}}

Web ブラウザで `dist/index.html` を開いて、Hello, React! と表示されれば成功です。

あとは必要に応じて、`npm run build` でビルドを実行できるようにスクリプトを定義しておきます。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "scripts": {
    "build": "webpack"
  },
  "devDependencies": {
    // ...
  }
}
{{< /code >}}

