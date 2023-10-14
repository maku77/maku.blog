---
title: "AWS CloudFormation で DyanamoDB のリソースを作成する"
linkTitle: "CloudFormation で DyanamoDB のリソースを作成する"
url: "p/h3gsjs2/"
date: "2021-04-07"
tags: ["AWS", "AWS/DynamoDB", "AWS/SAM"]
weight: 350
---

SAM で簡単な DynamoDB テーブルを生成してみる
---

DynamoDB のテーブルリソースも、Lambda 関数などのリソースと同様に AWS SAM で自動生成＆更新することができます。
CloudFormation のテンプレートをそのまま記述するより、拡張された SAM テンプレートの形式で記述することで、シンプルにリソースを定義することができます。

SAM テンプレート内で DynamoDB のテーブルを定義するときは、リソースタイプとして __`AWS::Serverless::SimpleTable`__ を指定します（CloudFormation スタック内に実際に生成されるリソースのタイプは `AWS::DynamoDB::Table` になります）。

DynamoDB テーブルを作成するための最低限の SAM テンプレートはとてもシンプルです。
次の例では、`MyTable` という 論理 ID (Logical ID) で DynamoDB のテーブルを定義しています。
論理 ID はスタック内でリソースを特定するための名前です。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: My sample app
Resources:
  MyTable:
    Type: AWS::Serverless::SimpleTable
{{< /code >}}

ひとつもプライマリキーを指定していませんが、その場合はデフォルトで `id` という名前のプライマリキー（`String` 型）が定義されます。
[AWS CLI](/p/zkxamw9) で次のように実行すると、CloudFormation のスタックを作成することができます。

{{< code >}}
$ aws cloudformation deploy --stack-name mystack \
    --template-file template.yml
{{< /code >}}

CloudFormation スタック内に、実際にどのような AWS リソースが作成されたかを調べるには以下のようにします。
ここではリソースタイプと、その物理 ID (Pysical ID) を table 形式で出力してみました。

{{< code >}}
$ aws cloudformation describe-stack-resources --stack-name mystack \
    --query 'StackResources[].[ResourceType,PhysicalResourceId]' \
    --output table
----------------------------------------------------------
|                 DescribeStackResources                 |
+-----------------------+--------------------------------+
|  AWS::DynamoDB::Table |  mystack-MyTable-ABD467NSG1D3  |
+-----------------------+--------------------------------+
{{< /code >}}

SAM テンプレート内では、DynamoDB テーブルの論理 ID (Logical ID) を `MyTable` と定義しましたが、実際のテーブル名（物理 ID）は `mystack-MyTable-ABD467NSG1D3` のように、スタック名と論理 ID、ハッシュ値を組み合わせたものから自動生成されていることが分かります。
この物理 ID が、DynamoDB のマネージメントコンソールなどで表示されるテーブル名になります。
もちろん、テーブル名は SAM テンプレート内で明示的に指定することができます（後述）。


DynamoDB テーブル用のプロパティ設定
----

SAM の `AWS::Serverless::SimpleTable` リソースのプロパティ設定には以下のようなものがあります。

### TableName ... テーブル名（物理 ID）

デフォルトでは、DynamoDB のテーブル名 (Physical ID) は CloudFormation のスタック名と Logical ID から自動生成されますが、__`TableName`__ プロパティで明示的に設定できます。

{{< code lang="yaml" >}}
Resources:
  BooksTable:
    Type: AWS::Serverless::SimpleTable
    Properties:
      TableName: myapp-table-books
{{< /code >}}

### PrimaryKey ... プライマリキー

テーブルのプライマリキーは __`PrimaryKey`__ プロパティで指定します（属性名を `Name`、属性タイプを `Type` で指定します）。
属性タイプは `String`、`Number`、`Binary` のいずれかの値を指定します（プライマリキー以外の属性には、他にも `Boolean` などの属性タイプがありますが、プライマリキーの属性タイプは 3 種類だけです）。
次の例では、プライマリキーとして、`Title` という名前の `String` 型属性を定義しています。

{{< code lang="yaml" >}}
Resources:
  BooksTable:
    Type: AWS::Serverless::SimpleTable
    Properties:
      PrimaryKey:
        Name: Title
        Type: String
{{< /code >}}

### Tags ... タグの配列

DynamoDB テーブルにタグを設定したいときは、__`Tags`__ プロパティにマップ形式（キー＆バリュー）で指定します。

{{< code lang="yaml" >}}
Resources:
  BooksTable:
    Type: AWS::Serverless::SimpleTable
    Properties:
      Tags:
        Project: OnlineShop
        Department: Engineering
{{< /code >}}


その他の情報源
----

{{% maku-common/reference %}}
- [AWS::Serverless::SimpleTable - AWS Serverless Application Model](https://docs.aws.amazon.com/ja_jp/serverless-application-model/latest/developerguide/sam-resource-simpletable.html)
- [AWS::DynamoDB::Table - AWS CloudFormation](https://docs.aws.amazon.com/ja_jp/AWSCloudFormation/latest/UserGuide/aws-resource-dynamodb-table.html)
{{% /maku-common/reference %}}

