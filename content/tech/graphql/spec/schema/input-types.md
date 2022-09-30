---
title: "GraphQL スキーマ仕様: 入力型 (input type) を定義する"
url: "p/nhhwqtu/"
date: "2022-09-28"
tags: ["GraphQL"]
---

GraphQL スキーマで定義する __入力型 (input type)__ は、クエリ／ミューテーション要求時のフィールド引数にのみに使用できるオブジェクト型です。
入力型を使うと、複数のフィールド引数をひとつのオブジェクトにまとめることができます。

次の `Mutation` 型の `createBook` フィールドは、フィールド引数として `CreateBookInput` という入力型を使っています。

{{< code lang="graphql" title="スキーマ定義" >}}
type Mutation {
  createBook(input: CreateBookInput!): Book
}
{{< /code >}}

入力型を定義するときは __`input`__ キーワードを使用します（フィールド自体を表す型の定義には `type` キーワードを使用します。こちらは出力型と呼ばれたりします）。
フィールドの定義方法は `type` も `input` も同じです。

次のスキーマは、上記の `createBook` フィールドの型 (`Book`) と、その引数の型 (`CreateBookInput`) を定義しています。
`type` キーワードと `input` キーワードを使い分けていることに注意してください。

{{< code lang="graphql" title="スキーマ定義" >}}
type Book {
  id: ID!
  title: String!
  author: String
  year: Int
}

input CreateBookInput {
  title: String!
  author: String
  year: Int
}
{{< /code >}}

`Book` 型と `CreateBookInput` 型のフィールドはほとんど共通ですが、`Book` 型には GraphQL サーバー側で自動生成される `id` 情報が含まれています。

クライアントがミューテーション要求を送るときは、ミューテーション操作の変数経由で入力型のデータを渡します。

{{< code lang="graphql" title="ミューテーションの実行" >}}
mutation CreateBook($input: CreateBookInput!) {
  createBook(input: $input) {
    id
    title
    author
    year
  }
}
{{< /code >}}

実際に使用する変数値は、次のような JSON 形式のデータとして、上記ドキュメントと一緒に GraphQL サーバーに送ります。

{{< code lang="graphql" title="variables" >}}
{
  "input": {
    "title": "書籍タイトル",
    "author": "まく"
  }
}
{{< /code >}}

ミューテーション操作の戻り値は、一般的には新規作成した（あるいは更新後の）オブジェクトの内容です。

{{< code lang="json" title="上記ミューテーションのレスポンス" >}}
{
  "data": {
    "createBook": {
      "id": "d3cn2sx",
      "title": "書籍タイトル",
      "author": "まく",
      "year": null
    }
  }
}
{{< /code >}}

上記の `createBook` フィールドの型（ミューテーション操作の戻り値）は `Book` 型でしたが、`CreateBookPayload` のような別の出力型を定義すれば、ミューテーション操作に関するメタ情報を含めることができます。
例えば、ミューテーション操作によって実際に追加されたデータの数などを戻り値として返すことができます。

```graphql
type Mutation {
  createBook(input: CreateBookInput!): CreateBookPayload!
}
```

