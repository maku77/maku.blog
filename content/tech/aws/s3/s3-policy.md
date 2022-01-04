---
title: "Amazon S3 バケット用のポリシー設定例"
url: "/p/r9kv6gq"
date: "2021-03-21"
tags: ["AWS"]
---

S3 用のポリシー設定例
----

### 指定したバケット内のオブジェクト一覧を取得できるようにする

{{< code lang="json" >}}
{
   "Version": "2012-10-17",
   "Statement": [
      {
         "Effect": "Allow",
         "Action": [
            "s3:ListBucket"
         ],
         "Resource": [
            "arn:aws:s3:::<バケット名>"
         ]
      }
   ]
}
{{< /code >}}

### 指定したバケット内のオブジェクトの内容を取得できるようにする

{{< code lang="json" >}}
{
   "Version": "2012-10-17",
   "Statement": [
      {
         "Effect": "Allow",
         "Action": [
            "s3:GetObject"
         ],
         "Resource": [
            "arn:aws:s3:::<バケット名>/*"
         ]
      }
   ]
}
{{< /code >}}

### 指定したバケットに対していろいろできるようにする

{{< code lang="json" >}}
{
   "Version": "2012-10-17",
   "Statement": [
      {
         "Effect": "Allow",
         "Action": [
            "s3:DeleteObject",
            "s3:GetObject",
            "s3:ListBucket",
            "s3:PutObject",
            "s3:PutObjectAcl"
         ],
         "Resource": [
            "arn:aws:s3:::<バケット名>",
            "arn:aws:s3:::<バケット名>/*"
         ]
      }
   ]
}
{{< /code >}}

### すべての S3 リソースに対して何でもできるようにする

{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": "s3:*",
            "Resource": "*"
        }
    ]
}
{{< /code >}}

- S3 で静的 Web サイトをホストするときは、バケットポリシーで __`s3:GetObject`__ アクションのみを指定すればよい。`ListObject` や `PutObject` アクセス許可しなくても OK。


（おまけ）CORS 設定（JSON 形式）
----

Web サイトの JavaScript などから S3 バケットにアクセスする場合は、バケットの設定で CORS アクセスを有効にしておく必要があります。
設定は次のように JSON 形式で行います。

{{< code lang="json" >}}
[
    {
        "AllowedHeaders": [
            "*"
        ],
        "AllowedMethods": [
            "HEAD",
            "GET",
            "PUT",
            "POST",
            "DELETE"
        ],
        "AllowedOrigins": [
            "*"
        ],
        "ExposeHeaders": [
            "ETag"
        ]
    }
]
{{< /code >}}

