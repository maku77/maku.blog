---
title: "textlint で表記揺れをチェックする (proofread-helper, prh)"
url: "/p/m4cks29"
date: "2020-06-18"
tags: ["textlint"]
---

prh と textlint-rule-prh
----

テキストファイルの表記揺れをチェックする Node.js 製ツールに、[proofread-helper (prh)](https://github.com/prh/prh) というものがあります。
これを使うと、例えば、テキストファイル内の `javascript` という文字列を `JavaScript` に自動的に修正できたりします（大文字・小文字の構成）。

textlint の作者の azu さんが、この prh を textlint から使えるようにするルール定義 [textlint-rule-prh](https://github.com/textlint-rule/textlint-rule-prh) を作成してくれています。
prh を直接使うよりも、Markdown ファイルの構文をうまく扱ってくれるみたいです（リンク内のテキストは対象外にするなど）。


インストール
----

### ルールのインストール

textlint 用のルールモジュールである `textlint-rule-prh` は、次のようにインストールします（textlint 自体はインストール済みであるとします）。
proofread-helper を別途インストールする必要はありません。

{{< code >}}
$ npm install --save-dev textlint-rule-prh
{{< /code >}}

### 設定ファイルの作成

proofread-helper (prh) の設定ファイル（表記揺れチェックルール）は、下記のような感じで Yaml ファイルで作成します。

{{< code lang="yaml" title="rules.yml" >}}
version: 1
rules:
  # 大文字小文字全角半角の統一
  - expected: JavaScript
  - expected: jQuery
{{< /code >}}

textlint の設定ファイルで `textlint-rule-prh` を有効にし、上記のルールファイルのパスを指定します。

{{< code lang="json" title=".textlintrc" >}}
{
  "rules": {
    "prh": {
      "rulePaths": [
        "./rules.yml"
      ]
    }
  }
}
{{< /code >}}


実行
----

あとは、textlint を実行すれば、表記揺れを検出してくれます。

{{< code >}}
$ textlint "content/**/*.md"

/Users/maku/blog/content/page1.md
  175:1  ✓ error  jquery => jQuery  prh

/Users/maku/blog/content/page2.md
  237:1  ✓ error  javascript => JavaScript  prh

✖ 2 problem (2 error, 0 warnings)
✓ 2 fixable problem.
Try to run: $ textlint --fix [file]
{{< /code >}}

`textlint-rule-prh` は auto fix に対応しているので、 __`--fix`__ オプションを指定して実行すれば、ファイル内の表記揺れを自動で修正して保存してくれます。

{{< code >}}
$ textlint --fix "content/**/*.md"
{{< /code >}}


表記揺れルール
----

proofread-helper (prh) では柔軟な表記揺れルールを定義することができます。
書き方のサンプルは [本家の記述例](https://github.com/prh/prh#%E8%A8%AD%E5%AE%9A%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6) を見ると分かりやすいですが、いくつかポイントをまとめておきます。

### 大文字と小文字の統一

{{< code lang="yaml" >}}
rules:
  - expected: jQuery
{{< /code >}}

このように `expected` のみを指定しておくと、大文字・小文字の違いだけがある単語を検出してくれます（例: `JQuery` → `jQuery`）。

### パターンに一致したものを修正

{{< code lang="yaml" >}}
rules:
  - expected: ソフトウェア
    pattern: ソフトウエア
{{< /code >}}

`pattern` に一致する単語が見つかったら、それを `expected` に修正するよう促します。

次のように `pattern` に複数の単語を指定することもできます（`pattern` と `patterns` は同義）。

{{< code lang="yaml" >}}
rules:
  - expected: ソフトウェア
    patterns:
      - ソフトウエア
      - ソフトウエアー
      - ソフトウェアー
{{< /code >}}

`pattern` の部分に `/正規表現/` という指定も可能です。

{{< code lang="yaml" >}}
rules:
  - expected: ソフトウェア
    patterns:
      - /ソフトウ[エ|ェ]アー/
      - ソフトウエア
{{< /code >}}

### 変換のテスト仕様を記述する

正規表現でパターン指定した場合は、 __`specs`__ プロパティでテストを記述しておくことよいでしょう。
設定したルールで `from` を `to` にうまく変換できない場合は、spec failed となり動作を停止します。

{{< code lang="yaml" >}}
rules:
  - expected: 技術書典
    pattern:  /技術書(店|点|展|てん)/
    specs:
      - from: 技術書点
        to:   技術書典
      - from: 技術書展
        to:   技術書典
{{< /code >}}

### サフィックスを追加するときの注意

{{< code lang="yaml" >}}
rules:
  - expected: サーバー
    pattern:  /サーバ(?!ー)/
    specs:
      - from: サーバ
        to:   サーバー
    prh: 最後に伸ばすのが最近の主流みたいです。
{{< /code >}}

例えば、「サーバ」を「サーバー」にするなど、既存の単語にサフィックスを追加する場合はちょっと注意が必要です。
単純に `pattern` に「サーバ」と指定すると、既に「サーバー」になっているものを「サーバーー」に修正しようとしてしまうので、上記のように指定しておく必要があります。
「ユーザ」を「ユーザー」にしたいときなども同様です。

### スペースが必要な単語

{{< code lang="yaml" >}}
rules:
  - expected: VS Code
    pattern: /vs ?code/i
    specs:
      - from: vscode
        to: VS Code
      - from: vs code
        to: VS Code
      - from: VS Code
        to: VS Code
{{< /code >}}

スペースなしの `VSCode` を、スペースありの `VS Code` に修正するよう促します。
上記の正規表現だと、もともと正しい `VS Code` にもヒットしてしまいそうですが、その場合はうまいこと無視してくれるみたいです。

### 短い英単語では wordBoundary 指定しておく

`js` や `id` といった、短い単語の大文字・小文字を変換するときは、次のように __`wordBoundary`__ オプションを `true` に設定しておくのがよいようです。
こうしておくと、ある単語の部分文字列として出現したときに無視してくれるようになります。

{{< code lang="yaml" >}}
rules:
  - expected: js
  # pattern: "/\b[JjＪｊ][SsＳｓ]\b/g" # と等価 \b が前後に付与される
    options:
      wordBoundary: true
    specs:
      - from: foo JS bar
        to:   foo js bar
      - from: foo altJS bar
        to:   foo altJS bar
      # 日本語+単語境界の仕様は自分で調べてね…！
      - from: 今日もJS祭り
        to:   今日もjs祭り
{{< /code >}}

