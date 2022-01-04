---
title: "AWS CloudFormation の設定例: S3 通知を SNS トピックに Publish する"
linkTitle: "CloudFormation の設定例: S3 通知を SNS トピックに Publish する"
url: "/p/3o2dpyb"
date: "2021-04-13"
tags: ["AWS", "AWS/CloudFormation"]
---

{{% private %}}
- [SNS ドキュメント](https://docs.aws.amazon.com/ja_jp/sns/latest/dg/welcome.html)
- [テンプレートスニペット - Amazon SNS トピックポリシーの宣言](https://docs.aws.amazon.com/ja_jp/AWSCloudFormation/latest/UserGuide/quickref-iam.html#scenario-sns-policy)
- [Amazon SNS でのアイデンティティベースのポリシーの使用](https://docs.aws.amazon.com/ja_jp/sns/latest/dg/sns-using-identity-based-policies.html)
- [Amazon SNS API権限: アクションとリソース参照](https://docs.aws.amazon.com/ja_jp/sns/latest/dg/sns-access-policy-language-api-permissions-reference.html)
{{% /private %}}

何をするか？
----

ここでは、CloudFormation のテンプレートを使って、次のような AWS リソースを定義してみます。

- S3 バケット（Logical ID: `MyBucket`)
- SNS トピック (Logical ID: `MyTopic`)
- 上記の S3 バケットへの書き込み時に、SNS トピックへ publish

動作のイメージとしてはこんな感じです。

{{< image src="img-s3-to-sns.svg" >}}

このような構成でリソースを作っておくと、S3 バケット上のデータ更新を、SNS トピックのサブスクライブによって監視できるようになります。
例えば、Lambda 関数を SNS トピックのサブスクライバーとして登録すれば、S3 バケットへの書き込みを Lambda 関数でハンドルできます。

### 参考情報

CloudFormation テンプレートで S3 バケットや SNS トピックを定義する方法は、下記の記事を参考にしてください。

- [CloudFormation で S3 バケットの作成](/p/ugt6gr4)
- [CloudFormation で SNS トピックの作成](/p/ymzbmx9)


テンプレートの記述例
----

次の CloudFormation テンプレートでは、S3 バケットと SNS トピックのリソースを定義しています。
デフォルトの名前はそれぞれ次のようになっています。

- S3 バケット名: `myapp-sample-bucket-<AccountId>`
- SNS トピック名: `myapp-sample-topic`

S3 バケット名は世界で一意でなければいけないため、末尾に使用中のアカウント ID を付加するようにしています。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'

Parameters:
  BucketPrefix:
    Type: String
    Default: myapp-sample-bucket
  TopicName:
    Type: String
    Default: myapp-sample-topic

Resources:
  MyTopic:
    Type: AWS::SNS::Topic
    Properties:
      TopicName: !Sub ${TopicName}

  MyTopicPolicy:
    Type: AWS::SNS::TopicPolicy
    Properties:
      PolicyDocument:
        Id: !Ref MyTopic
        Version: 2012-10-17
        Statement:
        - Effect: Allow
          Principal:
            Service: s3.amazonaws.com
          Action: sns:Publish
          Resource: !Ref MyTopic
          Condition:
            ArnLike:
              aws:SourceArn: !Sub arn:aws:s3:::${BucketPrefix}-${AWS::AccountId}
      Topics:
      - !Ref MyTopic

  MyBucket:
    Type: AWS::S3::Bucket
    Properties:
      BucketName: !Sub ${BucketPrefix}-${AWS::AccountId}
      NotificationConfiguration:
        TopicConfigurations:
          - Event: s3:ObjectCreated:Put
            Topic: !Ref MyTopic
{{< /code >}}

このテンプレートを使って CloudFormation スタックを生成するには、例えば AWS CLI を使って次のように実行します。

{{< code title="mystack スタックの生成" >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}

1 分ほど待つと、スタックの生成が完了します。

あとは、SNS トピックのコンソールから E メールなどをサブスクリプション登録して、S3 バケットに適当にファイルをアップロードすれば、JSON 形式のイベント通知が届くはずです。
その通知には S3 バケット名などが含まれているので、例えば、Lambda 関数から S3 バケットの情報を取り出すといったことが簡単に行えます。

{{< accordion title="サブスクライバーに通知される JSON の例" >}}
{{< code lang="json" >}}
{
  "Records": [
    {
      "eventVersion": "2.1",
      "eventSource": "aws:s3",
      "awsRegion": "ap-northeast-1",
      "eventTime": "2021-04-13T18:30:34.201Z",
      "eventName": "ObjectCreated:Put",
      "userIdentity": {
        "principalId": "AWS:AMKLZ6V3HGAIDAQXINMCQ"
      },
      "requestParameters": {
        "sourceIPAddress": "110.76.125.97"
      },
      "responseElements": {
        "x-amz-request-id": "7ACH2WP7F8SCVTP8",
        "x-amz-id-2": "tpjlZo9kIjD9kgHA6WjiEh/XW3eh7NyByw3jxho8pVTZnEUG0NixkAMO00zfwD4IRxyOJIk8WRziEIZR24bXxXr37fruczEB"
      },
      "s3": {
        "s3SchemaVersion": "1.0",
        "configurationId": "497e6001-c4fb-df40-9e93-c09289477696",
        "bucket": {
          "name": "myapp-sample-bucket-123456789012",
          "ownerIdentity": {
            "principalId": "FJF0VI7A2H51MM"
          },
          "arn": "arn:aws:s3:::myapp-sample-bucket-123456789012"
        },
        "object": {
          "key": "Key1",
          "size": 5,
          "eTag": "dcd0cda10b5681c4a9e9ff6fe9e95990",
          "sequencer": "F2B20061006075E34D"
        }
      }
    }
  ]
}
{{< /code >}}
{{< /accordion >}}


テンプレートの解説
----

まず、SNS トピックは次のような感じで簡単に定義できます。

{{< code lang="yaml" >}}
Resources:
  MyTopic:
    Type: AWS::SNS::Topic
    Properties:
      ...
{{< /code >}}

S3 バケットから SNS トピックに対して public できるようにする権限を設定するには、__S3 バケットを作成する前に__、次のような __`AWS::SNS::TopicPolicy`__ リソースを作成しておく必要があります。
形としては、`Topics` プロパティで指定した SNS トピックに対して、`PolicyDocument` プロパティで定義したポリシーが割り当てられるという意味になります。

{{< code lang="yaml" >}}
Resources:
  MyTopicPolicy:
    Type: AWS::SNS::TopicPolicy
    Properties:
      PolicyDocument:
        Id: <任意のID>
        Version: 2012-10-17
        Statement:
        - Effect: Allow
          Principal:
            Service: s3.amazonaws.com
          Action: sns:Publish
          Resource: <SNSトピックのARN>
          Condition:
            ArnLike:
              aws:SourceArn: <S3バケットのARN>
      Topics:
      - <SNSトピックのARN>
{{< /code >}}

S3 バケットの定義では、何らかの操作時（例えばオブジェクトの追加時）に、SNS トピックに対して publish を行うという指定を __`NotificationConfiguration`__ プロパティで設定します。
この設定を行った状態で CloudFormation スタックを生成しようとすると、S3 バケットリソースの生成時に、SNS トピックポリシーが正しく設定されているかの確認が行われます。
そのため CloudFormation は、依存する SNS トピックポリシー（ここでは前述の `MyTopicPolicy`）を先に生成しようとします。

{{< code lang="yaml" >}}
Resources:
  MyBucket:
    Type: AWS::S3::Bucket
    Properties:
      NotificationConfiguration:
        TopicConfigurations:
          - Event: s3:ObjectCreated:Put
            Topic: <通知先のSNSトピックのARN>
{{< /code >}}

まとめると、次のような順序でリソースが生成されることになります（生成順序は CloudFormation が依存関係に基づいて判断するので、特に意識して記述する必要はありません）。

1. SNS トピック
2. SNS トピックポリシー
    - 定義内で「SNS トピック」と「S3 バケット」を指定する
3. S3 バケット
    - 適切な「SNS トピックポリシー」が先に定義されている必要がある

ここで問題になるのは、2 番目のトピックポリシーの定義内で S3 バケットを指定するときに、まだ S3 バケットの生成が完了していないということです。
これは、CloudFormation によって __自動で割り当てられる S3 バケット名 (Physical ID) を使用できない__ ということを意味します。
例えば、トピックポリシーの定義で `!Ref MyBucket` のように S3 バケット名を参照しようとすると、そんなバケットはまだ定義されていないというエラーになります。

{{< code lang="S3バケットが参照できないエラー" >}}
ResourceStatus: CREATE_FAILED
ResourceStatusReason: 'Unable to validate the following destination configurations
  (Service: Amazon S3; Status Code: 400; Error Code: InvalidArgument; Request ID:
  ...
{{< /code >}}

CloudFormation はできる限り依存関係を解決するような順序でリソースを構築していきますが、S3 バケットに `TopicConfigurations` を設定するときは、その振る舞いを許可するための SNS トピックポリシーが事前に生成されている必要があります。
そのため、S3 バケットを SNS トピックポリシーより先に構築することはできません。
これにより、鶏と卵の関係のような「循環参照エラー」が発生します。

この問題の解決方法は [こちらの記事](https://aws.amazon.com/jp/premiumsupport/knowledge-center/unable-validate-destination-s3/) に、以下の 2 種類が紹介されています。

- 入力パラメータなどを使って、S3 バケット名を事前に決めておく（管理する名前が増える・・・）
- S3 バケットの `NotificationConfiguration` を無効にした状態でスタックを生成してから、もう一度今度は有効にした状態でスタックを更新する（手作業すぎるでしょ・・・）

<s>いやいやこれは CloudFormation の設計ミスでしょと言いたくなるのをグッとこらえて、</s> とにかく何らかの方法で S3 バケット名を参照できるようにしなければいけないので、上記のテンプレート例では、S3 バケットの名前を入力パラメータ (`BucketPrefix`) で指定できるようにしています。
あと、ついでに SNS トピック名も入力パラメータ (`TopicName`) で指定できるようにしています。

