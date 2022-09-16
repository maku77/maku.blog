---
title: "GraphQL 関連用語"
url: "p/jz78nmk/"
date: "2021-07-26"
tags: ["GraphQL"]
---

GraphQL 関連用語
----

__クエリ文字列 (query string)__
: `query {...}` という文字列全体のこと。

__操作タイプ (operation type, root type)__
: `query {...}` などの `query` というキーワード部分のこと。操作の種類を表しており、`query` の他には `mutation` と `subscription` があります。

__操作名 (operation name)__
: `query HogeHoge {...}` などの `HogeHoge` の部分のこと。特に query 操作の場合は __query name__、mutation 操作の場合は __mutation name__ と呼ぶことがあります。操作名の指定は multi-operation documents じゃない限りオプショナルですが、ログ解析やデバッグをしやすくするために、名前を付けることが推奨されています。

__クエリ変数 (query variables)__
: `query User($name: String!) {...}` などの `$name` の部分のこと。

__クエリ引数 (query arguments)__
: `book(id: "xyz") {...}` など、フィールド参照時に指定する `id: "xyz"` の部分のこと。クエリに変数が渡されている場合は、ここで `book(id: $bookId)` のような感じで参照できます。

__選択セット、セレクションセット (selection sets)__
: `query HogeHoge {...}` などの `{...}` の部分のこと。どのフィールドを取得したいかを選択するので、選択セットと呼びます。

__フィールド (field)__
: GraphQL オブジェクトの各プロパティ部分のこと。例えば、`book { id title }` というクエリで Book オブジェクトを取得するときの `id` とか `title` の部分。大きく分けて 2 種類あり、Scalar types（末端ノードそれ自体が値）と、Object types（ネストされたフィールドから構成される）があります。

__入力オブジェクト型 (input object type, input types)__
: 引数専用のオブジェクト型のこと。文字列 (String) や数値 (Int) などのスカラー値と比べて、複雑なデータを渡せます。例えば、新規データの作成要求 (mutation) を行うときに、新規データの内容を表現するために使われます。定義方法自体は普通のオブジェクト型 (`type`) と同様ですが、入力型 (`input`) はそれ自身のデータを取得する用途には使えないところが異なります（つまり、クエリ内のフィールドとしては指定できないということです）。

