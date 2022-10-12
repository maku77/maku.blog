---
title: "GraphQL スキーマ仕様: 列挙型 (enum type) を定義する"
url: "p/isotm77/"
date: "2022-09-28"
tags: ["GraphQL"]
---

列挙型の基本
----

GraphQL スキーマでは、__`enum`__ キーワードを使って列挙型を定義することができます。
列挙型は、スカラー型と同様に単一の値を表しますが、とり得る値があらかじめ限定されています。
`String` や `Int` などの代わりに適切な列挙型を使えば、API の使い方が明確になり、クライアントによる誤用を防ぐことができます。

次の例では、`Episode` という名前の列挙型を定義しています。

{{< code lang="graphql" title="GraphQL スキーマでの列挙型の定義" >}}
enum Episode {
  NEWHOPE
  EMPIRE
  JEDI
}
{{< /code >}}

列挙型は、フィールドのパラメーターの型として使用することができます。

```graphql
type Query {
  hero(episode: Episode): Character
}
```

列挙型は、フィールドの型としても使用できます。

```graphql
type Character {
  name: String!
  appearsIn: [Episode]!
}
```


列挙型の使用例
----

列挙型の典型的な使用例として、ソート用のフィールド・パラメーターがあります。
下記は、GitHub の GraphQL スキーマで定義されている列挙型の例です。
`GistOrder` [入力型](/p/nhhwqtu/) のフィールドに、`GistOrderField` 列挙型が使われています。

{{< code lang="graphql" title="GitHub の GraphQL スキーマ（抜粋）" >}}
"""
Ordering options for gist connections
"""
input GistOrder {
  """
  The ordering direction.
  """
  direction: OrderDirection!

  """
  The field to order repositories by.
  """
  field: GistOrderField!
}

"""
Properties by which gist connections can be ordered.
"""
enum GistOrderField {
  """
  Order gists by creation time
  """
  CREATED_AT

  """
  Order gists by push time
  """
  PUSHED_AT

  """
  Order gists by update time
  """
  UPDATED_AT
}
{{< /code >}}

意味はほとんど自明かと思いますが、Gist（コード断片）のリストを取得するときに、どのフィールドでソートして返して欲しいかを指定するのに使用します。
下記の GraphQL クエリを発行すると、自分（サインイン済みユーザー）が登録している Gist を、作成日時 (`CREATED_AT`) で降順ソート (`DESC`) された状態で取得できます。

{{< code lang="graphql" title="新しく追加された Gist を 10 件取得する" >}}
query GetGists {
  viewer {
    gists(first: 10, orderBy: {field: CREATED_AT, direction: DESC}) {
      nodes {
        name
        createdAt
        updatedAt
        description
        url
      }
    }
  }
}
{{< /code >}}

