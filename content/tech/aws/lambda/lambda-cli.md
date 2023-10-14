---
title: "AWS Lambda をコマンドライン (CLI) で操作する"
url: "p/n9nydjc/"
date: "2021-03-12"
tags: ["AWS", "AWS/Lambda"]
---

Lambda 関数の一覧を取得する (lambda list-functions)
----

{{< code >}}
$ aws lambda list-functions --max-items 10
{{< /code >}}

{{< code title="関数名だけ取り出す" >}}
$ aws lambda list-functions --max-items 10
      --query "Functions[].FunctionName"
{{< /code >}}

{{< code title="Node.js 10 を使ってる関数の ARN を調べる" >}}
$ aws lambda list-functions --function-version ALL --output text
      --query "Functions[?Runtime=='nodejs10.x'].FunctionArn
{{< /code >}}

AWS から来た「Node.js 10 のサポート切れるから乗り換えてね」というメール (2021-06-04) に書かれていたやり方です。


Lambda 関数の情報を取得する (lambda get-function)
----

{{< code >}}
$ aws lambda get-function --function-name my-function
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
{
    "Configuration": {
        "FunctionName": "my-function",
        "FunctionArn": "arn:aws:lambda:ap-northeast-1:123456789012:function:my-function",
        "Runtime": "nodejs12.x",
        "Role": "arn:aws:iam::123456789012:role/lambda-ex",
        "CodeSha256": "FpFMvUhayLkOoVBpNuNiIVML/tuGv2iJQ7t0yWVTU8c=",
        "Version": "$LATEST",
        "TracingConfig": {
            "Mode": "PassThrough"
        },
        "RevisionId": "88ebe1e1-bfdf-4dc3-84de-3017268fa1ff",
        ...
    },
    "Code": {
        "RepositoryType": "S3",
        "Location": "https://awslambda-ap-northeast-1-tasks.s3.ap-northeast-1.amazonaws.com/snapshots/123456789012/my-function-4203078a-b7c9-4f35-..."
    }
}
{{< /code >}}
{{< /accordion >}}

Lambda 関数のデプロイパッケージをダウンロードするために使用できる、Lambda 関数メタデータと署名付き URL が含まれています。


Lambda 関数を作成する (lambda create-function)
----

{{< code >}}
$ aws lambda create-function
      --function-name my-function \
      --runtime nodejs12.x \
      --handler index.handler \
      --zip-file fileb://function.zip \
      --role arn:aws:iam::123456789012:role/lambda-ex
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
{
    "FunctionName": "my-function",
    "FunctionArn": "arn:aws:lambda:ap-northeast-1:123456789012:function:my-function",
    "Runtime": "nodejs12.x",
    "Role": "arn:aws:iam::123456789012:role/lambda-ex",
    "Handler": "index.handler",
    "CodeSha256": "FpFMvUhayLkOoVBpNuNiIVML/tuGv2iJQ7t0yWVTU8c=",
    "Version": "$LATEST",
    "TracingConfig": {
        "Mode": "PassThrough"
    },
    "RevisionId": "88ebe1e1-bfdf-4dc3-84de-3017268fa1ff",
    ...
}
{{< /code >}}
{{< /accordion >}}

タグを付けるときは、関数の作成時に次のようなオプションを追加します。

{{< code >}}
--tags Department=Marketing,CostCenter=1234ABCD
{{< /code >}}


Lambda 関数を削除する (lambda delete-function)
----

{{< code >}}
$ aws lambda delete-function --function-name my-function
{{< /code >}}


Lambda 関数の ZIP パッケージをアップロードする (lambda update-function-code)
----

{{< code >}}
aws lambda update-function-code \
    --function-name my-function \
    --zip-file fileb://function.zip
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
{
    "FunctionName": "my-function",
    "FunctionArn": "arn:aws:lambda:us-west-2:123456789012:function:my-function",
    "Runtime": "nodejs12.x",
    "Role": "arn:aws:iam::123456789012:role/lambda-role",
    "Handler": "index.handler",
    "CodeSha256": "Qf0hMc1I2di6YFMi9aXm3JtGTmcDbjniEuiYonYptAk=",
    "Version": "$LATEST",
    "TracingConfig": {
        "Mode": "Active"
    },
    "RevisionId": "983ed1e3-ca8e-434b-8dc1-7d72ebadd83d",
    ...
}
{{< /code >}}
{{< /accordion >}}

