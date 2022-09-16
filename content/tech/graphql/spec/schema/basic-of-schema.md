---
title: "GraphQL のスキーマ定義の基本"
url: "p/5s5jvfz/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
weight: 100
---

{{% private %}}
- type ... 独自オブジェクト (custom object) を表現する。
- スキーマ定義は拡張子 `.graphql` のファイルで行う。
- 型の末尾に `!` が付いているものは Non-null フィールド。それ以外は Nullable フィールド。
- カスタム・スカラタイプの定義方法
    - `scalar DateType`
- Enum 型の定義方法
    - `enum PhotoCategory { ... }`
{{% /private %}}

GraphQL スキーマは、GraphQL API サーバーがどんなクエリを受け付けてくれるかを示したものです。
GraphQL スキーマは、__GraphQL Schema Language__ という独自言語 (DSL) で記述します。

スキーマとして、__Query__ 型のオブジェクトを定義すると、クエリのエントリポイント（トップレベルで指定するオブジェクト）として指定できるようになります。
例えば、

{{< code lang="graphql" title="例: スキーマ定義" >}}
type Query {
  hero(episode: Episode): Character
  droid(id: ID!): Droid
}
{{< /code >}}

となっていれば、次のようなクライアントからのクエリを処理できることを意味します。

{{< code lang="graphql" >}}
query {
  hero {
    name
  }
  droid(id: "2000") {
    name
  }
}
{{< /code >}}

__Mutation__ 型も同様で、`type Mutation` で定義されているフィールドを、`mutation { ... }` のようにトップレベルフィールドとして要求できます。


- - - -

Query root field は、Query 型の定義の直下に配置します。
これが API のエントリポイントとなるクエリ名になります。

```graphql
type Query {
  books: [Book!]
  # ...
}
```

上記の例では、ルートの `books` フィールドの型が `[Book!]` であると定義しています。
これは `Book` 要素 (non-null) の配列です。

