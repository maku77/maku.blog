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
次の `SampleMessage` メッセージ型は、1 つの `oneof` 型フィールド `test_oneof` を持っています。

```proto
message SampleMessage {
  oneof test_oneof {
    string name = 4;
    SubMessage sub_message = 9;
  }
}
```

上記の `oneof` ブロックには `name` フィールドと `sub_message` フィールドが定義されていますが、これらのいずれかのフィールドに値が格納されることを示しています。
実際にどのフィールドに値が格納されているかを調べる方法は、各言語の protobuf ライブラリ実装によって異なります。
下記は C++ の例です。

```cpp
SampleMessage message;

message.set_name("maku");
if (message.has_name()) {
    // ...
}
```

`oneof` のフィールドに値をセットすると、同じ `oneof` に含まれる他のフィールドの値はクリアされることに注意してください。

