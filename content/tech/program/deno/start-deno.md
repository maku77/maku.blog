---
title: "Deno プログラミングをはじめる"
url: "p/d8s6fpz/"
date: "2022-12-12"
tags: ["Deno"]
weight: 1
---

Deno とは？
----

[Deno](https://deno.land/)（発音: ディーノ）は、Node.js の作者であるライアン・ダール (Ryan Dahl) 氏が作成した JavaScript/TypeScript の実行環境です。
Deno という名前は、Node を逆にしたネーミングになっています。
Node.js での反省を活かした改善が行われており、今後は Node.js ではなく Deno で作成されるアプリケーションが増えていくと思われます。
Deno には次のような特徴があります。

- シンプルな 1 つの実行ファイル（`deno` コマンド）で提供
- ネイティブに TypeScript 実行をサポート（`tsc` コマンドのインストールが不要）
- ゼロコンフィグで開発を始められる（必要に応じて `deno.json` を用意することも可能）
- `package.json` で依存モジュールを管理する必要がない
- 各種開発ツールを `deno` サブコマンドで標準搭載
  - `deno fmt` ... フォーマッター
  - `deno lint` ... Lint ツール
  - `deno test` ... テストランナー
  - `deno bundle` ... バンドラー（1 つの JS や実行ファイルにまとめる）
- クロスプラットフォームな `deno task`（Node の `npm run` に相当）
- Web ブラウザ上の JavaScript と互換性のある API 定義 (Web Platform APIs)
- Promise ベースの API 実装
- Rust 実装による高速動作（当初は Golang 実装だった）
- npm モジュールのサポート（2022 年に追加）
- 公式のサーバレス環境 Deno Deploy が提供されている

Node.js の時代は、TypeScript 環境の準備や ESLint、Prettier の設定など、本質的ではないことに時間を取られがちでしたが、Deno であれば最初から全部入りなので、時間を有効活用できて幸せな人生を送ることができます。
Golang や Rust の開発体験のよさに、TypeScript/JavaScript が追いついた感じですね。


Deno のインストール
----

Deno（`deno` コマンド）のインストールは、下記ページの説明に従えば簡単に終わります。

- [Installation | Manual | Deno](https://deno.land/manual/getting_started/installation)

例えば、macOS/Linux や Window 環境では次のようにコマンド実行すれば、数秒でインストールは完了します。
`deno` はホームディレクトリ以下 (`~/.deno/bin`) にインストールされるので、管理者権限で実行する必要はありません（バイナリが 1 つ配置されるだけです）。

{{< code lang="console" title="macOS/Linux の場合" >}}
$ curl -fsSL https://deno.land/x/install/install.sh | sh
{{< /code >}}

{{< code lang="console" title="Windows の場合 (PowerShell)" >}}
$ irm https://deno.land/install.ps1 | iex
{{< /code >}}

`deno` コマンドが実行できるようになっていれば OK です。

```console
$ deno --version
deno 1.28.3 (release, x86_64-pc-windows-msvc)
v8 10.9.194.5
typescript 4.8.3
```

一度 `deno` コマンドをインストールしてしまえば、その後のバージョンアップは `deno` コマンド自身のサブコマンドで行えます。

{{< code lang="console" title="deno のバージョンを更新する場合" >}}
$ deno upgrade  # 最新バージョンへ更新
$ deno upgrade --version 1.28.3  # バージョンを指定（バージョンダウンも簡単）
{{< /code >}}

V8 エンジンと TypeScript 環境も同時に更新されるので、`deno` のバージョンだけ気にしていれば済みます。
とっても楽です。


Deno で Hello World
----

{{< code lang="ts" title="hello.ts" >}}
console.log("Hello World");
{{< /code >}}

Deno の Hello World プログラムです。
TypeScript コードで書かれていますが、JavaScript へトランスパイルする必要はなく、__`deno run`__ コマンドで直接実行できます。

```console
$ deno run hello.ts
Hello World
```

ここでは、ローカルの `.ts` ファイルを実行しましたが、次のように Web 上の `.ts` ファイルを実行することもできます。

```console
$ deno run https://deno.land/std@0.168.0/examples/welcome.ts
Welcome to Deno!
```

これで、Deno プログラミングの準備が整いました！

٩(๑❛ᴗ❛๑)۶ わーぃ

