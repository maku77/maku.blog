---
title: "TypeScriptの環境: Visual Studio Code で TypeScript の開発環境を構築する"
linkTitle: "Visual Studio Code で TypeScript の開発環境を構築する"
url: "/p/ak7u3h3"
date: "2019-09-24"
lastmod: "2020-09-10"
tags: ["JavaScript", "TypeScript", "VS Code"]
weight: 102
---

はじめに
----

TypeScript は Microsoft が開発している言語で、同じく Microsoft が開発している IDE である Visual Studio Code（以下 VS Code）が TypeScript コードの編集に適しています。

VS Code による TypeScript の開発環境を構築するには、下記をインストールする必要があります。

- Visual Studio Code
- Node.js（npm コマンド）
- TypeScript（tsc コマンド)


開発環境をインストールする
----

### Visual Studio Code のインストール

VS Code は下記からインストーラーをダウンロードしてインストールできます。

- [Download Visual Studio Code](https://code.visualstudio.com/download)

TypeScript のソースコード (.ts) ファイルは、単純なテキストエディタでも編集できますが、この VS Code を使って編集すると、**プロパティ名の自動補完などができて効率的**に開発を行えます。

コマンドラインから `code` と入力して、VS Code を起動できるようになれば OK です。
VS Code をインストールしたのに `code` コマンドが認識されない場合は、[こちらの記事](/p/f5iv9kx) を参考にしてパスを通してください。

### Node.js のインストール

TypeScript のトランスパイラである `tsc` コマンドは、Node.js のパッケージとして提供されているため、先に Node.js をインストールしておく必要があります。
Node.js は下記からインストーラーをダウンロードしてインストールできます。

- [Download Node.js](https://nodejs.org/ja/download/)

Node.js をインストールすると、パッケージ管理用の Node Package Manager（`npm` コマンド）も一緒にインストールされます。


Node.js アプリ用のプロジェクトを作成する
----

### VS Code でプロジェクトを開く

VS Code は 1 つのディレクトリを 1 つのプロジェクトとみなします。
まずはプロジェクト用のディレクトリを作成して、それを VS Code で開きます。

{{< code >}}
$ mkdir myapp
$ code myapp
{{< /code >}}

VS Code を起動するときは、__ディレクトリを開く__ というところがポイントで、`.js` ファイルや `.ts` ファイルを直接開いてしまうと、プロジェクトに設定したビルドタスク（後述）を実行できません。
Windows エクスプローラーなどでディレクトリを右クリックして <samp>Open with Code</samp> を選択する方法でも OK です。

### package.json の作成

このプロジェクトを Node.js アプリとして認識させるため、__`package.json`__ を作成します。
VS Code 上で <kbd>Ctrl + `</kbd> と入力するとターミナルを開くことができるので、下記のように実行します。

{{< code >}}
$ npm init -y
{{< /code >}}

プロジェクトのパッケージ依存関係やビルドスクリプトなどは、この `package.json` に記述します。


TypeScript のビルド設定を行う
----

VS Code で TypeScript コード (.ts) をトランスパイルして JavaScript コード (.js) に変換するには、下記のような状態にセットアップされている必要があります。

- VS Code でディレクトリ（ワークスペース）を開いている
- そのディレクトリ内に **TypeScript の設定ファイル (`tsconfig.json`)** が存在する

### TypeScript (tsc) のインストール

まず、TypeScript のトランスパイラ（`tsc` コマンド）をインストールします。
次のようにすると、プロジェクトに TypeScript がインストールされ、`package.json` の依存情報 (`devDependencies`) が更新されます。

{{< code >}}
$ npm install typescript --save-dev
$ npm install @types/node --save-dev
{{< /code >}}

インストールする際のパッケージ名は `tsc` ではなく `typescript` なので注意してください。
後者の `@types/node` パッケージは、Node.js コアライブラリ用の TypeScript 型定義です。
必須ではありませんが、これをインストールしておくと、例えば `process` モジュールなどの型補完が効くようになります。

プロジェクトのディレクトリから、次のように `tsc` コマンドを実行できるようになればインストールは完了です。

{{< code >}}
$ npx tsc -v
Version 4.0.2
{{< /code >}}

`npx` は、プロジェクト内にインストールした NPM パッケージが提供するコマンドを実行するためのコマンドです（`Node.js が提供する機能です）。
ここでは、プロジェクト内にインストールした `tsc` コマンドを実行しています。

{{< note title="TypeScript のグローバルインストール" >}}
`npm install` で TypeScript をインストールするときに、`-g` オプションを付けると `tsc` コマンドをシステムグローバルにインストールすることができます。
その場合は、どのディレクトリからでも `tsc` コマンドを実行できるようになります（`npx` コマンド経由にする必要がありません）。
ただ、Node.js アプリを作成する場合は、開発環境の依存を防ぐために、TypeScript はプロジェクトローカルにインストールしておいた方がよいでしょう。
{{< /note >}}

### tsconfig.json を作成する

TypeScript 用のビルド設定ファイルである **`tsconfig.json`** を作成します。
`tsc` コマンドを使って、ひな形となる設定ファイルを作成することができます。

{{< code >}}
$ npx tsc --init
message TS6071: Successfully created a tsconfig.json file.
{{< /code >}}

デフォルトでは、例えば下記のような感じの設定ファイルが生成されます（コメント類は削除）。
TypeScript のバージョンによって、生成される内容は多少異なります。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "es5",
    "module": "commonjs",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true
  }
}
{{< /code >}}

