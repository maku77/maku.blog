---
title: "GraphQL スキーマ仕様: ページネーション (pagination)"
url: "p/vuo4px3/"
date: "2022-10-24"
lastmod: "2022-11-07"
tags: ["GraphQL"]
---

{{% private %}}
- [Pagination | GraphQL](https://graphql.org/learn/pagination/)
{{% /private %}}

ページネーションとは？
----

GraphQL API で数百件を超えるような大量のデータを取得できるようにする場合は、ページネーション処理を考慮した API 定義を行うのが一般的です。
ページネーションというのは、Google の検索結果にあるような「次へ」「前へ」というような機能のことです。
ページネーションをどう実装すべきかは、GraphQL の仕様として定義されているわけではないのですが、デファクトスタンダードな実装方法が確立されており、大きく次の 2 種類の実装方法があります。

- __オフセット・ページネーション (offset pagination)__
- __カーソル・ページネーション (cursor pagination)__

それぞれの方法を順に見ていきます。


オフセット・ページネーション (offset pagination)
----

オフセット・ページネーションは、複数のデータを取得するときに、どの位置（オフセット）から取得するかを、ページやデータのインデックスで指定する方法です。

```graphql
type Query {
  books(limit: Int!, page: Int!): [Book!]
}
```

- クエリパラメーターの意味
  - `limit` ... いくつのデータを取得するか
  - `page` ... どの位置から取得するか（絶対位置）

オフセット・ページネーションは実装が簡単で使いやすいという利点がありますが、いくつかの問題があります。

### DB アクセスのコスト問題（スケールしない）

例えば、上記のような `books` クエリが次のような SQL にマッピングされたとします。

```sql
SELECT * FROM books LIMIT 100 OFFSET 500;
```

クライアントは 100 件分のデータしか要求していませんが、RDB 側の処理としては、500 + 100 件分の読み出しが必要になります。
しかも厄介なことに、`OFFSET` が増加するごとに読み出し量が増えていくため、ページを進めていくたびにパフォーマンスが悪化していきます。
クラウド系の RDB サービスを使っている場合は、多額の料金を請求されることになります。

### 結果の一貫性を保てないことがある

あるページを表示しているときに、バックグラウンドでレコードが追加されると、その次のページに移ったときに、同じレコードが表示される可能性があります。
逆にレコードが削除された場合は、次のページに移ったときに、表示されるべきレコードが飛ばされてしまったりします。
これは、表示位置を常に先頭レコードからのオフセットで指定していることが原因です。

結果の一貫性に関しては表示仕様で妥協することはできるかもしれませんが、1 つ目の DB アクセスのコスト問題は解決が難しく、多くの Public API では、次のカーソルベースのページネーションが採用されています。


カーソル・ページネーション (cursor pagination)
----

カーソル・ページネーションでは、リスト内の項目を指し示す「カーソル」を使って取得するデータの位置を表現します。

```graphql
type Query {
  books(limit: Int!, after: String): [Book!]
}
```

- クエリパラメーターの意味
  - `limit` ... いくつのデータを取得するか
  - `after` ... どの要素の次から取得するか（カーソル）

上記クエリを実装するリゾルバー側で発行する SQL は次のような感じになります。

```sql
SELECT * FROM books WHERE id >= 500 ORDER BY id DESC LIMIT 100
```

インデックスの張られたデータを位置指定で読み出す場合、指定した位置からの読み出しコストしかかかりません。
GraphQL サーバーはクエリ結果として、読み出したデータの末尾を示すカーソル (ID) を返すので、次のページのデータを取得するときにそれを `after` パラメーターで指定します。

{{< code lang="graphql" title="次ページをクエリする例" >}}
query GetBooks {
  products(first: 100, after: "e4d75f") {
    next
    items { title authors }
  }
}
{{< /code >}}

カーソル・ページネーションの明らかな欠点は、__ページのスキップができない__ ということです。
あるページにたどり着くには、先頭ページ、あるいは末尾ページからページ送りしていくしかありません。
また、全体で何ページ分のデータが存在するかを効率的に取得する方法がなかったりします。
とはいえ、このあたりの制約は、バックエンドで使用する DB サービス側で緩和されていく可能性があります。
例えば、Firebase の Firestore データベースでは、2022 年末にコレクション内のドキュメント数を取得する `count` 関数が [実験導入](https://firebase.blog/posts/2022/10/whats-new-at-Firebase-Summit-2022) されています。


Connection パターン（Relay ライブラリ）
----

{{% private %}}
- [GraphQL Server Specification | Relay](https://relay.dev/docs/guides/graphql-server-specification/#connections)
{{% /private %}}

カーソル・ページネーションの実装は、[Facebook の Relay ライブラリ](https://relay.dev/) の Connections という実装を参考にしていることが多く、これを __Connection パターン__ と呼んだりします。
GitHub の GraphQL API もこれを採用しています。
Connection パターンでは、ページングの必要なリストデータを返す際に、__Connection 型__ のオブジェクトを返します。

{{< code lang="graphql" title="Connection 型を返すフィールドの定義" >}}
type Query {
  books(
    after: String
    before: String
    first: Int
    last: Int
  ): BookConnection!
}
{{< /code >}}

`Connection` 型は、__`edges`__ と __`pageInfo`__ という 2 つのフィールドを持っています。

{{< code lang="graphql" title="Connection 型を扱うクエリ" >}}
query GetBooks {
  books(first: 100, after: "e4d75f") {
    edges {  # 指定されたサイズ (=100) の Edge オブジェクト
      cursor
      node { title authors }  # 欲しい Book 情報はこの node フィールドに入っている
    }
    pageInfo {  # ページング情報
      endCursor
      hasNextPage
    }
  }
}
{{< /code >}}

`edegs` フィールドには要求したデータ（この例では書籍情報）が含まれており、`pageInfo` フィールドにはページング用のデータが含まれています。
`hasNextPage` が true であれば、次のページのデータがまだ存在するので、`endCursor` で返されたカーソル文字列を次のクエリの `after` に指定する、という流れになります。
典型的なスキーマ定義は次のような感じになります。

```graphql
type BookConnection {
  edges: [BookEdge]
  pageInfo: PageInfo!
}

type BookEdge {
  cursor: String!
  node: Book!
}

type PageInfo {
  startCursor: String
  endCursor: String
  hasNextPage: Boolean!
  hasPreviousPage: Boolean!
}

type Book implements Node {
  id: ID!
  title: String!
  authors: [String!]!
}
```

`PageInfo` 型のフィールド定義を見ると分かるように、ページング処理では、前方に進む (`hasPreviousPage` & `startCursor`) ことも、後方に進む (`hasNextPage` & `endCursor`) こともできるようになっています。
ただし、これはいわゆるリンクリスト構造であり、前述の通りランダムアクセスはできません。

GitHub API の場合、`Edge` 型に追加の `role` フィールドを持たせていたりします。
取得したいデータ (上記の例では `Book` 型の `node` フィールド）が `Edge` 型でラップされているので、こういったメタ情報を付加することが可能です。
また、GitHub API は `Connection` 型にも追加のフィールドを用意しています。

{{< code lang="graphql" title="GitHub GraphQL スキーマの TeamMemberConnection オブジェクト型" >}}
"""
The connection type for User.
"""
type TeamMemberConnection {
  """
  A list of edges.
  """
  edges: [TeamMemberEdge]

  """
  A list of nodes.
  """
  nodes: [User]

  """
  Information to aid in pagination.
  """
  pageInfo: PageInfo!

  """
  Identifies the total count of items in the connection.
  """
  totalCount: Int!
}
{{< /code >}}

上記の例では、`nodes` フィールドが `edges { node }` というクエリのショートカットとして使えるようになっており、`totalCount` でデータの総数を取得できるようになっています（クエリで返された `nodes` のサイズではなく、全データの数）。
`nodes` フィールドに関しては採用してもよいかもしれませんが、__`Connection` 型への `totalCount` フィールドの追加は慎重に行うべき__ です。
データソースによっては、リゾルバー内で `totalCount` を求めるのにコストがかかることがあるからです。
`totalCount` フィールドの有無はフロントエンドの仕様に影響を与えやすいところであり、一度追加した `totalCount` フィールドを削除するのは困難です。

