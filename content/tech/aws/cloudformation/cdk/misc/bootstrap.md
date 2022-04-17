---
title: "AWS CDK メモ: ブートストラップ処理を実行する (cdk bootstrap)"
linkTitle: ""
url: "/p/q7q8p6m"
date: "2022-04-17"
tags: ["AWS", "AWS/CDK"]
weight: 1101
---

CloudFormation のスタックを生成するときに、一時的なファイル置き場として S3 バケットが必要になることがあります。
例えば、Lambda 関数をデプロイするときに、ZIP パッケージを置くためのステージングバケットが必要になります。
このステージングバケットへの実際のアップロード処理は CDK が自動でやってくれるのですが、バケットの準備だけはあらかじめ手動で行っておく必要があります。
このためのコマンドが __`cdk bootstrap`__ コマンドです。
AWS アカウント（およびリージョン）内で一度だけ実行しておけばよいので、このタイミングで実行しておきます。
実行時には次のように「アカウント番号」と「リージョン名」を指定する必要があります。

```
cdk bootstrap aws://<アカウント番号>/<リージョン名>
```

{{< code lang="console" title="実際の実行例" >}}
$ cdk bootstrap aws://123456789012/ap-northeast-1
 ⏳  Bootstrapping environment aws://123456789012/ap-northeast-1...
CDKToolkit: creating CloudFormation changeset...
...
 ✅  Environment aws://123456789012/ap-northeast-1 bootstrapped.
{{< /code >}}

これにより、ステージング用のバケットを含む `CDKToolkit` という名前のブートストラップ・スタックが生成されます。

ちなみに、現在使用している「アカウント番号」と「リージョン名」は、AWS CLI を使って次のように確認できます。

{{< code lang="console" title="アカウント番号の確認" >}}
$ aws sts get-caller-identity --output text --query Account [--profile xxxx]
123456789012
{{< /code >}}

{{< code lang="console" title="リージョン名の確認" >}}
$ aws configure get region [--profile xxxx]
ap-northeast-1
{{< /code >}}