`target` はトランスパイル後の JavaScript バージョンで、デフォルトでは若干古い Web ブラウザなどを想定したバージョン (`es5`) になっています。最終的な実行環境として Node.js を使うのであれば、__`ES2020`__ などの新しいバージョンを指定してもよいでしょう。

`.ts` ファイルの格納ディレクトリや、`.js` ファイルの出力先ディレクトリを変更したい場合は、次のような感じで設定します。
同じディレクトリに、トランスパイル前の `.ts` ファイルと、トランスパイル後の `.js` ファイルが混在すると紛らわしいので、この設定を行っておくことをオススメします。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    // ...
    "outDir": "./build",
  },
  "include": ["./src/**/*"]
}
{{< /code >}}


TypeScript コードをビルドして実行する
----

### ビルドと実行

<kbd>Cmd/Ctrl + N</kbd> で新規ファイルを作成し、下記のように実装して <kbd>Cmd/Ctrl + S</kbd> で `main.ts` という名前で保存します。
`tsconfig.json` で `include` ディレクトリの設定を行った場合は、それに合わせて `src` ディレクトリ以下に保存することに注意してください。

{{< code lang="typescript" title="src/main.ts" >}}
function greet(name: string): string {
  return `Hello, ${name}!`;
}
console.log(greet('Maku'));
{{< /code >}}

`main.ts` ファイルを作成したら、__`tsc`__ コマンドでビルド（トランスパイル）することができます。
あるいは、VS Code 上で次のようにビルドすることもできます（同様に `tsc` コマンドを呼び出すだけです）。

1. <kbd>Cmd/Ctrl + Shift + B</kbd> （あるいはメニューから <samp>Terminal</samp> → <samp>Run Build Task...</samp>）
2. <samp>tsc: build - tsconfig.json</samp> を選択して <kbd>Enter</kbd>

すると、下記のように JavaScript にトランスパイルされた `main.js` ファイルが生成されます。

{{< code lang="js" title="build/main.js" >}}
"use strict";
function greet(name) {
  return "Hello, " + name + "!";
}
console.log(greet('Maku'));
{{< /code >}}

あとは、次のように `node` コマンドで実行できます。

{{< code >}}
$ node build/main.js
Hello, Maku!
{{< /code >}}

### npm start で実行できるようにする

`package.json` に NPM スクリプトを定義しておくと、`npm run スクリプト名` と入力するだけで様々なコマンドを実行できるようになります。
この設定により、ビルドコマンドを簡略化したり、エントリポイント (`build/main.js`) を意識せずにプログラムを起動できるようになります。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    "start": "node build/main.js",
    "build": "tsc",
    "build:watch": "tsc --watch"
  },
  // ...
}
{{< /code >}}

