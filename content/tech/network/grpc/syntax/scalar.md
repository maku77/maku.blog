---
title: ".proto の文法: スカラー型の一覧"
url: "p/bi5jyer/"
date: "2022-12-13"
tags: ["Protocol Buffers"]
weight: 103
---

{{% private %}}
- [Scalar Value Types](https://developers.google.com/protocol-buffers/docs/proto3#scalar)
{{% /private %}}

Protocol Buffers の `.proto` ファイルの中で使用可能なスカラー型と、各言語の型の対応表です。
スカラー型は、[メッセージ型](/p/7h3hu8k/) の各フィールドの型として使用できます。

| protobuf | C++ | Java/Kotlin | Go | Dart |
| ---- | ---- | ---- | ---- | ---- |
| __`double`__ | `double` | `double` | `float64` | `double` |
| __`float`__ | `float` | `float` | `float32` | `double` |
| __`int32`__ | `int32` | `int` | `int32` | `int` |
| __`int64`__ | `int64` | `long` | `int64` | `Int64` |
| __`uint32`__ | `uint32` | `int` | `uint32` | `int` |
| __`uint64`__ | `uint64` | `long` | `uint64` | `Int64` |
| __`sint32`__ | `int32` | `int` | `int32` | `int` |
| __`sint64`__ | `int64` | `long` | `int64` | `Int64` |
| __`fixed32`__ | `uint32` | `int` | `uint32` | `int` |
| __`fixed64`__ | `uint64` | `long` | `uint64` | `Int64` |
| __`sfixed32`__ | `int32` | `int` | `int32` | `int` |
| __`sfixed64`__ | `int64` | `long` | `int64` | `Int64` |
| __`bool`__ | `bool` | `boolean` | `bool` | `bool` |
| __`string`__ | `string` | `String` | `string` | `String` |
| __`bytes`__ | `string` | `ByteString` | `[]byte` | `List` |

次のような __`repeated`__ フィールドは、配列やリスト（Golang ではスライス）に相当するコードに置き換えられます。

```proto
repeated float prices = 2;     // Golang なら []float32 になる
repeated string snippets = 3;  // Golang なら []string になる
```

