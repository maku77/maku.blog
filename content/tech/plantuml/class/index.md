---
title: "PlantUML でクラス図を作成する"
url: "/p/tn6y85z"
date: "2019-03-07"
tags: ["UML", "ツール"]
weight: 2
---

クラスの箱を描く
----

### 基本

クラスの箱を描くには、**`class`** キーワードを使用します。
他にも、**`interface`** でインタフェース、**`abstract`** で抽象クラス、**`enum`** で列挙型を定義することができます。

{{< image-code src="box.svg" >}}
@startuml
class クラス
interface インタフェース
abstract 抽象クラス
enum 列挙型
@enduml
{{</ image-code >}}

### クラス名にスペースや記号を含んでいる場合

クラス名やインタフェース名にスペースや記号を含めたい場合は、その文字列全体をダブルクォート **`""`** で囲みます。
さらに、**`as`** で別名を付けておくと、あとから参照しやすくなります。

{{< image-code w="350px" src="quoted.svg" >}}
@startuml
class "This is a class A" as A
class "This is a class B" as B
A -> B
@enduml
{{</ image-code >}}


### ステレオタイプ

クラス名の後ろに **`<<ステレオタイプ>>`** と記述することで、クラス名の上部に任意のステレオタイプを表示することができます。
独自のステレオタイプを表示することができますし、複数のステレオタイプを表示することもできます。

{{< image-code w="300px" src="stereotype.svg" >}}
@startuml
interface インタフェース <<interface>>
abstract 抽象クラス <<abstract>>
class ほげほげ <<独自ステレオタイプ>>
class へむへむ <<Serializable>> <<Model>>
@enduml
{{</ image-code >}}


### クラス名の横の記号を非表示にする

クラス名の左側に表示される C の記号などを非表示するには、**`hide circle`** コマンドを使用します。
下記のようにすると、クラスやインタフェースの記号がすべて非表示になります。
インタフェースに `<<inteface>>` というステレオタイプを表示するのであれば、記号は冗長なので消しておいた方がよいかもしれません。

{{< image-code w="300px" src="hide-circle1.svg" >}}
@startuml
hide circle

class MyClass
interface MyInterface <<interface>>
abstract MyAbstract
enum MyEnum
@enduml
{{</ image-code >}}

記号を非表示にするクラスの種類を絞り込むこともできます。
下記の例では、通常のクラス (`class`) の記号と、ステレオタイプ `<<interface>>` のついたクラスの記号を非表示にしています。
他にも、抽象クラス (`abstract`)、列挙型 (`enum`)、任意のクラス名の記号を非表示にすることもできます。

{{< image-code w="300px" src="hide-circle2.svg" >}}
@startuml
hide class circle
hide <<interface>> circle

class MyClass
interface MyInterface <<interface>>
abstract MyAbstract
enum MyEnum
@enduml
{{</ image-code >}}


### クラス名の横に独自の記号を表示する

クラスを表す C 記号や、インタフェースを表す I 記号はデフォルトで表示されますが、任意の記号を任意の色で表示することもできます。
独自の印を表示するには、クラスを定義するときに **`<< (記号, 色) >>`** というフォーマットでステレオタイプを付加します。

