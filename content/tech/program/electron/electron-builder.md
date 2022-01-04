---
title: "Electron アプリの配布パッケージを作る (electron-builder)"
url: "/p/2tcs8n2"
date: "2020-07-01"
tags: ["Electron"]
---

概要
----

electron-builder を使用すると、Electron アプリを Windows、macOS、Linux 用の配布用バイナリとしてパッケージングすることができます。
各 OS 用のインストーラはもちろん、ポータブルな zip パッケージを作成することもできます。

- [electron-builder](https://www.electron.build/)

公式サイトのドメイン名が `electron.build` っていうのがかっこいいですね。


electron-builder のインストール
----

Node.js のパッケージ管理ツールとして、`npm` よりも `yarn` を使うことが strongly recommended されているので、まず `yarn` をインストールしてから `yarn` で `electron-builder` をインストールすることにします。

{{< code >}}
$ npm install -g yarn
$ yarn add electron-builder --dev
{{< /code >}}

下記のように実行して、ヘルプを表示できればインストール完了です。

{{< code >}}
$ npx electron-builder --help
{{< /code >}}


ビルド設定
----

`electron-builder` 用の設定は、`package.json` の __`build`__ プロパティで行うことができます。
ポイントは、`files` の指定で、パッケージングする html ファイルや js ファイル、画像ファイルなどをすべてカバーするように指定しておく必要があります。

{{< code lang="json" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "main": "build/main.js",
  "scripts": {
    "start": "tsc && electron ."
  },
  "build": {
    "appId": "com.example.myapp",
    "productName": "MyApp",
    "files": [
      "build/**/*",
      "public/**/*"
    ]
  },
  "devDependencies": {
    "@types/node": "^14.0.14",
    "electron": "^9.0.5",
    "electron-builder": "^22.7.0",
    "typescript": "^3.9.5"
  }
}
{{< /code >}}


パッケージの作成
----

上記のビルド設定に従ってパッケージを作成するには、__`electron-builder`__ コマンドを使用します。
各種オプションにより、どの OS 用のパッケージを作成するかを指定することができます。
オプションは `npx electron-builder --help` で確認できますが、次のようにオプションを組み合わせてパッケージを作成できます。

| オプション | 説明 | 作成されるファイル |
| ---- | ---- | ---- |
| `--mac --x64` | macOS (x64) 用インストーラー | `MyApp-0.0.1.dmg` |
| `--mac --x64 --dir` | macOS (x64) 用ポータブルパッケージ | `MyApp.app` |
| `--win --x64` | Windows (x64) 用インストーラー | `MyApp Setup 0.0.1.exe` |
| `--win --x64 --dir` | Windows (x64) 用ポータブルパッケージ | `MyApp.exe` + 依存DLLなど |

ここでは、macOS のポータブルパッケージ (`MyApp.app`) を作成してみます。
ポータブルパッケージは、いわゆるインストーラーではなく、ディレクトリごとコピーすればそのまま実行できる形態のものです。

{{< code >}}
$ npx electron-builder --mac --x64 --dir
{{< /code >}}

デフォルトで、`dist` ディレクトリ以下に実行ファイルが作成されるので、例えば、macOS では次のようにアプリを起動することができます（Finder から `MyApp.app` アイコンをダブルクリックしても起動できます）。

{{< code >}}
$ open dist/mac/MyApp.app
{{< /code >}}

macOS 環境で Windows 用のパッケージを作成することもできます。
その場合は、自動的に `wine` がダウンロードされてビルドが実行されます。

一方、Windows 環境で macOS 用のパッケージを作成することはできないようです。
Apple イケてないですね。


応用
----

### .gitignore の修正

`.gitignore` ファイルには、パッケージの出力ディレクトリである `dist/` を追加しておきましょう。

### package.json へスクリプト追加

`package.json` に次のようにスクリプト定義しておけば、`npm run build:mac` コマンド（あるいは `yarn run build:mac`）で簡単に配布用パッケージを作成できるようになります。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "main": "build/main.js",
  "scripts": {
    "start": "tsc && electron .",
    "build:mac": "electron-builder --mac --x64",
    "build:win": "electron-builder --win --x64"
  },
  // ...
}
{{< /code >}}

