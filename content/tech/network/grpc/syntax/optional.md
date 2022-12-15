---
title: ".proto の文法: optional ラベル（フィールドを省略可能にする）"
url: "p/sp9q7o5/"
date: "2022-12-15"
tags: ["Protocol Buffers"]
---

.proto のフィールドを省略可能にする (option)
----

Protocol Buffers（`.proto` ファイル）のフィールド定義で __`optional`__ というラベルを付けると、そのフィールドへの値のセットがオプショナルであることを示すことができます。
`optional` ラベルは、`protoc` コンパイラの v3.15.0 以降で使用できます。

{{< code lang="proto" title="sample.proto" hl_lines="7" >}}
syntax = "proto3";

option go_package = "github.com/maku77/myapp/pb";

message SampleMessage {
  string message = 1;  // 通常のフィールド
  optional string description = 2;  // 省略可能なフィールド
}
{{< /code >}}

例えば、上記のような `.proto` ファイルから、次のように Golang コードを生成すると、

```console
$ protoc --go_out=. --go_opt=paths=source_relative sample.proto
```

次のような `SampleMessage` 型のコードが生成されます。

{{< code lang="go" title="sample.pb.go" hl_lines="7" >}}
type SampleMessage struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	Message     string  `protobuf:"bytes,1,opt,name=message,proto3" json:"message,omitempty"`               // 通常のフィールド
	Description *string `protobuf:"bytes,2,opt,name=description,proto3,oneof" json:"description,omitempty"` // 省略可能なフィールド
}
{{< /code >}}

Golang の場合は、オプショナルな `string` フィールドは、`*string` として表現されるようですね。

データの受信側で、実際にフィールドに値がセットされているかを調べる方法は、各言語の protobuf ライブラリによって異なります。
例えば、C++ の場合は、`has_description()` というメソッドで `description` フィールドに値がセットされているかどうかを調べることができます。
これは、[oneof フィールド](/p/vxixbp3/) に値がセットされていることを調べる方法と同様です。

{{< code title="各言語での optional フィールドの存在確認" >}}
if (m.has_foo()) {...}  // C++ の場合
if (m.HasFoo) {...}     // C# の場合
if m.Foo != nil {...}   // Go の場合
if (m.hasFoo()) {...}   // Java の場合
if (m.hasFoo()) {...}   // Javascript の場合
if m.HasField('foo'):   // Python の場合
if m.has_foo?           // Ruby の場合
{{< /code >}}


（おまけ）デフォルト値と Field presence
----

Protocol Buffers では、バイナリ化したデータ形式のこと __wire 形式__ と読んでいます。
例えば、ファイルへの保存時や、gRPC のリクエスト送信時に使われるときのデータ形式です。
Protocol Buffers では、フィールドの型ごとに下記のようなデフォルト値が定義されており、データを wire 形式にシリアライズするときに、これらの __デフォルト値を保存しない__ ようになっています（ネットワークなどで送信されません）。

- 数値型: `0`
- bool 型: `false`
- enum 型: `0` を持つ enum 値（つまり、先頭の `XXXX_UNSPECIFIED`）
- string 型、bytes 型、repeated ラベルの付いた型: 長さが 0 のデータ
- message 型: 各言語の null に相当するデータ

これは、データサイズを削減するための proto3 の仕様です。
データの受信側では、フィールドに対応する値が入っていなければ、デフォルト値がセットされているものとして振る舞います。
この仕様は、データの送信側がフィールドの値を明示的にセットしなかった場合に問題になります。
例えば、送信側がある数値型のフィールドをセットしなかった場合、受信側はそのフィールドにデフォルト値の `0` が格納されていると判断しますが、意図的に `0` がセットされたのか、値がセットされなかったのかを区別できません。

そこで、値がセットされない可能性があるフィールドには `optional` ラベルを付加することで、wire 形式のデータに、__フィールドの値の有無を示す情報__ を付加できるようにしています。
このようなフィールドのことを、__Explicit Presence__ と呼んでいます。
明示的に値の存在を示す、ということですね。
逆に、通常のフィールドでは、このような値の存在を示す情報は付加されないので、__No Presence__ と呼ばれています。

`options` ラベルの付いたフィールドと、通常のフィールドの本質的な違いは、このような Field presence 情報の扱いだけです。

- 参考: [Application note: Field presence](https://github.com/protocolbuffers/protobuf/blob/main/docs/field_presence.md)

