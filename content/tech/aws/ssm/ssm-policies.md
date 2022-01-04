---
title: "AWS SSM のポリシーステートメント例"
url: "/p/s3o57jv"
date: "2021-10-15"
tags: ["AWS", "AWS/SSM"]
---

パラメーターの説明情報の取得
----

{{< code lang="json" title="すべてのパラメーターの説明情報を取得できるようにする" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "ssm:DescribeParameters"
            ],
            "Resource": "*"
        }
    ]
}
{{< /code >}}

__`ssm:DescribeParameters`__ は、SSM パラメーターストアに格納されたパラメーターの説明情報を取得する権限です。
この権限だけでは、パラメーターの「値」自体は取得できないことに注意してください。


パラメーターの値の取得
----

{{< code lang="json" title="prod- で始まるパラメーターの値を取得できるようにする" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "ssm:GetParameter*"
            ],
            "Resource": "arn:aws:ssm:<Region>:<Account>:parameter/prod-*"
        }
    ]
}
{{< /code >}}

パラメーターの Get 系アクションには、GetParameter/GetParameters/GetParameterHistory/GetParametersByPath など [いろいろある](https://docs.aws.amazon.com/ja_jp/systems-manager/latest/userguide/sysman-paramstore-access.html) ため、上記のように __`ssm:GetParameter*`__ とワイルドカード指定しておくと確実に取得できるようになります。
`GetParameter` だけだと、パラメーターの一覧取得などが許可されません。


パラメーターの追加・削除
----

{{< code lang="json" title="prod- で始まるパラメーターの追加と削除を許可する" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "ssm:PutParameter",
                "ssm:DeleteParameter",
                "ssm:DeleteParameters"
            ],
            "Resource": "arn:aws:ssm:<Region>:<Account>:parameter/prod-*"
        }
    ]
}
{{< /code >}}


暗号化されたパラメーター (SecureString) の取得
----

{{< code lang="json" title="暗号化された prod- で始まるパラメーターを読み込む" >}}
{
   "Version": "2012-10-17",
   "Statement": [
      {
         "Effect": "Allow",
         "Action": [
            "ssm:GetParameter*"
         ],
         "Resource": [
            "arn:aws:ssm:<Region>:<Account>:parameter/prod-*"
         ]
      },
      {
         "Effect": "Allow",
         "Action": [
            "kms:Decrypt"
         ],
         "Resource": [
            "arn:aws:kms:<Region>:<Account>:key/<KmsKey>"
         ]
      }
   ]
}
{{< /code >}}

SSM パラメーターストアに SecureString タイプとして格納されたパラメーターは、KMS キーによって暗号化されています。
このパラメーターの値を取得するには、__`ssm:GetParameter*`__ によるパラメーター読み取り権限と、__`kms:Decrypt`__ による複合化の権限が必要です。


暗号化されたパラメーター (SecureString) の作成
----

{{< code lang="js" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "ssm:PutParameter"
            ],
            "Resource": [
                "arn:aws:ssm:<Region>:<Account>:parameter/prod-*"
            ]
        },
        {
            "Effect": "Allow",
            "Action": [
               "kms:Encrypt",
               "kms:GenerateDataKey" // 暗号化された詳細パラメータの作成に必要
            ],
            "Resource": [
                "arn:aws:kms:<Region>:<Account>:key/<KmsKey>"
            ]
        }
    ]
}
{{< /code >}}


Principal で特定のユーザーに権限を絞る
----

{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Sid": "Allow SSM parameter access for maku",
            "Effect": "Allow",
            "Principal": {"AWS": "arn:aws:iam::111122223333:user/maku"},
            "Action": [
                "ssm:PutParameter",
                "ssm:DeleteParameter",
                "ssm:DeleteParameters"
            ],
            "Resource": [
                "arn:aws:ssm:<Region>:<Account>:parameter/prod-*"
            ]
        }
    ]
}
{{< /code >}}

