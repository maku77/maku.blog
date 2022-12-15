---
title: ".proto の文法: サービス型 (service)"
url: "p/napwb4e/"
date: "2022-12-13"
tags: ["Protocol Buffers"]
---

{{% private %}}
- [サービスの定義](https://developers.google.com/protocol-buffers/docs/proto3#services)
{{% /private %}}

Protoco Buffers の __サービス型__ は、クライアントとサーバー間の通信方法 (RPC メソッド群) を定義するための型で、`.proto` ファイルの中で __`service`__ キーワードを使って定義します。

次の例では、`Echo` メソッドを持つ `EchoService` サービス型を定義しています。

```proto
// Echo メソッドを持つ EchoService の定義
service EchoService {
  rpc Echo (EchoRequest) returns (EchoResponse);
}

// Echo に送るリクエストメッセージの定義
message EchoRequest {
  string message = 1;
  optional string payload = 2;
}

// Echo が返すレスポンスメッセージの定義
message EchoResponse {
  string message = 1;
}
```

RPC の通信プロトコルは、独自で実装してしまうことも可能ですが、多くの場合は Google が作った __gRPC__ というプロトコルを使います。
というより、gRPC による通信を実現するために、Protocol Buffers (protobuf) によるシリアライズを使用することになるというケースが多いと思います。
`protoc` コマンド本体には、gRPC 用のスタブコードを生成する機能は入っていないので、`protoc` のプラグイン（`protoc-gen-go-grpc` など）を入れてコード生成することになります。

- 参考: [Go 言語で gRPC 通信してみる（Echo サーバー＆クライアント）](/p/ij4jv9k/)

