---
title: "Go 言語で AWS CDK V2 を使う (1) 導入編"
url: "/p/54s6es8"
date: "2022-04-13"
tags: ["AWS", "AWS/CDK"]
---

何をするか？
----

CDK プロジェクトでは多くのケースでは TypeScript を使ってコード記述されていると思いますが、Go 言語の勢いが出てきていることもあり、ここでは Go 言語を使った CDK プロジェクトを作成してみます。

CDK 自体の概要については下記に簡単にまとまっています。

- 参考: [AWS CDK 入門 (1) インストールから Hello World まで](/p/nujyfsy)

以下、Go 言語のインストールや、AWS の認証情報の設定 (`~/.aws/credentials`, `~/.aws/config`) はできているものとします。

- 参考: [Go 言語で AWS SDK を使う開発環境を整える](/p/xnogqgm)


AWS CDK のインストール
----

Go 言語で CDK のコードを記述する場合でも、AWS CDK のコマンドラインツール (`cdk`) 自体は、Node.js の NPM パッケージで提供されているものを使います。

```console
$ npm install -g aws-cdk
...

$ cdk --version
2.20.0 (build 738ef49)
```


CDK プロジェクトを作成する (cdk init)
----

CDK の Scaffold 機能を使って、Go 言語用の CDK プロジェクトを生成します。

```console
$ mkdir cdk-with-go
$ cd cdk-with-go
$ cdk init --language=go
```

`cdk init` すると、次のようなファイルが生成されます。
TypeScript 用のプロジェクトと比べてとってもシンプルです！

```
- README.md            # GitHub リポジトリのトップページ
- .gitignore           # CDK と Go 言語用の .gitignore
- cdk-with-go.go       # スタック生成コードのエントリポイント
- cdk-with-go_test.go  # 上記のテストコード
- cdk.json             # CDK 用の設定
- go.mod               # Go モジュールの定義（CDK パッケージの依存情報など）
```

依存モジュールをダウンロードして、不要な依存関係を削除しておきます。

```console
$ go mod tidy
```


CloudFormation にスタックを生成する、削除する (cdk deploy, cdk destroy)
----

自動生成されたスタック定義用の Go コードを見ると、次のような感じになっています。
初期状態では、空っぽの `CdkWithGoStack` スタックを作るようになっています。

{{< accordion title="cdk-with-go.go （自動生成されたコード）" >}}
{{< code lang="go" >}}
package main

import (
	"github.com/aws/aws-cdk-go/awscdk/v2"
	// "github.com/aws/aws-cdk-go/awscdk/v2/awssqs"
	"github.com/aws/constructs-go/constructs/v10"
	// "github.com/aws/jsii-runtime-go"
)

type CdkWithGoStackProps struct {
	awscdk.StackProps
}

func NewCdkWithGoStack(scope constructs.Construct, id string, props *CdkWithGoStackProps) awscdk.Stack {
	var sprops awscdk.StackProps
	if props != nil {
		sprops = props.StackProps
	}
	stack := awscdk.NewStack(scope, &id, &sprops)

	// The code that defines your stack goes here

	// example resource
	// queue := awssqs.NewQueue(stack, jsii.String("CdkWithGoQueue"), &awssqs.QueueProps{
	// 	VisibilityTimeout: awscdk.Duration_Seconds(jsii.Number(300)),
	// })

	return stack
}

func main() {
	app := awscdk.NewApp(nil)

	NewCdkWithGoStack(app, "CdkWithGoStack", &CdkWithGoStackProps{
		awscdk.StackProps{
			Env: env(),
		},
	})

	app.Synth(nil)
}

