---
title: "GitHub GraphQL API のクエリ例: ユーザー情報を取得する (viewer, user)"
linkTitle: "ユーザー情報を取得する (viewer, user)"
url: "/p/9bku4cj"
date: "2020-07-31"
tags: ["GitHub", "GraphQL"]
---

サインイン済みのユーザー情報を取得する (viewer)
----

`viewer` クエリを使用すると、現在アクセスしているユーザー（サインイン中のユーザー）のユーザー情報（[User オブジェクト](https://docs.github.com/en/graphql/reference/objects#user)）を取得することができます。

- [Queries - viewer](https://docs.github.com/en/graphql/reference/queries#user)

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  viewer {
    login  # ログインID
    name   # ユーザー名
    url    # ユーザーの GitHub ホームページ
    websiteUrl  # ユーザーの Web サイト
    avatarUrl   # ユーザーのアバター画像
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "viewer": {
      "login": "maku77",
      "name": "Maku Maku",
      "url": "https://github.com/maku77",
      "websiteUrl": "https://maku77.github.io/",
      "avatarUrl": "https://avatars2.githubusercontent.com/u/5519503?v=4"
    }
  }
}
{{< /code >}}


指定したログイン ID のユーザー情報を取得する (user)
----

`user` クエリを使用すると、指定したユーザー名のユーザー情報を取得することができます。

- [Queries - user](https://docs.github.com/en/graphql/reference/queries#user)

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  user(login:"maku77") {
    login  # ログインID
    name   # ユーザー名
    url    # ユーザーの GitHub ホームページ
    websiteUrl  # ユーザーの Web サイト
    avatarUrl   # ユーザーのアバター画像
  }
}
{{< /code >}}

取得できるオブジェクトは、`viewer` クエリの場合と同様に [User オブジェクト](https://docs.github.com/en/graphql/reference/objects#user) です。

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "user": {
      "login": "maku77",
      "name": "Maku Maku",
      "url": "https://github.com/maku77",
      "websiteUrl": "https://maku77.github.io/",
      "avatarUrl": "https://avatars2.githubusercontent.com/u/5519503?v=4"
    }
  }
}
{{< /code >}}

ただし、指定したユーザー名が存在しない場合は `data.user` フィールドの値は __`null`__ になり、`errors` フィールドの値が格納されます。

{{< code lang="graphql" title="GraphQL クエリ（存在しないユーザーを指定する）" >}}
query {
  user(login:"no-such-user") {
    login
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "user": null
  },
  "errors": [
    {
      "type": "NOT_FOUND",
      "path": [
        "user"
      ],
      "locations": [
        {
          "line": 7,
          "column": 3
        }
      ],
      "message": "Could not resolve to a User with the login of 'no-such-user'."
    }
  ]
}
{{< /code >}}


ある組織に所属するユーザー情報を取得する (Organization.membersWithRole)
----

- [こちらを参照](/p/3o2doyb/#org-members)

