---
title: "AWS CDK メモ: 謎の CDKMetadata を生成しないようにする"
url: "/p/2asq4k4"
date: "2021-10-04"
tags: ["AWS", "AWS/CDK"]
weight: 1105
---

AWS CDK で CloudFormation スタックを生成すると、デフォルトで `CDKMetadata` というリソースが生成されます（`cdk synth` コマンドで CloudFormation テンプレートを出力してみると確認できます）。

{{< code lang="yaml" >}}
Resources:
  CDKMetadata:
    Type: AWS::CDK::Metadata
    Properties:
      Analytics: v2:deflate64:IsH4AAAAAAAA/L9ZNTs....9mAAAA
    Metadata:
      aws:cdk:path: CdkStack/CDKMetadata/Default
    Condition: CDKMetadataAvailable
Conditions:
  CDKMetadataAvailable:
    Fn::Or:
      - Fn::Or:
          - Fn::Equals:
              - Ref: AWS::Region
              - af-south-1
          - Fn::Equals:
              - Ref: AWS::Region
              - ap-east-1
          ...
{{< /code >}}

自分で何もリソース生成していないのにテンプレートが汚されて邪魔だなぁと思っていたら、この情報は、CDK を開発しているチームが利用状況などを把握して今後の改善のために使うようです。
下記、[CDK のドキュメント](https://docs.aws.amazon.com/ja_jp/cdk/latest/guide/hello_world.html#hello_world_tutorial_synth) より抜粋。

> Every generated template contains a AWS::CDK::Metadata resource by default. (We haven't shown it here.) The AWS CDK team uses this metadata to gain insight into how the AWS CDK is used, so we can continue to improve it. For details, including how to opt out of version reporting, see Version reporting.

気にならなければこのまま協力すればよいし、このデータが邪魔な場合は Opt-out でオフにすることができます。
CDK アプリのトップディレクトリにある __`cdk.json`__ で次のように __`versionReporting`__ を設定すれば、`CDKMetadata` は生成されなくなります。

{{< code lang="js" hl_lines="3" title="cdk.json" >}}
{
  "app": "npx ts-node --prefer-ts-exts bin/my-app.ts",
  "versionReporting": false,
  "context": {
    // ...
  }
}
{{< /code >}}

{{< code >}}
$ cdk synth
{}
{{< /code >}}

スッキリ！カラッポ！

というか、デフォルトはこっちにしておいて欲しいですね。
CloudFormation ってただでさえ読みにくいのに、デフォルトでデータを突っ込んでくる AWS 開発者たちよ。。。

