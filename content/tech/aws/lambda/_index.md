---
title: "AWS Lambda のメモ"
linkTitle: "Lambda"
url: "/p/eyeq2do"
weight: 202
---

{{% private %}}
- AWS SDK for Python (boto3) は、Python 3.6/3.7 用の Lambda 実行環境にプリインされています。
- SAM (Serverless Application Model) で Lambda を作成すると、その関数用の IAM ロールが自動生成されます。例えば、CloudWatch ログへの書き込みを許可するマネージドポリシーが自動でその IAM ロールに関連付けられます。そのほかのポリシーは、デフォルトでは設定されないので、CloudFormation テンプレート内の `Policies:` プロパティでカスタムポリシーとして設定しておく必要があります。
{{% /private %}}

