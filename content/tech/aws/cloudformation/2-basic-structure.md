---
title: "AWS CloudFormation テンプレートの構成を理解する"
linkTitle: "CloudFormation テンプレートの構成を理解する"
url: "/p/iuhzcp3"
date: "2021-04-05"
tags: ["AWS", "AWS/CloudFormation"]
weight: 2
draft: true
---

パラメーターの定義
----

{{< code lang="yaml" >}}
Parameters:
  Demo:
    Type: Number
    Description: "This parameter is for demonstration"
    Default: 100  # デフォルト値が必要な場合
    NoEcho: true  # パスワードのとき
{{< /code >}}


リソースの定義
----

{{< code lang="yaml" >}}
Resources:
  <論理ID>:
    Type: <リソースタイプ>
    Properties:
      <プロパティ...>
{{< /code >}}

論理ID (Logical ID)
: テンプレート内で一意の ID です

リソースタイプ
: 作成するリソースの種類です。例えば、S3 バケットリソースを作成したいときは `AWS::S3::Bucket` を指定します。

プロパティ...
: リソースごとの詳細設定を記述します。リソースタイプごとに指定すべきプロパティが異なります。

リソースの定義では、リソースの種類 (`Type`) と、そのリソースに指定可能なプロパティ (`Properties`) を把握した上で設定する必要があります。

{{< code lang="yaml" title="例" >}}
Resources:
  VM:
    Type: "AWS::EC2::Instance"
    Properties:
      ImageId: "ami-3601ew1a"
      InstanceType: "t2.micro"
      ...
{{< /code >}}


出力の定義
----

{{< code lang="yaml" >}}
Outputs:
  ID:
    Value: !Ref Server
    Description: "ID of the EC2 instance"
  PublicName:
    Value: !GetAtt "Server.PublicDnsName"
    Description: "Public name of the EC2 insntance"
{{< /code >}}

出力の定義 (`Outputs`) では、次のような情報を並べていきます。

```
<名前>:
  Value: <値>
  Description: <説明文>
```

__`!Ref <名前>`__ という組み込み関数で、`Server` リソースの ID を参照しています。


テンプレート内で使える関数
----

### !Sub ... `${}` で囲まれた値を展開

> ```
> !Sub 'Your VPC ID: ${VPC}'
> !Sub '${VPC}'  # !Ref VPC と同様
> !Sub '${VPC.CidrBlock}'  # !GetAtt 'VPC.CidrBlock' と同様
> ```

### !Base64 ... Base64 エンコードする

> ```
> !Base64 'value'  #=> 'dmFsdWU='
> ```

