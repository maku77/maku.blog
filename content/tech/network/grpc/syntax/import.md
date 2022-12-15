---
title: ".proto の文法: 別の .proto ファイルをインポートする (import)"
url: "p/e8yeofc/"
date: "2022-12-14"
tags: ["Protocol Buffers"]
---

.proto ファイルをインポートする (import)
----

Protocol Buffers の `.proto` 内で __`import`__ ステートメントを使用すると、他の `.proto` ファイルの内容を取り込むことができます。
次の例では、`main.proto` から `other.proto` の内容をインポートして、`OtherMessage` というメッセージ型を参照しています。

{{< code lang="proto" title="proto/message/main.proto（インポートする側）" hl_lines="5" >}}
syntax = "proto3";

option go_package = "github.com/maku77/myapp/message";

import "message/other.proto";

message MainMessage {
  OtherMessage other = 1;
}
{{< /code >}}

{{< code lang="proto" title="proto/message/other.proto（インポートされる側）" >}}
syntax = "proto3";

option go_package = "github.com/maku77/myapp/message";

message OtherMessage {
  string content = 1;
}
{{< /code >}}

インポートのパスは、デフォルトでは `protoc` コマンドを実行したディレクトリからの相対パスで指定します。
起点となるディレクトリを変更したい場合は、`protoc` コマンドの __`--proto_path`__ オプションで、`.proto` ファイルを配置したルートディレクトリを指定します。

{{< code lang="console" title=".proto ファイルが proto ディレクトリ以下にある場合" >}}
$ protoc --go_out=. --go_opt=paths=source_relative --proto_path=proto message/main.proto
{{< /code >}}

上記のように実行すると、`proto/message/main.proto` ファイル（およびそこからインポートされているファイル）を入力情報として、`message/main.pb.go` が生成されます。

- 参考: [protoc コマンドで .proto ファイルをコンパイルする (Protocol Buffers Compiler)](/p/37e6uck/)
- 参考: [Go 言語で gRPC 通信してみる（Echo サーバー＆クライアント）](/p/ij4jv9k/)


インポートした内容を再公開する (import public)
----

別の `.proto` ファイルをインポートするときに、__`import public`__ を使用すると、インポートした内容（メッセージ型の定義など）を自分自身が定義しているかのように公開することができます。
以下の例では、`main.proto` は `other1.proto` しかインポートしていませんが、間接的に `other2.proto` で定義している `Other2Message` メッセージ型を参照しています。

{{< code lang="proto" title="main.proto" >}}
syntax = "proto3";

import "other1.proto";

message MainMessage {
  Other2Message other2 = 1;
}
{{< /code >}}

{{< code lang="proto" title="other1.proto" hl_lines="3" >}}
syntax = "proto3";

import public "other2.proto";
{{< /code >}}

{{< code lang="proto" title="other2.proto" >}}
syntax = "proto3";

message Other2Message {
  string content = 1;
}
{{< /code >}}

