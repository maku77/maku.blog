---
title: "GraphQL の仕様メモ"
url: "/p/jz78nmk"
date: "2021-07-26"
tags: ["GraphQL"]
---

GraphQL 関連用語
----

__クエリ文字列 (query string)__
: `query {...}` という文字列全体のこと。

__操作タイプ (operation type, root type)__
: `query {...}` などの `query` というキーワード部分のこと。操作の種類を表しており、`query` の他には `mutation` と `subscription` があります。

__操作名 (operation name)__
: `query HogeHoge {...}` などの `HogeHoge` の部分のこと。特に query 操作の場合は __query name__、mutation 操作の場合は __mutation name__ と呼ぶことがあります。操作名の指定は multi-operation documents じゃない限りオプショナルですが、ログ解析やデバッグをしやすくするために、名前を付けることが推奨されています。

__クエリ変数 (query variables)__
: `query User($name: String!) {...}` などの `$name` の部分のこと。

__クエリ引数 (query arguments)__
: `book(id: "xyz") {...}` など、フィールド参照時に指定する `id: "xyz"` の部分のこと。クエリに変数が渡されている場合は、ここで `book(id: $bookId)` のような感じで参照できます。

__選択セット、セレクションセット (selection sets)__
: `query HogeHoge {...}` などの `{...}` の部分のこと。どのフィールドを取得したいかを選択するので、選択セットと呼びます。

__フィールド (field)__
: GraphQL オブジェクトの各プロパティ部分のこと。例えば、`book { id title }` というクエリで Book オブジェクトを取得するときの `id` とか `title` の部分。大きく分けて 2 種類あり、Scalar types（末端ノードそれ自体が値）と、Object types（ネストされたフィールドから構成される）があります。

__入力オブジェクト型 (input object type, input types)__
: 引数専用のオブジェクト型のこと。文字列 (String) や数値 (Int) などのスカラー値と比べて、複雑なデータを渡せます。例えば、新規データの作成要求 (mutation) を行うときに、新規データの内容を表現するために使われます。定義方法自体は普通のオブジェクト型 (`type`) と同様ですが、入力型 (`input`) はそれ自身のデータを取得する用途には使えないところが異なります（つまり、クエリ内のフィールドとしては指定できないということです）。


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


フラグメント (Fragments)
----

[フラグメント](https://graphql.org/learn/queries/#fragments) は複数のオペレーションから再利用可能なセレクションセットです。
例えば、

```graphql
fragment basicUserInfo on User {
  name
  age
}
```

と定義すると、クエリのセレクションセットの __User オブジェクトの中で__、`...basicUserInfo` のように参照できるようになります。

```graphql
query {
  user(name: "maku") {
    ...basicUserInfo
  }
}
```

上記のクエリは、次のように記述するのと同様に振舞います。

```graphql
query {
  user(name: "maku") {
    name
    age
  }
}
```

{{< code lang="js" title="レスポンス" >}}
{
  "data": {
    "user" {
      "name": "maku",
      "age": 14
    }
  }
}
{{< /code >}}

フラグメントが役に立つのは、そのセレクションセットを使いまわすときです。

```graphql
query {
  maku: user(name: "maku") {
    ...basicUserInfo
  }
  chiko: user(name: "chiko") {
    ...basicUserInfo
  }
}
```

上記のように単一クエリの中で同種のデータ（ここでは `user`）を要求する場合は、__エイリアス__（ここでは `maku` と `chiko`）を付けることで、レスポンス内のプロパティ名がコンフリクトしないようにします。

{{< code lang="js" title="レスポンス" >}}
{
  "data": {
    "maku": {
      "name": "maku",
      "age": 14
    },
    "hemu": {
      "name": "chiko",
      "age": 5
    },
  }
}
{{< /code >}}


{{% private %}}
インライン・フラグメント
----

名前を持たないフラグメントのことです。
セレクションセットの中に直接記述します。

```graphql
... on Workout {
  name
  reps
}
```

Union 型の中で使用されますが、Union 型では名前付きのフラグメントも使用できます。

```graphql
query today {
  agenda {
    ...workout
    ...study
  }
}

fragment workout on Workout {
  name
  reps
}

fragment study on StudyGroup {
  name
  subject
  students
}
```
{{% /private %}}


イントロスぺクション
----

GraphQL サーバーに対して次のようなクエリを投げると、どんなフィールドを指定できるかを調べることができます。

```graphql
__schema { ... }
__type(name: "User") { ... }
```


{{% private %}}
スキーマ定義
----

- type ... 独自オブジェクト (custom object) を表現する。
- スキーマ定義は拡張子 `.graphql` のファイルで行う。
- 型の末尾に `!` が付いているものは Non-null フィールド。それ以外は Nullable フィールド。
- カスタム・スカラタイプの定義方法
    - `scalar DateType`
- Enum 型の定義方法
    - `enum PhotoCategory { ... }`
{{% /private %}}

