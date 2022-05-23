---
title: "GraphQL ドラフトメモ"
url: "p/xaj8rac/"
date: "2022-05-23"
tags: ["GraphQL"]
draft: true
---

GraphQL のスキーマ（どんなクエリを受け付けてくれるかの定義）は、__GraphQL Schema Language__ という独自言語で定義します。

スキーマとして、__Query__ タイプのオブジェクトを定義すると、クエリのエントリポイント（トップレベルで指定するオブジェクト）として指定できるようになります。
例えば、

{{< code lang="graphql" title="例: スキーマ定義" >}}
type Query {
  hero(episode: Episode): Character
  droid(id: ID!): Droid
}
{{< /code >}}

となっていれば、次のようなクエリを要求できます。

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

__Mutation__ タイプも同様で、`type Mutation` で定義されているフィールドを、`mutation { ... }` のようにトップレベルフィールドとして要求できます。