{{< image-code w="300px" src="original-circle.svg" >}}
@startuml
class DeprecatedClass << (D, red) >>
class PlayerView << (V, #00FF00) View >>
@enduml
{{</ image-code >}}


関係を示す線を引く
----

### シンプルな関連の線

2 つのクラス名をハイフンで繋ぐことで、関連 (association) があることを示すことができます。
ハイフン 1 つなら縦に、ハイフン 2 つなら横に並べて配置されます。

- `-`: 横方向に並べる
- `--`: 縦方向に並べる

{{< image-code w="300px" src="relation1.svg" >}}
@startuml
ClassA - ClassB
ClassC -- ClassD
@enduml
{{</ image-code >}}

ハイフン 2 つ以上は縦方向にクラスが並びますが、ハイフンが増えるごとに線の長さが伸びtていきます。

{{< image-code w="300px" src="relation2.svg" >}}
@startuml relation2
ClassA -- ClassB
ClassC --- ClassD
ClassE ---- ClassF
ClassG ----- ClassH
@enduml
{{</ image-code >}}

ただし、基本はハイフン 2 つを使っておいて、配置は PlantUML の最適化にまかせることが推奨されています。

依存の方向を示す矢印を表示する方法は下記で説明しますが、上記のように単純な線でクラスを結んだ場合は、**誘導可能性が未知、あるいは双方向の依存がある**ということを意味します。
簡単に言えば、とりあえずなんらかの関連がある、ということを示すということですね。


### 矢印の種類

下記のように依存の方向を表す矢印（誘導可能性矢印: navigability arrows）を表示することができます。
両端に同様の記号を配置すれば、双方向の矢印として表示することができます。
ハイフンやドットの数を 1 つにすれば、クラスを横方向に並べることができますが、ここでの紹介例ではすべて縦方向に並べています。

- `-->`: 関連 (Association) ※長期的な依存
- `..>`: 依存 (Dependency) ※短期的な依存
- `o--`: 集約 (Aggregation)
- `*--`: 合成、コンポジション (Composition)
- `<|--`: 汎化 (Extension)
- `<|..`: 実現 (Realization)

{{< image-code src="arrow1.svg" >}}
@startuml
A --> B
C ..> D
E o-- F
G *-- H
@enduml
{{</ image-code >}}

{{< image-code src="arrow2.svg" >}}
@startuml
abstract Parent
interface MyInterface
Parent <|-- ChildA
Parent <|-- ChildB
MyInterface <|.. ImplA
MyInterface <|.. ImplB
@enduml
{{</ image-code >}}


### 多重度と関連のラベル（制約）

次のようにして、線の両端に多重度を表示したり、関連の意味や制約を示すラベルを表示したりすることができます。

{{< image-code src="arrow-label1.svg" >}}
@startuml
BookShelf "1" o-- "*" Book : contains
@enduml
{{</ image-code >}}

ラベルの先頭か末尾に `<` や `>` を付けることでラベルに方向を表示することができます。

{{< image-code src="arrow-label2.svg" >}}
@startuml
Driver - Car : drives >
@enduml
{{</ image-code >}}

下記の例では、Customer が順序のある Purchase を保持していることを示しています。

{{< image-code src="arrow-label3.svg" >}}
@startuml
Customer --> "1..* {ordered}" Purchase
@enduml
{{</ image-code >}}


### 関連間の制約

関連の線の間に `{ xor }` 制約などを表示することは PlantUML ではまだできないっぽいです。
コメントなどで対応するしかなさそうです。


属性を定義する
----

### メソッドやフィールド（属性）を追加する

クラスにメソッドやフィールドを追加していくには、クラス名の後ろに `{}` で囲んだブロックを作成します。

{{< image-code w="150px" src="attr.svg" >}}
@startuml
class MyClass {
  property1 : Int
  property2 : String
  func1() : String
  func2(p1, p2) : Int
}
@enduml
{{</ image-code >}}


### メソッドやフィールドの可視性 (Visibility)

メソッドやフィールドの先頭に、`+`、`#`、`-`、`~` を付加することで、可視性を示すアイコンが表示されるようになります。

{{< image-code w="230px" src="visibility1.svg" >}}
@startuml
class MyClass {
  +publicField
  #protectedField
  -privateField
  ~packagePrivateField
  +publicMethod()
  #protectedMethod()
  -privateMethod()
  ~packagePrivateMethod()
}
@enduml
{{</ image-code >}}

可視性を示すアイコンが分かりにくい場合は、最初に **`skinparam classAttributeIconSize 0`** というコマンドを実行しておけば、`+` や `-` 記号がそのまま表示されるようになります。

{{< image-code w="230px" src="visibility2.svg" >}}
@startuml
skinparam classAttributeIconSize 0

class MyClass {
  +publicField
  #protectedField
  -privateField
  ~packagePrivateField
  +publicMethod()
  #protectedMethod()
  -privateMethod()
  ~packagePrivateMethod()
}
@enduml
{{</ image-code >}}


### Static メソッドと Abstract メソッド

メソッドの前に、**`{static}`** モディファイアを付けることで静的メソッド (static method)、**`{abstract}`** モディファイアを付けることで抽象メソッド (abstract method) であることを示すことができます。

{{< image-code w="200px" src="static-and-abstract.svg" >}}
@startuml static-and-abstract
class MyClass {
  {static} staticField: String
  {abstract} abstractField: String
  {static} staticMethod(): void
  {abstract} abstractMethod(): void
}
@enduml
{{</ image-code >}}


### セパレータで自由に区切る

デフォルトでは、括弧の有無によってメソッドとフィールドがグルーピングされて表示されます。
下記のようなセパレータを使用することで、自由な位置に区切り線を入れて表示をグルーピングすることができます。

| セパレータ | 線の種類 |
| ---- | ---- |
| `..` | 破線 |
| `--` | ノーマル |
| `__` | 太線 |
| `==` | 二重線 |

{{< image-code w="130px" src="separator1.svg" >}}
@startuml
class ClassA {
  property1
  ..
  property2
  --
  property3
  __
  property4
  ==
  property5
}
@enduml
{{</ image-code >}}

セパレータの線の上にテキストを表示することもできます。

{{< image-code w="130px" src="separator2.svg" >}}
@startuml
class ClassB {
  property1
  .. dashed line ..
  property2
  -- normal line --
  property3
  __ heavy line __
  property4
  == double line ==
  property5
}
@enduml
{{</ image-code >}}


### フィールドとメソッドのどちらに割り振るかを制御する

デフォルトの振る舞いでは、末尾に `()` の付いた属性はメソッドだと認識され、それ以外はフィールドだと認識されます。
この振る舞いを強制的に変更するには、先頭に **`{field}`** や **`{method}`** モディファイアを付けます。

{{< image-code w="250px" src="field-and-method-modifier.svg" >}}
@startuml field-and-method-modifier
class MyClass {
  これは括弧がないのでフィールド
  {field} これは括弧があるけどフィールド()
  これは括弧があるのでメソッド()
  {method} これは括弧がないけどメソッド
}
@enduml
{{</ image-code >}}

セパレータを入れることでも同様の表現が可能なので、こちらの方がシンプルでよいかもしれません。

{{< image-code w="250px" src="field-and-method-modifier.svg" >}}
@startuml field-and-method-modifier2
class MyClass {
  これは括弧がないのでフィールド
  これは括弧があるけどフィールド()
  __
  これは括弧があるのでメソッド()
  これは括弧がないけどメソッド
}
@enduml
{{</ image-code >}}


### メソッドがない場合にメソッドの領域を非表示にする

フィールドやメソッドが存在しない場合、その表示領域を非表示にするには、**`hide empty fields`** コマンドや、**`hide empty methods`** コマンドを使用します。

{{< image-code src="hide-empty.svg" >}}
@startuml
hide empty fields
hide empty methods

class ClassA
class ClassB {
  field
}
class ClassC {
  method()
}
class classD {
  field
  method()
}
@enduml
{{</ image-code >}}


パッケージ
----

### クラスをパッケージ囲む

クラスを **`package`** ブロックで囲むことで、パッケージ内にクラスを表示することができます。
パッケージ名の後ろには背景色を指定することもできます。

{{< image-code src="package1.svg" >}}
@startuml package1
package util {
  class UtilClass1
  class UtilClass2
}
package main #acf {
  class MainClass1
  class MainClass2
}
UtilClass1 <-- MainClass1
UtilClass2 <-- MainClass2
@enduml
{{</ image-code >}}

