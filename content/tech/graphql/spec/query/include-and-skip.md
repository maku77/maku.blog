---
title: "GraphQL クエリ仕様: ディレクティブによる条件分岐 (@include, @skip)"
url: "p/dcdv7rv/"
date: "2022-09-14"
tags: ["GraphQL"]
---

GraphQL クエリで、各フィールド名（あるいはフラグメント）の後ろに __`@include`__ ディレクティブや __`@skip`__ ディレクティブを配置すると、`if` 引数で指定した条件によってそのフィールドを取得するかどうかを制御できます。

```graphql
fieldName @include(if: $someTest)
fieldName @skip(if: $someTest)
```

`@include` ディレクティブと `@skip` ディレクティブの違いは、`if` 引数の判定が逆転しているだけです。

- __`@include(if: Boolean)`__ ディレクティブ
  - `if` 引数の値が `true` のときにのみフィールドを取得します
- __`@skip(if: Boolean)`__ ディレクティブ
  - `if` 引数の値が `false` のときにのみフィールドを取得します

{{< code lang="graphql" title="クエリ内での @include ディレクティブによる条件分岐" >}}
query QueryOrganization($org: String!, $details: Boolean = false) {
  organization(login: $org) {
    name
    description
    websiteUrl @include(if: $details) {
  }
}
{{< /code >}}

{{< code lang="json" title="variables" >}}
{
  "org": "graphql",
  "details": true
}
{{< /code >}}

