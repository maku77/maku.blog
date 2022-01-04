---
title: "DynamoDB 用のポリシー設定例"
url: "/p/gk6jx9k"
date: "2021-06-28"
tags: ["AWS", "AWS/DynamoDB"]
---

{{% private %}}
- https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/using-identity-based-policies.html
{{% /private %}}


あるテーブルに対するすべての操作を可能にする
----

{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "AllAPIActionsOnBooks",
            "Effect": "Allow",
            "Action": "dynamodb:*",
            "Resource": "arn:aws:dynamodb:us-west-2:123456789012:table/Books"
        }
    ]
}
{{< /code >}}

`Action` に `dynamodb:*` というワイルド―カードを指定することで、DynamoDB のすべての API を使った操作を可能にしています。
通常は、特定のアクションのみを許可すべきです。


あるテーブルの読み取りを行えるようにする
----

{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "DescribeQueryScanBooksTable",
            "Effect": "Allow",
            "Action": [
                "dynamodb:DescribeTable",
                "dynamodb:Query",
                "dynamodb:Scan"
            ],
            "Resource": "arn:aws:dynamodb:us-west-2:123456789012:table/Books"
        }
    ]
}
{{< /code >}}

このポリシーステートメントは、アカウント `123456789012` が所有する `Books` テーブルの読み取り（Query や Scan）が可能であることを示します。

下記は、もう少し可能な操作を増やしたバージョンです。

{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "ReadOnlyAPIActionsOnBooks",
            "Effect": "Allow",
            "Action": [
                "dynamodb:GetItem",
                "dynamodb:BatchGetItem",
                "dynamodb:DescribeTable",
                "dynamodb:Query",
                "dynamodb:Scan",
                "dynamodb:ConditionCheckItem"
            ],
            "Resource": "arn:aws:dynamodb:us-west-2:123456789012:table/Books"
        }
    ]
}
{{< /code >}}

