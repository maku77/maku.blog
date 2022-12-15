---
title: ".proto の文法: マップ型 (map)"
url: "p/xpbnycm/"
date: "2022-12-14"
tags: ["Protocol Buffers"]
---

{{% private %}}
- https://developers.google.com/protocol-buffers/docs/proto3#maps
{{% /private %}}

Protocol Buffers でマップ型のフィールドを定義するには、__`map<key_type, value_type>`__ という形式で型を指定します。
次の `SampleMessage` メッセージ型は、`string` 型のキーと `Project` 型の値を持つマップフィールドを定義しています。

{{< code lang="proto" title="sample.proto（map 型フィールドの例）" >}}
message SampleMessage {
  // ...
  map<string, Project> projects = 3;
}

message Project {
  // ...
}
{{< /code >}}

__キーの型 (`key_type`) には、整数あるいは文字列__ を指定することができます。
浮動小数点数 (`float`, `double`) やバイトデータ (`bytes`) を `key_type` にすることはできません。

__値の型 (`value_type`) としては、ほぼすべての型__ を指定できますが、マップ型だけは `value_type` にすることはできません。
つまり、マップのマップは表現できないことになるのですが、`value_type` として他のメッセージ型を持たせることができるので（上記の例では `Project`）、実際にはほぼ同様の情報を表現できます。

あと、あまり困ることはないでしょうが、マップの配列（リスト）は表現できません（つまり、`repeated map<xxx, yyy>` とはできません）。

