---
title: "AWS CloudFormation テンプレートの構成を理解する"
linkTitle: "CloudFormation テンプレートの構成を理解する"
url: "/p/iuhzcp3"
date: "2021-04-05"
tags: ["AWS", "AWS/CloudFormation"]
weight: 2
draft: true
---

{{< code lang="yaml" >}}
Resources:
  <論理ID>:
    Type: <リソースタイプ>
    Properties:
      <プロパティ...>
{{< /code >}}

- {{< label "論理ID (Logical ID)" >}} ... テンプレート内で一意の ID です
- {{< label "リソースタイプ" >}} ... 作成するリソースの種類です。例えば、S3 バケットリソースを作成したいときは `AWS::S3::Bucket` を指定します。
- {{< label "プロパティ..." >}} ... リソースごとの詳細設定を記述します。リソースタイプごとに指定すべきプロパティが異なります。

