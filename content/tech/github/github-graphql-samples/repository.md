---
title: "GitHub GraphQL クエリ例: リポジトリの情報を取得する (repository)"
linkTitle: "リポジトリの情報を取得する (repository)"
url: "/p/du6env5"
date: "2020-09-14"
tags: ["GitHub", "GraphQL"]
---

GitHub の GraphQL API を使ってリポジトリの情報を取得するには、次のような方法があります。

- `repository クエリ` ... 組織名（あるいはユーザ名）とリポジトリ名が分かっている場合
- `organization クエリ` ... ある組織内のリポジトリの一覧を取得する場合（[こちらの記事](/p/3o2doyb) を参考にしてください）
- `search クエリ` ... [汎用的な検索用クエリ](https://docs.github.com/ja/github/searching-for-information-on-github/searching-for-repositories) でリポジトリを検索する


search クエリでリポジトリを検索する
----

特定の組織 (organization) 内のリポジトリを取得するには、[organization クエリを使って](/p/3o2doyb) 得られた `Organization` オブジェクトの `repositories` を参照する方法もありますが、最初から `search` クエリを使っていろいろな条件を指定して検索する方が早いです。

- 参考: [リポジトリを検索する - GitHub Docs](https://docs.github.com/ja/free-pro-team@latest/github/searching-for-information-on-github/searching-for-repositories)

次の例では、`myorg` という組織内の、`product` というトピックの付けられたリポジトリの一覧を取得しています。

{{< code lang="graphql" title="クエリ例" >}}
query {
  search(type: REPOSITORY, query: "org:myorg topic:product", last: 100) {
    repositoryCount
    nodes {
      ... on Repository {
        id
        url
        name
        description
        createdAt
      }
    }
  }
}
{{< /code >}}

{{< code title="実行結果" >}}
{
  "data": {
    "search": {
      "repositoryCount": 3,
      "nodes": [
        {
          "id": "DEMwOlJlcG9zaXRvcnkzODA5MTQ0OQ==",
          "url": "https://github.com/myorg/Repo1",
          "name": "Repo1",
          "description": "Repo1 の概要説明",
          "createdAt": "2015-06-26T04:54:51Z"
        },
        {
          "id": "DEMwOlJlcG9zaXRvcnkxNjI2NjczMjY=",
          "url": "https://github.com/myorg/Repo2",
          "name": "Repo2",
          "description": "Repo2 の概要説明",
          "createdAt": "2018-12-21T04:51:54Z"
        },
        {
          "id": "DEMwOlJlcG9zaXRvcnkyNDUzMjU1NDI=",
          "url": "https://github.com/myorg/Repo3",
          "name": "Repo3",
          "description": "Repo3 の概要説明",
          "createdAt": "2020-03-06T03:56:40Z"
        },
      ]
    }
  }
}
{{< /code >}}


あるリポジトリのマイルストーンの一覧を取得する {#milestones}
----

[repository クエリ](https://docs.github.com/en/graphql/reference/queries#repository) を使用すると、指定した組織（ユーザー）の、指定したリポジトリの情報を取得することができます。

次の GraphQL クエリは、`apollographql/apollo-client` リポジトリで設定されているマイルストーンの一覧を要求しています。
__`milestones`__ クエリで最初の 3 件分を取得していますが、最大 100 件まで同時に取得することができます。
100 件を超えるデータを取得したいときは、[ページネーション処理](/p/h5s6ht5) が必要です。

{{< code lang="graphql" title="クエリ例" >}}
query {
  repository(owner: "apollographql", name: "apollo-client") {
    name
    url
    milestones(states: [OPEN,CLOSED], first: 3) {
      totalCount
      nodes {
        url
        title
        dueOn
        closed
        description
      }
      pageInfo {
        endCursor
        hasNextPage
      }
    }
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "repository": {
      "name": "apollo-client",
      "url": "https://github.com/apollographql/apollo-client",
      "milestones": {
        "totalCount": 17,
        "nodes": [
          {
            "url": "https://github.com/apollographql/apollo-client/milestone/1",
            "title": "alpha",
            "dueOn": "2016-04-19T07:00:00Z",
            "closed": true,
            "description": "Alpha release of the client, with enough features to be useful in a real app."
          },
          {
            "url": "https://github.com/apollographql/apollo-client/milestone/2",
            "title": "newspring-production",
            "dueOn": null,
            "closed": true,
            "description": ""
          },
          {
            "url": "https://github.com/apollographql/apollo-client/milestone/3",
            "title": "5/10 cycle",
            "dueOn": null,
            "closed": true,
            "description": "Some things to button down in the short term, that a lot of people have been asking about!"
          }
        ],
        "pageInfo": {
          "endCursor": "Y3Vyc29yOnYyOpHOABqZMg==",
          "hasNextPage": true
        }
      }
    }
  }
}
{{< /code >}}

### マイルストーンの一覧をソートする

`milestones` クエリを発行するときに、`orderBy` パラメータを指定すると、特定のフィールドの値を使ってソートされた結果を取得することができます。
次の例では、マイルストーンの Due Date が遅いものから順番に 100 件分を取得します。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  repository(owner: "apollographql", name: "apollo-client") {
    milestones(states: [OPEN,CLOSED], last: 100,
        orderBy: {field: DUE_DATE, direction: DESC}) {
      nodes {
        url
        title
        dueOn
        closed
      }
    }
  }
}
{{< /code >}}

### マイルストーンに所属するイシューを取得する

`milestones` クエリを発行するときに、入れ子の形で `issues` を要求することで、各マイルストーンに所属するイシューのリストまで同時に取得してしまうことができます。

{{< code lang="graphql" title="クエリ例" >}}
query {
  repository(owner: "apollographql", name: "apollo-client") {
    name
    url
    milestones(states: [OPEN,CLOSED], first: 3) {
      totalCount
      nodes {
        url
        title
        dueOn
        closed
        description
        issues(states: [OPEN,CLOSED], first: 10, labels: ["EPIC"]) {
          nodes {
            url
            title
            closed
          }
          pageInfo {
            endCursor
            hasNextPage
          }
        }
      }
      pageInfo {
        endCursor
        hasNextPage
      }
    }
  }
}
{{< /code >}}

{{< code title="実行結果" >}}
省略
{{< /code >}}

