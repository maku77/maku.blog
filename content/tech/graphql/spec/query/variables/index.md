---
title: "GraphQL クエリ仕様: クエリで変数を使用する"
url: "p/dtwtpzj/"
date: "2022-09-28"
tags: ["GraphQL"]
---

変数定義と変数値の指定
----

特定のパラメーターのみ値を変えて複数の GraphQL のクエリを発行したい場合は、ドキュメント内で `query` や `mutation` を定義するときに、__変数定義 (variable definitions)__ を追加します。

次の `GetBook` クエリでは、`$id: String!` という変数定義を行っています。
これは、クエリ実行時に `id` という名前の文字列変数を参照することを示しています。
そして、型名に `!` が付いているので、変数値の指定は必須です。

{{< code lang="graphql" title="変数を参照するクエリの例" >}}
query GetBook($id: String!) {
  book(id: $id) {
    id
    title
    price
  }
}
{{< /code >}}

指定された `id` 変数の値は、`book` フィールドの `id` 引数として渡され、対応する書籍を取得するリクエストとして処理されます。

肝心の変数の値は、`query` や `mutation` などのドキュメントとは異なる __`variables`__ という HTTP ペイロード・フィールドで GraphQL サーバーへ送ります（HTTP POST 経由での GraphQL API 呼び出しの場合）。
ただ、このあたりの `variables` フィールドの送信に関しては、通常は各種言語用の GraphQL ライブラリに隠蔽されているため、明示的に HTTP POST リクエストのペイロードを設定することはありません。
例えば、React 用の Apollo Client ライブラリは、次のような感じで変数を設定するインタフェースを提供しています。

```ts
const { loading, error, data } = useQuery<QueryType, VarType>(query, {
  variables: { id: "book-123" }
})
```

Web ブラウザ上で GraphQL API のテストを行える [GraphiQL（グラフィカル）](https://github.com/graphql/graphiql) の UI では、次のようにドキュメントと変数を別々に記述できるようになっています。

{{< image w="400" border="true" src="img-001.png" title="GraphiQL（グラフィカル）での変数指定" >}}


変数にデフォルト値を設定する
----

変数定義時にデフォルト値を設定すると、変数値の指定をオプションにすることができます。
下記の GraphQL クエリは、GitHub の GraphQL API で organization 情報を取得するクエリの例です。

{{< code lang="graphql" hl_lines="1" title="オプショナルな変数" >}}
query GetOrganization($org: String = "graphql") {
  organization(login: $org) {
    name
    description
    websiteUrl
    createdAt
  }
}
{{< /code >}}

上記のクエリ変数 `$org` には、デフォルト値として `"graphql"` が設定されています。
クエリ実行時に `$org` 変数の指定を省略した場合、このデフォルト値が使用されます。
変数の指定を省略可能にするため、変数の型は __`String!` ではなく `String`__ にしなければいけないことに注意してください。


変数の型として指定できるもの
----

各変数の型は、次のいずれかである必要があります。

- スカラー型 (scalar types)
  - __`ID`__、__`String`__、__`Int`__、__`Float`__ などの単一の値を示す型
- 列挙型 (enum types)
  - スキーマ定義において __`enum`__ で定義された列挙型
- 入力型 (input types)
  - スキーマ定義において __`input`__ で定義された入力用のオブジェクト型

GraphQL スキーマで列挙型や入力型を定義する方法は、下記を参考にしてください。

{{< reference >}}
- [GraphQL スキーマ仕様: 入力型 (input type) を定義する](/p/nhhwqtu/)
- [GraphQL スキーマ仕様: 列挙型 (enum type) を定義する](/p/isotm77/)
{{< /reference >}}

