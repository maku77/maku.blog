---
title: "Doxygen の基本的な使い方と初期設定"
url: "/p/ne3ocwx"
date: "2009-06-09"
tags: ["Doxygen"]
---

コマンド
----

{{< code title="設定ファイル Doxyfile を作成する" >}}
$ doxygen -g
{{< /code >}}

次のように `-s` オプションを付けて生成すれば、最小限のコメントを含んだ `Doxyfile` を生成できます。

```
$ doxygen -s -g
```

{{< code title="ドキュメントを生成する" >}}
$ doxygen [configName]
{{< /code >}}

※設定ファイル名を省略した場合は、`Doxyfile` が使用されます。



Doxyfile で最低限設定しておいた方がよさそうなタグ
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

{{< code title="下位ディレクトリも処理" >}}
RECURSIVE = YES
{{< /code >}}

