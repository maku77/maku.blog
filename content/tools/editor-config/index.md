---
title: "EditorConfig でコーディングスタイルを統一する"
linkTitle: ""
url: "/p/7aoa6x5"
date: "2020-11-20"
lastmod: "2021-03-16"
tags: ["エディタ", "EditorConfig", "AndroidStudio", "VS Code"]
---

EditorConfig とは
----

{{< image src="img-001.png" >}}

[EditorConfig](https://editorconfig.org/)（`.editorconfig` ファイル）を導入すると、各種エディタ（Visual Studio、Android Studio、Vim など）に共通のコーディングスタイルを設定することができます。

ソフトウェア開発プロジェクトでは、通常なんらかのコーディングルールが決められています。
ただ、開発者ごとに使用するエディタ、IDE が異なると、設定方法を統一できないという問題が発生します。
EditorConfig は、特定のエディタに依存しない、__コーディングスタイルを記述するためのフォーマット（およびツール群）__ です。

EditorConfig で設定可能な項目は、エンコーディング形式や改行コード、インデントサイズといったごく基本的な項目のみですが、最低限の記述スタイルをプロジェクト全体で素早く統一することができます。


EditorConfig の導入（.editorconfig ファイルの作成）
----

EditorConfig の導入はとても簡単で、次のような設定ファイル (__`.editorconfig`__) をプロジェクトのルートに作成するだけです。

{{< code title=".editorconfig" >}}
# ここが最上位の設定ファイル
root = true

# 全種類のテキストファイルの基本設定
[*]
charset = utf-8
end_of_line = lf
indent_size = 4
indent_style = space
insert_final_newline = true
trim_trailing_whitespace = true

# あとは、ファイルの種類ごとに設定を上書き
[*.{js,jsx,ts,tsx}]
indent_size = 2
max_line_length = 80

[*.{kt,kts}]
max_line_length = 100

[*.md]
trim_trailing_whitespace = false

[*.py]
max_line_length = 80

[*.rb]
indent_size = 2
max_line_length = 80

[*.{yml,yaml,json}]
indent_size = 2
{{< /code >}}

各プロパティの内容は、ほぼ自明ですが、それぞれ次のような意味を持っています。

| プロパティ名 | 設定例 | 説明 |
| ---- | ---- | ---- |
| root | `true` / `false` | これがルートの設定ファイルなら `true` |
| indent_style | `tab` / `space` | インデントをタブにするかスペースにするか |
| indent_size | `4` | インデントのスペース数 |
| end_of_line | `lf` / `cr` / `crlf` | 改行コード |
| charset | `utf-8` / `utf-8-bom` / `latin1` | ファイルのエンコーディング形式 |
| trim_trailing_whitespace | `true` / `false` | 行末のスペースを削除するか |
| insert_final_newline | `true` / `false` | ファイル末尾に空白行を入れるか |

各エディタの EditorConfig プラグインは、__編集中のファイルを起点にして、上位ディレクトリの `.editorconfig` ファイルを検索します__。
上の設定例のように、`root` プロパティが `true` になっている `.editorconfig` ファイルが見つかると、それ以上ディレクトリを上って検索しないようになります。
プロジェクトのルートに置く設定ファイルには、__`root = true`__ と記述しておくのがよいでしょう。

設定可能なプロパティの詳細は、下記サイトの一覧を参照してください。
`max_line_length` など、特定のエディタでしかサポートされていないプロパティもあります。

- [EditorConfig Properties](https://github.com/editorconfig/editorconfig/wiki/EditorConfig-Properties)


各エディタへのプラグイン導入
----

`.editorconfig` の設定に従って実際にフォーマットを行うには、各エディタに __EditorConfig プラグイン__ を導入する必要があります。

Android Studio (IntelliJ IDEA) のように、__最初からプラグインが組み込まれた状態でリリースされている__ 環境もあり、その場合は特に何もインストールする必要はありません。
一方で、Vim エディタなどは `.editorconfig` ファイルを認識させるために、専用のプラグインをインストールする必要があります。

各エディタのプラグインは下記からダウンロードできます。

- [EditorConfig Plugin Download](https://editorconfig.org/#download)

（各エディタのアイコン画像をクリックすると、該当ページにジャンプします。分かりにくい…）

### Android Studio の場合

Android Studio (IntelliJ IDEA) は、デフォルトで `.editorconfig` を認識してくれます。
プロジェクトのディレクトリ内に `.editorconfig` ファイルを配置するだけで、フォーマット時 (`Ctrl + Alt + L`) の振る舞いに反映されます。

Android Studio の設定画面を見ると、次のようにデフォルトで `EditorConfig` が有効になっていることが分かります。

{{< image src="img-002.png" title="Android Studio の設定画面" >}}

### Visual Studio Code (VS Code) の場合

VS Code 用のプラグイン（拡張）は、下記のマーケットプレイスで `Install` ボタンを押すか、VS Code の `Extensions` ペーンから `EditorConfig for VS Code` を検索してインストールできます（どちらも VS Code のインストール画面に飛びます）。

- [EditorConfig for VS Code](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig)

これだけで、すぐに `.editorconfig` の設定が有効になります。
VS Code を再起動する必要もありません。

### Vim の場合

Vim 用の EditorConfig プラグインをインストール方法はいくつかあるので、詳しくは下記のページを参照してください。

- [EditorConfig plugin for Vim](https://github.com/editorconfig/editorconfig-vim#readme)

例えば、プラグイン管理に Vundle を使用しているのであれば、`.vimrc` に次のような行を追加し、

{{< code lang="vimrc" title=".vimrc" >}}
Plugin 'editorconfig/editorconfig-vim'
{{< /code >}}

次のように実行するだけでインストールできます。

{{< code >}}
:PluginInstall
{{< /code >}}


（おまけ）パターンに一致するファイルのルールをオーバーライドする
----

`.editorconfig` ファイルの中で、同じ名前のプロパティが複数個所で設定されている場合は、最後に見つかったものが優先的に使われます。
この性質を利用して、特定のファイルに対するルールだけをオーバーライドすることができます。

例えば、ユニットテスト関連のファイルだけルールを緩くしたいという場合は、次のような感じで設定値を上書きするようにします（優先度の高いものが下に来るように記述するのがポイントです）。

{{< code title=".editorconfig" >}}
# Kotlin のファイルは基本 100 文字で折り返し
[*.{kt,kts}]
max_line_length = 100

# 名前が Test で終わるファイルでは長い関数名を付けたいことがあるので折り返し制限しない
[*Test.kt]
max_line_length = off

# test ディレクトリ以下のファイルも同様に折り返し制限しない
[test/**.kt]
max_line_length = off
{{< /code >}}


（おまけ）ktlint などのフォーマットツールでの利用
----

Kotlin の lint ツールである [ktlint](https://github.com/pinterest/ktlint) は、設定フリーなツール（デフォルトで Kotlin の標準的なルールに従う）として開発されていますが、プロジェクトに `.editorconfig` ファイルが存在する場合はその設定に従って Kotlin コードのスタイルチェック（および自動フォーマット）を行うようになっています。
例えば、EditorConfig の `indent_size` が定義されていると、`ktlint` はその設定に従ってインデントのチェックを行います。

`ktlint` のデフォルトのルールは、[Standard rules](https://github.com/pinterest/ktlint#standard-rules) のところに一覧があります。
それぞれのルールには ID が割り当てられており、`.editorconfig` 内で個別に無効化することができます。
例えば、`ktlint` はデフォルトでワイルドカードを使ったインポートを禁止 (ID: `no-wildcard-imports`) していますが、このルールを無効にするには、次のように `disabled_rules` でルール ID を指定します（カンマ区切りで複数指定可能です）。

{{< code title=".editorconfig" >}}
# ktlint のルールを無効化する
[*.{kt,kts}]
disabled_rules = no-wildcard-imports
{{< /code >}}

