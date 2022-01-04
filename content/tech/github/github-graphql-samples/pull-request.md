---
title: "GitHub GraphQL クエリ例: PullRequest の情報を取得する (search)"
linkTitle: "PullRequest の情報を取得する (search)"
url: "/p/z69a3tk"
date: "2020-09-18"
tags: ["GitHub", "GraphQL"]
---

自分のレビュー待ちになっている PR を取得する
----

`search` クエリの `query` パラメータで、__`is:pr review-requested:@me`__ と指定すると、自分がレビューワー (reviewer) として設定されているプルリクエストを検索することができます。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  search(type: ISSUE, last: 100, query: "is:open is:pr review-requested:@me") {
    issueCount
    nodes {
      ... on PullRequest {
        number
        title
        url
        createdAt
        author { login avatarUrl url }
        reviewRequests(first: 100) {
          nodes {
            requestedReviewer {
              ... on User { login avatarUrl url }
            }
          }
        }
      }
    }
  }
}
{{< /code >}}

[PullRequest オブジェクトの詳細はこちら](https://docs.github.com/en/graphql/reference/objects#pullrequest)。

