---
title: "GitHub GraphQL のスキーマ情報を取得する"
url: "/p/whv8it5"
date: "2020-09-09"
tags: ["GitHub", "GraphQL"]
---

{{% private %}}
- [GraphQL APIの発見](https://docs.github.com/ja/graphql/guides/introduction-to-graphql#discovering-the-graphql-api)
{{% /private %}}


GitHub API のサイトからダウンロードする方法
----

次のサイトから GitHub GraphQL API のスキーマ定義ファイル (`schema.docs.graphql`) をダウンロードすることができます。

- [パブリックスキーマ - GitHub Docs](https://docs.github.com/ja/graphql/overview/public-schema)

例えば、このファイル内の `Query` オブジェクト (`type Query`) の定義を見ると、クエリのトップレベルにどのようなオブジェクト（フィールド）を指定できるかが分かります。

{{< code lang="graphql" title="schema.docs.graphql" >}}
type Query {

  ...

  organization(
    login: String!
  ): Organization

  ...

  user(
    login: String!
  ): User
}
{{< /code >}}

上記の場合、`Query` オブジェクト内に `organization` や `user` フィールドが定義されているので、クライアントアプリからクエリ要求を出すときに、次のような感じで指定できるということが分かります。

{{< code lang="graphql" >}}
query {
  organization(login: "netflix") {
    name
    description
    avatarUrl
  }
  user(login: "octocat") {
    name
    company
    url
    avatarUrl
  }
}
{{< /code >}}

スキーマ定義ファイル (`.graphql`) とクライアントアプリ側のクエリ定義 (`query {...}`) を組み合わせることで、TypeScript 用の型定義ファイルを自動生成することができます（クエリ要求の戻り値の型を定義できます）。
詳しくは、[Apollo CLI](https://www.apollographql.com/docs/devtools/cli/) などのコマンドラインツールを確認してください。


GraphQL サーバーから直接取得する方法
----

GitHub の GraphQL API サーバーに HTTP GET リクエストを送ることで、スキーマ定義を直接取得することができます。
[パーソナルアクセストークンはこちら](https://github.com/settings/tokens) から生成してください。

{{< code lang="console" >}}
$ token=<YOUR_PERSONAL_ACCESS_TOKEN>
$ curl -H "Authorization: bearer $token" https://api.github.com/graphql
{{< /code >}}

結果は圧縮された JSON テキストで返されます。
`python -m json.tool` にパイプすることで見やすく整形された JSON テキストを取得できます。

{{< code lang="sh" >}}
$ curl -H "Authorization: bearer $token" https://api.github.com/graphql | python -m json.tool
{{< /code >}}

返されたスキーマ定義をファイルに保存すると、サイズは約 5MB になります。

