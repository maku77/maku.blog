---
title: "PlantUML でオブジェクト図を作成する"
url: "/p/t7cfj92"
date: "2018-10-15"
tags: ["UML", "ツール"]
working: true
weight: 4
---

オブジェクト図はクラス図と同様の文法で記述することができます。
下記のページも参考にしてください。

- [PlantUML でクラス図を作成する](/p/tn6y85z)


オブジェクト図の基本
----

オブジェクト図 (object diagram) は、システム内のオブジェクト群のある時点のスナップショットを表現します。
具体的なオブジェクト同士の関係を表現することができるため、複雑な依存関係を持つオブジェクト

インスタンス名は下線付きで「**<u>インスタンス名：クラス名</u>**」という形式で記述します。
インスタンス名とクラス名はどちらか一方を省略して記述することもできますが、クラス名だけを記述する場合は、「**<u>：クラス名</u>**」のようにコロン部分だけは残して記述します。

> オブジェクト図は、メッセージなしのコミュニケーション図と考えることもできます。
> ─『UMLモデリングのエッセンス第3版』マーチン・ファウラー

PlantUML で登場させるオブジェクト群は、**`object`** キーワードを使って定義します。
`as` を使って別名を付けておくことができます。

{{< image-code src="object.svg" >}}
@startuml
object object1
object "My second object" as o2
@enduml
{{</ image-code >}}


関連の線を引く
----

オブジェクト同士の依存関係を示すには、`--` でオブジェクト名（あるいは別名）を繋ぎます。
代わりに `..` を使用すると、破線 (dashed line) で繋ぐことができます。

{{< image-code src="object-lines1.svg" >}}
@startuml
object object1
object "Second object" as o2
object "Third object" as o3
object "Fourth object" as o4

object1 -- o2
object1 -- o3
object1 -- o4
o2 .. o3
o3 .. o4
@enduml
{{</ image-code >}}

ハイフンやドットの数を 1 つに減らすことで、オブジェクトを横方向に配置することができます。

{{< image-code src="object-lines2.svg" >}}
@startuml object-lines2
object object1
object "Second object" as o2
object "Third object" as o3
object "Fourth object" as o4

object1 -- o2
object1 -- o3
object1 -- o4
o2 . o3
o3 . o4
@enduml
{{</ image-code >}}


オブジェクト名に下線を引く
----

オブジェクト図らしく、オブジェクト名に下線を引くには下記のようなマークアップを使用します。

{{< image-code src="object-underline.svg" >}}
@startuml
rectangle "<u>tools: Organization</u>" as tools
rectangle "<u>John : Person</u>" as john
rectangle "<u>Don : Person</u>" as don
tools -- john
tools -- don
@enduml
{{</ image-code >}}

上記では、`object` キーワードの代わりに `rectangle` キーワードを使ってオブジェクトを定義しているため、属性部分が表示されず、単純な矩形としてオブジェクトが表示されています。


関連を示すテキストを追加する
----

関連の線に沿う形でテキストを表示するには下記のようにします。
それぞれのオブジェクトに近い位置、あるいは中央に配置できます。

{{< image-code src="object-relation.svg" >}}
@startuml
object Parent
object Child1
object Child2
object Child3

Parent "parent" -- Child1
Parent -- "child" Child2
Parent -- Child3 : family
@enduml
{{</ image-code >}}


属性を表示する
----

オブジェクトの属性（フィールド）を表示するには、下記のように、オブジェクトを定義した後で、`オブジェクト名 : フィールド` というフォーマットで記述していきます。

{{< image-code src="object-attr1.svg" >}}
@startuml
object "<u>tools: Organization</u>" as tools
object "<u>apps: Organization</u>" as apps
tools : location = "Chicago"
apps : location = "Saba"
@enduml
{{</ image-code >}}

複数の属性を表示したいときは、次のように属性定義の行を追加していくか、

{{< image-code src="object-attr2.svg" >}}
@startuml
object "<u>tools: Organization</u>" as tools
object "<u>apps: Organization</u>" as apps
tools : location = "Chicago"
tools : id = 100
apps : location = "Saba"
apps : id = 200
@enduml
{{</ image-code >}}

あるいは、`object` 定義時にブロックを付加して属性を列挙します。
属性が 1 つだけの場合も、このようにブロックを使って記述しちゃった方がまとまりがあって分かりやすいかもしません。

{{< code >}}
@startuml
object "<u>tools: Organization</u>" as tools {
  location = "Chicago"
  id = 100
}
object "<u>apps: Organization</u>" as apps {
  location = "Saba"
  id = 200
}
@enduml
{{</ code >}}


オブジェクト図のサンプル
----

最後に複雑なオブジェクト図のサンプルを載せておきます。

{{< image src="object-example.svg" >}}

{{< code >}}
@startuml
object "<u>engineering : Organization</u>" as engineering {
  location = "Boston"
}
object "<u>tools : Organization</u>" as tools {
  location = "Chicago"
}
object "<u>apps : Organization</u>" as apps {
  location = "Saba"
}
object "<u>Don : Person</u>" as don {
  location = "Champaign"
}
object "<u>John : Person</u>" as john {
  location = "Champaign
}

engineering "parent" -- tools
engineering -- apps
tools "parent" -- don
tools "parent" -- john
@enduml
{{</ code >}}

出典：『UMLモデリングのエッセンス第3版』マーチン・ファウラー著

