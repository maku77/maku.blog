---
title: "GitHub の GraphQL API Explorer の使い方"
url: "/p/sk3rykz"
date: "2020-09-14"
tags: ["GitHub", "GraphQL"]
weight: 1
---

{{< private >}}
- [Explorerの利用](https://docs.github.com/ja/graphql/guides/using-the-explorer)
{{< /private >}}

GraphQL API Expolorer とは
----

GitHub の GraphQL API (API ver.4) を使用すると、GitHub で管理されているリポジトリの情報やユーザーの情報などを、柔軟な GraphQL クエリを使って取得することができます。
しかし、いろいろなクエリ方法が用意されていて、実際にどのような情報が取得できるのかが分かりにくかったりします。
そんなとき便利なのが、GitHub が Web サイトとして用意してくれている、__GraphQL API Explorer__ です。

- [GraphQL API Explorer | GitHub Developer Guide](https://developer.github.com/v4/explorer/)

GraphQL API Explorer を使用すると、GraphQL API を使ってどのような情報を取得できるのか、実際にクエリを実行して確かめることができます。
GitHub アカウントでサインインした状態であれば、プライベートリポジトリの情報も取得することができます。
GitHub GraphQL API を利用するアプリケーションを作成するときは、このサイトでどのようなクエリを発行すればよいのかを調べながら作っていくことになると思います。

クエリエディタでは、下記のようなショートカットキーを使用することができます。

- __`Ctrl + Space`__ ... 入力補完
- __`Ctrl + Enter`__ ... 実行


History 機能と Explorer 機能
----

__`History`__ ボタンを押すと、過去に実行したクエリをロードすることができます。
クエリに次のように名前を付けておくと、History にその名前が表示されるので、後ほど再利用する予定があれば、わかりやすい名前を付けておくとよいでしょう。

{{< code lang="graphql" >}}
query GetApolloRepo {
  repository(owner: "apollographql", name: "apollo-client") {
    owner {
      login
      url
    }
    name
    url
  }
}
{{< /code >}}

__`Explorer`__ ボタンを押すと、スキーマ定義に基づいて、入力可能なフィールドをツリー形式で参照することができます（これを使うより、`Ctrl + Space` による補完の方が便利ですが）。


クエリのパラメータ化 (QUERY VARIABLES)
----

クエリ文字列の中で、実行のたびに変化させたい部分は変数にしておくと便利です。
次の例では、クエリ実行時に `$owner` と `$name` パラメーターを受け取るように定義しています。
`String!` は、省略できない文字列型パラメーターであることを示しています。

{{< code lang="graphql" >}}
query GetRepo($owner: String!, $name: String!) {
  repository(owner: $owner, name: $name) {
    owner {
      login
      url
    }
    name
    url
  }
}
{{< /code >}}

このようなパラメーター付きのクエリを実行するときは、__QUERY VARIABLES__ の入力欄で、次のように JSON 形式で変数の値をセットします。

{{< code lang="json" >}}
{
  "owner": "apollographql",
  "name": "apollo-client"
}
{{< /code >}}


呼び出し回数制限に注意 (rateLimit)
----

GraphQL Explorer では無制限に API を実行できるわけではありません。
現状は GitHub ユーザーごとに、1 時間あたり 5000 ポイントまでの呼び出しを行えるようになっています。
あるクエリで消費したポイントと残りのポイントは、次のように `rateLimit` フィールドを指定することで調べることができます。

{{< code lang="graphql" >}}
query {
  rateLimit { cost remaining }

  ...残りはいつも通り記述...
}
{{< /code >}}

{{< code title="実行結果" >}}
{
  "data": {
    "rateLimit": {
      "cost": 1,
      "remaining": 4989
    },

    ...
  }
}
{{< /code >}}

上記の結果は、今回のクエリ実行に 1 ポイント消費し、残り 4989 ポイント残っていることを示しています。

- 参考: [GitHub GraphQL API の呼び出し回数制限 (rate limit) の情報を取得する](/p/bqz7emt)

