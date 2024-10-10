---
title: "Vite で複数の TypeScript ファイルやリソースファイルを 1 つの JavaScript ライブラリとしてバンドルする"
url: "p/59mfj7b/"
date: "2024-10-02"
lastmod: "2024-10-06"
tags: ["TypeScript"]
changes:
  - 2024-10-06: Vite で Svelte を使う方法を追記
---

何をするか
----

[Vite](https://vitejs.dev/) は TypeScript ビルドや、開発用サーバー、バンドリングなどの機能を備えた統合的なフロントエンド開発ツールです。
ここでは、Vite で複数の TypeScript ファイルを 1 つの JavaScript ファイルの形にビルドする方法を説明します（この作業をバンドルと呼びます）。

{{< image w="300" src="img-001.drawio.svg" title="複数の .ts/.css/.svg ファイルを 1 つの JavaScript ファイルに結合" >}}

最終的な成果物として、複数の Web サイトから __`<script>` 要素で読み込み可能な JavaScript ライブラリ__ を作成することを想定しています。
画像ファイルやスタイルシート (CSS) などのリソースファイルも、単一の JavaScript に埋め込む形でバンドルできます。


Vite プロジェクトの作成
----

__`npm create vite`__ コマンドを使って Vite のプロジェクトを作成します。
React などのフレームワークを使わず、純粋な TypeScript ファイルをビルドするだけであれば、ウィザードに従って、__`Vinilla`__ → __`TypeScript`__ と選択していきます。

{{< code lang="console" title="Vite プロジェクト (myscript) を生成する" >}}
$ npm create vite@latest myscript
✔ Select a framework: › Vanilla
✔ Select a variant: › TypeScript
$ cd myscript
{{< /code >}}

次のようなファイルが生成されます。

```
myscript/
  +-- .gitignore
  +-- index.html  # <script> 要素による表示の確認用
  +-- package.json
  +-- tsconfig.json
  +-- public  # 最終的には必要なし
  |    +-- vite.svg
  +-- src/  # 最終的には全部書き換える
       +-- counter.ts
       +-- main.ts
       +-- style.css
       +-- typescript.svg
       +-- vite-env.d.ts
```

`src` ディレクトリにはサンプル実装の TypeScript コードや画像ファイルが入っているので、この中身は最終的には全部書き換えることになります。
その他の `package.json` や `tsconfig.json` などは大部分は使いまわせるはずです。

プロジェクトが生成されたら、依存パッケージ (tsc, vite) をインストールしておきます。

```console
$ npm install
```


bundle.js 出力用のプロジェクトに書き換える
----

他の Web サイトから 1 つの `<script>` 要素で利用可能なライブラリにするには、複数の TypeScript ファイルを単一の JavaScript ファイルの形にバンドルする必要があります。
テンプレートとして生成されたプロジェクトを `npm run build (tsc & vite build)` でそのままビルドすると、Web サイト用の `index.html` や各種アセットファイル (`.css`, `.js`) などが出力されてしまいます。
そこで、下記の設定手順に従い、1 つの JavaScript ファイルとして出力するようカスタマイズする必要があります。

### CSS ファイルを .js ファイルに埋め込む

テンプレートプロジェクトの `main.ts` ファイルでは、`import "./style.css"` のように外部の CSS ファイルを読み込んでいます。
デフォルトでは CSS ファイルは独立したアセットファイルとして出力されてしまうので、1 つの JavaScript ファイルに埋め込むように設定する必要があります。
もちろん、CSS ファイルを使わないプロジェクトではこの設定は必要ありません。

CSS を JavaScript に埋め込むための __`vite-plugin-css-injected-by-js`__ という Vite プラグインが公開されているのでこれをインストールします。

```console
$ npm install vite-plugin-css-injected-by-js --save-dev
```

### vite.config.ts の作成

__`vite.config.ts`__（`.js` でも可）を作成して、単一の `dist/bundle.js` ファイルとして出力するように設定します（この設定を [Library Mode](https://ja.vite.dev/guide/build.html#library-mode) と呼びます）。
上記でインストールした Vite プラグインはここで読み込みます。
出力ファイル名は `dist/bundle.js` としています。

{{< code lang="js" title="vite.config.ts" >}}
import { defineConfig } from "vite";
import cssInjectedByJsPlugin from "vite-plugin-css-injected-by-js";

export default defineConfig({
  plugins: [cssInjectedByJsPlugin()], // CSS を JS に埋め込むプラグイン
  build: {
    lib: {
      entry: "src/main.ts",
      formats: ["iife"], // Web サイト組み込み用に即時実行形式 (function(){})() で出力
      name: "MyScript", // static API 用のグローバルオブジェクト名
    },
    rollupOptions: {
      output: {
        dir: "dist", // 出力ディレクトリ (default: dist)
        entryFileNames: "bundle.js", // 出力ファイル名
      },
    },
  },
});
{{< /code >}}

### ビルド

リリース用の JavaScript ファイルを生成するには、__`npm run build`__ (=`tsc & vite build`) コマンドでビルドします。
次のように `dist/bundle.js` だけが生成されれば成功です。

```console
$ npm run build
...
✓ 5 modules transformed.
dist/bundle.js  3.81 kB │ gzip: 2.02 kB
```

`bundle.js` ファイルの内容を見ると、`svg` ファイルや `css` ファイルの内容がインラインで埋め込まれていることを確認できます。

### 開発用サーバー

Vite の開発サーバーを起動すると、`src/main.ts` ファイルの修正を監視してリアルタイムに表示内容を確認できます。
開発中はこの機能を使うと便利です。

{{< code lang="console" title="開発サーバーを起動してついでにブラウザで開く" >}}
$ npm run dev -- --open
{{< /code >}}

次のように `index.html` の内容が表示されれば成功です。
ちなみに今回の用途では、`index.html` ファイルは完全に開発時の表示確認用になります。

{{< image w="500" border="true" src="img-002.png" title="bundle.js の動作確認" >}}

とはいっても、`dist/bundle.js` ファイルは特にサーバーサイドで実行する必要はないので、次のような HTML ファイルを作成して、ローカルファイルとして Web ブラウザで開くことでも動作確認できます（`public/` ディレクトリ以下の `vite.svg` ファイルなどは参照できなくなりますが）。

{{< code lang="html" title="test.html" >}}
<!doctype html>
<html lang="ja">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Test</title>
    <script src="./dist/bundle.js" defer></script>
  </head>
  <body>
    <div id="app"></div>
  </body>
</html>
{{< /code >}}

### その他

`public/` ディレクトリには、そのまま Web サイト用のリソースとしてデプロイされる画像ファイルなどが格納されていますが、今回の用途では必要ないのでディレクトリごと削除してしまっても大丈夫です。
`public/` ディレクトリを削除した場合は、ソースコード内の `vite.svg` を参照している箇所も削除してください。


（応用）UI 部分に Svelte を使う方法
----

HTML の UI 部分を出力するような JavaScript ライブラリーを作成する場合は、Svelte などのフレームワークを使うと便利です。
React や Vue などのフレームワークも同様に使えますが、最終的なリリース物が大きくなりがちなので、小規模なライブラリーの場合は Svelte を使うことをお勧めします。

- 参考: [Svelte 関連メモ｜まくろぐ](/p/td962u6/)

### Vite + Svelte プロジェクトの作成

Vite のプロジェクトを作成する際に、フレームワークとして Svelte を使うよう指定します。

{{< code lang="console" title="Vite プロジェクトを生成する（Svelte 用のテンプレートを使用）" >}}
$ npm create vite@latest myscript
✔ Select a framework: › Svelte
✔ Select a variant: › TypeScript

$ cd myscript
$ npm install
{{< /code >}}

先ほどと同様に、CSS コードを JavaScript に埋め込むための Vite プラグインをインストールしておきます。

{{< code lang="console" title="CSS 埋め込み用の Vite プラグインをインストール" >}}
$ npm install vite-plugin-css-injected-by-js --save-dev
{{< /code >}}

### 実装

`vite.config.ts` ファイルを編集して、Svelte や CSS のコードを単一の JavaScript ファイル (`dist/bundle.js`) にバンドルするように設定します。

{{< code lang="js" title="vite.config.ts (Svelte)" >}}
import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import cssInjectedByJsPlugin from "vite-plugin-css-injected-by-js";

export default defineConfig({
  plugins: [svelte(), cssInjectedByJsPlugin()],
  build: {
    lib: {
      entry: "src/main.ts",
      formats: ["iife"], // Web サイト組み込み用に即時実行形式 (function(){})() で出力
      name: "MyScript", // static API 用のグローバルオブジェクト名
    },
    rollupOptions: {
      output: {
        dir: "dist", // 出力ディレクトリ (default: dist)
        entryFileNames: "bundle.js", // 出力ファイル名
      },
    },
  },
});
{{< /code >}}

下記は、Svelte コンポーネントによる UI 実装の例です。
ここでは同じディレクトリにある `svelte.svg` という画像ファイルをインポートしていますが、このようにインポートした画像ファイルは、最終的に出力ファイルである JavaScript ファイルにバンドルされます。

{{< code lang="svelte" title="src/App.svelte" >}}
<script lang="ts">
  import svelteLogo from './svelte.svg';

  let count = 0;
  function increment() {
    count += 1;
  }
</script>

<div class="container">
  <img src={svelteLogo} alt="Svelte Logo" />
  <button on:click={increment}>
    Clicked {count} {count === 1 ? 'time' : 'times'}
  </button>
</div>

<style>
  .container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: 100vh;
  }

  img {
    height: 6em;
    padding: 1.5em;
    will-change: filter;
    transition: filter 300ms;
  }

  img:hover {
    filter: drop-shadow(0 0 2em #ff3e00aa);
  }
</style>
{{< /code >}}

エントリーポイントとなる `src/main.ts` では、上記の `App.svelte` を読み込んで `document.body` にマウントするようにしておきます。

{{< code lang="ts" title="src/main.ts" >}}
import App from "./App.svelte";

new App({ target: document.body });
{{< /code >}}

### テスト

ローカルテスト用の `index.html` ファイルでは、`<script>` タグでエントリーポイントとなる `/src/main.ts` を読み込むようにしておきます。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Vite + Svelte + TS</title>
  </head>
  <body>
    <script type="module" src="/src/main.ts"></script>
  </body>
</html>
{{< /code >}}

開発サーバーを起動して、ブラウザで表示を確認します。

{{< code lang="console" title="開発サーバーを起動してついでにブラウザで開く (Svelte)" >}}
$ npm run dev -- --open
{{< /code >}}

次のように表示されれば成功です。

{{< image w="213" border="true" src="img-003.png" title="Vite + Svelte の動作確認" >}}

### リリース用ビルド

__`npm run build`__ コマンドでリリース用の JavaScript ファイルを生成します。

{{< code lang="console" title="リリース用ビルド（bundle.js の生成）" >}}
$ npm run build
{{< /code >}}

出力された `dist/bundle.js` は、他の Web サイトから `<script>` 要素で読み込むことができる単一の JavaScript ライブラリーになっています。
次のように __`defer`__ 属性を付けて読み込むと、ページの読み込みが完了してから JavaScript ファイルが実行され、自動的に `App.svelte` コンポーネントが表示されます。

{{< code lang="html" title="他の Web サイトから読み込む例" hl_lines="7" >}}
<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Vite + Svelte + TS</title>
    <script src="./bundle.js" defer></script>
  </head>
  <body>
    <!-- ... -->
  </body>
{{< /code >}}

もちろん、別のサーバーで `bundle.js` をホスト（共有）して、各 Web サイトから読み込むことも可能です。

٩(๑❛ᴗ❛๑)۶ わーぃ

