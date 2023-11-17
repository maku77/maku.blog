---
title: "Stylelint で CSS の記述ミスを防ぐ（Stylelint 本体と VS Code 拡張の導入）"
url: "p/y2fru7o/"
date: "2023-11-17"
tags: ["Stylelint", "VS Code"]
draft: true
---

Stylelint とは
----

- [https://stylelint.io/](https://stylelint.io/)

__Stylelint__ は CSS の記述が正しいかをチェックしてくれる lint ツールです。
正しい CSS コードを記述するのはとても難しいので、Stylelint にいろいろ教えてもらいましょう。
Stylelint は、`.css` ファイルだけでなく、`.scss`  などの CSS プリプロセッサ用のファイルも解析してくれます。

既存のプロジェクトに Stylelint をかけると、おそらく大量の指摘事項が表示されます。
まずは、Stylelint の導入だけしておいて、指摘事項は徐々に直していくというやり方で進めるとよいです。


Stylelint のインストール
----

Stylelint は Node.js の NPM モジュールとして配布されています。
既存のプロジェクトに `package.json` が存在しない場合は、先に作成しておいてください。

{{< code lang="console" title="package.json の作成" >}}
$ npm init -y
{{< /code >}}

あるいは、下記のような最小限の `package.json` ファイルを手動で作成するのでもよいです。

{{< code lang="json" title="package.json" >}}
{
  "name": "example.com",
  "version": "0.0.1",
  "license": "UNLICENSED"
}
{{< /code >}}

`package.json` があるディレクトリで、次のように実行すると Stylelint がインストールされ、初期設定ファイル (`.stylelintrc.json`) が作成されます。

{{< code lang="console" title="Stylelint のインストール" >}}
$ npm init stylelint
{{< /code >}}

`package.json` の内容は次のように更新されます。
__`stylelint`__ コマンド本体と、__`stylelint-config-standard`__ というコンフィグモジュールがインストールされていることがわかります（このあたりは、バージョンごとに変わる可能性があります）。

{{< code lang="json" title="package.json" hl_lines="6 7" >}}
{
  "name": "example.com",
  "version": "0.0.1",
  "license": "UNLICENSED",
  "devDependencies": {
    "stylelint": "^15.11.0",
    "stylelint-config-standard": "^34.0.0"
  }
}
{{< /code >}}

初期設定ファイルは次のような内容になっており、上記でインストールされたコンフィグモジュールを使うように設定されています。

{{< code lang="json" title=".stylelintrc.json" >}}
{ "extends": ["stylelint-config-standard"] }
{{< /code >}}


Stylelint を実行する
----

コマンドラインから Stylelint を実行するには次のようにします。

```console
$ npx stylelint "assets/**/*.css"

assets/css/main.css
 2:20  ✖  Expected "#ffccdd" to be "#fcd"  color-hex-length

1 problem (1 error, 0 warnings)
```

`package.json` にスクリプトを追加しておくと、プロジェクト内でのツールの使い方を統一できます。

{{< code lang="js" title="package.json" >}}
{
  // ...
  "scripts": {
    "lint": "stylelint \"assets/**/*.css\""
  },
  // ...
}
{{< /code >}}

これで、次のように Stylelint を実行できるようになります。

```console
$ npm run lint
```

これで、デフォルト設定での Stylelint の導入は完了です。


SCSS ファイルを解析できるようにする
----

Stylelint で `.scss` ファイルを解析できるようにするには、Stylelint 本体 (`stylelint`) に加え、SCSS 用のコンフィグモジュール [stylelint-config-standard-scss](https://www.npmjs.com/package/stylelint-config-standard-scss) をインストールします。

```console
$ npm install --save-dev stylelint stylelint-config-standard-scss
```

Stylelint の設定ファイルで、このコンフィグモジュールを使用するように設定します。

{{< code lang="json" title=".stylelintrc.json" >}}
{ "extends": ["stylelint-config-standard-scss"] }
{{< /code >}}

これで、`.scss` ファイルの解析が可能になります。

```console
$ npx stylelint "assets/**/*.scss"
```

`.css` と `.scss` の両方を解析するには、次のいずれかの方法で指定します。

```console
$ npx stylelint "assets/**/*.css" "assets/**/*.scss"
$ npx stylelint "assets/**/*.{css,scss}"
```

`npm init stylelint` の実行により `stylelint-config-standard` モジュールがインストールされている場合は、削除してしまって構いません。
`stylelint-config-standard-scss` だけで `.css` と `.scss` の両方を処理できます。

```console
$ npm uninstall stylelint-config-standard
```