### パッケージ間の矢印

パッケージも、クラスと同様に関連の矢印を引くことができます。

{{< image-code src="package2.svg" >}}
@startuml
package main {
  class Main1
}
package feature {
  class Feature1
  class Feature2
}
main --> feature
@enduml
{{</ image-code >}}

### パッケージのスタイル

パッケージ名の後ろに特定のステレオタイプを指定することで、パッケージのスタイル（形状）を変更することができます。
デフォルトは `<<Folder>>` を指定したものと同じ形状で表示されます。

{{< image-code src="package3.svg" >}}
@startuml
package node <<Node>> {
  class Class1
}
package rectangle <<Rectangle>> {
  class Class2
}
package folder <<Folder>> {
  class Class3
}
{{</ image-code >}}

{{< image-code src="package4.svg" >}}
@startuml
package frame <<Frame>> {
  class Class4
}
package cloud <<Cloud>> {
  class Class5
}
package database <<Database>> {
  class Class6
}
@enduml
{{</ image-code >}}

個々のパッケージのスタイルを指定する代わりに、先頭で **`skinparam packageStyle`** コマンドを実行することで、すべてのパッケージのスタイルを変更することができます。

{{< image-code src="package5.svg" >}}
@startuml package5
skinparam packageStyle rectangle
package package1 {
  class Class1
}
package package2 {
  Class1 -> Class2
}
package package3 {
  Class2 --> Class3
}
package package4 {
  Class3 .> Class4
}
@enduml
{{</ image-code >}}

