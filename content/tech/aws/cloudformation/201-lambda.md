---
title: "AWS CloudFormation で Lambda 関数のリソースを生成する"
linkTitle: "CloudFormation で Lambda 関数のリソースを生成する"
url: "/p/w8s8kx9"
date: "2021-04-05"
tags: ["AWS", "AWS/SAM", "AWS/Lambda"]
weight: 201
---

{{% private %}}
- [AWS::Lambda::Function](https://docs.aws.amazon.com/AWSCloudFormation/latest/UserGuide/aws-resource-lambda-function.html)
- [AWS::Lambda::Function (JP)](https://docs.aws.amazon.com/ja_jp/AWSCloudFormation/latest/UserGuide/aws-resource-lambda-function.html)
- [AWS::IAM::Role (JP)](https://docs.aws.amazon.com/ja_jp/AWSCloudFormation/latest/UserGuide/aws-resource-iam-role.html)
{{% /private %}}


何をするか？
----

AWS CloudFormation を使うと Lambda 関数を含むインフラ（AWS リソース群）をまとめて生成することができますが、AWS CloudFormation の拡張である AWS SAM を使うと、もっと簡潔に Lambda 関数のリソースを生成することができます。

例えば、CloudFormation テンプレートの `AWS::Lambda::Function` リソースでは、`Role` プロパティが必須でしたが、SAM テンプレートの `AWS::Serverless::Function` リソースでは、`Role` プロパティはオプショナルになっています。
`Role` プロパティを省略すると、Lambda 関数に付けた論理 ID (Logical ID) をもとに、`<論理ID>Role` という名前のロールが自動生成されます。

ここでは、AWS SAM を使った Lambda 関数生成の基本として、次のようなパターンで CloudFormation スタックを生成してみます。

1. SAM テンプレートに関数コードを埋め込んでデプロイ
2. S3 バケット上の関数の ZIP ファイルを使ってデプロイ


SAM テンプレートに関数コードを埋め込んでデプロイ
----

まずは、一番シンプルな例として、SAM テンプレート内に Lambda 関数の実装をハードコーディングしてしまい、それをデプロイ（CloudFormation スタックの生成）してみます。

下記が SAM テンプレートです。
SAM で Lambda 関数のリソースを定義するときは、リソースタイプとして __`AWS::Serverless::Function`__ を指定します（CloudFormation では `AWS::Lambda::Function` です）。
ここでは関数の実装を __`InlineCode`__ プロパティでハードコーディングし、単純な `Hello World!` メッセージをレスポンスとして返すようにしています。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: Simple Lambda Function
Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      Handler: index.handler
      InlineCode: |
        def handler(event, context):
            return {'body': 'Hello World!', 'statusCode': 200}
{{< /code >}}

この例では省略していますが、`Function` リソースのプロパティ `PackageType` はデフォルトで `Zip` となっており、この場合は、関数の実体を `CodeUri`（ZIP ファイルのパス）か、`InlineCode`（ハードコーディング）のどちらかで指定します。
この例では、`InlineCode` で関数の内容を埋め込んでいます。

SAM テンプレートを記述したら、次のようにして CloudFormation のスタックを生成できます。
Lambda 関数のサービスロール（実行ロール）を自動生成するために、`--capabilities CAPABILITY_IAM` オプションの指定が必要です。

{{< code title="スタックの生成" >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}

スタックの作成が完了したら、次のようにしてスタック情報を確認できます。
スタック内に生成された Lambda 関数リソースの情報を見てみましょう。

{{< code title="作成したスタックの情報を表示" >}}
# スタックの情報
$ aws cloudformation describe-stacks --stack-name mystack

# リソースの一覧（ここでは Lambda 関数と自動生成された IAM ロール）
$ aws cloudformation describe-stack-resources --stack-name mystack
{{< /code >}}

{{< accordion title="describe-stack-resources の実行結果" >}}
{{< code lang="yaml" >}}
StackResources:
- DriftInformation:
    StackResourceDriftStatus: NOT_CHECKED
  LogicalResourceId: HelloFunction
  PhysicalResourceId: mystack-HelloFunction-1DVW05LYWG4L2
  ResourceStatus: CREATE_COMPLETE
  ResourceType: AWS::Lambda::Function
  StackId: arn:aws:cloudformation:ap-northeast-1:123456789012:stack/mystack/bc300a10-967f-11eb-9e10-0a2f97775449
  StackName: mystack
  Timestamp: '2021-04-06T02:29:10.784000+00:00'
- DriftInformation:
    StackResourceDriftStatus: NOT_CHECKED
  LogicalResourceId: HelloFunctionRole
  PhysicalResourceId: mystack-HelloFunctionRole-OCUMJ1FS9U0I
  ResourceStatus: CREATE_COMPLETE
  ResourceType: AWS::IAM::Role
  StackId: arn:aws:cloudformation:ap-northeast-1:123456789012:stack/mystack/bc300a10-967f-11eb-9e10-0a2f97775449
  StackName: mystack
  Timestamp: '2021-04-06T02:29:07.591000+00:00'
{{< /code >}}
{{< /accordion >}}

作成された Lambda 関数リソース (`AWS::Lambda::Function`) の __`PhysicalResouceID`__ を見ると、Lambda 関数の物理 ID (Physical ID) が、スタック名と論理 ID (Logical ID) の組み合わせで自動的に作成されていることが分かります（ここでは `mystack-HelloFunction-1DVW05LYWG4L2`）。
この物理 ID は、Lambda のマネージメントコンソール上で関数名として表示される名前です。
つまり、CloudFormation の外の世界から見える名前であり、アカウント内で一意な Lambda 関数名です。

Lambda 関数の物理 ID が分かれば、次のように CLI からテスト実行することができます。

{{< code title="Lambda 関数を呼び出してみる" >}}
$ aws lambda invoke --function-name mystack-HelloFunction-1DVW05LYWG4L2 out.txt
ExecutedVersion: $LATEST
StatusCode: 200

$ cat out.txt
{"body": "Hello World!", "statusCode": 200}
{{< /code >}}

最後に CloudFormation のスタックを削除しておきます。

{{< code title="スタックごと全てのリソースを削除" >}}
$ aws cloudformation delete-stack --stack-name mystack
{{< /code >}}

これで、スタック内に生成した Lambda 関数（およびそれ用のロール）などのリソースが全て消え去ります。すっきりすっきり。


S3 バケット上の関数の ZIP ファイルを使ってデプロイ
----

外部ライブラリなどを利用する Lambda 関数を作成する場合は、それらをまとめて ZIP 化して S3 バケットにアップロードしておく必要があります。
SAM テンプレート（CloudFormation テンプレート）では、S3 バケット上の ZIP ファイルを参照して Lambda 関数リソースを生成するように記述します。

### S3 バケットに Lambda 関数の ZIP をアップロード

まずは ZIP ファイル置き場にする S3 バケットを作成しておきます。
関数のコード自体は GitHub などで管理するでしょうから、この S3 バケットは一時的な転送用のバッファ、くらいの感覚で使えばよいと思います。

{{< code >}}
$ aws s3 mb s3://bucket-123456789012-functions
{{< /code >}}

ここでは、次のような Lambda 関数コードを ZIP 化することにします（簡単にするため依存ライブラリはなし）。

{{< code lang="python" title="src/index.py" >}}
def handler(event, context):
    return {'body': 'Hello World!', 'statusCode': 200}
{{< /code >}}

`src` ディレクトリ内のコードを ZIP ファイルにまとめます。

{{< code >}}
$ (cd src && zip -r ../function.zip *)
{{< /code >}}

あとは、次のように S3 バケットにアップロードします。

{{< code >}}
$ aws s3 cp function.zip s3://bucket-123456789012-functions
{{< /code >}}

### S3 バケットの ZIP を使って Lambda 関数をデプロイ（CloudFormation スタックを生成）

SAM テンプレート内から、S3 バケット上の ZIP ファイルを参照するには次のように記述します。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: Simple Lambda Function
Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      Handler: index.handler
      CodeUri: s3://bucket-123456789012-functions/function.zip
{{< /code >}}

`InlineCode` プロパティで関数実装をハードコードしていた部分を削除し、__`CodeUri`__ プロパティで S3 バケット上の ZIP ファイルを指定するようにします。

スタックの生成コマンドは、前述の例と同じです。

{{< code title="スタックの生成" >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}


（応用）S3 への ZIP アップロードも CloudFormation で行う
----

上記の例では、`aws s3 cp` コマンドで Lambda 関数の ZIP ファイルを S3 バケットにアップロードしていましたが、__`aws cloudformation package`__ コマンドで同様のことを行うことができます。

前提条件として、ローカルに `function.zip` の作成はできているとします。

SAM テンプレートを次のように書き換えます。
変更点は、__`CodeUri`__ プロパティの値が、S3 バケット上の ZIP オブジェクトではなく、ローカルファイルを示す `function.zip` になっているところです。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: Simple Lambda Function
Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      CodeUri: function.zip
      Handler: index.handler
{{< /code >}}

次のように実行すると、ローカルの `function.zip` ファイルが __`--s3-bucket`__ オプションで指定した S3 バケットにアップロードされ、新しい SAM テンプレートファイル (`template-pkg.yml`) が生成されます。

{{< code title="function.zip を S3 バケットへアップロードしてテンプレート生成" >}}
$ aws cloudformation package --template-file template.yml \
    --s3-bucket bucket-123456789012-functions \
    --output-template-file template-pkg.yml
{{< /code >}}

生成される SAM テンプレートの内容は次のようになっています。

{{< code lang="yaml" title="template-pkg.yml（自動生成）" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: Simple Lambda Function
Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      CodeUri: s3://bucket-123456789012-functions/c70430a6ec3e54f77073b5209ecea744
      Handler: index.handler
{{< /code >}}

`CodeUri` の値が置換されており、S3 バケット上にアップロードされた ZIP ファイルを示す URI に変わっています。
オブジェクト名は `function.zip` ファイルのハッシュコードになっており、ZIP ファイルの内容が変わるとオブジェクト名も変わるようになっています（つまり、`package` するたびに、S3 バケット内に別名の ZIP ファイルが溜まっていきます）。

あとは、前述の例と同様に、自動生成された SAM テンプレートを使って、CloudFormation スタックを生成するだけです（`template.yml` ではなく、自動生成された `template-pkg.yml` を指定することに注意してください）。

{{< code >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template-pkg.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}


（応用）ZIP パッケージ化も CloudFormation で行う
----

ここまでは、ローカルに Lambda 関数用の ZIP ファイルを作成しておく前提でしたが、実は __ZIP 化と S3 へのアップロードは `aws cloudformation package` コマンドでまとめて行うことができます__。

例えば、プロジェクトのディレクトリ構成が次のようになっていて、`src` 以下のファイル群を ZIP 化したいとします。

{{< code >}}
- template.yml
- src/
   +-- index.py
{{< /code >}}

SAM テンプレート内の `CodeUri` プロパティ で、ZIP ファイル名の代わりに __`src`__ ディレクトリを指定するようにします。
`Handler` プロパティは、`src` ディレクトリからの相対パスで指定します。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Resources:
  HelloFunction:
    Type: AWS::Serverless::Function
    Properties:
      Runtime: python3.7
      CodeUri: src/
      Handler: index.handler
{{< /code >}}

あとはこれまでと同様に、次のように `package` コマンドを実行します。

{{< code >}}
$ aws cloudformation package --template-file template.yml \
    --s3-bucket bucket-123456789012-functions \
    --output-template-file template-pkg.yml
{{< /code >}}

すると、`src` ディレクトリ内のファイル群が内部的に ZIP 化されて S3 バケットにアップロードされます（ローカルに ZIP ファイルは生成されません）。

`CodeUri` が S3 バケットの URI に書き換わったテンプレートが生成されるので、次のように `deploy` コマンドで CloudFormation スタックを生成（更新）すればデプロイ完了です。

{{< code >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template-pkg.yml \
    --capabilities CAPABILITY_IAM
{{< /code >}}

この方法の注意点としては、__`CodeUri` で指定したディレクトリ（ここでは `src`）の下のファイル群がすべて ZIP にパッケージングされてしまう__ という点です。
カスタマイズされた方法で ZIP パッケージを作成したい場合（特定のファイルを含めたくないときなど）は、この方法は使えないかもしれません。

{{< reference >}}
- [AWS Lambda にデプロイするための ZIP パッケージを npm で作成する (npm-pack-zip)](/p/zmydq3f)
{{< /reference >}}

