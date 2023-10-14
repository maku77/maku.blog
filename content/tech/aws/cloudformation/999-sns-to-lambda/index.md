---
title: "AWS CloudFormation の設定例: SNS トピックを Lambda 関数からサブスクライブする"
linkTitle: "CloudFormation の設定例: SNS トピックを Lambda 関数からサブスクライブする"
url: "p/5q4epyb/"
date: "2021-04-19"
tags: ["AWS", "AWS/CloudFormation"]
weight: 999
---

何をするか？
----

ここでは、CloudFormation (SAM) のテンプレートを使って、SNS トピックをサブスクライブする Lambda 関数を定義してみます。

{{< image src="img-sns-to-lambda.svg" >}}

サブスクライブ対象とする SNS トピック自体は、あらかじめ何らかの方法で作成済みであり、次のような ARN を取得できているものとします。

{{< code title="SNS トピックの ARN" >}}
arn:aws:sns:ap-northeast-1:123456789012:mytopic
{{< /code >}}

上記のような CloudFormation スタックが完成すると、マネージメントコンソールや CLI で SNS トピックのメッセージを発行して、Lambda 関数にイベントが届くことを確認できます。

{{% maku-common/reference %}}
- [CloudFormation で SNS トピックの作成](/p/ymzbmx9)
- [CloudFormation の設定例: S3 通知を SNS トピックに Publish する](/p/3o2dpyb)
{{% /maku-common/reference %}}


テンプレートの記述例
----

次の SAM テンプレートでは、Lambda 関数を定義しつつ、そのイベントソースとして SNS トピックを設定しています。
イベントソースの指定は、実際には、SNS トピックに Lambda 関数をサブスクライブすることを意味しています。
SNS トピックの ARN は、入力パラメータ `TopicArn` のデフォルト値として指定しています。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: 2010-09-09
Transform: AWS::Serverless-2016-10-31

Parameters:
  TopicArn:
    Type: String
    Default: arn:aws:sns:ap-northeast-1:123456789012:mytopic

Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      Handler: index.handler
      InlineCode: |
        import json
        def handler(event, context):
          s = json.dumps(event, indent=2)
          print('Message received from SNS:' + s)
          return {'body': s, 'statusCode': 200}

      # Lambda 関数を SNS トピックにサブスクライブする
      Events:
        MySnsEvent:
          Type: SNS
          Properties:
            Topic: !Ref TopicArn
{{< /code >}}

Lambda 関数の実装は `InlineCode` としてテンプレートに埋め込んでいます。
ここでは、単純に第一引数 (`event`) の内容を出力しています。

### デプロイ

AWS CLI で次のように実行して、CloudFormation スタックを生成します。
もちろん、CloudFormation のマネージドコンソール（Web サイト）から実行しても構いません。

{{< code >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}

しばらく待つと、SNS トピックにサブスクライブされた Lambda 関数リソースが生成されます。

### テスト実行

SNS トピックのマネージメントコンソールから「メッセージを発行」を実行すると、Lambda 関数は次のようなメッセージを受信します。

{{< code lang="js" >}}
Message received from SNS:
{
  "Records": [
    {
      "EventSource": "aws:sns",
      "EventVersion": "1.0",
      "EventSubscriptionArn": "arn:aws:sns:ap-northeast-1:123456789012:mytopic:0884c0db-5d81-4e13-829f-596f7ea9f8ad",
      "Sns": {
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
    }
  ]
}
{{< /code >}}

ちなみに、AWS CLI を使って SNS トピックにメッセージ発行 (publish) することもできます。

{{< code >}}
$ aws sns publish \
    --subject "Message Title" \
    --message "Message Body" \
    --topic-arn arn:aws:sns:ap-northeast-1:123456789012:mytopic
{{< /code >}}

{{% private %}}
- [SNS - AWS Serverless Application Model](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/sam-property-function-sns.html)
{{% /private %}}

