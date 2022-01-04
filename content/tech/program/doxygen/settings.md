---
title: "Doxygen の設定いろいろ (Doxyfile)"
url: "/p/x79fc82"
date: "2009-06-09"
tags: ["Doxygen"]
description: "Doxygen の設定ファイル (Doxyfile) でよく使用する設定のまとめです。"
---

基本的な設定
----

{{< code title="プロジェクト名を指定" >}}
PROJECT_NAME = MyProject
{{< /code >}}

{{< code title="HTML 形式のドキュメントだけを作成するように設定" >}}
GENERATE_HTML  = YES
GENERATE_CHI   = NO
GENERATE_LATEX = NO  ※これがデフォルトで YES になってる
GENERATE_RTF   = NO
GENERATE_MAN   = NO
{{< /code >}}

{{< code title="HTML 出力形式の設定" >}}
SOURCE_BROWSER = YES  # Files tab includes *.cpp files.
GENERATE_TREEVIEW = YES  # A side panel will be generated.
{{< /code >}}

{{< code title="doxygen コマンド実行時に warning 以外の出力を抑制する" >}}
QUIET    = NO
WARNINGS = YES
{{< /code >}}


指定したディレクトリ内のファイルを対象にする
----

{{< code title="例: カレントディレクトリ以下を対象にする（デフォルト）" >}}
INPUT =
RECURSIVE = YES
{{< /code >}}

{{< code title="例: 指定したディレクトリ以下を対象にする" >}}
INPUT = /home/john/src
RECURSIVE = YES
{{< /code >}}


特定のディレクトリを対象外にする
----

パターンに一致する名前のディレクトリをドキュメント化の対象から外すことができます。

{{< code title="例: test という名前のディレクトリを対象から外す" >}}
EXCLUDE_PATTERNS = */test/*
{{< /code >}}

パターンではなく、ディレクトリパスを明示する場合は、`EXCLUDE` タグを使用できます。


指定した拡張子のファイルを対象にする
----

{{< code title="例: .cpp と .h ファイルのみをドキュメント化の対象にする" >}}
FILE_PATTERNS = *.cpp *.h
{{< /code >}}


ドキュメントの出力先ディレクトリの設定
----

ドキュメントの出力先ディレクトリは、ドキュメントのファイル形式ごとに設定できます。
さらに `OUTPUT_DIRECTORY` を設定しておくと、ディレクトリのプレフィックスとして使用されます。
例えば、以下のようにすると、HTML ドキュメントは `doc/html` 以下に生成されるようになります。

{{< code >}}
HTML_OUTPUT = html
RTF_OUTPUT = rtf
LATEX_OUTPUT = latex
XML_OUTPUT = xml
MAN_OUTPUT = man

OUTPUT_DIRECTORY = doc
HTML_OUTPUT = html
{{< /code >}}


Doxygen コメントの付いていないメンバメソッドなども対象にする
----

```
EXTRACT_ALL = YES  # default:NO
```


ドキュメントにソースコードを含める
----

```
INLINE_SOURCES = YES
```

