---
title: "GraphQL スキーマ仕様: ページネーション (pagination)"
url: "p/vuo4px3/"
date: "2022-10-24"
tags: ["GraphQL"]
draft: true
---

ページネーションとは？
----

GraphQL API で数百件を超えるような大量のデータを取得できるようにする場合は、ページネーション処理を考慮した API 定義を行うのが一般的です。
ページネーションというのは、Google の検索結果にあるような「次へ」「前へ」というような機能のことです。
ページネーションをどう実装すべきかは、GraphQL の仕様として定義されているわけではないのですが、デファクトスタンダードな実装方法が確立されています。

- 方法1: オフセット・ページネーション (offset pagination)
- 方法2: カーソル・ページネーション (cursor pagination)

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
しかも厄介なことに、`OFFSET` が増加するごとに読み出し量が増えていくため、後ろの方のページを表示しようとするとパフォーマンスが悪くなります。
クラウド系の RDB サービスを使っている場合は、多額の料金を請求されることになります。

### 結果の一貫性が保てないことがある

あるページを表示しているときに、バックグラウンドでレコードが追加されると、その次のページに移ったときに、同じレコードが表示されたりします。
まぁこのあたりは表示仕様で妥協することはできるかもしれませんが、1 つ目の DB アクセスのコスト問題は解決が難しく、多くの API では、次のカーソルベースのページネーションが採用されています。


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

カーソル・ページネーションの明らかな欠点は、ページのスキップができないということです。
あるページにたどり着くには、先頭ページ、あるいは末尾ページからページ送りしていくしかありません。
また、全体で何ページ分のデータが存在するかを簡単に取得する方法もありません（これはバックエンドで使用する DB にもよりますが）。


Connection パターン（Relay ライブラリ）
----

カーソル・ページネーションの実装は、Facebook の Relay ライブラリの Connections という設計を参考にしていることが多く、これを __Connection パターン__ と呼んだりします。
GitHub の GraphQL API もこれを採用しています。
Connection パターンでは、ページングの必要なリストデータを返す際に、__Connection 型__ のオブジェクトを返します。
Connection 型は、__`edges`__ と __`pageInfo`__ という 2 つのフィールドを持っています。

{{< code lang="graphql" title="Connection オブジェクトを返すクエリの例" >}}
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

type Book {
  title: String!
  authors: [String!]!
}
```

`PageInfo` 型のフィールド定義を見ると分かるように、ページング処理では、前方に進む (`hasPreviousPage` & `startCursor`) ことも、後方に進む (`hasNextPage` & `endCursor`) こともできるようになっています。
ただし、前述の通り、ランダムアクセスはできません。

GitHub API の場合、`Edge` 型に追加の `role` フィールドを持たせていたりします。
取得したいデータ (上記の例では `Book` 型の `node` フィールド）が `Edge` 型でラップされているので、こういったメタ情報を付加することが可能です。

