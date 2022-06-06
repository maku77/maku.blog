---
title: "AWS Lambda のポリシーステートメントの記述例"
url: "p/yn7hqx8/"
date: "2021-06-06"
tags: ["AWS/Lambda"]
---

参考: [AppSync](/p/cexcpyc/) / [SSM](/p/s3o57jv/)


Lambda 関数の呼び出しを許可する
----

```json
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [ "lambda:invokeFunction" ],
            "Resource": [
                "arn:aws:lambda:us-west-2:123456789012:function:my-function",
                "arn:aws:lambda:us-west-2:123456789012:function:my-function:*"
            ]
        }
    ]
}
```

