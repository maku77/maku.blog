---
title: "TypeScriptの環境: 既存の JavaScript プロジェクトを TypeScript に乗り換える"
linkTitle: "既存の JavaScript プロジェクトを TypeScript に乗り換える"
url: "/p/3eccb2t"
date: "2019-10-01"
tags: ["JavaScript", "TypeScript"]
---

{{% private %}}
- [Migrating from JavaScript｜TypeScript](https://www.typescriptlang.org/docs/handbook/migrating-from-javascript.html)
{{% /private %}}

TypeScript プロジェクトへの移行は簡単！
----

TypeScript の構文は、JavaScript のコードとの互換性を考慮して仕様が決められているため、既存の JavaScript プロジェクトを簡単に TypeScript プロジェクトに移行することができます。
後述するように設定をうまく行えば、JavaScript ファイルの拡張子を `.ts` に変える必要すらありません。

TypeScript の構文を使わないと意味がないのでは？と思うかもしれませんが、TypeScript のトランスパイラ (`tsc`) には、強力な**静的解析の仕組みが備わっている**ため、この機能を既存の JavaScript ファイルに適用するだけでも TypeScript を導入する価値があります。

ここでは、`tsc` コマンドは下記のようにグローバルにインストールしてあることを前提とします。

{{< code >}}
$ npm install -g typescript
{{< /code >}}


おすすめのディレクトリ構成
----

既存の JavaScript コードを TypeScript トランスパイラ (`tsc`) の変換対象にする場合は、入力ファイルとなる .js ファイルと、生成される .js ファイルのディレクトリを明確に区別しておくべきです。
そうしておかないと、どのファイルが自分が作成したファイルで、どのファイルが自動生成されたファイルなのか分かりにくくなってしまいます。

ここでは、既存の JavaScript プロジェクトのソースコード (.js) を、下記のように `src` ディレクトリに格納します。
このディレクトリには、最終的に .js ファイルと .ts ファイルが混在していても構いません。

{{< code >}}
project/
  +-- tsconfig.js  （設定ファイル）
  +-- build/       （出力用ディレクトリ）
  +-- src/         （入力用ディレクトリ）
       +-- main.js
       +-- module1.js
       +-- module2.js
{{< /code >}}

上記では、`src` ディレクトリの直下に .js ファイルを入れていますが、ディレクトリ構造を保ったまま入れてしまって大丈夫です。
出力先のディレクトリ (`build`) には、このディレクトリ階層を再現する形で JavaScript ファイルが出力されます。

`build` ディレクトリは、下記の設定によって自動的に生成されるので、あらかじめ作成しておく必要はありません。
上記は、最終的な出力後のディレクトリ構成になります（`build` ディレクトリができるだけですが）。


設定ファイルの作成 (tsconfig.js)
----

上記のようなディレクトリ構成のプロジェクトを扱うには、例えば下記のような設定ファイルを作成します。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "outDir": "./build",
    "allowJs": true,
    "target": "ES2015",
    "module": "commonjs",
    "strict": true
  },
  "include": [
    "./src/**/*"
  ]
}
{{< /code >}}

今回ポイントとなるのは下記の 3 つのプロパティです。

- **`compilerOptions.outDir`**: 出力先のディレクトリをプロジェクトルートから `build` に変更
- **`compilerOptions.allowJs`**: 入力ファイルとして JavaScript の拡張子 (.js) も対象にする
- **`include`**: 入力ファイルは `src` ディレクトリ以下のすべての .js および .ts ファイル

この設定ファイルを作成したら、あとは `src` ディレクトリ内に既存の .js ファイルをつっこんで、下記のようにトランスパイルを実行するだけです。

{{< code >}}
$ tsc
{{< /code >}}

これで、変換後の JavaScript ファイルが `build` ディレクトリに出力されます（今回は元々が JavaScript で記述されたコードなので、生成されるコードにあまり変化はないかもしれません）。


