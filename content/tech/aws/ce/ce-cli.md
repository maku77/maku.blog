---
title: "AWS のコスト情報をコマンドライン (CLI) で取得する"
url: "/p/ct4ckt3"
date: "2021-02-23"
tags: ["AWS"]
---

{{< code >}}
aws ce get-cost-and-usage ...
{{< /code >}}

次の例では、指定した期間の AWS 利用料金を求めています。

{{< code >}}
aws ce get-cost-and-usage \
    --granularity MONTHLY \
    --time-period Start=2021-01-01,End=2021-02-01 \
    --metrics BlendedCost
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="yaml" >}}
DimensionValueAttributes: []
ResultsByTime:
- Estimated: false
  Groups: []
  TimePeriod:
    End: '2021-02-01'
    Start: '2021-01-01'
  Total:
    BlendedCost:
      Amount: '0.0900932477'
      Unit: USD
{{< /code >}}
{{< /accordion >}}

次の例では、サービスごとに料金表示しています。

{{< code >}}
aws ce get-cost-and-usage \
    --granularity MONTHLY \
    --time-period Start=2021-01-01,End=2021-02-01 \
    --metrics BlendedCost \
    --group-by Type=DIMENSION,Key=SERVICE
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="yaml" >}}
DimensionValueAttributes: []
GroupDefinitions:
- Key: SERVICE
  Type: DIMENSION
ResultsByTime:
- Estimated: false
  Groups:
  - Keys:
    - AWS CloudShell
    Metrics:
      BlendedCost:
        Amount: '0.0000032477'
        Unit: USD
  - Keys:
    - AWS CodeCommit
    Metrics:
      BlendedCost:
        Amount: '0'
        Unit: USD
  - Keys:
    - AWS Key Management Service
    Metrics:
      BlendedCost:
        Amount: '0'
        Unit: USD
  - Keys:
    - AWS Lambda
    Metrics:
      BlendedCost:
        Amount: '0'
        Unit: USD
  - Keys:
    - Amazon Simple Notification Service
    Metrics:
      BlendedCost:
        Amount: '0'
        Unit: USD
  - Keys:
    - Amazon Simple Storage Service
    Metrics:
      BlendedCost:
        Amount: '0.00009'
        Unit: USD
  - Keys:
    - AmazonCloudWatch
    Metrics:
      BlendedCost:
        Amount: '0'
        Unit: USD
  - Keys:
    - CodeBuild
    Metrics:
      BlendedCost:
        Amount: '0.08'
        Unit: USD
  - Keys:
    - Tax
    Metrics:
      BlendedCost:
        Amount: '0.01'
        Unit: USD
  TimePeriod:
    End: '2021-02-01'
    Start: '2021-01-01'
  Total: {}
{{< /code >}}
{{< /accordion >}}

サービスごとにコストを一行でシンプルに表示します。

{{< code >}}
$ aws ce get-cost-and-usage \
    --granularity MONTHLY \
    --time-period Start=2021-01-01,End=2021-02-01
    --metrics BlendedCost \
    --group-by Type=DIMENSION,Key=SERVICE \
    --query 'ResultsByTime[0].Groups[*].[Keys[0], Metrics.BlendedCost.Amount]' \
    --output=text
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code lang="yaml" >}}
AWS CloudShell  0.0000032477
AWS CodeCommit  0
AWS Key Management Service      0
AWS Lambda      0
Amazon Simple Notification Service      0
Amazon Simple Storage Service   0.00009
AmazonCloudWatch        0
CodeBuild       0.08
Tax     0.01
{{< /code >}}
{{< /accordion >}}

