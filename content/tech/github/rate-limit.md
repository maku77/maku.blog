---
title: "GitHub GraphQL API の呼び出し回数制限 (rate limit) の情報を取得する"
url: "/p/bqz7emt"
date: "2020-08-02"
tags: ["GitHub", "GraphQL"]
---

GitHub API を使ったアプリケーションを作成するときは、API の呼び出し回数制限を意識した設計を行う必要があります。
GitHub API バージョン3 の REST API には、1 時間に 5000 リクエストまでという明確な呼び出し回数制限がありましたが、API バージョン4 の GraphQL API はそのクエリの性質上、ちょっと異なるコスト計算方法が採用されています。

- [GraphQL resource limitations | GitHub Developer Guide](https://developer.github.com/v4/guides/resource-limitations/)

正確な計算方法は上記のサイトに記述されていますが、簡単にいうと、GraphQL クエリの入れ子階層が深くなり、複雑になるほどコストがかかるという計算になっています。

ある GraphQL クエリが実際にどれだけのコストがかかるかは、下記のように __`rateLimit`__ 情報を取得することで調べることができます。

{{< code lang="graphql" >}}
query {
  rateLimit {
    limit
    cost
    remaining
    resetAt
  }
  search(query: "repo:electron/electron is:issue", type: ISSUE, first: 5) {
    issueCount
    nodes {
      ... on Issue {
        title
        labels(first: 10) { nodes { name }}
      }
    }
  }
}
{{< /code >}}

上記のクエリを実行すると、次のような感じの JSON レスポンスが返ってきます。

{{< code lang="js" >}}
{
  "data": {
    "rateLimit": {
      "limit": 5000,
      "cost": 1,
      "remaining": 4999,
      "resetAt": "2020-08-02T09:15:20Z"
    },
    "search": {
      "issueCount": 13278,
      "nodes": [
        // ...
      ]
    }
  }
}
{{< /code >}}

`rateLimit` の各プロパティの値は次のような意味を持っています。

- __`limit: 5000`__ ... 1時間あたりに使用可能なコスト（合計 5000 までのコストの呼び出しが可能ということ）
- __`cost: 1`__ ... 今回のクエリで消費したコスト
- __`remaining: 4999`__ ... あとどれだけのコストのクエリを実行できるか
- __`resetAt: 2020-08-02...`__ ... remaining が 5000 にリセットされる時間 (UTC)

つまり、__`cost`__ プロパティの値ができるだけ小さくなるように GraphQL のクエリを設計するべきだということです。
GraphQL API の呼び出し回数を減らしても、この `cost` 値が大きいとすぐにリミットに到達してしまいます。

