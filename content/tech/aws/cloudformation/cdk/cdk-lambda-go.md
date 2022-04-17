---
title: "AWS CDK で Go 言語で実装した Lambda 関数をデプロイする (aws-lambda-go)"
url: "/p/38jt3cm"
date: "2022-04-17"
tags: ["AWS", "AWS/CDK"]
weight: 1501
---

{{% private %}}
- Lambda 実装用の Go SDK
  - [Developer Guide | AWS SDK for Go V2](https://aws.github.io/aws-sdk-go-v2/docs/)
  - [aws/aws-sdk-go-v2: AWS SDK for the Go programming language.](https://github.com/aws/aws-sdk-go-v2)
  - [Go による Lambda 関数の構築 - AWS Lambda](https://docs.aws.amazon.com/ja_jp/lambda/latest/dg/lambda-golang.html)
- CDK
  - [@aws-cdk/aws-lambda-go-alpha コンストラクトパッケージ](https://docs.aws.amazon.com/cdk/api/v2/docs/aws-lambda-go-alpha-readme.html)
{{% /private %}}


何をするか？
----

ここでは、CDK アプリとして作成した CloudFormation スタック内に、__Go 言語で実装した Lambda 関数__ を追加してみます。
Lambda 関数のビルドもデプロイ時に自動で行われるようにします。

CDK のプロジェクト自体（スタックを定義する CDK コード）は TypeScript で作成します。
以降の説明では、CDK プロジェクトの作成自体は済んでいるものとします。

- 参考: [AWS CDK 入門: cdk コマンドのインストールから Hello World まで](/p/nujyfsy)


Go 言語で Lambda 関数を実装する
----

Lambda 関数のコードは、プロジェクトのルートに `lambda` ディレクトリを作成して、そこに配置していくことにします。

{{< code hl_lines="3-6" >}}
myapp/
  +-- bin/          ... CDK の App コンストラクト (.ts)
  +-- lambda/       ... ラムダ関数用のディレクトリ (.go)
  |     +-- go.mod
  |     +-- go.sum
  |     +-- main.go
  +-- lib/          ... CDK の Stack コンストラクトなど (.ts)
  ...
{{< /code >}}

`lambda` ディレクトリの下で、Go パッケージの依存情報 (`go.mod`、`go.sum`) を追加します。

```console
$ mkdir lambda
$ cd lambda
$ go mod init lambda
$ go get github.com/aws/aws-lambda-go/lambda
```

{{% private %}}
SDK V2 なら

```
$ go get github.com/aws/aws-sdk-go-v2/service/lambda
```
{{% /private %}}

Hello World 的な Lambda 関数を実装します。
次の Lambda 関数は、`name` 情報をイベントとして受け取り、挨拶テキストをレスポンスとして返します。

{{< code lang="go" title="lambda/main.go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-lambda-go/lambda"
)

// Lambda 関数への入力イベントの型
type MyEvent struct {
	Name string `json:"name"`
}

// Lambda 関数からのレスポンスの型
type MyResponse struct {
	Message    string `json:"message"`
	StatusCode int    `json:"statusCode"`
}

// Lambda 関数のエントリポイント
func handle(ctx context.Context, evt MyEvent) (MyResponse, error) {
	res := MyResponse{
		Message:    fmt.Sprintf("Hello %s!", evt.Name),
		StatusCode: 200,
	}
	return res, nil
}

// Go 言語での Lambda 関数実装にはこれが必要
func main() {
	lambda.Start(handle)
}
{{< /code >}}

この時点で次のようにフォーマットやビルドを行えますが、後述の `cdk deploy` でビルドは自動で行われるようになるため、ここでビルドしておく必要はありません。

```console
$ go fmt            # Go コードのフォーマット
$ go build -o main  # Go コードのビルド
```


Lambda 関数を含むスタックの定義とデプロイ
----

### @aws-cdk/aws-lambda-go パッケージ

AWS CDK が提供する __GoFunction コンストラクト__ クラスを使用すると、Go 言語で実装した Lambda 関数を簡単にデプロイできます。

- [CDK V2 用の @aws-cdk/aws-lambda-go-alpha](https://docs.aws.amazon.com/cdk/api/v2/docs/aws-lambda-go-alpha-readme.html)
- [CDK V1 用の @aws-cdk/aws-lambda-go](https://docs.aws.amazon.com/cdk/api/v1/docs/aws-lambda-go-readme.html)

コンストラクトパッケージは次のようにインストールしますが、使用している CDK 本体のバージョン（V2 あるいは V1）に合わせたパッケージをインストールしてください。
まだ V2 用のパッケージには `-alpha` サフィックスが付いているようです（2022 年 4 月現在）。


```console
$ npm install @aws-cdk/aws-lambda-go-alpha  # npm の場合
$ yarn add @aws-cdk/aws-lambda-go-alpha     # yarn の場合
```

Lambda 関数用の汎用的なコンストラクトとしては、`@aws-cdk/aws-lambda パッケージ` が提供する `Function` コンストラクトもあるのですが、`GoFunction` コンストラクトは Go 言語に特化しており、デプロイ時に Lambda 関数のビルド (`go build`) を自動で行ってくれます。


### スタックの定義

CDK アプリのひな型として、CloudFormation スタックを構築するためのコンストラクト (`lib/myapp-stack.ts`) が生成されているはずなので、そのスタック内に、`GoFunction` コンストラクトを生成するよう記述します。

{{< code lang="ts" title="lib/myapp-stack.ts" >}}
import { Stack, StackProps } from "aws-cdk-lib"
import { Construct } from "constructs"
import * as lambda from "@aws-cdk/aws-lambda-go-alpha"

export class GoLambdaAppStack extends Stack {
  constructor(scope: Construct, id: string, props?: StackProps) {
    super(scope, id, props)

    // このスタックに Lambda 関数を追加する
    new lambda.GoFunction(this, "MyLambda", {
      entry: "lambda",
    })
  }
}
{{< /code >}}

最小構成では、上記のように `entry` プロパティに Lambda 関数のディレクトリ名を指定するだけで OK です。
このディレクトリで `go build` によるビルドが行われるようになります。


### デプロイとテスト

スタックの定義が済んだら、__`cdk deploy`__ を実行して Lambda 関数をデプロイすることができます。
Lambda 関数のビルド (`go build`) も自動で行われます。

{{< code >}}
$ npm run cdk -- deploy
{{< /code >}}

デプロイが完了したら、[CloudFormation コンソール](https://console.aws.amazon.com/cloudformation/) を開いて、実際にスタックと Lambda 関数が生成されているか確認してください。
次のように AWS CLI を使って関数を呼び出すことができます。

```console
$ FUNC_NAME=MyappStack-MyLambdaCCE802FB-SRU9ijhbiKZ1
$ echo '{ "name": "Maku" }' > event.json

$ aws lambda invoke --function-name $FUNC_NAME --payload fileb://event.json output.txt
ExecutedVersion: $LATEST
StatusCode: 200

$ cat output.txt
{"message":"Hello Maku!","statusCode":200}
```

- 参考: [AWS CDK メモ: Lambda 関数コードだけ高速デプロイする (cdk deploy --hotswap)](/p/ap8p7n4)

