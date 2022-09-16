---
title: "GraphQL スキーマ仕様: ユニオン型 (Union types)"
url: "p/vqsyz9j/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

```graphql
union TaskOr Approach = Task | Approach

type Query {
    search(term: String!): [TaskOrApproach!]
}
```

{{< code lang="graphql" title="GitHub API の union 型の例" >}}
"""
Used for return value of Repository.issueOrPullRequest.
"""
union IssueOrPullRequest = Issue | PullRequest


"""
Types that can be inside a Milestone.
"""
union MilestoneItem = Issue | PullRequest
{{< /code >}}

