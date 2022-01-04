---
title: "gnuplot: 実行したコマンドのファイルへの保存・読み込み"
url: "/p/ncyuy3e"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 105
---

実行したコマンドをファイルへ保存
----

{{< code title="Syntax" >}}
save '<filename>'
{{< /code >}}

今まで実行したコマンドをファイルに保存したい場合は、**`save`** を使います。
ファイルの拡張子には **`.plt`** をつけることが多いみたいです。
上書きのチェックはしてくれないので注意してください。

{{< code lang="gnuplot" title="例: sample.plt にコマンドを保存" >}}
save 'sample.plt'
{{< /code >}}


ファイルからコマンドを読み込み
----

{{< code title="Syntax" >}}
load '<filename>'
{{< /code >}}

逆に保存したファイルを読み込んで実行するには、**`load`** を使います。

{{< code lang="gnuplot" title="例: sample.plt を実行" >}}
load 'sample.plt'
{{< /code >}}
