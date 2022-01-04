---
title: "GitHub GraphQL クエリ例: 組織の情報を取得する (organization)"
linkTitle: "組織の情報を取得する (organization)"
url: "/p/3o2doyb"
date: "2020-09-14"
tags: ["GitHub", "GraphQL"]
---

GitHub GraphQL API で指定した組織の情報を（ここでは `github` organization）の情報を取得には、[organization クエリ](https://docs.github.com/en/graphql/reference/queries#organization) を使用します。
`organization` クエリには、`login` パラメーターで組織名を渡します。

`organization` クエリが返す [Organization オブジェクト](https://docs.github.com/en/graphql/reference/objects#organization) を参照すると、そこに所属する __メンバーの一覧__、__チームの一覧__、__リポジトリの一覧__ などを取得することができます。


ある組織に所属するユーザー情報を取得する (Organization.membersWithRole) {#org-members}
----

`Organization` オブジェクトの `membersWithRole` フィールドは [OrganizationMemberConnection](https://docs.github.com/en/graphql/reference/objects#organizationmemberconnection) オブジェクトを保持しており、この `nodes` を参照することで、組織に所属するメンバーの一覧を取得できます。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  organization(login: "github") {
    name
    description
    url
    membersWithRole(first: 3) {
      nodes {
        name
        email
        updatedAt
      }
    }
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "organization": {
      "name": "GitHub",
      "description": "How people build software.",
      "url": "https://github.com/github",
      "membersWithRole": {
        "nodes": [
          {
            "name": "Josh Nichols",
            "email": "technicalpickles@github.com",
            "updatedAt": "2020-09-09T11:22:35Z"
          },
          {
            "name": "Matt Todd",
            "email": "matt@github.com",
            "updatedAt": "2020-09-02T21:54:56Z"
          },
          {
            "name": "Pat Nakajima",
            "email": "patnakajima@gmail.com",
            "updatedAt": "2020-09-04T21:45:37Z"
          }
        ]
      }
    }
  }
}
{{< /code >}}


ある組織のリポジトリの一覧を取得する (Organization.repositories)
----

`Organization` オブジェクトの `repositories` フィールドを参照することで、その組織に作成されたリポジトリの一覧を取得することができます。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  organization(login: "github") {
    name
    description
    url
    repositories(first: 3) {
      nodes {
        name
        url
      }
    }
  }
}
{{< /code >}}

{{< code lang="json" title="実行結果" >}}
{
  "data": {
    "organization": {
      "name": "GitHub",
      "description": "How people build software.",
      "url": "https://github.com/github",
      "repositories": {
        "nodes": [
          {
            "name": "media",
            "url": "https://github.com/github/media"
          },
          {
            "name": "albino",
            "url": "https://github.com/github/albino"
          },
          {
            "name": "hubahuba",
            "url": "https://github.com/github/hubahuba"
          }
        ]
      }
    }
  }
}
{{< /code >}}

取得される各リポジトリの情報は、[Repository オブジェクト](https://docs.github.com/en/graphql/reference/objects#repository) として表現されています。

ここでは、組織内のリポジトリのリストを取得する方法を示しましたが、組織名（ユーザー名）とリポジトリ名があらかじめ分かっている場合は、`repository` クエリを使って、直接そのリポジトリの情報を取得することができます。

- 参考: [リポジトリの情報を取得する (repository)](/p/du6env5)

