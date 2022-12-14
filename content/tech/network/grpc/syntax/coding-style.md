---
title: "protobuf (.proto) ファイルのコーディングスタイル"
url: "p/esbs9o5/"
date: "2022-05-17"
tags: ["Protocol Buffers"]
weight: 1
---

`.proto` ファイルでメッセージやサービスを定義するときのコーディング規約をまとめておきます。
Google が [Style Guide](https://developers.google.com/protocol-buffers/docs/style) として指針をまとめていますが、プロジェクト内に既に `.proto` ファイルがある場合は、一貫性を保つように記述するのがよいとされています。


{{< code lang="proto" title=".proto ファイルの例" >}}
syntax = "proto3";

package endpoints.examples.bookstore;

option java_multiple_files = true;
option java_outer_classname = "BookstoreProto";
option java_package = "com.google.endpoints.examples.bookstore";

import "google/protobuf/empty.proto";

service Bookstore {
  rpc ListShelves(google.protobuf.Empty) returns (ListShelvesResponse) {}
  rpc CreateShelf(CreateShelfRequest) returns (Shelf) {}
  rpc GetShelf(GetShelfRequest) returns (Shelf) {}
  rpc DeleteShelf(DeleteShelfRequest) returns (google.protobuf.Empty) {}
  rpc ListBooks(ListBooksRequest) returns (ListBooksResponse) {}
  rpc CreateBook(CreateBookRequest) returns (Book) {}
  rpc GetBook(GetBookRequest) returns (Book) {}
  rpc DeleteBook(DeleteBookRequest) returns (google.protobuf.Empty) {}
}

message Shelf {
  int64 id = 1;
  string theme = 2;
}

message Book {
  int64 id = 1;
  string author = 2;
  string title = 3;
}

enum FooBar {
  FOO_BAR_UNSPECIFIED = 0;
  FOO_BAR_FIRST_VALUE = 1;
  FOO_BAR_SECOND_VALUE = 2;
}
{{< /code >}}


全般
----

- 1 行は __80 文字__ まで
- インデントはスペース __2 文字__
- 文字列リテラルは __ダブルクォート__ で囲む（例: `"Hoge"`）


ファイル構造
----

- `.proto` ファイル名はすべて小文字 + アンダースコア（例: __`lower_snake_case.proto`__）
- `.proto` ファイルの内容は次のような順番で記述する
  1. ライセンス / コピーライト（必要があれば）
  1. 概要 / Overview
  1. 構文バージョン（例: `syntax = "proto3";`）
     - これを省略すると、デフォルトで `"proto2"` とみなされてしまいます。
  1. パッケージ定義（例: `package example.echo;`）
  1. インポート（ソートしておく）
  1. オプション (File options)
     - 例: `go_package = "example.com/hello";`
     - 例: `java_package = "com.example.hello";`
  1. その他（残り全部）


パッケージ
----

```proto
package example.echo;
```

- パッケージ名はすべて小文字 (__`lowercase`__)。できるだけ `.proto` ファイルのパスと対応させる


メッセージ
----

```proto
message SongServerRequest {
  optional string song_name = 1;
}
```

- メッセージ名は __`CamelCase`__ で、フィールド名は __`lower_snace_case`__ の形にする。`.proto` ファイル内で `song_name` というフィールド名で定義しても、Java や Kotlin のコードを生成したときは、正しく `getSongName()` のような名前になるので心配しなくていい
- フィールド名のサフィックスに数値を付ける場合は、アンダースコアを挟まない
  ```proto
  string song_name_1;  // NG
  string song_name1;   // Good
  ```
- `repeated` 修飾子のついたフィールドの名前は複数形にする
  ```proto
  repeated string keys = 1;
  ```


Enum
----

- Enum 名は __`CamelCase`__ で、各値は __`CAPITALS_WITH_UNDERSCORES`__ の形にする
- 各値の行末はセミコロン (__`;`__) で終わる
- 各値のプレフィックスとして Enum 名に対応する名前を付ける。下記の例では `FOO_BAR_`
- 0 の値を持つものは、__`_UNSPECIFIED`__ サフィックスを付ける
  ```proto
  enum FooBar {
    FOO_BAR_UNSPECIFIED = 0;
    FOO_BAR_FIRST_VALUE = 1;
    FOO_BAR_SECOND_VALUE = 2;
  }
  ```


サービス
----

- サービス名は __`CamelCase`__ で、RPC メソッド名も __`CamelCase`__ の形にする
  ```proto
  service FooService {
    rpc GetSomething(GetSomethingRequest) returns (GetSomethingResponse);
    rpc ListSomething(ListSomethingRequest) returns (ListSomethingResponse);
  }
  ```


その他
----

- `required` は `proto3` 以降は使わない
- `groups` は `proto3` 以降は使わない

