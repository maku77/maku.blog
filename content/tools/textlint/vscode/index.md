---
title: "VSCode の textlint プラグインで文章を校正する (vscode-textlint)"
url: "/p/d3fs5gs"
date: "2020-05-20"
tags: ["textlint", "VS Code"]
weight: 2
---

事前準備（textlint のインストール）
----

__vscode-textlint__ は、Visual Studio Code でテキスト校正ツールの textlint を実行するためのプラグインです。
プレーンテキスト (.txt) や、Markdown ファイル (.md) の校正を VSCode 上で実行できるようになります。
textlint の本体の方は、Node.js の `npm` コマンドを使ってあらかじめインストールしておく必要があります。

- 参考: [textlint のインストールと基本的な使い方](/p/3veuap5)

こんな感じでインストールできます。

{{< code title="textlint 本体のインストール" >}}
$ cd myproject  # テキストファイルのあるディレクトリへ移動
$ npm init -y   # package.json がない場合は作成
$ npm install -D textlint  # textlint のインストール
{{< /code >}}

もし、既存のプロジェクトで、`package.json` や `.textlintrc` などがすでに存在しているのであれば、次のように実行すれば一発で textlint の実行環境が整います。

{{< code title="package.json に従って環境構築する場合" >}}
$ npm install
{{< /code >}}


vscode-textlint プラグインをインストールする
----

vscode-textlint プラグインは、VS Code の Extensions タブから、`textlint` で検索してインストールすることができます。

{{< image border="true" src="img-001.png" >}}


vscode-textlint プラグインによる校正を実行する
----

vscode-textlint プラグインは、VS Code で開いたディレクトリ内に設定ファイル __`.textlintrc`__ が存在すると自動的に校正を実行します（ファイルではなく、ディレクトリを開かないと動作しません）。
たとえば次の設定ファイルでは、[「ら抜き言葉」を検出するルール設定](https://github.com/textlint/textlint/wiki/Collection-of-textlint-rule#textlint-rule-no-dropping-the-ra) を行なっています。

{{< code lang="yaml" title=".textlintrc" >}}
{
  "rules": {
    "no-dropping-the-ra": true
  }
}
{{< /code >}}

このとき、事前にこのルールを実行するためのモジュールがインストールされていないと、

{{< code >}}
Failed to load textlint's rule module: "no-dropping-the-ra" is not found.
{{< /code >}}

といったエラーが Output ウィンドウに表示されます。
次のように、対応する textlint ルールをインストールすれば正しく実行されるようになります（次回からは `npm install` でまとめてインストールできるようなります）。

{{< code >}}
$ npm install -D textlint-rule-no-dropping-the-ra
{{< /code >}}

textlint のルールによりエラーが検出されると、次のようにエディタ上に下線が引かれ、Problems ウィンドウにエラーの一覧が表示されます。

{{< image w="600" border="true" src="img-002.png" title="「ら抜き言葉」の検出" >}}

{{< note title="Quick Fix による自動修正" >}}
`textlint --fix` コマンドで自動修正可能なエラーであれば、エラーのある行にカーソルがある状態で __`Alt + Enter`__ あるいは __`Cmd + .`__ と入力すれば、自動的に正しい表現に修正してくれます。
自動修正に対応しているかどうかは、各ルールの NPM モジュールの説明ページを参照してください。
例えば、[textlint-rule-ja-hiragana-fukushi](https://github.com/lostandfound/textlint-rule-ja-hiragana-fukushi) は自動修正に対応しています（例: `且つ` → `かつ`）。
{{< /note >}}

あとは、同様にして [いろいろな設定](https://github.com/textlint/textlint/wiki/Collection-of-textlint-rule) を追加していくことができます。


文字を入力するたびに textlint を実行する
----

textlint の実行タイミングは、デフォルトではテキストファイルの保存時 (`onSave`) になっています。
文字を入力するたびに校正を実行したい場合は、設定メニュー (Settings) から、`Extensions` → `textlint` と辿り、設定を __`onType`__ に変更します。

{{< image w="400" border="true" src="img-003.png" title="textlint.run 設定" >}}