ちなみに、上記のサンプルコードからも分かるように、すべてのクラスを `package` ブロック内で `class` キーワードを使って定義する必要はなく、`package` ブロック内で初めて登場したクラスは、自動的にそのパッケージ内に所属するものとみなされます。


名前空間
----

### 名前空間にクラスを配置する

名前空間 (`namespace`) の機能によりクラス名を修飾すると、同じ名前のクラスを 1 つのダイアグラム内に登場させることができます。
下記の例では、名前空間 `ns1` と `ns2` に、同じ名前のクラス `MyClass` を定義しています。

{{< image-code src="namespace1.svg" >}}
@startuml
namespace ns1 {
  class MyClass
}
namespace ns2 {
  class MyClass
}
ns1.MyClass -> ns2.MyClass
@enduml
{{</ image-code >}}

`namespace` キーワードを使った名前空間の定義は必須ではなく、`aaa.bbb.MyName` のように、ドットで区切られたクラス名を使用することで、自動的にその名前空間に所属するクラスとして定義されます。

{{< image-code src="namespace2.svg" >}}
@startuml
class ns1.MyClass
ns1.MyClass -> ns2.MyClass
ns2.MyClass -> ns3.MyClass
@enduml
{{</ image-code >}}


### 名前空間の機能を無効にする

上記の例のように、デフォルトではクラス名にドット (`.`) を含めることで自動的にそのクラスは名前空間に配置されてしまいます。
この機能を無効にして、クラス名にドットを含められるようにするには、**`set namespaceSeparator none`** コマンドを実行します。

{{< image-code src="namespace-none.svg" >}}
@startuml namespace-none
set namespaceSeparator none

class aaa.MyClass
aaa.MyClass -> bbb.MyClass
bbb.MyClass -> ccc.MyClass
@enduml
{{</ image-code >}}


### 名前空間のセパレータ文字を変更する

名前空間 (namespace) のセパレータ文字は、デフォルトではドット (`.`) になっていますが、**`set namespaceSeparator`** コマンドを使用して任意の文字列に変更することができます。
下記の例では、セパレータ文字列として `::` を設定しています。

{{< image-code src="namespace-separator.svg" >}}
@startuml
set namespaceSeparator ::

class X::Y::Foo {
  func()
}
X::Y::Foo -> X::Y::Z::Bar
@enduml
{{</ image-code >}}


注釈（コメント）を表示する
----

### クラスに直接注釈 (note) を付ける

**`note`** コマンドを使用すると、クラスの箱の上下左右の位置に注釈を表示することができます。

{{< image-code src="note1.svg" >}}
@startuml
class MyClass
note left of MyClass : 左に表示される注釈
note right of MyClass : 右に表示される注釈
note top of MyClass : 上に表示される注釈
note bottom of MyClass : 下に表示される注釈1
note bottom of MyClass : 下に表示される注釈2
@enduml
{{</ image-code >}}

`of MyClass` などのターゲット指定を省略すると、直前に登場したオブジェクトに対しての注釈だとみなされます。

{{< image-code src="note2.svg" >}}
@startuml
class Foo
note left : "注釈1"
Foo <-- Bar
note left : "注釈2"
note left : "注釈3"
@enduml
{{</ image-code >}}

次のようにして複数行に渡る注釈を記述することもできます。
改行位置はそのまま表示に反映されます。

{{< image-code src="note3.svg" >}}
@startuml
class Foo
note bottom of Foo
  これは複数行にわたる
  注釈です。改行の位置も
  そのまま反映されます。
endnote
@enduml
{{</ image-code >}}

### 注釈 (note) 定義しておいて後からクラスに付加する

**`note テキスト as 注釈名`** という形で注釈 (note) に名前を付けて定義しておくと、後から **`クラス名 .. 注釈名`** のような感じでクラスに割り当てることもできます。
この方法を使うと、同じ注釈を複数のクラスに適用することができます。

注釈の表示位置は次のような感じで、ある程度制御することができます。

