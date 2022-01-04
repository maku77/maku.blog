---
title: "TypeScriptの環境: tsc-watch で ts ファイルの変更監視＆アプリの再起動を自動化する"
linkTitle: "tsc-watch で ts ファイルの変更監視＆アプリの再起動を自動化する"
url: "/p/nxzsnkf"
date: "2019-10-17"
tags: ["JavaScript", "TypeScript"]
---

TypeScript の監視と JavaScript の監視の両立
----

`tsc --watch` コマンドを使うと、TypeScript (`.ts`) ファイルの変更を監視して自動的に `tsc` （トランスパイル）を実行してくれるようになります。
`nodemon` コマンドを使うと、JavaScript (`.js`) ファイルの変更を監視して自動的に `node` コマンドを再起動してくれるようになります。

これらを組みあわせれば、TypeScript (`.ts`) ファイルの変更時に、自動的に Node.js アプリを再起動できそうな気がしますが、一連のコマンドとしてこれら 2 つを組み合わせて実行するのにはみんな苦労しているようです（もちろんコマンドシェルを 2 つ立ち上げればできますが）。

そこで便利なのが [tsc-watch コマンド](https://www.npmjs.com/package/tsc-watch) です。


tsc-watch パッケージでまるごと解決
----

**`tsc-watch`** という NPM パッケージ（コマンド）を使用すると、TypeScript の変更監視と Node アプリの再起動の連動を簡単に行えるようになります。
まず、必要なパッケージを下記のようにインストールします。
`nodemon` は使わなくて済むので、ここでは依存パッケージとして `typescript` と `tsc-watch` をインストールしておきます。
`--save-dev` オプションを付加して、`package.json` の `devDependencies` に登録するのを忘れないようにしましょう。

{{< code >}}
$ npm install --save-dev typescript
$ npm install --save-dev tsc-watch
{{< /code >}}

`tsc-watch` コマンドはほとんど `tsc --watch` のラッパーなので、そのまま実行すると、TypeScript ファイルの変更を監視してトランスパイルを実行するという動作になります。
これだけの用途であれば `tsc` コマンドで十分なのですが、`tsc-watch` コマンドに追加されている **`--onSuccess`** オプションを使うと、トランスパイル後に任意の追加コマンドを実行することができます。
この追加コマンドとして `npm start` などを実行するように設定しておけば、TypeScript のトランスパイル後に Node.js アプリを起動するところまでを自動化することができます。
すでに Node.js アプリが起動している場合は、そのプロセスを終了してから立ち上げ直してくれるので、サーバー系の Node.js アプリを作成している場合も安心です。

下記の `package.json` では、**`watch`** という名前のスクリプトで `tsc-watch` を実行するように設定しています。

{{< code lang="json" title="package.json" >}}
{
  "private": true,
  "scripts": {
    "start": "node build/index.js",
    "compile": "tsc",
    "watch": "tsc-watch --onSuccess \"npm start\""
  },
  "devDependencies": {
    "tsc-watch": "^4.0.0",
    "typescript": "^3.6.4"
  }
}
{{< /code >}}

TypeScript の設定ファイルである `tsconfig.json` の内容も載せておきます。
ここでは、`src` ディレクトリ内の `.ts` ファイルをトランスパイルして、生成された `.js` ファイルを `build` ディレクトリに出力するようにしています。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "outDir": "./build",
    "target": "ES2015",
    "module": "commonjs",
    "strict": true
  },
  "include": [
    "./src/**/*"
  ]
}
{{< /code >}}

あとは、`npm run` コマンドで `watch` スクリプトを実行すれば、TypeScript ファイルの変更監視＆Node.jsアプリの自動再起動の仕組みが作動します。

{{< code title="ts ファイル変更があったら自動でトランスパイルしてアプリを再起動" >}}
$ npm run watch
{{< /code >}}

説明は長くなりましたが、実際には `packages.json` に `script.watch` プロパティを追加するだけでよいので、とてもお手軽です。


（おまけ） tsc-watch は tsc のラッパー
----

下記のように、`tsc-watch` のヘルプを表示してみると、その内容は `tsc` コマンドのものに独自の拡張を加えたものになっていることがわかります。

{{< code >}}
$ ./node_modules/.bin/tsc-watch --help
Version 3.6.4
Syntax:   tsc [options] [file...]

Examples: tsc hello.ts
          tsc --outFile file.js file.ts
          tsc @args.txt
          tsc --build tsconfig.json

Options:
 -h, --help                Print this message.
 -w, --watch               Watch input files. [always on]
 --onSuccess COMMAND       Run the COMMAND on each successful compilation
 --onFirstSuccess COMMAND  Run the COMMAND on the first successful compilation (Will not run the onSuccess)
 --onFailure COMMAND       Run the COMMAND on each failed compilation
 --noColors                Removes the red/green colors from the compiler output
 --noClear                 Prevents the compiler from clearing the screen
 --compiler PATH           The PATH will be used instead of typescript compiler. Defaults typescript/bin/tsc.

 ...省略...
{{< /code >}}

なので、`tsc` のオプションは `tsc-watch` のオプションとしても使えるし、`tsc` コマンドの設定ファイルである `tsconfig.json` もそのまま使えます。

