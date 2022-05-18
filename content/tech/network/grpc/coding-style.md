---
title: "protobuf (.proto) ファイルのコーディングスタイル"
url: "p/esbs9o5/"
date: "2022-05-17"
tags: ["Protocol Buffers"]
draft: true
---

`.proto` ファイルでメッセージやサービスを定義するときのコーディング規約をまとめておきます。
Google が [Style Guide](https://developers.google.com/protocol-buffers/docs/style) として指針をまとめていますが、プロジェクト内に既に `.proto` ファイルがある場合は、一貫性を保つように記述するのがよいとされています。


全般
----

- 1 行は __80 文字__ まで
- インデントはスペース __2 文字__
- 文字列リテラルは __ダブルクォーテーション (`"`)__ で囲む


ファイル構造
----

- `.proto` ファイル名はすべて小文字（例: `lower_snake_case.proto`）
- `.proto` ファイル内は次のような順番で記述する
  1. ライセンス（必要があれば）
  1. 概要説明 / Overview
  1. 構文バージョン（例: `syntax = "proto3";`）
  1. パッケージ名（例: `package example.echo;`）
  1. インポート（ソートする）
  1. オプション
     - 例: `go_package = "example.com/hello";`
     - 例: `java_package = "com.example.hello";`
  1. その他


パッケージ
----

- パッケージ名はすべて小文字。できるだけ `.proto` ファイルのパスと対応させる


メッセージ
----

- メッセージ名は `CamelCase` で、フィールド名は `underscore_separated_names` の形にする
  ```proto
  message SongServerRequest {
    optional string song_name = 1;
  }
  ```
  上記のように定義しても、Java のコードを生成したときは、正しく `getSongName()` のような名前になるので心配しなくていい。
- フィールド名のサフィックスに数値を付ける場合は、アンダースコアを挟まない（例: `song_name_1` ではなく `song_name1` とする）
- Repeated フィールドの名前は複数形で
  ```proto
  repeated string keys = 1;
  ...
  repeated MyMessage accounts = 17;
  ```


Enum
----

- Enum 名は `CamelCase` で、各値は `CAPITALS_WITH_UNDERSCORES` の形にする
  ```proto
  enum FooBar {
    FOO_BAR_UNSPECIFIED = 0;
    FOO_BAR_FIRST_VALUE = 1;
    FOO_BAR_SECOND_VALUE = 2;
  }
  ```
- 各値のプレフィックスとして Enum 名に対応する名前を付ける。上記の例の場合は `FOO_BAR_`
- 0 の値を持つものは、`_UNSPECIFIED` サフィックスを付ける


サービス
----

- サービス名は `CamelCase` で、RPC メソッド名も `CamelCase` の形にする
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

