---
title: ".proto の文法: repeated ラベル（フィールドを配列にする）"
url: "p/b2q2jmh/"
date: "2022-12-16"
tags: ["Protocol Buffers"]
---

Protocol Buffers (protobuf) のフィールドを配列／リスト扱いにするには、`.proto` ファイルのフィールド定義の先頭に __`repeated`__ ラベルを付加します。
次の `FindBookResponse` メッセージ型は、`Book` 配列のフィールドを持っています。

{{< code lang="proto" title="books.proto（抜粋）" >}}
message FindBookResponse {
  repeated Book books = 1;
}

message Book {
  string title = 1;
}
{{< /code >}}

`repeated` ラベルを付けたフィールドの名前は複数形にしましょう（上記例の場合は `books`）。

Protocol Buffers では `repeated` を連続させた多次元配列は定義できませんが、任意のメッセージ型に `repeated` を付けることはできるので、そのメッセージ型の中に `repeated` フィールドを配置すれば、多次元配列と同様のデータを表現することができます。

`repeated` ラベルは、`oneof` フィールドに付加することはできません。

- 参考: [.proto の文法: oneof 型](/p/vxixbp3/)

