---
title: "AWS CloudFormation/SAM/CDK のメモ"
linkTitle: "CloudFormation/SAM/CDK"
url: "/p/sdq2cnx"
weight: 210
---

{{% private %}}
- CloudFormation を使うと、複数のリージョンに対して同じリソースを簡単にプロビジョニングできるようになる。テンプレートファイル (.yml) を CloudFormation サービスに送信することで、CloudFormation のスタックが生成される。スタックは複数の AWS リソースから構成されており、スタック単位で更新や削除を行える（Azure のリソースグループに近い概念）。
- CloudFormation のテンプレート内に入力パラメーターを定義しておくと、スタックの作成時に任意の値を渡せるようになる。
- CloudFormation のスタックを作成・削除するには、個々の AWS リソースの作成・削除の権限がそのユーザーに与えられていなければいけない（当然ですが）。
{{% /private %}}

