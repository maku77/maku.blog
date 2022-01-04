---
title: "GitHub GraphQL クエリ例: イシュー情報を取得する (search)"
linkTitle: "イシュー情報を取得する (search)"
url: "/p/i5ht5ep"
date: "2020-09-14"
tags: ["GitHub", "GraphQL"]
---

イシュー情報の取得方法
----

イシュー情報を取得する方法は次のような方法があります。

- {{< label "ユーザーからたどる方法" >}} -- [viewer/user クエリ](/p/9bku4cj) で [User オブジェクト](https://docs.github.com/en/graphql/reference/objects#user) を取得し、そのユーザーに関連するイシュー（issues フィールド）を参照する方法
- {{< label "リポジトリからたどる方法" >}} -- [repository クエリ](/p/du6env5) で [Repository オブジェクト](https://docs.github.com/en/graphql/reference/objects#repository) を取得し、そのリポジトリ内のイシュー（issues フィールド）を参照する方法
- {{< label "ダイレクトに検索する方法" >}} -- [search クエリ](https://docs.github.com/en/graphql/reference/queries#searchresultitemconnection) でもろもろの検索条件（リポジトリ名、タイプなど）を使ってダイレクトに検索する方法

`search` クエリによる検索はとても柔軟で、様々な条件（`query` パラメータ）を指定してイシューを検索することができます。
`query` パラメータには、GitHub の Issues ページの検索窓に入力できる `is:issue is:open` といった文字列で、下記のサイトに詳細仕様が記述されています。

- [Issue およびプルリクエストを検索する - GitHub Docs](https://docs.github.com/ja/github/searching-for-information-on-github/searching-issues-and-pull-requests)


あるリポジトリのイシューの一覧を取得する
----

次の GraphQL クエリでは、`myorg/myrepo` リポジトリの最新 5 件のイシュー情報を取得しています。

{{< code lang="graphql" title="GraphQL クエリ" >}}
query {
  search(type: ISSUE, query: "repo:myorg/myrepo is:issue", last: 100) {
    issueCount
    nodes {
      ... on Issue {
        id
        number
        title
        closed
        url
        author { login }
        assignees(first: 100) {
          nodes { login name email }
        }
        labels(first: 100) {
          nodes { name color }
        }
      }
    }
  }
}
{{< /code >}}

`search` クエリはそのパラメータの意味を理解するのが重要です。

- __`type: ISSUE`__
    - 検索対象を示す必須パラメータで、`ISSUE` / `REPOSITORY` / `USER` のいずれかを指定する必要があります。ここでは、イシューやプルリクエストの情報を検索することを示しています（`ISSUE` はプルリクエストの情報まで含むことに注意）。
- __`query: "repo:myorg/myrepo is:issue"`__
    - 検索対象を指定したリポジトリに絞り込み、さらに、`is:issue` を指定することでプルリクエストを除外します。[他にもいろいろな指定方法があります](https://docs.github.com/ja/github/searching-for-information-on-github/searching-issues-and-pull-requests)。
- __`last: 100`__
    - 最新の 100 件までのイシュー情報を取得します。100 件を超えるデータを取得するときはページネーション処理により、複数回のクエリ実行が必要です。


その他のクエリ方法いろいろ
----

### あるマイルストーンが設定されたイシューを取得する

{{< code lang="graphql" >}}
search(type: ISSUE, last: 100,
  query: "repo:myorg/myrepo milestone: \"Milestone #123\" is:issue")
{{< /code >}}

この例では、`myorg/myrepo` リポジトリから、マイルストーン名 `Milestone #123` が設定されたイシューの一覧を取得しています。
マイルストーン名にスペースを含んでいる場合は、上記のようにエスケープされたダブルクォートでマイルストーン名を囲む必要があります。

