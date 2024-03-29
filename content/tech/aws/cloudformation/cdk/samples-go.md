---
title: "AWS CDK のサンプルコード集（Go 言語版）"
url: "/p/r47yngk"
date: "2022-04-13"
tags: ["AWS", "AWS/CDK"]
draft: true
weight: 1901
---

{{% private %}}
- [Go で IaC してみる / AWS CDK・CDK for Terraform・Pulumi - Techtouch Developers Blog](https://tech.techtouch.jp/entry/go-iac)
- https://techdo.mediado.jp/entry/2021/05/19/110000
{{% /private %}}


AWS CDK V2 を Go 言語から使用するときのサンプルコード集です。
AWS CDK V2 for Go の導入については下記ページを参照してください。

- [Go 言語で AWS CDK V2 を使う (1) 導入編](/p/54s6es8)


Lambda
----

```
- main.go        # IaC code
- handler/
    +-- main.go  # Lambda function
```

{{< code lang="console" title="パッケージ追加" >}}
$ go get github.com/aws/aws-cdk-go/awscdk/v2/awslambda
$ go get github.com/aws/aws-cdk-go/awscdk/v2/awss3assets
{{< /code >}}

{{< code lang="go" title="myapp.go" >}}
package main

import (
	"github.com/aws/aws-cdk-go/awscdk/v2"
	"github.com/aws/aws-cdk-go/awscdk/v2/awslambda"
	"github.com/aws/aws-cdk-go/awscdk/v2/awss3assets"
	"github.com/aws/constructs-go/constructs/v10"
	"github.com/aws/jsii-runtime-go"
)

func createStack(scope constructs.Construct) {
	// スタックを作成する
	stack := awscdk.NewStack(scope, jsii.String("CdkWithGoStack"), &awscdk.StackProps{})

	// Lambda 関数を作成する
	awslambda.NewFunction(stack, jsii.String("MyLambda"), &awslambda.FunctionProps{
		Runtime: awslambda.Runtime_GO_1_X(),
		Code:    awslambda.AssetCode_FromAsset(jsii.String("lambda"), &awss3assets.AssetOptions{}),
		Handler: jsii.String("main"),
	})
}

func main() {
	app := awscdk.NewApp(nil)
	createStack(app)
	app.Synth(nil)
}
{{< /code >}}


API Gateway
----

- [API Gateway Construct Library](https://docs.aws.amazon.com/cdk/api/v2/docs/aws-cdk-lib.aws_apigateway-readme.html)
  - [Go - github.com/aws/aws-cdk-go/awscdk/v2/awsapigateway](https://pkg.go.dev/github.com/aws/aws-cdk-go/awscdk/v2/awsapigateway)
- [Api Gateway V2 Construct Library](https://docs.aws.amazon.com/cdk/api/v2/docs/aws-cdk-lib.aws_apigatewayv2-readme.html)
  - [Go - github.com/aws/aws-cdk-go/awscdk/v2/awsapigatewayv2](https://pkg.go.dev/github.com/aws/aws-cdk-go/awscdk/v2/awsapigatewayv2)

{{< code lang="console" title="パッケージ追加" >}}
$ go get github.com/aws/aws-cdk-go/awscdk/v2/awsapigateway
{{< /code >}}


