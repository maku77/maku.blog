---
title: "TypeScript のコーディング規約（ルール／ガイドライン）"
linkTitle: "コーディング規約（ルール／ガイドライン）"
url: "/p/avssq37"
date: "2020-02-27"
tags: ["TypeScript"]
weight: 2
---

有名な JavaScript/TypeScript スタイル
----

TypeScript のコーディングスタイルは下記のサイトが参考になります。

- [スタイルガイド（コーディング規約） - TypeScript Deep Dive 日本語版](https://typescript-jp.gitbook.io/deep-dive/styleguide)
    - TypeScript Deep Dive のスタイルガイドは、重要なポイントが簡潔にまとまっていてわかりやすいです。
- [standard/standard: JavaScript Standard Style](https://github.com/standard/standard)
    - セミコロンの省略を推奨していますが、2021 年時点ではまだセミコロン有り派の方が多いようです。モダンなプログラミング言語では行末にセミコロンを付けないものが多いので、セミコロン省略派が増えてきているというのも何となく頷けます。
    - 関数定義の際に、カッコの前にスペースを強制するところがちょっと気持ち悪いです。
    - Node.js、npm、GitHub、Electron など有名どころが採用しています。
    - [ドキュメントサイト](https://standardjs.com/rules-ja.html)
- [airbnb/javascript: JavaScript Style Guide](https://github.com/airbnb/javascript)
    - Airbnb は JS スタイルを細かく定義しています。ただ、長大すぎるので、重要なポイントをかいつまんで読むのには向いてません。[ESLint などのツールを使ってスタイル強制する](/p/xz9iry9) 場合にもよく使われますが、TypeScript 対応が不完全で、モジュールインポート時の拡張子省略がエラーになったりします（2021-05 時点）。
- [Coding guidelines · microsoft/TypeScript Wiki](https://github.com/Microsoft/TypeScript/wiki/Coding-guidelines)
    - TypeScript 自体のコントリビューター用のコーディングガイドラインです。ユーザーレベルの開発プロジェクトに強制するものではないと注記がありますが、多くのルールはそのまま採用できると思います。インデントサイズは 4 とされていますが、多くの TypeScript プロジェクトではインデントサイズは 2 が採用されているので、ここだけ例外的です。
- [google/styleguide: Google Style Guides](https://github.com/google/styleguide)
    - JavaScript/TypeScript 以外にもいろいろな言語の Google スタイルが説明されています。
    - JavaScript/TypeScript の世界では、上記の JavaScript Standard か Airbnb のスタイルの方が人気があるようです。
    - [ESLint 用の設定ファイルはこちら](https://github.com/google/eslint-config-google)

まとめると、多くのスタイルガイドでは次のようなルールが採用されています。

- インデントは __2 文字__（4 文字を採用しているのはごく一部のプロジェクトのみ）
- ドキュメンテーションコメントには __JSDoc__ の記法を使う
- 命名規則
    - 型名（クラス名、列挙型、エイリアスなど）は __`PascalCase`__
    - 変数名や関数名は __`camelCase`__
    - プライベートプロパティ名を `_` で始めない
- 文字列リテラル は __`'シングルクォーテーション'`__ で囲む
    - シングルクォーテーションを含む文字列はダブルクォーテーションで囲んで OK
- 比較には `==` ではなく __`===`__ を使用する。ただし、下記は例外。
    - `obj == undefined` ... `null` あるいは `undefined` であることのチェック
    - `obj != undefined` ... `null` でも `undefined` でもないことのチェック
- 値の設定には `null` も `undefined` も使用せず、代わりに `{ valid: boolean, value?: Foo}` のようなオブジェクトを返すことを検討する
    - どうしても明示しなければいけないケースでは __`undefined`__ に統一（`undefined` と `null` は同じものとして使って通常は問題ない）
    - `null` は API で指定されている場合のみ使う（コールバック関数の `error` 引数として `null` を渡すとか）
- 開き括弧 (`{`) は同じ行の末尾に記述する


結局どのスタイルがよい？
----

多くのケースでは、JavaScript Standard か Airbnb スタイルのどちらかを採用しているようですが、一応 GitHub リポジトリのスター数比較（2021-05時点）を載せておきます。
スター数だけでいうと、圧倒的に Airbnb スタイルが人気がありますが、とりあえずシンプルなスタイルだけでも適用したいということであれば、Standard スタイルがよいかもしれません。

- [Github Compare](https://www.githubcompare.com/airbnb/javascript+standard/standard+google/eslint-config-google)
    - airbnb/javascript: 109,050 stars
    - standard/starndard: 25,509 stars
    - google/eslint-config-google: 1,510 stars

