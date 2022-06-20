---
title: "GitHub GraphQL API のクエリ例: ユーザー情報を取得する (viewer, user)"
linkTitle: "ユーザー情報を取得する (viewer, user)"
url: "/p/9bku4cj"
date: "2020-07-31"
lastmod: "2022-06-20"
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
    email  # メールアドレス（ユーザーが公開していれば）
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
      "email": "maku@example.com",
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
    email  # メールアドレス（ユーザーが公開していれば）
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
      "email": "maku@examle.com",
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


ユーザーのコミット統計情報を取得する（貢献度情報）
----

[User オブジェクト](https://docs.github.com/en/graphql/reference/objects#user) の __`contributionsCollection`__ フィールドを参照すると、そのユーザーの一定期間内の __コミット数、PR 作成数、PR レビュー数__ などの情報を取得することができます。
`from` 引数と `to` 引数で統計情報の取得期間を指定します（最長 1 年間）。
日時を省略した場合は、直近の 1 年間の統計情報を取得します。

次の例では、あるユーザーの 2021 年の Contribution 情報を取得しています。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  user(login:"maku77") {
    login  # ログインID
    name   # ユーザー名

    contributionsCollection(from: "2021-01-01T00:00:00Z") {
      startedAt  # 統計情報の開始日時
      endedAt    # 統計情報の終了日時
      totalCommitContributions  # 合計コミット数
      totalPullRequestContributions  # 合計 PR 作成数
      totalPullRequestReviewContributions  # 合計 PR レビュー数

      commitContributionsByRepository {  # リポジトリごとの情報
        repository {
          nameWithOwner  # Organization/リポジトリ名
        }
        contributions {
          totalCount  # このリポジトリへのコミット数
        }
      }
    }
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "user": {
      "login": "maku77",
      "name": "Maku Maku",
      "contributionsCollection": {
        "startedAt": "2021-01-01T00:00:00Z",
        "endedAt": "2022-01-01T00:00:00Z",
        "totalCommitContributions": 2000,
        "totalPullRequestContributions": 400,
        "totalPullRequestReviewContributions": 150,
        "commitContributionsByRepository": [
          {
            "repository": {
              "nameWithOwner": "maku77/repo1"
            },
            "contributions": {
              "totalCount": 1200
            }
          },
          {
            "repository": {
              "nameWithOwner": "maku77/repo2"
            },
            "contributions": {
              "totalCount": 800
            }
          }
        ]
      }
    }
  }
}
{{< /code >}}


ある組織に所属するユーザー情報を取得する (Organization.membersWithRole)
----

ルートフィールドで `organization` を指定すれば、特定の組織に所属するユーザーの一覧を取得できます。

- 参考: [組織 (org) 内のユーザーの一覧を取得](/p/3o2doyb/#org-members)

そこには `User` オブジェクトの情報が含まれているので、例えば次のようにすれば、組織のメンバー全員 (`membersWithRole`) の過去 1 年間の貢献情報をまとめて取得することができます。

```graphql
query {
  organization(login: "Organization名") {
    name
    membersWithRole(first: 100) {
      nodes {
        login  # ログインID
        name   # ユーザー名
        email  # メールアドレス（ユーザーが公開していれば）
        contributionsCollection {
          startedAt  # 統計情報の開始日時
          endedAt    # 統計情報の終了日時
          totalCommitContributions  # 合計コミット数
          totalPullRequestContributions  # 合計 PR 数
          totalPullRequestReviewContributions  # 合計レビュー数
        }
      }
    }
  }
}
```

ただし、GitHub GraphQL API の制約上、ユーザー情報は一度に 100 ユーザー分しかとれないので、それ以上のメンバーがいる組織の場合は、[GraphQL のページネーション処理](https://graphql.org/learn/pagination/) で繰り返しクエリを実行する必要があります。

