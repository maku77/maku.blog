---
title: ".proto の文法: メッセージ型 (message)"
url: "p/7h3hu8k/"
date: "2022-12-13"
tags: ["Protocol Buffers"]
weight: 101
---

メッセージ型とは
----

Protocol Buffers の __メッセージ型__ は、基本的なデータ構造を表すもので、`.proto` ファイルの中で __`message`__ キーワードを使って定義します。
メッセージ型は、プログラミング言語でいうところの構造体に相当するものです。

```proto
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}
```

{{% accordion title="Go 言語のコードに変換した場合の例" %}}
{{< code lang="console" title="変換コマンド" >}}
$ protoc --go_out=. --go_opt=Msample.proto=message sample.proto
{{< /code >}}

{{< code lang="go" title="sample.pb.go（抜粋）" >}}
type SearchRequest struct {
	// ...
	Query         string `protobuf:"bytes,1,opt,name=query,proto3" json:"query,omitempty"`
	PageNumber    int32  `protobuf:"varint,2,opt,name=page_number,json=pageNumber,proto3" json:"page_number,omitempty"`
	ResultPerPage int32  `protobuf:"varint,3,opt,name=result_per_page,json=resultPerPage,proto3" json:"result_per_page,omitempty"`
}
{{< /code >}}

自動生成されたコードの型情報には、`.proto` で明示的に定義したフィールド以外の情報も含まれています。
それらの情報は、各種 protobuf ライブラリが内部的に使用します。
{{% /accordion %}}

上記の `SearchRequest` というメッセージ型は 3 つのフィールド（文字列 1 つと数値 2 つ）を持っています。
ここでは、protobuf が標準で用意している [スカラー型](/p/bi5jyer/) の `string` と `int32` を使っていますが、他のメッセージ型や列挙型、マップ型などのフィールドとして定義することもできます。
このあたりは、一般的なプログラミング言語と同様です。
各フィールドの末尾には、フィールドを一意に特定する整数である [フィールド番号](/p/w7xkvnb/) を割り当てる必要があります。

### 配列を示す repeated ラベル

```proto
repeated float prices = 2;     // Golang なら []float32 になる
repeated string snippets = 3;  // Golang なら []string になる
```

フィールドの定義に __`repeated`__ ラベルを付けると、配列やリスト（Golang ではスライス）を表すフィールドとして扱われます。

- [repeated ラベル（フィールドを配列にする）](/p/b2q2jmh/)

### 省略可能を示す optional ラベル

```proto
optional string name = 1;
optional int32 id = 2;
optional string email = 3;
```

フィールドの定義に __`optional`__ ラベルを付けると、省略可能なフィールドとして扱われます。
__`optional`__ ラベルの付いたフィールドは、実際に値がセットされているかどうかを調べることができます。

- 参考: [optional ラベル（フィールドを省略可能にする）](/p/sp9q7o5/)

### キー＆バリューのマップを示す map 型

```proto
map<int32, string> my_map = 2;
map<string, Project> projects = 3;
```

フィールドを定義するときに、__`map<キーの型, 値の型>`__ という型を使うと、任意の整数あるいは文字列をキーとするマップを表現することができます（浮動小数点数をキーにすることはできません）。

- 参考: [マップ型 (map)](/p/xpbnycm/)


ネストされた型の定義 (Nested Types)
----

ネストされた形でメッセージ型を定義することができます。
次の例では、`SearchResponse` 型の中で `Result` 型を定義しており、`results` フィールドでその型の配列を持つように定義しています。
`SearchResponse` 型が持っているフィールドは、あくまで `results` の 1 つだけであることに注意してください。

```proto
message SearchResponse {
  message Result {
    string url = 1;
    string title = 2;
    repeated string snippets = 3;
  }

  // 上記の型の配列をフィールドとして持つ
  repeated Result results = 1;
}
```

ネスト定義された型を、別のメッセージ型から参照するときには、次のようにドット区切りで名前を指定します。

```proto
message SomeOtherMessage {
  SearchResponse.Result result = 1;
}
```

ネストの深さに制限はありません。

```proto
message Outer {       // Level 0
  message MiddleA {   // Level 1
    message Inner {   // Level 2
      int64 ival = 1;
      bool booly = 2;
    }
  }
  message MiddleB {   // Level 1
    message Inner {   // Level 2
      int32 ival = 1;
      bool booly = 2;
    }
  }
}
```

メッセージ型に限らず、列挙型 (`enum`) もネストして定義することができます。
次の例では、`Person` メッセージ型の下に `PhoneType` 列挙型を定義しています。

```proto
message Person {
  // ...

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    string number = 1;
    PhoneType type = 2;
  }

  repeated PhoneNumber phones = 4;
}
```

