---
title: "textlint のインストールと基本的な使い方"
url: "/p/3veuap5"
date: "2020-05-20"
tags: ["textlint"]
weight: 1
---

textlint とは
----

{{< image w="400" src="img-001.png" >}}

__textlint__ は、テキストファイルや Markdown ファイル用の記述内容をチェックするための校正ツールです。

- [textlint · The pluggable linting tool for text and markdown](https://textlint.github.io/)

たとえば、「である・ですます調の不一致」、「単語の表記揺れ」、「句読点の使いすぎ」など、様々なルールを定義して文章をチェックすることができます。
多人数で管理する社内ドキュメントはもちろんのこと、個人のブログなどで使用すれば、すべての文章で一定の品質を保つことができるようになります。


textlint のインストール
----

textlint は [Node.js](https://nodejs.org/) のパッケージとして配布されているので、まだインストールされていない場合は先に Node.js をインストールしておく必要があります。

### カレントプロジェクト用にインストールする

カレントプロジェクト用に textlint をインストールするには、プロジェクト（テキストファイルが入ったディレクトリ）のルートディレクトリに移動し、次のように `npm` コマンドを実行します。

{{< code title="textlint をインストール" >}}
$ npm init -y  # なければ package.json を作成
$ npm install --save-dev textlint
{{< /code >}}

{{< note >}}
この方法でインストールすると、カレントディレクトリの `node_modules` ディレクトリ以下に、ローカルモジュールとして textlint がインストールされます。
`--save-dev (-D)` オプションを付けることで、依存パッケージの情報が `package.json` および `pacakge-lock.json` ファイルに記録され、他のユーザーが `npm install` で同じ環境を構築できるようになります。
{{< /note >}}

次のように textlint コマンドを実行できれば成功です。

{{< code >}}
$ npx textlint -v
v11.6.3
{{< /code >}}

{{< note title="npx コマンドについて" >}}
`npx` コマンドはローカルパッケージを起動するためのユーティリティで、`npx textlint` は、`./node_modules/.bin/textlint` のショートカットとして使用できます。
`npx` コマンドは `npm` コマンドに付属しているので、別途インストールする必要はありません。
{{< /note >}}


### システム全体にインストールする

次のようにして、textlint コマンドをシステム全体にインストールすることもできます。

{{< code title="システム全体にインストール" >}}
$ npm install --global textlint
{{< /code >}}

このようにインストールした場合は、`npx` プレフィックスなしで直接実行できるようになります。

{{< code >}}
$ textlint -v
v11.6.3
{{< /code >}}

どちらの方法でインストールするか迷ったら、前者の方法でインストールしておきましょう。他の人とファイルを共有したときに環境を合わせやすくなります。


textlint のルール設定
----

textlint はデフォルトでは何のチェックも有効になっておらず、__`.textlintrc`__ という設定ファイルでルールを有効化していく必要があります。
使用できるルールは、下記のサイトにまとめられています。

- [Collection of textlint rule](https://github.com/textlint/textlint/wiki/Collection-of-textlint-rule)

例として、ここでは、次のようなルールを有効化してみます。

- `textlint-rule-no-dropping-the-ra` ... 「ら抜き言葉」が使われていないかをチェック
- `textlint-rule-no-mix-dearu-desumasu` ... 「ですます」調と「である」調の混在をチェック

textlint のルールは、本体とは別に Node パッケージとして提供されているので、次のようにインストールします。

{{< code title="textlint のルールをインストール" >}}
$ npm install --save-dev textlint-rule-no-dropping-the-ra
$ npm install --save-dev textlint-rule-no-mix-dearu-desumasu
{{< /code >}}

{{< note >}}
グルーバルにインストールした textlint コマンドを使用する場合は、ルールをインストールするときも `--global` オプションを使ってインストールする必要があります。
{{< /note >}}

あとは、プロジェクトルートに置いた設定ファイルで次のように有効化します（[設定ファイルの書き方の詳細はこちら](https://textlint.github.io/docs/configuring.html)）。

{{< code lang="yaml" title=".textlintrc" >}}
{
  "rules": {
    "no-dropping-the-ra": true,
    "no-mix-dearu-desumasu": true
  }
}
{{< /code >}}

設定ファイル内で指定するルール名には、`textlint-rule-` というプレフィックスを除いた名前を指定します。

{{< note >}}
インストール済みのルールをすべて有効にした `.textlintrc` を新しく作成する場合は、 __`textlint --init`__ コマンドを使用できます。
{{< /note >}}


textlint を実行する
----

ここでは、サンプル文章として次のようなテキストファイルを用意します。

{{< code title="sample.txt" >}}
吾輩は猫である。名前はまだないです。
猫は食べれません。
{{< /code >}}

カレントディレクトリ以下のすべての `.txt` ファイルに対して textlint をかけるには次のようにします。

{{< code >}}
$ npx textlint "**/*.txt"
{{< /code >}}

{{< code title="実行結果" >}}
/Users/maku/sample.txt
  1:5  error  本文: "である"調 と "ですます"調 が混在
=> "ですます"調 の文体に、次の "である"調 の箇所があります: "である。"
Total:
である  : 1
ですます: 1
  no-mix-dearu-desumasu
  2:5  error  ら抜き言葉を使用しています。                                            no-dropping-the-ra

✖ 2 problems (2 errors, 0 warnings)
{{< /code >}}

1 行目に「である」調と「ですます」調が混在しているというエラーと、2 行目に「ら抜き言葉」があるというエラーが出ています。

以上が textlint の基本的な使い方になります。
ここではごく簡単なルールだけを紹介しましたが、他にも [WEB+DB PRESS用語統一ルール](https://github.com/textlint/textlint/wiki/Collection-of-textlint-rule#textlint-rule-web-plus-db) や、ルール設定をまとめた [技術文書向けのtextlintルールプリセット](https://github.com/textlint/textlint/wiki/Collection-of-textlint-rule#textlint-rule-preset-ja-technical-writing) など、複雑なルールも用意されています。
ただ、既存の文書に複雑なルールをまとめて適用すると、大量にエラーが出て悲しくなるので、少しずつ自分に合ったチェックだけ有効にしていくという使い方がよいかもしれません。


npm のスクリプトとして textlint を実行できるようにする
----

`textlint` コマンドを頻繁に実行する場合は、次のように __`package.json`__ ファイルに `lint` スクリプトとして定義しておくとよいでしょう。

{{< code lang="json" title="package.json" >}}
{
  "name": "my-site",
  "version": "0.0.1",
  "scripts": {
    "lint": "textlint \"content/**/*.md\""
  },
  "devDependencies": {
    "textlint": "^11.6.3"
  }
}
{{< /code >}}

これで、下記のように `textlint` を実行できるようになります。

{{< code >}}
$ npm run lint
{{< /code >}}


textlint でキャッシュを有効にする
----

`textlint` の実行はものすごく遅い（数百ファイルを対象にすると何十秒もかかったりする）ので、 __`--cache`__ オプションを使いましょう。

{{< code >}}
$ textlint --cache "content/**/*.md"
{{< /code >}}

このように実行すると、カレントディレクトリに `.textlintcache` というキャッシュファイルが作成されて、次回の実行からは変更されたファイルのみがチェック対象になります（それでも何秒もかかりますけど...）。

このファイルにはローカル PC 内のファイル情報が含まれることになるので、Git でファイル管理している場合は、 __`.gitignore`__ に `.textlintcache` を登録しておきましょう。

