---
title: "GraphQL クエリ仕様: フィールド名にエイリアスを付ける"
url: "p/vnkd4ur/"
date: "2022-09-14"
tags: ["GraphQL"]
---

GraphQL クエリ言語の __フィールド・エイリアス__ 機能を使うと、取得したフィールド値を参照するときに、任意の別名を付けて参照することができます。
例えば、GraphQL スキーマとしては `company` というフィールド名で定義されているものを、`companyName` という JSON プロパティ名で返して欲しいときは、次のようにフィールド・エイリアスを使用します。

{{< code lang="graphql" hl_lines="4" title="GraphQL クエリ" >}}
query ProfileInfoWithAlias {
  user(login: "maku77") {
    name
    companyName: company
    bio
  }
}
{{< /code >}}

{{< code lang="json" title="レスポンス" hl_lines="5" >}}
{
  "data": {
    "user": {
      "name": "Makkuma",
      "companyName": "maku maku company",
      "bio": "すーぱーぷにぐらまー"
    }
  }
}
{{< /code >}}

フィールド・エイリアスは、同名のフィールドを異なる引数で取得するときに役立ちます。

{{< code lang="graphql" hl_lines="2 5" title="GraphQL クエリ" >}}
query {
  maku77: user(login: "maku77") {
    ...basicUserInfo
  }
  puni: user(login: "puni") {
    ...basicUserInfo
  }
}

fragment basicUserInfo on User {
  name
  bio
}
{{< /code >}}

上記のように 1 つのクエリの中で同名のフィールド（ここでは `user`）を要求する場合に、フィールド・エイリアス（ここでは `maku` と `puni`）を付けることで、レスポンスのプロパティ名がコンフリクトしないようにします。
というより、このケースではエイリアスを設定しないと、リクエスト時にエラーになります。

{{< code lang="json" title="レスポンス" >}}
{
  "data": {
    "maku77": {
      "name": "Makkuma",
      "bio": "すーぱーぷにぐらまー"
    },
    "puni": {
      "name": "Puni Puni",
      "bio": "ぷにぷにぷにょーん"
    },
  }
}
{{< /code >}}

{{% private %}}
GraphQL in Action p.91 3.4.3 がよい例。
{{% /private %}}

