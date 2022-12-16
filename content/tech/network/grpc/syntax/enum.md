---
title: ".proto の文法: 列挙型 (enum)"
url: "p/p5wjbwq/"
date: "2022-12-14"
tags: ["Protocol Buffers"]
---

列挙型とは
----

Protocol Buffers の __列挙型__ は、取り得る値が、あらかじめ定義された定数の中からのみ選択可能であることを示す型で、`.proto` ファイルの中で __`enum`__ キーワードを使って定義します。
列挙型は、ひとことで言うと、複数の定数をグルーピングする機能です。
下記の例では、`Corpus` という列挙型を定義し、`SearchRequest` メッセージ型のフィールドとして使用しています。

{{< code lang="proto" title="sample.proto" >}}
enum Corpus {
  CORPUS_UNSPECIFIED = 0;
  CORPUS_UNIVERSAL = 1;
  CORPUS_WEB = 2;
  CORPUS_IMAGES = 3;
  CORPUS_LOCAL = 4;
  CORPUS_NEWS = 5;
  CORPUS_PRODUCTS = 6;
  CORPUS_VIDEO = 7;
}

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
  Corpus corpus = 4;
}
{{< /code >}}

[.proto のコーディングスタイル](/p/esbs9o5/) では、すべての列挙値の名前には、__型名に相当するプレフィックス__ を付けるべしとされています（上記の `CORPUS_` の部分）。
型名が 3 語以上で構成されていたりして長い場合は、略称のプレフィックスが使われることもあるようです。

__0 という値にマップされるものには、サフィックスとして `_UNSPECIFIED`__ を付けるよう推奨されています（上記の `CORPUS_UNSPECIFIED`）。
これは、コーディングスタイルという観点だけではなく、`proto2` との互換性確保の理由もあります。

列挙型の定数値には、32 ビット整数の範囲内で値を割り当てあることができますが、__負の値は非効率__ なので使うべきではないとされています。


エイリアス値の定義 (allow_alias)
----

列挙値はデフォルトでは同じ値を持つことはできませんが、次のように __`allow_alias`__ オプションを `true` に設定することでエイリアス（別名）を定義できるようになります。

{{< code lang="proto" title="エイリアスの定義" hl_lines="2 4 5" >}}
enum EnumAllowingAlias {
  option allow_alias = true;
  EAA_UNSPECIFIED = 0;
  EAA_STARTED = 1;
  EAA_RUNNING = 1;
  EAA_FINISHED = 2;
}
{{< /code >}}

上記の `EAA_RUNNING` は、`EAA_STARTED` のエイリアスとして使用することができます。