{{% maku-common/reference %}}
- [Windows のコマンドラインから zip ファイルを作成する (Compress-Archive)](/p/vin35f7)
{{% /maku-common/reference %}}


Lambda 関数を呼び出す (lambda invoke)
----

{{< code title="Lambda 関数のレスポンスをファイルに取得する" >}}
$ aws lambda invoke --function-name my-function output.yml
ExecutedVersion: $LATEST
StatusCode: 200

$ cat output.yml
{"body": "Hello World!", "statusCode": 200}
{{< /code >}}

{{% private %}}
Lambda 関数を呼び出したときのレスポンスは、基本的には上記のようにファイルに取得しますが、次のようにして標準出力に直接表示することもできます。

{{< code title="Lambda 関数のレスポンスを標準出力に出力する" >}}
$ aws lambda invoke --function-name my-function out --log-type Tail
ExecutedVersion: $LATEST
LogResult: U1RBUlQgUmVxdWVzdElk...（Base64エンコードされている）
StatusCode: 200
{{< /code >}}

ただし、ログ部分 (`LogResult`) は上記のように をデコードするには、`base64` ユーティリティを使用します。

{{< code >}}
$ aws lambda invoke --function-name my-function out --log-type Tail \
      --query 'LogResult' --output text | base64 -d
{{< /code >}}

macOS の場合は `base64 -d` の部分を `base64 -D` にします。
{{% /private %}}


Lambda 関数のリソースベースポリシー設定
----

Lambda のリソースベースポリシーは、他のサービスからの Lambda 関数へのアクセス許可設定を保持しています。
Lambda 関数は 1 つの __リソースベースポリシー__ を持ち、その中にアクセス権限を表現する複数の __ステートメント__ が含まれている、という構成になります。

{{< shout "Lambda 関数 ◇── ポリシー x 1 ◇── ステートメント x N" >}}

### ステートメントを追加する (lambda add-permission)

他のサービスに Lambda 関数を呼び出す権限を与えるには、Lambda 関数のリソースベースポリシーに、ステートメントを追加します。

{{< code title="例: すべての SNS トピックからの呼び出しを許可" >}}
$ aws lambda add-permission \
      --function-name my-function \
      --action lambda:InvokeFunction \
      --statement-id sns \
      --principal sns.amazonaws.com \

Statement: '{"Sid":"sns","Effect":"Allow","Principal":{"Service":"sns.amazonaws.com"},"Action":"lambda:InvokeFunction","Resource":"arn:aws:lambda:ap-northeast-1:123456789012:function:my-function"}'
{{< /code >}}

この例では、`sns` という名前のステートメントを Lambda 関数のポリシーに追加しています。
コマンドの実行結果として、追加されたステートメントの内容（JSON 形式）が出力されます。
ここで割り当てたステートメント ID（上記の例では `sns`）は、ステートメントを削除する場合などに使用します。

上記の例では、すべての SNS トピックからのアクセスを許可していますが、特定の SNS トピックからの Lambda 関数呼び出し (`lambda:InvokeFunction`) を許可するには、__`--source-arn`__ パラメータを追加で指定します。

{{< code title="例: 特定の SNS トピックからの呼び出しを許可" >}}
$ aws lambda add-permission \
      --function-name my-function \
      --action lambda:InvokeFunction \
      --statement-id sns-my-topic \
      --principal sns.amazonaws.com \
      --source-arn arn:aws:sns:ap-northeast-1:123456789012:my-topic

Statement: '{"Sid":"sns-my-topic","Effect":"Allow","Principal":{"Service":"sns.amazonaws.com"},"Action":"lambda:InvokeFunction","Resource":"arn:aws:lambda:ap-northeast-1:123456789012:function:my-function","Condition":{"ArnLike":{"AWS:SourceArn":"arn:aws:sns:ap-northeast-1:123456789012:my-topic"}}}'
{{< /code >}}

例外的に S3 バケットの ARN にはアカウント ID が含まれないため、特定のアカウントの S3 バケットからのアクセスを許可するには、__`source-account`__ パラメータでアカウントを指定する必要があります。

{{< code title="例: 特定の S3 バケットからの呼び出しを許可" >}}
$ aws lambda add-permission \
      --function-name my-function \
      --action lambda:InvokeFunction \
      --statement-id s3-account \
      --principal s3.amazonaws.com \
      --source-arn arn:aws:s3:::my-bucket-123456 \
      --source-account 123456789012
{{< /code >}}

