---
title: "Go 言語で AWS SDK を使う開発環境を整える"
url: "/p/"
date: "2022-04-11"
tags: ["AWS", "Go"]
---

{{% private %}}
- [AWS SDK for Go V2](https://aws.github.io/aws-sdk-go-v2/docs/)
- [sdk package - github.com/aws/aws-sdk-go-v2 - pkg.go.dev](https://pkg.go.dev/github.com/aws/aws-sdk-go-v2)
{{% /private %}}

何をするか？
----

ここでは、Go 言語用の AWS SDK V2 をセットアップして、各種 AWS サービス用の API を呼び出す準備をします。

Go 言語は Google が開発したプログラミング言語で、ライブラリインポートの手軽さや、生成された実行バイナリの実行速度に定評があるため、今後も利用ユーザーは増えていきそうです。

- 参考: [Go 言語とは？／Go をインストールする | まくまくHugo/Goノート](https://maku77.github.io/hugo/go/what-is-go.html)

Go の実行環境は [Go のインストーラー](https://go.dev/doc/install) で簡単にインストールできます。
これ以降の説明では、Go の実行環境はインストール済みであると想定しています。


テスト用の Go プロジェクトを作成する
----

Go プロジェクト用に適当なディレクトリを作成し、その中で __`go mod init`__ コマンドを実行して __`go.mod`__ （依存関係などが保存されるファイル）を作成しておきます。
このファイルが存在するディレクトリを、Go はモジュールとして認識します。

```console
$ mkdir aws-study
$ cd aws-study
$ go mod init aws-study
```

上記の例では、`go mod init` の引数（モジュールパス）を `aws-study` としましたが、GitHub リポジトリで管理する前提であれば、__`github.com/<user>/aws-study`__ のようなモジュールパスを指定するようにしてください。
これで、Go 言語プロジェクトの準備完了です。


AWS SDK でコンフィグ情報を参照してみる
----

AWS SDK を使って AWS のサービスにアクセスするには、認証情報として IAM ユーザーの「アクセスキー ID」や「シークレットアクセスキー」などが必要になります。
これらは、一般的には AWS CLI の `aws configure` コマンドによって、`~/.aws/credentials` や `~/.aws/config` に保存された情報です。

- 参考: [AWS の初期設定: AWS CLI と認証情報の設定](/p/j5iu7it)

AWS SDK にはこれらの設定ファイルから認証情報（クレデンシャル情報）を読み取る API が用意されています。
まず、__`go get`__ コマンドを実行して、設定情報を読み込むための [github.com/aws/aws-sdk-go-v2/config パッケージ](https://pkg.go.dev/github.com/aws/aws-sdk-go-v2/config) の依存情報を追加します。

```console
$ go get github.com/aws/aws-sdk-go-v2/config
```

`go get` コマンドを実行すると、パッケージの依存情報が記載された `go.mod` と `go.sum` ファイルが更新されるので、これらは忘れずに Git などにコミットするようにしてください。
下記のサンプルコードでは、AWS のコンフィグ情報を取得して、画面上に出力しています。

{{< code lang="go" title="main.go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-sdk-go-v2/config"
)

func main() {
	// 外部リソース（~/.aws/config など）からコンフィグ情報 (aws.Config) を生成
	cfg, err := config.LoadDefaultConfig(context.TODO())
	if err != nil {
		panic(fmt.Sprintf("failed loading config, %v", err))
	}

	// コンフィグ情報を表示
	fmt.Printf("Region = %s\n", cfg.Region)
	cred, err := cfg.Credentials.Retrieve(context.TODO())
	if err != nil {
		panic(fmt.Sprintf("failed retrieving credentials, %v", err))
	}
	fmt.Printf("AccessKeyID = %s\n", cred.AccessKeyID)
	fmt.Printf("SecretAccessKey = %s\n", cred.SecretAccessKey)
	fmt.Printf("SessionToken = %s\n", cred.SessionToken)
}
{{< /code >}}

次のように実行してアクセスキーの情報が表示されれば、各種 AWS サービスの API を呼び出す準備は整っています。

{{< code code="console" title="実行例" >}}
$ go run main.go
Region = ap-northeast-1
AccessKeyID = TAZASBDCD4ASIAVTXAP4
SecretAccessKey = S2VmkhmfeN8n9TXe7G2L1XAOVhK80jui/eNcdgna
SessionToken = Ub7oU0AhI0k4NIGo3J（省略）D9f3YiTZ2xyyqi44w=
{{< /code >}}

[config.LoadDefaultConfig 関数](https://pkg.go.dev/github.com/aws/aws-sdk-go-v2/config#LoadDefaultConfig) は、次のような外部リソースを情報源として AWS のコンフィグ情報 (`aws.Config`) を構築しています。
コンフィグ情報がうまく表示されないときは、これらの設定を確認すると原因が分かるかもしれません。

- 環境変数
  - `export AWS_REGION=ap-northeast-1`
  - `export AWS_ACCESS_KEY_ID=TAZASBDCD4ASIAVTXAP4`
  - `export AWS_SECRET_ACCESS_KEY=S2VmkhmfeN8n9TXe7G2L1XAOVhK80jui/eNcdgna`
  - `export AWS_SESSION_TOKEN=Ub7oU0AhI0k4NIGo3J（省略）D9f3YiTZ2xyyqi44w=`
  - `export AWS_PROFILE=default`
- Shared Credentials ファイル (`~/.aws/credentials`)
- Shared Configuration ファイル (`~/.aws/config`)


AWS サービスの API を呼び出してみる
----

{{% private %}}
- [AWS SDK for Go V2 のサンプルコード](https://aws.github.io/aws-sdk-go-v2/docs/code-examples/)
  - [S3 のサンプルコード](https://aws.github.io/aws-sdk-go-v2/docs/code-examples/s3/)
{{% /private %}}

コンフィグ情報 (`aws.Config`) を取得できるようになったら、あとは各 AWS サービス（S3、API Gateway、DynamoDB など）の API を呼び出すだけです。
例としては何でもよいのですが、ここでは、S3 バケットの情報を取得してみます。

各 AWS サービス用のパッケージは __`github.com/aws/aws-sdk-go-v2/service/サービス名`__ という名前で提供されているので、これを `go get` しておきます。

```console
$ go get github.com/aws/aws-sdk-go-v2/service/s3
```

下記のサンプルコードを実行すると、S3 バケットの一覧（作成日とバケット名）を出力します。

{{< code lang="go" title="main.go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-sdk-go-v2/aws"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/s3"
)

func main() {
	cfg := loadAwsConfig()
	output := listS3Buckets(cfg)

	for _, bucket := range output.Buckets {
		created := bucket.CreationDate.Format("2006-01-02 15:04:05 Mon")
		fmt.Println(created + ": " + *bucket.Name)
	}
	fmt.Printf("Found %d buckets\n", len(output.Buckets))
}

// 外部リソース（~/.aws/config など）からコンフィグ情報 (aws.Config) を生成します。
func loadAwsConfig() aws.Config {
	cfg, err := config.LoadDefaultConfig(context.TODO())
	if err != nil {
		panic(fmt.Sprintf("failed loading config, %v", err))
	}
	return cfg
}

// S3 バケットの一覧を取得します。
func listS3Buckets(cfg aws.Config) *s3.ListBucketsOutput {
	client := s3.NewFromConfig(cfg)
	input := &s3.ListBucketsInput{}
	output, err := client.ListBuckets(context.TODO(), input)
	if err != nil {
		panic(fmt.Sprintf("failed listing S3 buckets, %v", err))
	}
	return output
}
{{< /code >}}

{{< code lang="console" title="実行例" >}}
$ go run main.go
2021-12-24 00:59:06 Fri: my-sample-bucket-name-1
2022-04-08 05:15:52 Fri: my-sample-bucket-name-2
2022-04-04 08:47:23 Mon: my-sample-bucket-name-3
2020-07-10 06:54:33 Fri: my-sample-bucket-name-4
Found 4 buckets
{{< /code >}}

このサンプルコードでは S3 サービスの API を使用しましたが、他の AWS サービスに関しても同様に操作することができます。

