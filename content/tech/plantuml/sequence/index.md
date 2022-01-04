---
title: "PlantUML でシーケンス図を作成する"
url: "/p/n8p7qmw"
date: "2018-10-12"
tags: ["UML", "ツール"]
working: true
weight: 3
---

シーケンス図は相互作用図 (interaction diagram) の代表的なダイアグラムです。
シーケンス図は、特定のユースケースに関するオブジェクトの典型例と、オブジェクト間のメッセージを示します。


要素の並び順の制御 (participant)
----

必須の定義ではないですが、`participant` であらかじめ要素を列挙しておくと、その後のメッセージ定義の順序に関係なく、`participant` に並べた順に左から要素が配置されます。

{{< image-code src="participant.svg" >}}
@startuml
participant Class1
participant Class2
participant Class3
Class3 -> Class1
Class2 -> Class3
Class1 -> Class2
@enduml
{{< /image-code >}}

`participant` の代わりに **`actor`** というキーワードを使用すると、アクターのシンボルを表示することができます。

{{< image-code src="actor.svg" >}}
@startuml
actor Class1
participant Class2
participant Class3

Class3 -> Class1
Class2 -> Class3
Class1 -> Class2
@enduml
{{</ image-code >}}


同期メッセージと非同期メッセージ、リターン
----

オブジェクト間のメッセージは、下記のように描き分けることができます。

