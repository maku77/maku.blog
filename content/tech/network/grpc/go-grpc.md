---
title: "Go 言語で gRPC 通信してみる（Echo サーバー＆クライアント）"
url: "p/ij4jv9k/"
date: "2022-05-18"
tags: ["Go", "gRPC", "Protocol Buffers"]
---

何をするか？
----

ここでは、Go 言語用の gRPC ライブラリである [gRPC-Go (google.golang.org/grpc)](https://pkg.go.dev/google.golang.org/grpc) を使って、簡単な gRPC サーバーとクライアントを作ってみます。
通信用のスタブコードなどは、__`protoc`__ コマンド (Protocl Buffers Compiler) で `.proto` ファイルから自動生成するので、あらかじめ `protoc` コマンドをインストールしておいてください。

- 参考: [protoc コマンドで .proto ファイルをコンパイルする (Protocol Buffers Compiler)](/p/37e6uck/)

`protoc` コマンドで Go 言語用のコードを生成するには、__`protoc-gen-go`__ プラグインと __`protoc-gen-go-grpc`__ プラグインをインストールしておく必要があります。
前者がシリアライズ用のコード、後者が gRPC 用のスタブコードを生成するための `protoc` プラグインです。

```console
# バージョンを指定してインストールする方法（推奨）
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28
$ go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@v1.2

# 最新バージョンをインストールする方法
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
$ go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest
```


プロジェクトの作成と gRPC-Go のインストール
----

まずは Go 言語用のプロジェクトを作成します。
モジュール名は `com.example/grpc-sample` としていますが、GitHub で管理する予定であれば、リポジトリ名に合わせて `github.com/<USER>/grpc-sample` のような名前にしてください。
これが、プロジェクト内で作成する Go パッケージをインポートするときのプレフィックスになります。

```console
$ mkdir grpc-sample && cd grpc-sample
$ go mod init com.example/grpc-sample
```

あとは、gRPC サーバーとクライアントの実装に使用する gRPC-Go パッケージの依存関係を追加しておきます。

```console
$ go get google.golang.org/grpc
```


.proto ファイルを作成する
----

`proto/echo/echo.proto` ファイルを作成して、次のように記述します。
Go 言語用のオプション `option go_package` で、出力する `.go` ファイルを `echo` パッケージに配置するように指定しています。

{{< code lang="proto" title="proto/echo/echo.proto" >}}
syntax = "proto3";

package echo;

option go_package = "example.com/grpc-sample/echo";

// Echo メソッドを持つ EchoService の定義
service EchoService {
  rpc Echo (EchoRequest) returns (EchoResponse);
}

// Echo に送るリクエストメッセージの定義
message EchoRequest {
  string message = 1;
}

// Echo が返すレスポンスメッセージの定義
message EchoResponse {
  string message = 1;
}
{{< /code >}}

ここでは、`EchoService` というサービスが、`Echo` というメソッドを提供するよう定義しています。

- 参考: [.proto の文法: サービス型 (service)](/p/napwb4e/)


.proto ファイルをコンパイルする
----

`protoc` コマンドを実行して、`.proto` ファイルからシリアライズ用のコードと、gRPC 関連のスタブコードを生成します。

```
$ protoc --go_out=. \
         --go_opt=paths=source_relative \
         --go-grpc_out=. \
         --go-grpc_opt=paths=source_relative \
         --proto_path=proto \
         proto/echo/*.proto
```

それぞれのオプションは次のような意味があります。

- __`--go_out`__ ... `protoc-gen-go` プラグインによる生成コードの出力先ディレクトリ
- __`--go_opt`__ ... `protoc-gen-go` プラグインに渡すオプション
- __`--go-grpc_out`__ ... `protoc-gen-go-grpc` プラグインによる生成コードの出力先ディレクトリ
- __`--go-grpc_opt`__ ... `protoc-gen-go-grpc` プラグインに渡すオプション
- __`--proto_path`__ ... `.proto` ファイル内で別の `.proto` ファイルをインポートしようとする場合、ここで指定したディレクトリからの相対パスで指定することになります。また、出力ファイル (`.pg.go`) のディレクトリ構造を入力ファイル (`.proto`) のディレクトリ構造に合わせるとき（後述）、ここで指定したパスを取り除いた階層構造で出力されます。

__`--go_out`__ オプションを指定することでシリアライズ用のコード (`echo.pb.go`) を生成、__`--go-grpc_out`__ オプションを指定することで gRPC 用のスタブコード (`echo_grpc.pb.go`) を生成してくれます。
追加のオプションで、__`paths=source_relative`__ を指定することにより、入力ファイル (`.proto`) と同じディレクトリ構成で `.go` ファイルを出力するようにしています。

さらに、__`--proto_path=proto`__ というオプションで、`.proto` ファイル群のルートディレクトリを指定しています。
これにより、出力先のディレクトリには `proto` ではなく `echo` ディレクトリが生成されるようになります。

今回はカレントディレクトリ (`.`) を出力のルートに指定しているので、結果的に次のように `.go` ファイルが生成されることになります。

| 入力ファイル | 使う protoc プラグイン | 生成されるファイル |
| ---- | ---- | ---- |
| `proto/echo/echo.proto` | `protoc-gen-go` | __`echo/echo.pb.go`__ |
| `proto/echo/echo.proto` | `protoc-gen-go-grpc` | __`echo/echo_grpc.pb.go`__ |

生成された `echo.pb.go` ファイルや `echo_grpc.pb.go` ファイルを覗いてみると、次のようなパッケージ名で定義されていることがわかります。

{{< code lang="go" title="echo/echo.pb.go（抜粋）" >}}
package echo
{{< /code >}}

このパッケージ名は、`.proto` ファイル内の `options go_package` で指定したパスに従って自動生成されています（パスの最後の `/echo` という部分が採用されています）。
また、今回はモジュール名を `example.com/grpc-sample` と定義したので（`go.mod` ファイルに書かれているので）、自動生成されたこれらのパッケージをインポートするときは、次のような感じで記述することになります。

```go
import "example.com/grpc-sample/echo"
```

生成された `echo/echo_grpc.pb.go` ファイルを覗いてみると、次のようなクライアント実装用の `EchoServiceClient` インタフェースや、サーバー実装用の `EchoServiceServer` インタフェースが生成されていることが分かります。

{{< code lang="go" title="echo/echo_grpc.pb.go" >}}
type EchoServiceClient interface {
	Echo(ctx context.Context, in *EchoRequest, opts ...grpc.CallOption) (*EchoResponse, error)
	// ...
}

type EchoServiceServer interface {
	Echo(context.Context, *EchoRequest) (*EchoResponse, error)
	// ...
}

// ...（省略）...
{{< /code >}}

クライアントスタブに関しては、実装も提供されているので、`NewEchoServiceClient` 関数でそのままインスタンス化して gRPC の API を呼び出すことができます。
サーバー側は各 API の実装を行う必要があります。


gRPC サーバーとクライアントの実装
----

gRPC を使って通信するサーバーとクライアントは、それぞれ独立したコマンドとして `cmd/echo-server` ディレクトリ、`cmd/echo-client` ディレクトリ以下に作成することにします（それぞれ `main` 関数を作成します）。
Go 言語のプロジェクトで生成する実行ファイルのコードを `cmd` ディレクトリ以下に配置するのはよくあるプラクティスです。

### gRPC サーバーの実装

まずは、`EchoServiceServer` を実装します。
`protoc` によって自動生成された `echo/echo_grpc.pb.go` ファイルで定義されている `EchoServiceServer` インタフェースを実装していくわけですが、このとき、同じく自動生成されている次のようなモック実装を利用することができます。
このモック実装は空っぽの実装なので、名前に __`Unimplemented`__ プレフィックスが付いています。

{{< code lang="go" title="自動生成されたサーバーのモック実装（echo/echo_grpc.pb.go 内）" >}}
// UnimplementedEchoServiceServer must be embedded to have forward compatible implementations.
type UnimplementedEchoServiceServer struct {
}

func (UnimplementedEchoServiceServer) Echo(context.Context, *EchoRequest) (*EchoResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Echo not implemented")
}
{{< /code >}}

サーバー実装用の構造体（下記のコードでは `type server struct`）を定義するときに、上記のモック実装を [Golang の構造体埋め込みの仕組み](https://maku77.github.io/p/tbf357g/) で埋め込むことで、各 API をとりあえず `not implemented` エラーを返すだけの実装として提供することができます。
こうすることで、すべての API を一度に実装せずに、1 つずつ実装して提供していくことができます。

{{< code lang="go" title="cmd/echo-server/server.go" >}}
package main

import (
	"context"
	"log"

	"example.com/grpc-sample/echo"
)

// EchoService を実装するサーバーの構造体
type server struct {
	echo.UnimplementedEchoServiceServer // とりあえず Not implemented の実装を入れておく
}

// EchoServiceServer インタフェースの Echo メソッドの実装（本物の Echo 実装）
func (s *server) Echo(ctx context.Context, in *echo.EchoRequest) (*echo.EchoResponse, error) {
	log.Printf("Received from client: %v", in.GetMessage())
	return &echo.EchoResponse{Message: "*" + in.GetMessage()}, nil
}
{{< /code >}}

上記の例では、`server` 構造体に `UnimplementedEchoServiceServer` のモック実装を埋め込みつつ、`Echo` メソッドをオーバーライドしています。
結果として、モック実装側の `Echo` メソッドは使われないのですが、`UnimplementedEchoServiceServer` は `server` 構造体に埋め込んだままにしておいて大丈夫です。
上記の `Echo` メソッドは、クライアントから受信したテキストの先頭に `*` を付加したテキストをレスポンスとして返しています。

あとは、`main` 関数で gRPC サーバー (`grpc.Server`) のインスタンスを生成して、上記の実装を登録すれば OK です。

{{< code lang="go" title="cmd/echo-server/main.go" >}}
package main

import (
	"fmt"
	"log"
	"net"

	"google.golang.org/grpc"

	"example.com/grpc-sample/echo"
)

const port = 52000

func main() {
	// TCP ポートをオープンできるか確認
	lis, err := net.Listen("tcp", fmt.Sprintf(":%d", port))
	if err != nil {
		log.Fatalf("Failed to listen: %v\n", err)
	}

	// gRPC サーバーを生成し、EchoService サーバーの実装を登録する
	s := grpc.NewServer()
	echo.RegisterEchoServiceServer(s, &server{})

	// gRPC サーバーを稼働開始
	log.Printf("Server listening at %v\n", lis.Addr())
	if err := s.Serve(lis); err != nil {
		log.Fatalf("Failed to serve: %v\n", err)
	}
}
{{< /code >}}

### gRPC クライアントの実装

gRPC サーバー側が実装できたら、次はクライアント側の実装です。
通信用のクライアントスタブは、`protoc` で自動生成された __`NewEchoServiceClient`__ 関数を使って生成することができます。
下記の gRPC クライアント実装では、`Echo` メソッドを呼び出して `AAAAA` というメッセージを送り、その応答 (`*AAAAA`) を単純に出力しています。

{{< code lang="go" title="cmd/echo-client/main.go" >}}
package main

import (
	"context"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"

	"example.com/grpc-sample/echo"
)

const addr = "localhost:52000"

func main() {
	// EchoService サーバーへ接続する
	conn, err := grpc.Dial(addr, grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatalf("Did not connect: %v\n", err)
	}
	defer conn.Close()
	client := echo.NewEchoServiceClient(conn)

	// Echo メソッドを呼び出す
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()
	r, err := client.Echo(ctx, &echo.EchoRequest{Message: "AAAAA"})
	if err != nil {
		log.Fatalf("Could not echo: %v\n", err)
	}
	log.Printf("Received from server: %s\n", r.GetMessage())
}
{{< /code >}}


実行してみる
----

まず、gRPC のサーバー側を起動します。

```console
$ go run ./cmd/echo-server
2022/05/17 22:00:47 Server listening at [::]:52000
```

次に、gRPC のクライアント側を起動すると、サーバーと通信してメッセージを受信できていることを確認できます。

```console
$ go run ./cmd/echo-client
2022/05/17 22:01:32 Received from server: *AAAAA
```

やったー！