（応用）ビルド用の NPM スクリプトを定義する
----

ここまでは、`tsc` コマンドを直接実行することで `build` ディレクトリにトランスパイル後の JavaScript ファイルを出力することを前提としていました。
Node.js でのアプリ開発に使用する `package.json` には、任意のスクリプトを定義することができるので、ここでは `build` というスクリプトを定義して一連のビルドを行えるようにしてみましょう。
`npm run build` と実行したときに、下記のような処理が走るようにしてみます。

1. `build` ディレクトリがあれば削除する
2. `src` ディレクトリ内の TS コードをトランスパイルして `build` ディレクトリへ出力する
3. `static` ディレクトリ内のファイルをそのまま `build` ディレクトリへコピーする（画像ファイルや設定ファイルなどのリソースファイルを想定）

### 必要な NPM パッケージのインストール

まず、ビルドコマンドを構成するために必要な NPM パッケージ群をインストールします。

{{< code >}}
$ npm install --save-dev typescript   # tsc コマンド
$ npm install --save-dev rimraf       # ディレクトリの削除用コマンド
$ npm install --save-dev cpx          # ファイルコピー用コマンド
$ npm install --save-dev npm-run-all  # NPMコマンドの逐次実行
{{< /code >}}

`package.json` の `devDependencies` エントリに追加するために、`--save-dev` オプションを付けて実行してください。
ここではパッケージをひとつずつインストールしていますが、パッケージ名を列挙すればまとめてインストールすることができます。


### package.json で build スクリプトを定義する

下記は `package.json` のスクリプト定義部分の抜粋で、4 つの NPM スクリプトを定義しています。

{{< code lang="json" >}}
{
  "scripts": {
    "clean": "rimraf build",
    "build": "run-s clean build:tsc build:static",
    "build:tsc": "tsc",
    "build:static": "cpx static/** build"
  },
{{< /code >}}

下記のように `build` スクリプトを実行すると、

{{< code >}}
$ npm run build
{{< /code >}}

次の NPM スクリプトが順番に実行されます（`npm-run-all` パッケージが提供する `run-s` コマンドによって逐次実行が実現されます）。

- `clean`: `build` ディレクトリの削除
- `build:tsc`: `src` ディレクトリ内の TypeScript コードのトランスパイル
- `build:static`: `static` ディレクトリ内のファイル群を `build` へコピー

これらの NPM スクリプトは単独で実行することもできます。

{{< code >}}
$ npm run clean
{{< /code >}}

ここで使用している各種 NPM パッケージの使い方は、下記のサイトで分かりやすく紹介されています。

- 参考: [まくまく Node.js ノート](https://maku77.github.io/nodejs/)


その他の調整
----

Node.js などで開発している場合は、プロジェクトのディレクトリに `package.json` などが存在すると思います。
今回の修正により、最終的な生成物が `build` ディレクトリ以下に配置されることになるので、必要に応じて `package.json` 内に記述されたファイルパスを修正する必要があるかもしれません。

下記の例では、`npm start` で起動するスクリプトファイルとして `build` ディレクトリ以下の JS ファイルを参照するように修正しています。

{{< code lang="json" title="package.json（抜粋）" >}}
{
  "scripts": {
    "start": "node build/index.js",
{{< /code >}}

あるいは、ルートの `index.js` から、`build/index.js` のメイン関数を呼び出すようにしておくのもよいかもしれません。

{{< code lang="js" title="index.js（ルートの）" >}}
require('./build/index.js').main();
{{< /code >}}

{{< code lang="js" title="build/index.js" >}}
// エントリポイント
function main() {
  // ...
}

// ルートの index.js からも起動できるように
exports.main = main;

// 自分自身がエントリポイントとなったときに起動できるように
if (require.main === module) {
  main();
}
{{< /code >}}

こうしておけば、`package.json` の中の `scripts.start` の定義は `node index.js` のままでも起動できます。