// env determines the AWS environment (account+region) in which our stack is to
// be deployed. For more information see: https://docs.aws.amazon.com/cdk/latest/guide/environments.html
func env() *awscdk.Environment {
	// If unspecified, this stack will be "environment-agnostic".
	// Account/Region-dependent features and context lookups will not work, but a
	// single synthesized template can be deployed anywhere.
	//---------------------------------------------------------------------------
	return nil

	// Uncomment if you know exactly what account and region you want to deploy
	// the stack to. This is the recommendation for production stacks.
	//---------------------------------------------------------------------------
	// return &awscdk.Environment{
	//  Account: jsii.String("123456789012"),
	//  Region:  jsii.String("us-east-1"),
	// }

	// Uncomment to specialize this stack for the AWS Account and Region that are
	// implied by the current CLI configuration. This is recommended for dev
	// stacks.
	//---------------------------------------------------------------------------
	// return &awscdk.Environment{
	//  Account: jsii.String(os.Getenv("CDK_DEFAULT_ACCOUNT")),
	//  Region:  jsii.String(os.Getenv("CDK_DEFAULT_REGION")),
	// }
}
{{< /code >}}
{{< /accordion >}}

ここでは、このコードはそのまま使わずに、次のような最小限のコードを最初のステップとして使います。

{{< code lang="go" title="cdk-with-go.go" >}}
package main

import (
	"github.com/aws/aws-cdk-go/awscdk/v2"
	"github.com/aws/jsii-runtime-go"
)

func main() {
	app := awscdk.NewApp(nil)
	awscdk.NewStack(app, jsii.String("CdkWithGoStack"), &awscdk.StackProps{})
	app.Synth(nil)
}
{{< /code >}}

やっていることは単純で、空っぽのスタック (`CdkWithGoStack`) を作成しているだけです。
`jsii.String` というのは、文字列リテラルを `String*` 型の引数にそのまま渡すためのユーティリティです（これがないと文字列変数をいちいち作らないといけない...）。

この定義に従って CloudFormation へデプロイを行うには、__`cdk deploy`__ コマンドを実行します。

```console
$ go mod tidy  # 必要に応じて Go モジュールの依存解決
$ cdk deploy
```

CloudFormation スタックの生成処理に 1 分くらいかかるのでしばらく待ちます。
コマンドの実行が完了してから AWS コンソールを覗いてみると、実際に `CdkWithGoStack` というスタックが生成されていることを確認できます。

{{< image border="true" src="img-001.png"  title="AWS コンソール上の表示" >}}

このお試し作業が終わったら、__`cdk destory`__ で作成したスタックを削除しておきます。

```console
$ cdk destroy
Are you sure you want to delete: CdkWithGoStack (y/n)? y
```


スタックに AWS リソースを追加する
----

ここでは、例としてスタック上に S3 バケットを作成してみます。
S3 を扱うパッケージを __`go get`__ で追加します。
最後に `go mod tidy` でまとめて依存解決する方法もありますが、コーディング時に入力補完するためには先に `go get` しておく必要があります。

```console
$ go get github.com/aws/aws-cdk-go/awscdk/v2/awss3
```

- 参考: [AWS CDK V2 の API ドキュメント](https://docs.aws.amazon.com/cdk/api/v2/docs/aws-cdk-lib.aws_s3-readme.html)
- 参考: [Go 言語用の awss3.NewBucket 関数](https://pkg.go.dev/github.com/aws/aws-cdk-go/awscdk/v2/awss3#NewBucket)

スタック生成用のコードを書き換えて、スタック上に S3 バケット (`MyBucket`) を追加します。

{{< code lang="go" title="cdk-with-go.go" >}}
package main

import (
	"github.com/aws/aws-cdk-go/awscdk/v2"
	"github.com/aws/aws-cdk-go/awscdk/v2/awss3"
	"github.com/aws/jsii-runtime-go"
)

func main() {
	app := awscdk.NewApp(nil)
	stack := awscdk.NewStack(app, jsii.String("CdkWithGoStack"), &awscdk.StackProps{})

	// スタック内に S3 バケットを生成
	awss3.NewBucket(stack, jsii.String("MyBucket"), &awss3.BucketProps{})

	app.Synth(nil)
}
{{< /code >}}

あとは、次のようにデプロイすれば S3 バケットを持つ CloudFormation スタックが生成されます。

```console
$ cdk deploy
```

他の AWS リソースに関しても、同様にスタックに追加していくことができます。

