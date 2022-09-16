---
title: "GraphQL スキーマ仕様: 組み込みのスカラー型とユーザー定義の型"
url: "p/tw75a7p"
date: "2022-09-16"
tags: ["GraphQL"]
draft: true
---

5 つの組み込みスカラー型 (Scalar types)
----

{{% private %}}
- [Scalar types](https://graphql.org/learn/schema/#scalar-types)
{{% /private %}}

### 組み込みのスカラー型

| 型名 | 意味 |
| ---- | ---- |
| `Int` | 符号付き 32 ビット整数 |
| `Float` | 符号付き倍精度浮動小数点数 |
| `String` | 文字列（UTF-8 エンコーディング） |
| `Boolean` | `true` か `false` |
| `ID` | 一意の識別子。データ形式としては `String` と同様だが、`ID` はリーダブル（意味のある単語）にはなっていないことを示唆します。 |

その他はオブジェクト型、あるいはユーザー定義のスカラー型です。

### ユーザー定義の型

ユーザーが自由に定義できる型は、オブジェクト型、スカラー型 (scalar)、列挙型 (enums) の 3 種類です。

{{< code lang="graphql" title="スキーマ内での独自型の定義" >}}
# オブジェクト型
type Character {
  name: String!
  appearsIn: [Episode]!
}

# スカラー型
scalar Date

# 列挙型
enum Episode {
  NEWHOPE
  EMPIRE
  JEDI
}
{{< /code >}}


