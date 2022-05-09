---
title: "Go 言語で gRPC を扱う"
url: "/p/ij4jv9k"
date: "2022-04-20"
tags: ["Go", "gRPC"]
draft: true
---

何をするか？
----

gRPC-Go のインストール
----

Go 言語用の gRPC ライブラリとして、[gRPC-Go (google.golang.org/grpc)](https://pkg.go.dev/google.golang.org/grpc) というパッケージが提供されています。
Go 言語用のプロジェクトを作成して、このパッケージの依存関係を追加します。

```console
$ mkdir study-grpc
$ cd study-grpc
$ go mod init study-grpc
$ go get google.golang.org/grpc
```
