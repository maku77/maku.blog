---
title: "GraphQL 関連用語"
url: "p/jz78nmk/"
date: "2021-07-26"
tags: ["GraphQL"]
---

ドキュメント (document)
: GraphQL リクエストで送る全体の文字列のこと。
HTTP Get リクエストで指定する場合は、`query` というクエリパラメーターで指定します（これは紛らわしいので、本当は `document` という名前になっているべきでした）。
HTTP Post リクエストで指定する場合は、ペイロードの JSON 内の `query` プロパティで指定します。
複数の GraphQL Operation（操作）や Fragment（フラグメント）を含むことができます。

クエリ (query)
: ドキュメント全体のことを示す場合と、`query {...}` という個々のクエリ操作を示す場合があります。後者の場合は、データの取得要求を表しています。

操作タイプ (operation type, root type)
: `query {...}` などの `query` というキーワード部分のこと。操作の種類を表しており、__`query`__ の他には __`mutation`__ と __`subscription`__ があります。

操作名 (operation name)
: `query GetHoge {...}` などの `GetHoge` の部分のこと。特に query 操作の場合は __query name__、mutation 操作の場合は __mutation name__ と呼ぶことがあります。操作名の指定は multi-operation documents じゃない限りオプショナルですが、ログ解析やデバッグをしやすくするために、名前を付けることが推奨されています。
HTTP Get リクエストで指定する場合は、`operationName` というクエリパラメーターで指定します。
HTTP Post リクエストで指定する場合は、ペイロードの JSON 内の `operationName` プロパティで指定します。

マルチ操作ドキュメント (multi-operation documents)
: 複数の query や mutation リクエストを含むドキュメント。どの操作を実行するかは、操作名 (operation name) で指定します。

クエリ変数 (query variables)
: `query User($name: String!) {...}` などの `$name` の部分のこと。`{"name": "maku"}` のように表現します。
HTTP Get リクエストで指定する場合は、`variable` というクエリパラメーターで指定します。
HTTP Post リクエストで指定する場合は、ペイロードの JSON 内の `variables` プロパティで指定します。

クエリ引数 (query arguments)
: `book(id: "xyz") {...}` など、フィールド参照時に指定する `id: "xyz"` の部分のこと。クエリに変数が渡されている場合は、ここで `book(id: $bookId)` のような感じで参照できます。

選択セット、セレクションセット (selection sets)
: `query GetHoge {...}` などの `{...}` の部分のこと。どのフィールドを取得したいかを選択するので、選択セットと呼びます。

フィールド (field)
: GraphQL オブジェクトの各プロパティ部分のこと。例えば、`book { id title }` というクエリで Book オブジェクトを取得するときの `id` とか `title` の部分。大きく分けて 2 種類あり、Scalar types（末端ノードそれ自体が値）と、Object types（ネストされたフィールドから構成される）があります。

入力オブジェクト型 (input object type, input types)
: 引数専用のオブジェクト型のこと。文字列 (String) や数値 (Int) などのスカラー値と比べて、複雑なデータを渡せます。例えば、新規データの作成要求 (mutation) を行うときに、新規データの内容を表現するために使われます。定義方法自体は普通のオブジェクト型 (`type`) と同様ですが、入力型 (`input`) はそれ自身のデータを取得する用途には使えないところが異なります（つまり、クエリ内のフィールドとしては指定できないということです）。

GraphQL over HTTP
: [GraphQL の仕様 (Spec)](https://spec.graphql.org/) はクエリやスキーマの扱い方などを定義したものですが、現実的に GraphQL API は HTTP 経由で提供することが多いので、[GraphQL over HTTP](https://graphql.github.io/graphql-over-http/) という仕様が別途定義されています。

