---
title: "GraphQL クエリ仕様: 条件によってフィールドを含めるか制御する（@include / @skip ディレクティブ）"
url: "p/dcdv7rv/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

GraphQL クエリで、各フィールド名（あるいはフラグメント）の後ろに __`@include`__ ディレクティブや __`@skip`__ ディレクティブを配置すると、`if` 引数で指定した条件によってそのフィールドを実際に取得するかどうかを制御できます。

```graphql
fieldName @include(if: $someTest)
fieldName @skip(if: $someTest)
```

`@include` ディレクティブの場合、`if` の条件が true のときにのみフィールドを取得します。
逆に、`@skip` ディレクティブの場合、`if` の条件が false のときにのみフィールドを取得します。

{{< code lang="graphql" title="@include ディレクティブの使用例" >}}
query OrgInfo($org: String!, $details: Boolean = false) {
  organization(login: $org) {
    name
    description
    websiteUrl @include(if: $details) {
  }
}
{{< /code >}}

