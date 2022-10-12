---
title: "GraphQL スキーマ仕様: @deprecated ディレクティブで非推奨のフィールドを示す"
url: "p/bg9g8rc/"
date: "2022-09-14"
tags: ["GraphQL"]
---

{{% private %}}
- [Directives - Apollo GraphQL Docs](https://www.apollographql.com/docs/apollo-server/schema/directives/)
{{% /private %}}

GraphQL スキーマ内のある型のフィールドや Enum 値が非推奨になったことを表すには、__`@deprecated`__ ディレクティブを使用します。
__`reason`__ 引数で非推奨とされた理由を記述することができるので、その文章中に代替となるフィールド名を記述しておくと親切です。

次の例では、`email` フィールドが非推奨 (deprecated) になり、代わりに `emailAddress` フィールドを参照して欲しいことを示しています。

{{< code lang="graphql" hl_lines="3" >}}
type User {
    emailAddress: String
    email: String @deprecated(reason: "Use 'emailAddress'.")
}
{{< /code >}}