#### その他の例

{{< accordion title="すべての AppSync から Lambda 関数を呼び出せるようにする" >}}
{{< code lang="console" >}}
$ aws lambda add-permission \
      --function-name "my-function" \
      --action lambda:InvokeFunction \
      --statement-id "appsync" \
      --principal appsync.amazonaws.com \
      --output text
{{< /code >}}
{{< /accordion >}}

{{< accordion title="特定の AppSync から Lambda を呼び出せるようにする" >}}
{{< code lang="console" >}}
$ aws lambda add-permission \
      --function-name "my-function" \
      --action lambda:InvokeFunction \
      --statement-id "appsync" \
      --principal appsync.amazonaws.com \
      --source-arn "<AppSync API ARN>" \
      --output text
{{< /code >}}
{{< /accordion >}}

### ステートメントを削除する (lambda remove-permission)

{{< code title="例: sns という名前のポリシーステートメントを削除" >}}
$ aws lambda remove-permission \
      --function-name my-function \
      --statement-id sns
{{< /code >}}

### リソースベースポリシーを取得する (lambda get-policy)

{{< code title="例: 関数に設定されたポリシーを表示" >}}
$ aws lambda get-policy \
     --function-name my-function \
     --query "Policy" --output text

{"Version":"2012-10-17","Id":"default","Statement":[{"Sid":"sns","Effect":"Allow","Principal":{"Service":"sns.amazonaws.com"},"Action":"lambda:InvokeFunction","Resource":"arn:aws:lambda:ap-northeast-1:123456789012:function:my-function"},{"Sid":"sns-my-topic","Effect":"Allow","Principal":{"Service":"sns.amazonaws.com"},"Action":"lambda:InvokeFunction","Resource":"arn:aws:lambda:ap-northeast-1:123456789012:function:my-function","Condition":{"ArnLike":{"AWS:SourceArn":"arn:aws:sns:ap-northeast-1:123456789012:my-topic"}}}]}
{{< /code >}}

Lambda 関数のリソースベースポリシーの内容は、上記のような一行の JSON テキストとして保存されているので、そのままでは見にくいかもしれません。
下記はこの JSON レスポンスを Ruby で YAML 形式に整形する例です。

{{< code >}}
$ aws lambda get-policy \
      --function-name my-function \
      --query "Policy" --output text \
      | ruby -ryaml -e 'puts YAML.load(STDIN).to_yaml'
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="yaml" >}}
---
Version: '2012-10-17'
Id: default
Statement:
- Sid: sns
  Effect: Allow
  Principal:
    Service: sns.amazonaws.com
  Action: lambda:InvokeFunction
  Resource: arn:aws:lambda:ap-northeast-1:123456789012:function:my-function
- Sid: sns-my-topic
  Effect: Allow
  Principal:
    Service: sns.amazonaws.com
  Action: lambda:InvokeFunction
  Resource: arn:aws:lambda:ap-northeast-1:123456789012:function:my-function
  Condition:
    ArnLike:
      AWS:SourceArn: arn:aws:sns:ap-northeast-1:123456789012:my-topic
{{< /code >}}
{{< /accordion >}}

この Lambda 関数のポリシーとしては、`sns` と `sns-my-topic` というステートメントで、外部からのアクセスを許可していることがわかります。


Lambda 関数のタグを制御する
----

### タグを追加する (lambda tag-resource)

{{< code >}}
$ aws lambda tag-resource \
      --resource arn:aws:lambda:ap-northeast-1:123456789012:function:my-func
      --tags Department=Marketing,CostCenter
{{< /code >}}

### タグを削除する (lambda untag-resource)

{{< code >}}
$ aws lambda untag-resource \
      --resource arn:aws:lambda:ap-northeast-1:123456789012:function:my-func
      --tag-keys Department
{{< /code >}}

### タグの一覧を取得する (lambda list-tags)

{{< code >}}
$ aws lambda list-tags \
      --resource arn:aws:lambda:ap-northeast-1:123456789012:function:my-func
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
Tags:
  Key1: Value1
  Key2: Value2
  Key3: Value3
{{< /code >}}
{{< /accordion >}}

ARN じゃなくて関数名で取得したいのであれば、`get-function` で。

{{< code >}}
$ aws lambda get-function \
      --function-name my-func --query "Tags"
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
Key1: Value1
Key2: Value2
Key3: Value3
{{< /code >}}
{{< /accordion >}}