| 記述 | 注釈の表示位置 |
| ---- | ---- |
| `注釈名 .. クラス名` | クラスの上 |
| `クラス名 .. 注釈名` | クラスの下 |
| `注釈名 . クラス名` | クラスの左 |
| `クラス名 . 注釈名` | クラスの右 |


{{< image-code src="note-define1.svg" >}}
@startuml
class Foo
class Bar
note "注釈1です" as note1
note "注釈2です" as note2
note "注釈3です" as note3

note1 . Foo
Foo .. note2
Bar .. note2
Bar . note3
@enduml
{{</ image-code >}}

複数行に渡る注釈を定義しておくこともできます（直接クラスに付加してしまう場合と同様です）。
改行位置はそのまま表示に反映されます。

{{< image-code src="note-define2.svg" >}}
@startuml
class Foo
note as note1
  これは複数行にわたる
  注釈です。改行の位置も
  そのまま反映されます。
endnote
Foo .. note1
@enduml
{{</ image-code >}}


### フィールドやメソッドに注釈を付ける

注釈を付けるターゲットとして、**`クラス名::フィールド名`** のように指定すると、フィールドやメソッドに対して注釈を付けることができます。

{{< image-code src="note-method.svg" >}}
@startuml
class Foo {
  field1
  method1()
}
note left of Foo::field1
  フィールドの注釈
endnote
note right of Foo::method1
  メソッドの注釈
endnote
@enduml
{{</ image-code >}}

ポジションの指定は `left` あるいは `right` のみで、複数行コメントの形 (`note`~`endnote`) で指定しないとうまくいかないようです。


### 関連（リンク）に対して注釈をつける

関連の線に注釈 (note) を付加したいときは、リンクの定義の直後に **`note on link`** コマンドを使用します。


{{< image-code src="note-on-link.svg" >}}
@startuml
Foo --> Bar
note on link : これは関連に対する注釈です

Foo -> Hoge
note on link #pink : <b>背景色</b>を変更できます

Hoge --> Hemu
note right on link
  <b>複数行に渡る</b>注釈を
  記述できます。
endnote
@enduml
{{</ image-code >}}


### 注釈テキストを HTML タグで装飾する

注釈内のテキストは、下記のような HTML 風タグで装飾することができます。
ダイアログの目的は設計情報を伝えることなので、あまり装飾にこだわりすぎないように、**`<b>`** による強調くらいにしておくのがよいでしょう。

| タグ | 意味 |
| ---- | ---- |
| `<b>` | 太字 |
| `<u>` | 下線 |
| `<i>` | イタリック |
| `<s>`, `<del>`, `<strike>` | 打ち消し線 |
| `<color:#AAAAAA>`, `<font color="#AAAAAA">` | フォント色 |
| `<size:nn>` | フォントサイズ |
| `<img:file>`, `<img src="file">` | 画像表示 |


{{< image-code src="note-decoration.svg" >}}
@startuml
class Foo
note bottom of Foo
  注釈テキストは<b>HTMLタグ</b>で装飾できます。
  <color:red>色の変更</color>などもできますが、
  <size:20>必要最小限</size>にしましょう。
endnote
@enduml
{{</ image-code >}}


レイアウト（配置）を調整する
----

クラスの配置はできるだけ PlantUML の自動レイアウトにまかせるべきですが、どうしてもよい配置にならないときがあります。
そのような場合は、下記のようなテクニックを使ってある程度は配置の調整を行うことができます。

### クラスの箱の配置順

クラスの箱は、基本的に登場した順に左上から配置されていきます。
クラス間の関連を表す矢印のハイフンが 1 つだと横方向、2 つ以上だと縦方向に配置されます。
依存関係によって PlantUML は最適な位置にクラスを配置していきますが、矢印とクラス配置の関係はおおよそ次のようになります。

| 矢印 | クラスの配置 |
| ---- | ---- |
| `->` | 左から右 |
| `<-` | 右から左 |
| `-->` | 上から下 |
| `<--` | 下から上 |

思った位置と逆に表示されてしまったら、クラスが登場する順序を逆にして、それに合わせて矢印の向きを逆転させるとうまくいくことが多いです。

{{< image-code src="layout1.svg" >}}
@startuml
A --|> B
B -> C
@enduml
{{</ image-code >}}

