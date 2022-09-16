---
title: "GraphQL スキーマ仕様: 型やフィールドに説明文 (description) を記述する"
url: "p/c5pbs6p/"
date: "2022-09-14"
tags: ["GraphQL"]
draft: true
---

GraphQL スキーマには、Python の docstring と同様のフォーマットで __description__ というドキュメントを付加できます。
description は型の直前、あるいはフィールドの直前に、3 つのクォート (`"""`) で囲む形で記述します。

{{< code lang="graphql" title="description の記述例" >}}
"""The root query entry point for the API"""
type Query {
  """The current time in ISO UTC"""
  currentTime: String
}
{{< /code >}}

{{< code lang="graphql" title="description の記述例" >}}
"""
Represents an auto-merge request for a pull request
"""
type AutoMergeRequest {
  """
  The email address of the author of this auto-merge request.
  """
  authorEmail: String

  """
  The commit message of the auto-merge request. If a merge queue is required by
  the base branch, this value will be set by the merge queue when merging.
  """
  commitBody: String

  ...
}
{{< /code >}}

description 情報は、各種ツール（GraphiQL など）が参照して利用します。

GraphQL スキーマの型名やフィールド名だけで表現できる情報は限られているので、ほぼすべての型、フィールドに対して description を設定するのはよいプラクティスです。
下記は GitHub の GraphQL API のスキーマです。
description の記述方法の参考になります。

- https://docs.github.com/public/schema.docs.graphql

description では Markdown が使えるようになっていますが、様々なツールから参照されることを考えると、通常のテキストで記述しておくのが無難ですし、通常はそれで十分です。

