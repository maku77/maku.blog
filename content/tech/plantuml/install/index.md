---
title: "PlantUML の基本（インストール〜画像ファイルへの保存）"
url: "/p/opxhnho"
date: "2018-10-12"
tags: ["UML", "ツール"]
description: "PlantUML はテキストファイルから UML 図（PNG や SVG ファイル）を生成することができる Java 製のツールです。ここでは、plantuml というコマンドを使用できるようにするまでの手順を示します。"
weight: 1
---

{{< image w="200" src="install1.svg" >}}

Graphviz のインストール
----

PlantUML は図の生成のために**内部で Graphviz の dot コマンドを使用する**ので、先に Graphviz をインストールしましょう。

- [Graphviz - Graph Visualization Software](https://www.graphviz.org/)

ここでは、上記のサイトから ZIP ファイル `graphviz-2.38.zip` をダウンロードし、展開してできた `release` ディレクトリ内のファイル群を `C:\app\graphviz-2.38` というディレクトリ内に配置するとします。
Graphviz に含まれる `dot` コマンドのパスを環境変数 **`GRAPHVIZ_DOT`** に設定すれば Graphviz のインストールは完了です（この環境変数を PlantUML が参照します）。

{{< code title="環境変数 GRAPHVIZ_DOT の設定" >}}
GRAPHVIZ_DOT=C:\app\graphviz-2.38\bin\dot.exe
{{< /code >}}


PlantUML のインストール
----

下記から `plantuml.jar` をダウンロードして、適当なディレクトリに配置します。

- [PlantUML - plantuml.jar のダウンロード](http://plantuml.com/download)

ここでは、`C:\app\plantuml\plantuml.jar` というパスで配置することにします。
コマンドプロンプトから、下記のように実行して、特にエラーが発生しなければ OK です。

{{< code title="実行テスト" >}}
C:\> java -jar C:\app\plantuml\plantuml.jar -testdot
The environment variable GRAPHVIZ_DOT has been set to C:\app\graphviz-2.38\bin\dot.exe
Dot executable is C:\app\graphviz-2.38\bin\dot.exe
Dot version: dot - graphviz version 2.38.0 (20140413.2041)
Installation seems OK. File generation OK
{{< /code >}}


plantuml コマンドを簡単に使えるようにする
----

PlantUML を使って図を生成するために、毎回 `java -jar plantuml.jar xxx` と入力するのは面倒なので、`plantuml` というコマンドで一発で実行できるようにします。

`plantuml.jar` と同じディレクトリに、下記のようなバッチファイルを作成してください。

{{< code lang="bat" title="C:\app\plantuml\plantuml.cmd" >}}
@java -jar %~dp0\plantuml.jar -charset UTF-8 %*
{{< /code >}}

ついでに、UML ダイアグラムの中で日本語が文字化けしないように、`-charset` の指定も行っています。
最後に、上記のバッチファイルをどこからでも実行できるように、上記のパスを環境変数 `PATH` に追加しておきます。

{{< code lang="bat" title="環境変数 PATH の設定" >}}
PATH=%PATH%;C:\app\plantuml
{{< /code >}}

これで、次のようにどこのディレクトリからでも `plantuml` コマンドを実行できるようになります。

{{< code title="実行テスト（ヘルプの表示）" >}}
C:\> plantuml -h
{{< /code >}}


画像ファイルとして出力する
----

最後に `plantuml` コマンドを使って、UML ダイアグラムを画像ファイルとして出力するテストをしてみます。
まず、下記のようなシーケンス図のためのサンプルファイルを作成してください。

{{< code title="sequence.txt" >}}
@startuml
Alice -> Bob: Authentication Request
Bob --> Alice: Authentication Response
Alice -> Bob: Another authentication Request
Alice <-- Bob: another authentication Response
@enduml
{{< /code >}}

コマンドラインから下記のように実行すれば、`sequence.png` という名前の PNG ファイルが生成されます。

{{< code >}}
$ plantuml sequence.txt
{{< /code >}}

### SVG ファイルとして出力する

デフォルトでは PNG ファイルとして図が出力されますが、**`-tsvg`** オプションを付けることで SVG ファイルとして出力することができます。
下記のように実行すると、`sequence.svg` ファイルが生成されます。

{{< code >}}
$ plantuml -tsvg sequence.txt
{{< /code >}}

{{< image w="350" src="install2.svg" title="出力結果 (sequence.svg)" >}}