{{< image-code src="layout2.svg" >}}
@startuml
B <|-- A
C <- B
@enduml
{{</ image-code >}}


### 矢印の方向

矢印の内側に **`left`**、**`right`**、**`up`**、**`down`** と記述することで、その方向に矢印が向かうように配置を調整することができます。

{{< image-code src="layout3.svg" >}}
@startuml
Center -up-> Up
Center -left-> Left
Center -right-> Right
Center -down-> Down
@enduml
{{</ image-code >}}


### 見えない関連の線を引いて配置

次の例では、6 つのクラスを自動レイアウトで配置していますが、無駄に横長に配置されてしまっています。

{{< image-code src="hidden-line1.svg" >}}
@startuml hidden-line1
class A
A -> B
C -> D
E --> F
@enduml
{{</ image-code >}}

クラス A と B の下に、C と D を配置できればもっとコンパクトに表示できるはずです。
このようなケースでは、**`[hidden]`** キーワードを使って、A と C の間に見えない関連の線（縦方向の線）を引いてやることで、うまく A と C が縦に並ぶようになります。

{{< image-code src="hidden-line2.svg" >}}
@startuml hidden-line2
class A
A -> B
A -[hidden]- C
C -> D
E --> F
@enduml
{{</ image-code >}}


関連クラス
----

クラスをある 2 つのクラス A, B の関連クラスとして表示するには、**`(クラスA, クラスB) .. 関連クラス名`** という記述を使います。

クラス間の接続では `クラス .. クラス` と記述していましたが、この片方を `(クラス, クラス)` というペアの記述に変えたものだと考えると分かりやすいです。

{{< image-code src="relation-class1.svg" >}}
@startuml
Student "1" - "1" Course
(Student, Course) .. Enrollment
@enduml
{{</ image-code >}}

関連クラスの表示位置も、クラスの表示位置と同様にある程度制御することができます。

{{< image-code src="relation-class2.svg" >}}
@startuml
Student "1" - "1" Course
Enrollment .. (Student, Course)
@enduml
{{</ image-code >}}


全体のカラー（配色）を変更する
----

**`skinparam`** コマンドを使って、クラス図全体の配色をまとめて変更することができます。


{{< image-code src="skinparam1.svg" >}}
@startuml
skinparam classBackgroundColor #FFFFFF
skinparam classBorderColor #black
skinparam classArrowColor #black
skinparam classFontColor #blue
skinparam classFontSize 20
skinparam classFontName Helvetica
' 可視性をアイコンではなく文字で表示
skinparam classAttributeIconSize 0
' 影をなくす
skinparam shadowing false

Foo <|-- Bar
@enduml
{{</ image-code >}}

`skinparam` コマンドはブロック化することで、プレフィックス部分を省略して記述することができます。

{{< code >}}
skinparam class {
  BackgroundColor #FFFFFF
  BorderColor #black
  ArrowColor #black
  FontColor #blue
  FontSize 20
  FontName Helvetica
  AttributeIconSize 0
}
skinparam shadowing false
{{< /code >}}

シンプルにグレースケール表示にしたい場合は、**`skinparam monochrome true`** とするのが手っ取り早いです。

{{< image-code src="skinparam2.svg" >}}
@startuml
skinparam monochrome true
' 可視性をアイコンではなく文字で表示
skinparam classAttributeIconSize 0
' 影をなくす
skinparam shadowing false

Foo <|-- Bar
@enduml
{{</ image-code >}}

一連の `skinparam` の定義は別ファイル (.iuml) に保存してしまい、プロジェクト全体でインクルード (**`!include`**) して使用するのがよいでしょう。
ネット上のファイルをインクルード (**`!includeurl`**) することもできます。

{{< code title="skinparams.iuml" >}}
skinparam class {
  BackgroundColor #white
  BorderColor #black
  ArrowColor #black
  FontColor #black
  FontSize 20
  FontName Helvetica
  ' 可視性をアイコンではなく文字で表示
  AttributeIconSize 0
}

' 影をなくす
skinparam shadowing false
{{< /code >}}

{{< code title="使用例" >}}
@startuml
!include skinparams.iuml

class Foo
class Bar
@enduml
{{< /code >}}

- 参考: [PlantUML -- Changing colors and fonts](http://plantuml.com/ja/skinparam)

