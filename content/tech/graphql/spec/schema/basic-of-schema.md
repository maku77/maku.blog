---
title: "GraphQL スキーマ仕様: スキーマ定義の基本"
url: "p/5s5jvfz/"
date: "2022-09-14"
tags: ["GraphQL"]
weight: 1
draft: true
---

GraphQL スキーマとは
----

GraphQL スキーマは、GraphQL API サーバーがどんなクエリを受け付けてくれるかを示すもので、__GraphQL Schema Language__ という独自言語 (DSL) で記述します。
スキーマは型情報だけを定義したものであり、サーバーの実装ロジックは考慮しません（各型のデータを返すための具体的な実装は「リゾルバー」と呼ばれており、こちらはプログラミング言語に依存した実装を行います）。

GraphQL スキーマは、クライアントのニーズを把握して、クライアントファーストで考えるのがポイントです。
サーバーのバックエンドで使用する技術（RDB など）に影響されたスキーマ定義をしてはいけません。

{{% note title=".graphql vs .graphqls" %}}
GraphQL スキーマを独立したファイルとして作成する場合は、ファイルの拡張子を __`.graphqls`__ あるいは __`.graphql`__ とするのが一般的です。
`.graphqls` の方は「GraphQL のスキーマ」を表し、`.graphql` の方は「GraphQL のクエリドキュメント」を表すので明確に分けるべきだという主張もあります。
迷ったら、スキーマファイルには `.graphqls` を使っておけば間違いないと思います。
{{% /note %}}


オブジェクト型
----

スキーマの中で基本的な要素となるのはオブジェクト型で、__`type`__ キーワードを使って次のように定義します。

```graphql
type オブジェクト型の名前 {
  フィールド名: 型
  フィールド名: 型
  フィールド名: 型
}
```

これは、GraphQL クエリによって取得可能なデータの構造を表しています。
例えば、下記は書籍情報を表すオブジェクト型の例です。

```graphql
type Book {
  id: ID!
  year: Int
  title: String!
  authors: [String!]
  tags: [String!]!
}
```

`Book` オブジェクトが持つフィールドは、__`フィールド名: 型`__ という形式で定義します。
型には、[スカラー型](/p/tw75a7p)、[列挙型](/p/isotm77/)、リスト、オブジェクト型などを指定できます。
すべての型はデフォルトで Nullable であり、Non-null なフィールドであることを示すためには、型名の末尾に __`!`__ を付ける必要があります。
上記の例の場合、各フィールドの定義は次のような意味を持ちます。

`id: ID!`
: `id` フィールドは `ID` 型（組み込みのスカラー型のひとつ）であり、`null` になることはない。

`year: Int`
: `year` フィールドは整数型であり、`null` になる可能性がある。

`title: String!`
: `title` フィールドは文字列型であり、`null` になることはない。

`authors: [String!]`
: `authors` フィールドは文字列を要素に持つリストであり、各要素が `null` になることはない。ただし、`authors` フィールド自体は `null` になる可能性がある。

`tags: [String!]!`
: `tags` フィールドは文字列を要素に持つリストであり、各要素が `null` になることはない。`tags` フィールド自体も `null` になることはない（少なくとも空リストが返される）。


Query のルートフィールドを定義する
----

__Query__ 型のフィールドを定義すると、GraphQL クエリのエントリポイント（トップレベルで指定するフィールド）として指定できるようになります。
例えば、次の `Query` 型は、すべての書籍を取得するためのフィールド (`books`) と、指定した ID の書籍を取得するためのフィールド (`book`) を定義しています。

{{< code lang="graphql" title="スキーマ定義の例" >}}
type Query {
  books: [Book!]!
  book(id: ID!): Book
}
{{< /code >}}

`books` フィールドの型は `[Book!]!` なので、`Book` オブジェクトのリストが返ってきます。
`book` フィールドの型は `Book` なので、単一の `Book` オブジェクト、あるいは `null` が返ってきます。
このようなスキーマを実装する GraphQL サーバーは、クライアントからの次のようなクエリを処理できることを意味します。

{{< code lang="graphql" title="クエリの例" >}}
query {
  books {
    id
    title
  }
  book(id: "a1b2c3d") {
    id
    title
  }
}
{{< /code >}}


Mutation のルートフィールドを定義する
----

GraphQL におけるデータの変更要求は、ミューテーション処理 (`mutation`) を送信するによって行います。
クライアントが要求可能なミューテーション処理の一覧は、__Mutation__ 型のフィールドとして定義します。
次のスキーマでは、`Mutation` 型のフィールドとして `createBook` と `deleteBook` を定義しています。

{{< code lang="graphql" title="スキーマ定義の例" >}}
type Mutation {
  createBook(title: String!): Book
  deleteBook(id: String!): Boolean!
}
{{< /code >}}

GraphQL クライアントは、次のように __`mutation { ... }`__ ドキュメントのトップレベル・フィールドとして、`createBook` 操作や `deleteBook` 操作を指定できます。

{{< code lang="graphql" title="ミューテーション要求の例" >}}
mutation CreateBook {
  createBook(title: "My book") {
    id
    title
  }
}
{{< /code >}}

- 参考: [GraphQL スキーマ仕様: mutation を定義する（データ更新 API）](/p/pk9c9qs/)

