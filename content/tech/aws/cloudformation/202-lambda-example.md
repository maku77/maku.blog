---
title: "AWS CloudFormation の設定例: Lambda 関数の設定いろいろ"
linkTitle: "CloudFormation の設定例: Lambda 関数の設定いろいろ"
url: "p/pbo2dpy/"
date: "2021-04-08"
tags: ["AWS", "AWS/CloudFormation", "AWS/Lambda", "AWS/SAM"]
weight: 202
---

{{% private %}}
- [Globals セクション - AWS SAM テンプレート](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/sam-specification-template-anatomy-globals.html)
{{% /private %}}


タイムアウト時間 (Timeout)
----

Lambda 関数のタイムアウト時間（最大実行可能時間）を設定するには、`Timeout` プロパティで秒単位の指定を行います。

{{< code lang="yaml" title="例: タイムアウトを 15 秒にする" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31

Resources:
  MyLambda:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: nodejs14.x
      CodeUri: function.zip
      Handler: index.handler
      Timeout: 15
{{< /code >}}

すべての Lambda 関数の Timeout 値をまとめて設定したいときは、`Globals` セクションを使います。

{{< code lang="yaml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31

Globals:
  Function:
    Runtime: nodejs14.x
    CodeUri: function.zip
    Timeout: 15

Resources:
  MyLambda1:
    Type: AWS::Serverless::Function
    Properties:
      Handler: index.handler1
  MyLambda2:
    Type: AWS::Serverless::Function
    Properties:
      Handler: index.handler2
{{< /code >}}


環境変数 (Environment)
----

Lambda 関数の中から参照可能な環境変数を設定するには、__`Variables`__ プロパティに、キーと値のペアを指定します。
すべての Lambda 関数に共通で設定する環境変数は、`Globals` セクションで定義します。

{{< code lang="yaml" >}}
Globals:
  Function:
    Environment:
      Variables:
        STAGE: Production
        TABLE_NAME: global-table

Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Environment:
        Variables:
          TABLE_NAME: resource-table
          NEW_VAR: hello
{{< /code >}}


トリガー設定 (Events)
----

Lambda 関数を呼び出すためのトリガー（イベント）は、__`Events`__ プロパティで定義できます。
`Resouces` プロパティと同様に、`Events` プロパティの一階層目にはイベントの論理 ID (Logical ID) を記述して、その下にそのイベントの設定を記述します。

{{< code lang="yaml" title="Events プロパティの構成" >}}
Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Events:
        イベントの論理ID1:
          イベントの設定...
        イベントの論理ID2:
          イベントの設定...
{{< /code >}}

### 一定時間ごとに呼び出す (Type: Schedule)

Lambda 関数を一定時間ごとに呼び出したり、毎日決まった時刻に呼び出したりするには、`Type: Schedule` のイベントを設定します。
実際に生成される AWS リソースは EventBridge (CloudWatch Events) ルールです（リソースタイプは `AWS::Events::Rule`）。

次の例では、5 分おきに Lambda 関数を呼び出すように設定しています。

{{< code lang="yaml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31

Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      Handler: index.handler
      CodeUri: src/
      Events:
        MySchedule:
          Type: Schedule
          Properties:
            Schedule: 'rate(5 minutes)'
            # Schedule: 'cron(0 * * * ? *)'
            Description: test schedule
            # Enabled: False
{{< /code >}}

生成される `AWS::Events::Rule` の物理 ID (Physical ID) は、スタックやリソースの名前（論理 ID）を組み合わせて、次のような名前が自動的に割り当てられます。

{{< code >}}
mystack-HelloFunctionMySchedule-H5JKY9WW8AOI
{{< /code >}}

物理 ID を明示的に指定したい場合は、`Schedule` イベントの `Name` プロパティを指定します。

{{% maku-common/reference %}}
- [Schedule - AWS Serverless Application Model](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/sam-property-function-schedule.html)
- [Amazon EventBridge ルールのスケジュール式](https://docs.aws.amazon.com/ja_jp/eventbridge/latest/userguide/eb-schedule-expressions.html)
{{% /maku-common/reference %}}

### S3 バケットからのイベント (Type: S3)

次の例では、指定したバケット（論理 ID: `SrcBucket`）にオブジェクトが作成されたときに Lambda 関数を呼び出すように指定しています。

{{< code lang="yaml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31

Resources:
  CreateThumbnail:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      Handler: index.handler
      CodeUri: src/
      Events:
        CreateThumbnailEvent:
          Type: S3
          Properties:
            Bucket: !Ref SrcBucket
            Events: s3:ObjectCreated:*

  SrcBucket:
    Type: AWS::S3::Bucket
{{< /code >}}


Lambda のポリシーの設定 (Policies)
----

SAM で Lambda 関数 (`AWS::Lambda::Function`) を生成すると、暗黙的に生成されるロールがデフォルトロールとして設定されます。
__`Policies`__ プロパティを使うと、このデフォルトロールに対して任意の管理ポリシーや、インラインポリシーを追加で設定することができます。
なお、`Role` プロパティで明示的にロールを指定している場合は、デフォルトロールではなく、そちらのロールを使うことになるので、`Policies` プロパティを指定することはできません。

{{< code lang="yaml" >}}
Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Handler: index.handler
      ...
      Policies:
        - AWSLambdaInvocation-DynamoDB
        - Version: '2012-10-17'
          Statement:
            - Effect: Allow
              Action:
                - s3:GetObject
                - s3:GetObjectACL
              Resource: 'arn:aws:s3:::my-bucket/*'
{{< /code >}}

`Policies` プロパティには、リスト形式で複数のポリシーを指定することができます。
上記の例では、管理ポリシーとして [AWSLambdaInvocation-DynamoDB](https://console.aws.amazon.com/iam/home?#/policies/arn:aws:iam::aws:policy/AWSLambdaInvocation-DynamoDB$jsonEditor) を指定し、その下に YAML 形式のインラインポリシーを指定しています。
管理プロパティを 1 つだけ指定すればよいのであれば、`Policies: AWSLambdaInvocation-DynamoDB` のように一行で記述することができます。

{{< accordion title="(参考) AWSLambdaInvocation-DynamoDB ポリシー" >}}
{{< code lang="json" >}}
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "lambda:InvokeFunction"
            ],
            "Resource": "*"
        },
        {
            "Effect": "Allow",
            "Action": [
                "dynamodb:DescribeStream",
                "dynamodb:GetRecords",
                "dynamodb:GetShardIterator",
                "dynamodb:ListStreams"
            ],
            "Resource": "*"
        }
    ]
}
{{< /code >}}
{{< /accordion >}}

次の例では、[SAM ポリシーテンプレート](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/serverless-policy-templates.html) として標準で定義されている [S3CrudPolicy](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/serverless-policy-template-list.html#s3-crud-policy) を設定し、S3 バケット（論理 ID: `MyBucket`）への読み書きを許可しています。

{{< code lang="yaml" >}}
Resources:
  MyFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: nodejs14.x
      Handler: build/index.handler
      CodeUri: function.zip
      Policies:
        - S3CrudPolicy:
            BucketName: !Ref MyBucket
      Environment:
        Variables:
          BUCKET_NAME: !Ref MyBucket
  MyBucket:
    Type: AWS::S3::Bucket
{{< /code >}}

上記では、S3 バケット用に `S3CrudPolicy` を使用していますが、DynamoDB の場合も同様の [DynamoDBCrudPolicy](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/serverless-policy-template-list.html#dynamo-db-crud-policy) ポリシーが定義されています。

{{< code lang="yaml" >}}
Policies:
  - DynamoDBCrudPolicy:
      TableName: !Ref MyTable
{{< /code >}}


Lambda の実行ロールの設定 (Role)
----

SAM で Lambda 関数 (`AWS::Lambda::Function`) を生成すると、暗黙的に実行ロールが生成されてアタッチされますが、明示的に作成した IAM ロールを __`Role`__ プロパティで指定することもできます。

{{< code lang="yaml" >}}
Resources:
  AppendItemToListFunction:
    Type: AWS::Lambda::Function
    Properties:
      Runtime: nodejs14.x
      Handler: index.handler
      Role: !GetAtt LambdaExecutionRole.Arn
      Code:
        ZipFile: !Sub |
          var response = require('cfn-response');
          exports.handler = function(event, context) {
             var responseData = {Value: event.ResourceProperties.List};
             responseData.Value.push(event.ResourceProperties.AppendedItem);
             response.send(event, context, response.SUCCESS, responseData);
          };

  LambdaExecutionRole:
    Type: AWS::IAM::Role
    Properties:
      AssumeRolePolicyDocument:
        Version: '2012-10-17'
        Statement:
        - Effect: Allow
          Principal:
            Service:
            - lambda.amazonaws.com
          Action:
          - sts:AssumeRole
      Path: "/"
      Policies:
      - PolicyName: root
        PolicyDocument:
          Version: '2012-10-17'
          Statement:
          - Effect: Allow
            Action:
            - logs:*
            Resource: arn:aws:logs:*:*:*
{{< /code >}}

上記の例では、テンプレート内で定義した IAM ロールを次のように論理 ID で指定しています。

{{< code lang="yaml" >}}
Role: !GetAtt LambdaExecutionRole.Arn
{{< /code >}}

すでに別のスタックなどで生成したロールをアタッチする場合は、次のように ARN で指定することができます。

{{< code lang="yaml" >}}
Role: !Sub "arn:aws:iam::${AWS::AccountId}:role/service-role/my-function-role-hfow8uki"
{{< /code >}}