<kbd>Ctrl + `</kbd> でターミナルを開き、次のように入力すると、ビルドやアプリの起動を行えます。

{{< code >}}
$ npm run build  # ビルド（トランスパイル）の実行
$ npm start      # ビルド済みプログラムの起動
$ npm run build:watch  # watch モードでビルド（後述）
{{< /code >}}

ちなみに、`npm start` は `npm run start` のエイリアスです。
NPM スクリプトは、基本的に `npm run スクリプト名` で起動するのですが、`start` スクリプトは頻繁に使用するので、このようなエイリアスで起動できるようになっています。
`tsc` コマンドをターミナルから実行するときは `npx tsc` と `npx` 経由で実行していましたが、NPM スクリプト内では `npx` を省略することができます。


{{< private >}}
### Ctrl + F5 でトランスパイル＆実行

<kbd>Ctrl + F5</kbd> キーを押すと、VSCode の実行 (Run) 機能が起動します。
この実行処理は、`.vscode/launch.json` という設定ファイルに基づいた処理が行われるのようになっており、例えば、 __TypeScript のトランスパイルと生成された JavaScript の実行を自動化__ することができます。
この設定ファイルは、最初に実行 (Run) 機能を使ったときに自動生成されるので、手動で作成する必要はありません。

ファイルの内容は下記のような感じになっています。

{{< code lang="json" title=".vscode/launch.json" >}}
{
  "version": "0.2.0",
  "configurations": [
    {
      "type": "node",
      "request": "launch",
      "name": "Launch Program",
      "skipFiles": [
        "<node_internals>/**"
      ],
      "program": "${workspaceFolder}\\build\\index.js",
      "preLaunchTask": "tsc: build - tsconfig.json",
      "outFiles": [
        "${workspaceFolder}/build/**/*.js"
      ]
    }
  ]
}
{{< /code >}}

実行する `js` ファイルは、 __`program`__ プロパティで指定されています。
上記の例では、`build/index.js` と設定されているので、どのファイルを編集中であっても、<kbd>Ctrl + F5</kbd> で起動するプログラムは `build/index.js` になります。
うまく実行できない場合は、このパスが正しいかを確認してみてください。

さらに、 __`preLaunchTask`__ のところで、先に `tsc` によるビルドタスクを実行することが指定されています。
そのため、<kbd>Ctrl + F5</kbd> を押すだけで、`tsc` によるビルド → `node main.js` が連続して実行されます。

実行結果（`console.log()` などの出力）は、Debug Console というウィンドウに表示されます。
表示されていない場合は、メニューから `View` → `Debug Console` で表示できます。

{{< code title="Debug Console" >}}
Hello Maku!
{{< /code >}}

{{< note title="tsc watch と組み合わせた方が速い" >}}
`.vscode/launch.json` の `preLaunchTask` プロパティで `tsc` ビルドタスクを実行するように指定すると、<kbd>Ctrl + F5</kbd> で JS プログラムを起動する前に __毎回トランスパイルが走る__ ので時間がかかります。
`preLaunchTask` プロパティを削除してしまえば、`main.js` は一瞬で起動するようになります。

この場合、TypeScript のトランスパイルは別に実行しておく必要があるのですが、<kbd>Ctrl + Shift + B</kbd> で `tsc watch` を起動しておけば、`.ts` ファイルの更新を監視して自動化できます（後述）。
頻繁にプログラムの起動を行うようなケースでは、このように使った方が開発効率が上がります。
{{< /code >}}

{{< /private >}}


（応用）TypeScript コードの変更を監視する (tsc: watch)
----

### tsc: watch

上記の説明では、<kbd>Ctrl + Shift + B</kbd> から <samp>tsc: build</samp> というビルドタスクを実行して TypeScript コードをトランスパイルしましたが、代わりに <samp>tsc: watch</samp> というビルドタスクを実行すると、TypeScript ファイル (.ts) の変更を監視する watch モードに入ります。
これは、コマンドラインから下記のように、__`tsc --watch`__ を起動するのと同様です。

{{< code >}}
$ npx tsc --watch
{{< /code >}}

前述のように、`package.json` 内の NPM スクリプトとして `build:watch` を定義している場合は、次のように起動することもできます。

{{< code >}}
$ npm run build:watch
{{< /code >}}

この watch モードに入ってから、TypeScript ファイルを編集して <kbd>Ctrl + S</kbd> で保存すると、その度に自動的に JavaScript ファイルへのトランスパイル (`tsc`) が実行されます。

VS Code 上で <samp>tsc: watch</samp> を起動すると、ターミナルが `tsc watch` によって占有されたように見えますが、実は新しい起動したターミナル上で実行されています。
別のコマンド (`node build/main.js` など）を入力したくなったら、ドロップダウンリストから使用するターミナルを切り替えれば実行できます。
あるいは、<kbd>Ctrl + Shift + `</kbd> と入力して新しいターミナルを開くこともできます。

### nodemon

さらに、Node.js でサーバープログラムを開発している場合は、 __`nodemon`__ コマンドを使って Node サーバーを起動しておくと、JS コードが生成されるたびに自動的にサーバーを再起動してくれるので、開発効率がすごく上がります。

{{< code >}}
$ nodemon build/index.js
{{< /code >}}

- 参考: [nodemon で Node.js サーバの再起動を自動化する | まくまくNode.jsノート](https://maku77.github.io/nodejs/env/nodemon.html)

### VS Code のターミナルを分割して効率化

VS Code のターミナル上で `tsc --watch` などの監視ビルドを起動すると、別のコマンドを実行するときにターミナルを切り替えないといけないので面倒です。
そんなときは、ターミナルの分割機能 (<kbd>Cmd + \ </kbd> ) を使って、一方を監視ビルド用、もう一方を別のコマンド実行用（`npm start` など）に使うと便利です。

{{< image border="true" src="img-001.png" >}}

これで、`.ts` ファイルを修正したときに左側のペインで自動ビルド結果を確認しつつ、必要に応じて右側のペインでアプリを起動する、といったことが可能になります。

