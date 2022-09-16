---
title: "GraphQL クエリ仕様: クエリ変数にデフォルト値を設定する"
url: "p/dtwtpzj/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

下記の GraphQL クエリは、GitHub の GraphQL API で organization 情報を取得するクエリの例です。

{{< code lang="graphql" hl_lines="1" >}}
query OrgInfoWithDefault($org: String = "graphql") {
  organization(login: $org) {
    name
    description
    websiteUrl
    createdAt
  }
}
{{< /code >}}

上記のクエリ変数 `$org` には、デフォルト値として `"graphql"` を設定しています。
クエリ実行時に `$org` 変数の指定を省略した場合、このデフォルト値が使用されます。
変数の指定を省略可能にするため、変数の型は __`String!` ではなく `String`__ にしなければいけないことに注意してください。

