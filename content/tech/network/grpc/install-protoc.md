---
title: "protoc コマンド (protocol buffer compiler) をインストールする"
url: "p/4k5m3jy"
date: "2022-05-09"
tags: ["gRPC", "Protocol Buffers"]
draft: true
---

protoc コマンドとは
----

__`protoc`__ コマンドは、__`.proto`__ ファイルをコンパイルするためのコマンドラインツールです。
`.proto` ファイルは、Protocol Buffers (protobuf) 用のメッセージ（データ構造）やサービス (RPC) の型を記述するスキーマ定義ファイルです。

`.proto` ファイルの情報をもとに各種プログラミング言語用のコードを生成するわけですが、そのためには、__protoc コマンド本体__ と __各言語用のプラグイン__（`protoc-gen-go` など）がインストールされている必要があります。


protoc 本体のインストール
----

{{% private %}}
- 参考: [Protocol Buffer Compiler Installation | gRPC](https://grpc.io/docs/protoc-installation/)
{{% /private %}}

`protoc` コマンドは、各 OS 用のパッケージマネージャを使ってインストールするのが簡単です。
インストールするパッケージの名前は __`protobuf`__ や __`protobuf-compiler`__ であることに注意してください。

### macOS (Homebrew) の場合

```console
$ brew install protobuf  # インストール
$ brew upgrade protobuf  # バージョン更新

$ protoc --version
libprotoc 3.19.4
```

### Linux (apt) の場合

```console
$ apt install -y protobuf-compiler
$ protoc --version
```

### プリビルド版を使う場合

GitHub のリリースページから、各 OS 用の実行ファイルをダウンロードすることもできます。

- [Releases · protocolbuffers/protobuf](https://github.com/protocolbuffers/protobuf/releases)
- [Download Protocol Buffers  |  Google Developers](https://developers.google.com/protocol-buffers/docs/downloads)


各種言語用プラグインのインストール
----

`protoc` コマンドは標準でいくつかの言語用の出力に対応しており、次のようにヘルプ表示すると対応言語を確認できます。

```console
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
```

{{< code lang="console" title="例: Python のコードを生成" >}}
% protoc --python_out=. hello.proto
{{< /code >}}

その他の言語用のコードを生成するには、追加のプラグイン（実際はただのコマンド）をインストールする必要があります。
追加でインストールするコマンドは __`protoc-gen-<言語名>`__ という名前であり、そのコマンドがシステムに存在していると、`protoc` コマンドの __`--<言語名>_out`__ というオプションが有効になります。
例えば、Go 言語用のプラグインである `protoc-gen-go` をインストールすると、`--go_out` オプションが使えるようになります。

{{< code lang="console" title="例: Go のコードを生成" >}}
$ protoc --go_out=. hello.proto
{{< /code >}}

### 例: Go 言語用のプラグイン

{{< code lang="console" title="protoc-gen-go のインストール" >}}
# バージョン指定でインストールする場合（推奨）
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28.0

# 最新版をインストールする場合
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

# 確認
$ protoc-gen-go --version
protoc-gen-go v1.28.0
{{< /code >}}

{{< code lang="console" title="使用例" >}}
$ protoc
    --go_out=. \
    --go_opt=Mhello.proto=example.com/myapp/protos/hello \
    hello.proto
{{< /code >}}

