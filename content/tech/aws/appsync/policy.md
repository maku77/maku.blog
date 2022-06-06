---
title: "AWS AppSync のポリシーステートメントの記述例"
url: "p/cexcpyc/"
date: "2021-06-06"
tags: ["AWS/AppSync"]
---

参考: [Lambda](/p/yn7hqx8/) / [SSM](/p/s3o57jv/)

特定アカウントからのアクセスのみ許可する
----

{{< code lang="json" title="アカウント 123456789012 からのアクセスのみ許可" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {
        "Service": "appsync.amazonaws.com"
      },
      "Action": "sts:AssumeRole",
      "Condition": {
        "StringEquals": {
          "aws:SourceAccount": "123456789012"
        }
      }
    }
  ]
}
{{< /code >}}


特定の API へのアクセスのみ許可する
----

{{< code lang="json" title="abcdefg API のみ許可" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {
        "Service": "appsync.amazonaws.com"
      },
      "Action": "sts:AssumeRole",
      "Condition": {
        "ArnEquals": {
          "aws:SourceArn": "arn:aws:appsync:us-west-2:123456789012:apis/abcdefg"
        }
      }
    }
  ]
}
{{< /code >}}


特定 のリージョンからのアクセスのみ許可する
----

{{< code lang="json" title="us-east-1 のみ許可" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {
        "Service": "appsync.amazonaws.com"
      },
      "Action": "sts:AssumeRole",
      "Condition": {
        "ArnEquals": {
          "aws:SourceArn": "arn:aws:appsync:us-east-1:123456789012:apis/*"
        }
      }
    }
  ]
}
{{< /code >}}

