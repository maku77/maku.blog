---
title: "AWS SNS トピックから通知されるイベントデータの例"
linkTitle: "SNS トピックから通知されるイベントデータの例"
url: "/p/qcpybnx"
date: "2021-04-19"
tags: ["AWS"]
---

AWS SNS トピックからメッセージが発行されたときに、サブスクライバー（Lambda 関数など）にどのようなイベントデータが配信されるかの例です。

基本的なメッセージの構造
----

### Lambda 関数で受け取る場合

SNS トピックに対して Lambda 関数をサブスクライブしておくと、Lambda 関数の第一引数 (event) で、次のようなオブジェクトを受信できます。
重要な情報は、`Records[0].Sns` の下に格納されています。

{{< code lang="js" >}}
{
  "Records": [
    {
      "EventSource": "aws:sns",
      "EventVersion": "1.0",
      "EventSubscriptionArn": "arn:aws:sns:ap-northeast-1:123456789012:mytopic:0884-5d81c0db-4e13-829f-596f7ea9f8ad",
      "Sns": {
        "Type": "Notification",
        ...
        "Subject": "Message Title",
        "Message": "Message Body",
        ...
      }
    }
  ]
}
{{< /code >}}

### メールで受け取る場合

SNS トピックに「JSON 形式のメール」をサブスクライブしておくと、SNS のメッセージが発行されたときに、次のような内容のメールが届きます。

{{< code lang="js" >}}
{
  "Type": "Notification",
  ...
  "Subject": "Message Title",
  "Message": "Message Body",
  ...
}
{{< /code >}}

Lambda 関数の第一引数で渡される `event` オブジェクトで表現すると、__`event.Records[0].Sns`__ に相当する部分の情報がメールで送られてきます。


具体的な SNS メッセージの例
----

下記の例では、`Records[0].Sns` 以下の情報のみを示しています（JSON 形式のメールで送られる内容です）。

### SNS のマネージメントコンソールからメッセージ発行したとき

件名に `Message Title`、本文に `Message Body` を設定してメッセージ発行したときに送られるイベントデータの例です。

{{< code lang="json" >}}
{
  "Type" : "Notification",
  "MessageId" : "789b67e7-9bd8-5a6e-99d0-3f10e520edf7",
  "TopicArn" : "arn:aws:sns:ap-northeast-1:123456789012:mytopic",
  "Subject" : "Message Title",
  "Message" : "Message Body",
  "Timestamp" : "2021-04-19T12:51:04.019Z",
  "SignatureVersion" : "1",
  "Signature" : "bKXAFppLnPMeajvylkQvjiH/9RNeAlNfxbBC7HJbiOt7SKrw9oVk+CI1ZYsADcNt9aAtAOysdHrFD97J/N4n5o+tV+Dz5hsjxqKskYOCrYDRTTqLyhwa5OBtjAhU74IZy9ByfBSQWfOD1I5AFp0FLUWR8ieop/ZTV5buf4FodNm4scwW18nUJ1D5iTPNy3NinWq0wVP2FT7Ykt9HCdNleaFamMZ+war4OHsRhOgvDqOV4auZ2yvayMf70eJPHLbuQn09E0IlQYsvUArTOSknbFK3lsbeLfJBHmIa2qnuZm+VTknNgJxkCJNyer+7EKTrOABYqXsz55ENYJsn5xvjCA==",
  "SigningCertURL" : "https://sns.ap-northeast-1.amazonaws.com/SimpleNotificationService-94bdb98bd93083010a507c1833636cda.pem",
  "UnsubscribeURL" : "https://sns.ap-northeast-1.amazonaws.com/?Action=Unsubscribe&SubscriptionArn=arn:aws:sns:ap-northeast-1:123456789012:mytopic:97989c34-955f-49c8-856c-1b8e3e926a6f"
}
{{< /code >}}

### S3 の PutObject イベントを SNS から通知したとき

S3 バケットに対して `sample.txt` をアップロードしたときの SNS 通知の例です。
`Message` プロパティの中に JSON 形式の情報が格納されていて、この中身を見ると、アップロードされたファイルの名前（S3 オブジェクトのキー）などが分かります。

{{< code lang="json" >}}
{
  "Type" : "Notification",
  "MessageId" : "da5edc2e-1ccf-51d9-8601-2ada4b82a998",
  "TopicArn" : "arn:aws:sns:ap-northeast-1:123456789012:mytopic",
  "Subject" : "Amazon S3 Notification",
  "Message" : "{\"Records\":[{\"eventVersion\":\"2.1\",\"eventSource\":\"aws:s3\",\"awsRegion\":\"ap-northeast-1\",\"eventTime\":\"2021-04-19T13:16:16.834Z\",\"eventName\":\"ObjectCreated:Put\",\"userIdentity\":{\"principalId\":\"AWS:AIDAQXINMCQAMKLZ6V3HG\"},\"requestParameters\":{\"sourceIPAddress\":\"110.76.125.97\"},\"responseElements\":{\"x-amz-request-id\":\"JWTTJDZ40Y077BRS\",\"x-amz-id-2\":\"TNX1leb/NXvnS3fN/slo373HsAi6Wy0OUXK0LLlxLTfWHr+HhyxMckvUvkN91A7BDx2/UEFM0EFiCRJw0mGxlZBAy68j43vt\"},\"s3\":{\"s3SchemaVersion\":\"1.0\",\"configurationId\":\"497e6001-fbc4-40df-939e-c09289477696\",\"bucket\":{\"name\":\"myapp-sample-bucket-123456789012\",\"ownerIdentity\":{\"principalId\":\"FJF0VI7A2H51MM\"},\"arn\":\"arn:aws:s3:::myapp-sample-bucket-123456789012\"},\"object\":{\"key\":\"sample.txt\",\"size\":5,\"eTag\":\"cda10b5681c9e9ff6fe9e95990dcd04a\",\"sequencer\":\"48084D47F00607D82A\"}}}]}",
  "Timestamp" : "2021-04-19T13:16:21.315Z",
  "SignatureVersion" : "1",
  "Signature" : "EDu9KfAsGc4ajlurH+R+fALKFBGLbQEvm+DPMfI6GWyWk0e1tfg3VxpsSwRaE7+QTKeqjCwCXXLhsALo3eIZbFh7xL4mTX+4tDyPtw3zV4OpSlEfWJr0F3g9lP1ZbO0DbSqKqrLnthw3CPsG3yNDGgyEVTPR8BGIL0tTsmpNDDcHfOsEIwoxUCpR4HZfImLWKua4lZd8U1Yib/8r9p033T9kp5VoQlaK0lMRJI0WuI3Rbn7YC2Qvh2G3Lp8mszoqkUjmftmk7wCfgguB88dCLn5FU7ylHNanOao0do1BbV12rTcBMqZH3vbcezaqa9KVRcetDEHB5BJHQRGeG6t0og==",
  "SigningCertURL" : "https://sns.ap-northeast-1.amazonaws.com/SimpleNotificationService-94bdb98bd93083a010a507c1833636cd.pem",
  "UnsubscribeURL" : "https://sns.ap-northeast-1.amazonaws.com/?Action=Unsubscribe&SubscriptionArn=arn:aws:sns:ap-northeast-1:123456789012:mytopic:97989c34-955f-49c8-856c-1b8e3e926a6f"
}
{{< /code >}}

