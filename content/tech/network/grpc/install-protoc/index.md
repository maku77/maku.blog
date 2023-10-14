---
title: "protoc コマンドで .proto ファイルをコンパイルする (Protocol Buffers Compiler)"
url: "p/37e6uck/"
date: "2022-04-20"
lastmod: "2022-05-09"
tags: ["gRPC", "Protocol Buffers"]
---

{{% private %}}
- [Protocol Buffers  |  Google Developers](https://developers.google.com/protocol-buffers)
  - [Guides / Overview](https://developers.google.com/protocol-buffers/docs/overview)
{{% /private %}}

Protcol Buffers とは
----

プロトコルバッファー (Protocol Buffers、または protobuf) は、Google が開発した、構造化したデータをシリアライズするためのフォーマットです。
同じく Google が開発した __gRPC__ 通信プラットフォームで採用されており、XML や JSON などのテキストベースの API より効率的な通信を行うことができるという特徴を持っています。

- データをコンパクトに表現できるため、通信やパース処理が高速
- 強い型付けを行うことでき、サーバー、クライアントの安全なコーディングが可能
- 定義変更時の互換性を考慮したフォーマット
- OS やプログラミング言語などに非依存

データ構造やサービス形式の定義は、__`.proto`__ 拡張子を持つ __プロトコル定義ファイル (Proto Definition file)__ で行います。
この `.proto` ファイルを __`protoc`__ コマンド（プロトコルバッファーコンパイラ）でコンパイルすると、各言語用のソースコードを生成することができます。

{{< image src="img-001.drawio.svg" title="protoc コマンドによるコード生成" >}}

`protoc` コマンドは各種プログラミング言語用のコードを生成するわけですが、そのためには、__protoc コマンド本体__ と __各言語用のプラグイン__（`protoc-gen-go` など）がインストールされている必要があります。
C++ や C#、Kotlin、Python、Ruby などのコード生成は組み込みで対応していますが、Go 言語用のプラグインなどは別途インストールする必要があります。


protoc 本体のインストール
----

__`protoc`__ コマンド (Protocol Buffers Compiler) は、Linux（Ubuntu 系）や macOS ではパッケージマネージャーを使ってインストールしてしまうのが簡単です。
インストールするパッケージの名前は __`protobuf`__ や __`protobuf-compiler`__ であることに注意してください。

### macOS (Homebrew) の場合

```console
$ brew install protobuf  # インストール
$ brew upgrade protobuf  # バージョン更新
```

### Linux (apt) の場合

```console
$ apt install -y protobuf-compiler
```

### プリビルド版を使う場合

あるいは、各 OS 用のバイナリを [GitHub のリリースページ](https://github.com/protocolbuffers/protobuf/releases) からダウンロードし、`protoc` コマンドにパスを通します。
例えば、Windows であれば `protoc-3.20.0-win64.zip` などをダウンロードします。

次のように `protoc` コマンドを実行できるようになれば OK です。

```console
$ protoc --version
libprotoc 3.20.0
```

- 参考: [Protocol Buffer Compiler Installation | gRPC](https://grpc.io/docs/protoc-installation/)


.proto ファイルをコンパイルしてみる
----

次のような簡単な `.proto` ファイルを入力ファイルとして用意します。

{{< code lang="proto" title="proto/person.proto" >}}
syntax = "proto3";

message Person {
  optional string name = 1;
  optional int32 id = 2;
  optional string email = 3;
}
{{< /code >}}

### C# のコードを生成

例えば、次のように実行すると、C# 用のソースコードを生成することができます。
__`--csharp_out=<OUT_DIR>`__ というオプションで、C# ソースコードの出力先ディレクトリを指定しています。

```console
$ mkdir gen
$ protoc --csharp_out=gen proto/person.proto
$ ls gen
Person.cs
```

### Python のコードを生成

Python 用のソースコードを生成したければ、同様に次のようにします。
出力ディレクトリは __`--python_out=<OUT_DIR>`__ オプションで指定します。

```console
$ protoc --python_out=gen proto/person.proto
```

### その他の言語のコードを生成

他にも各言語用のソースコードを生成するオプションが用意されており、次のようにヘルプ表示すると標準でサポートしている言語を確認できます。

{{< code lang="console" title="protoc の言語別出力オプション" >}}
$ protoc --help | grep OUT_DIR
  --cpp_out=OUT_DIR           Generate C++ header and source.
  --csharp_out=OUT_DIR        Generate C# source file.
  --java_out=OUT_DIR          Generate Java source file.
  --js_out=OUT_DIR            Generate JavaScript source.
  --kotlin_out=OUT_DIR        Generate Kotlin file.
  --objc_out=OUT_DIR          Generate Objective-C header and source.
  --php_out=OUT_DIR           Generate PHP source file.
  --python_out=OUT_DIR        Generate Python source file.
  --ruby_out=OUT_DIR          Generate Ruby source file.
{{< /code >}}


言語拡張 (protoc-gen-xxx)
----

{{% private %}}
- https://developers.google.com/protocol-buffers/docs/gotutorial
{{% /private %}}

`protoc` が標準でサポートしてない言語のコードを生成するには、追加のプラグイン（実際はただのコマンド）をインストールする必要があります。
追加でインストールするコマンドは __`protoc-gen-<言語名>`__ という名前であり、そのコマンドがシステムに存在していると、`protoc` コマンドの __`--<言語名>_out`__ というオプションが有効になります。

### 例: Go 言語用プラグイン (protoc-gen-go)

例えば、Go 言語用のプラグインである [`protoc-gen-go`](https://developers.google.com/protocol-buffers/docs/reference/go-generated) をインストールすると、__`--go_out`__ オプションが使えるようになります。

{{< code lang="console" title="protoc-gen-go のインストール" >}}
# バージョン指定でインストールする場合（推奨）
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28.0

# 最新版をインストールする場合
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

# 確認
$ protoc-gen-go --version
protoc-gen-go v1.28.0
{{< /code >}}

Go 言語用のコードを出力する場合は、`.proto` ファイル内の __`option go_package`__ でパッケージ名を設定しておく必要があります。

{{< code lang="proto" title="proto/person.proto" >}}
syntax = "proto3";

option go_package = "example.com/myapp";

message Person {
  optional string name = 1;
  optional int32 id = 2;
  optional string email = 3;
}
{{< /code >}}

次のようにすると、`gen` ディレクトリ以下に Go コードが生成されます。

```console
$ protoc --go_out=gen proto/person.proto
```

`.proto` ファイルで指定したパッケージ名に従ってディレクトリ階層ができます。

```
gen/example.com/myapp/person.pb.go
```

`.proto` ファイル内でパッケージ名を指定するのではなく、`protoc` コマンドの __`--go_opt=M...`__ オプションで次のように指定することもできます。

```
$ protoc --go_out=gen \
         --go_opt=Mproto/person.proto=example.com/myapp \
         proto/person.proto
```

`=` が 2 回出てくるのでちょっと分かりにくいですが、`proto/person.proto` ファイルのパッケージ名を `example.com/myapp` に設定しています。

{{% maku-common/reference %}}
- [Go 言語で gRPC 通信してみる（Echo サーバー＆クライアント）](/p/ij4jv9k/)
{{% /maku-common/reference %}}

