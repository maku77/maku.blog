---
title: ".proto の文法: oneof 型"
url: "p/vxixbp3/"
date: "2022-12-14"
tags: ["Protocol Buffers"]
---

{{% private %}}
- https://developers.google.com/protocol-buffers/docs/proto3#using_oneof
{{% /private %}}

Protocol Buffers の __oneof 型__ は、定義したフィールドのうち、いずれか 1 つのフィールドのみに値が格納されていることを示す型です。
次の `SampleMessage` メッセージ型は、1 つの `oneof` 型フィールド `status` を持っています。

```proto
message SampleMessage {
  string content = 1;
  oneof status {
    int32 status_code = 2;
    string status_name = 3;
  }
}
```

上記の `oneof` ブロックには `status_code` フィールドと `status_name` フィールドが定義されていますが、これらのいずれかのフィールドに値が格納されることを示しています。
実際にどのフィールドに値が格納されているかを調べる方法は、各言語の protobuf ライブラリ実装によって異なります。
下記は C++ の例です。

```cpp
SampleMessage message;

message.set_status_code(123);
if (message.has_status_code()) {
    // ...
}
```

`oneof` のフィールドに値をセットすると、同じ `oneof` に含まれる他のフィールドの値はクリアされることに注意してください。

`oneof` フィールドに、[`repeated` ラベル](/p/b2q2jmh/) や [`optional` ラベル](/p/sp9q7o5/) を設定することはできません。