- `->`: 同期メッセージ
- `-->`: 戻り値（リターン）
` `->>`: 非同期メッセージ

同期メッセージには戻り値（リターン）(`-->`) がありますが、UML の仕様としては省略することができます。

{{< image-code src="message.svg" >}}
@startuml
A -> B : 同期メッセージ
A <-- B : リターン
A ->> C : 非同期メッセージ
@enduml
{{</ image-code >}}


ライフラインの活性区間 (activation) と終了
----

ライフラインの上に描かれる長方形は「**活性区間 (activation)**」で、その区間の間はオブジェクトがアクティブな状態になっていることを表します。
活性区間は、**`activate`** と **`deactivate`** で示します。

ライフラインの終了（生存期間の終わり）を示すには、`deactivate` の代わりに **`destory`** で示します（×が表示されます）。

{{< image-code src="activate.svg" >}}
@startuml
A -> B : DoWork
activate B
B -> C : DoWork
activate C
C --> B : Done
deactivate C
B --> A : Done
destroy B
@enduml
{{</ image-code >}}


参加者 (participant) の生成と削除
----

参加者 (participant) の生成と削除には、**`create`** と **`destroy`** を使用します。

- `create <participant>`: オブジェクトを生成する
- `destroy <participant>`: オブジェクトを削除する

{{< image-code src="create-and-destroy.svg" >}}
@startuml
participant Alice
participant Bob
participant Carol

create Bob
Alice -> Bob: new
note left: 生成
activate Bob

create Carol
Bob -> Carol: new
Bob -> Carol: close
destroy Carol
note right: 他のオブジェクト\nによる削除

deactivate Bob
Alice <-- Bob: results
destroy Bob
note right: 自己削除
@enduml
{{</ image-code >}}

コンストラクタなどの処理を示すには、参加者 (participant) のボックスの下に接する形でアクティベーションボックスを描きます。

×マークに向かってメッセージの矢印が伸びているときは、他のオブジェクトによって削除されることを示します。
ライフライン（生存性）の最後に×マークがあるときは、そのオブジェクトが GC などで自動的に削除されることを示します。

インとアウトのメッセージ (incoming and outgoing messeage)
----

メッセージの両端に何も表示したくない場合は、左側の要素の代わりに `[`、右側の要素の代わりに `]` を指定します（何も指定せずに矢印だけ記述するとエラーになります）。

{{< image-code src="incoming-and-outgoing.svg" >}}
@startuml
[-> A: DoWork
activate A
A -> A: Internal call
activate A
A ->] : << createRequest >>
A <--] : RequestCreated
deactivate A
[<- A: Done
deactivate A
@enduml
{{</ image-code >}}

最初のメッセージのように、ソースが不確定なメッセージを **found message（見出されたメッセージ）** と呼びます（{{< amazon-inline id="4798107956" title="UMLモデリングのエッセンス" >}}より）。


メッセージのテキストに改行を入れる（複数行のメッセージ）
----

メッセージのテキスト内で改行したいときは `\n` を使用します。

{{< image src="new-line.svg" >}}

{{< code >}}
@startuml
Alice -> Alice: メッセージの\nテキスト内で\n\\n を使用すると\n改行できます
@enduml
{{< /code >}}


ループと条件分岐
----

シーケンス図は制御ロジックのモデル化には適していませんが、下記のような**相互作用フレーム**を使用することで、ループ処理や条件分岐を表現することができます。

- `alt ~ else ~ end`: ガード条件に一致した部分だけを実行する。
- `opt ~ end`: ガード条件に一致した場合のだけ実行する（上記の特殊バージョン）。
- `loop ~ end`: ガード条件に一致する限り繰り返し実行する。

{{< image-code src="frame.svg" >}}
@startuml
hide footbox

alt successful case
  Alice -> Bob: message
else some kind of failure
  Alice -> Bob: message
else Another type of failure
  Alice -> Bob: message
end

opt value > $10000
  Alice -> Bob: message
end

loop for each line item
  Alice -> Bob: message
end
@enduml
{{</ image-code >}}

インデントは自由にできるので、上記のように各ブロックの中をインデントして記述するとわかりやすくなります。
フレームは入れ子で使用することもできます。


ノート (note) を付ける
----

### メッセージのノート

メッセージの定義の次の行で、**`note left`** や **`note right`** を使用すると、そのメッセージに対してのノート（注釈）を表示することができます。

{{< image-code src="message-note.svg" >}}
@startuml
A -> B: message1
note left: メッセージの\n左側のノート

A <- B: message2
note right: メッセージの\n右側のノート

A -> B: message3
note right
複数行に渡る
記述も可能！
end note
@enduml
{{</ image-code >}}


### ライフライン（生存線）のノート

**`note left of`**、**`note right of`**、**`note over`** を使用すると、参加者（participant) に対するノートを表示することができます。

- `note left of <participant> [色]: テキスト`: ライフラインの左側にノートを表示
- `note right of <participant> [色]: テキスト`: ライフラインの右側にノートを表示
- `note over <participant> [色]: テキスト`: ライフラインに重ねる形でノートを表示

{{< image-code src="lifeline-note.svg" >}}
@startuml
participant Alice
participant Bob
Alice -> Bob: Long long long dummy message

note left of Alice
このノートは Alice の
ライフラインの左側に
表示されます。
end note

note right of Bob
このノートは Bob の
ライフラインの右側に
表示されます。
end note

note right of Alice: これは Alice の右側に\n表示されます。
note left of Bob: これは Bob の左側に\n表示されます。

note over Alice: これは Alice の上に\n重ねて表示されます。
note over Alice, Bob: これは Alice と Bob の上に\n重ねて表示されます。
@endumls
{{</ image-code >}}


### ノート (note) の背景色を変更する

ノートの色を設定することもできます。

{{< image-code src="note-color.svg" >}}
@startuml
A -> B: message1
note left #LightGreen: LightGreen な色のノート

A <- B: message2
note right #LightPink: LightPink な色のノート

A -> B: message3
note left #CEF
RGB 表記でもノート
の色は設定できるよ
end note
@enduml
{{</ image-code >}}


スタイル設定
----

### 下の参加者 (participant) ボックスを非表示にする

`hide footbox` を記述することで、下側のオブジェクトのボックスを非表示にすることができます。

{{< image-code src="hide-footbox.svg" >}}
@startuml
hide footbox
Alice -> Bob: message
Bob -> Carol: message
@enduml
{{</ image-code >}}

