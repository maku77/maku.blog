---
title: "GitHub GraphQL クエリ例: マイルストーン情報を取得する (milestone)"
linkTitle: "マイルストーン情報を取得する (milestone)"
url: "/p/fdnpuro"
date: "2020-09-24"
tags: ["GitHub", "GraphQL"]
---

マイルストーン番号で Milestone オブジェクトを取得する
----

リポジトリ名（組織名/リポジトリ名）が分かっている場合は、[repository クエリ](https://docs.github.com/en/graphql/reference/queries#repository) で取得した [Repository オブジェクト](https://docs.github.com/en/graphql/reference/objects#repository) の `milestone` フィールドに __マイルストーン番号__ を指定することで、そのマイルストーンの [Milestone オブジェクト](https://docs.github.com/en/graphql/reference/objects#milestone) を取得することができます。
例えば、Web アプリなどでは、`/milestone/123` のようなパスで指定したマイルストーンの情報を表示したいことがありますが、このようなケースで `123` という番号を使って情報を取得することができます。

**記述中**

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  repository(owner: "myorg", name: "myrepo") {
    milestone(number: 123) {
      number
      title
      dueOn
      description
      url
      issues(first: 100) {
        nodes {
          number
          title
          closed
          url
        }
      }
    }
  }
}
{{< /code >}}


マイルストーンが設定されていないイシューを取得する
----

[search クエリ](https://docs.github.com/en/graphql/reference/queries#searchresultitemconnection) で、Issue や PullRequest を検索するときに、`query` パラメータで渡す文字列に __`no:milestone`__ を含めると、マイルストーンの設定されていないものだけを検索することができます。

次の例では、`myorg/myrepo` リポジトリにおいて、マイルストーンの設定されていない Issue の一覧を取得しています（`query` 引数に `is:issue` を含めることで、PullRequest まで取得されてしまうのを防いでいます）。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  rateLimit { cost remaining }
  search(type: ISSUE, last: 100, query: "repo:myorg/myrepo is:issue no:milestone") {
    nodes {
      ... on Issue {
        number
        title
        closed
        url
        assignees(first: 100) {
          nodes { login avatarUrl url }
        }
        labels(first: 100) {
          nodes { name color }
        }
      }
    }
  }
}
{{< /code >}}

`no:milestone` の他にも、`no:label`（ラベルのないもの）、`no:assignee`（アサインされていないもの）、といった条件で検索することができます。

- 参考: [欠損しているメタデータで検索 - GitHub Docs](https://docs.github.com/ja/github/searching-for-information-on-github/searching-issues-and-pull-requests#search-by-missing-metadata)


あるリポジトリのマイルストーンの一覧を取得する
----

→ [こちらを参照](/p/du6env5#milestones)

