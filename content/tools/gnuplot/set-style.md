---
title: "gnuplot: 線のスタイル、ポイントのスタイルを変更する"
url: "/p/e4vuhjy"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 308
---

線のスタイルを変更する
----

{{< code lang="gnuplot" title="Syntax" >}}
plot <expression> with <line_style>
{{< /code >}}

線のスタイルを変更するには `plot` のオプションで **`with`** を使います。
`with` の後ろには次のようなものを指定できます。
括弧は省略形を示しています。

* `boxes` (b) -- 箱（サブスタイル: `linesize` (ls), `linetype` (lt)）
* `boxerrorbars` -- 箱とエラーバー
* `dots` (d) -- 点
* `errorbars` (e) -- エラーバー
* `impulses` (i) -- 縦棒
* `lines` (l) -- 線
* `linespoints` -- 線と記号
* `points` (p) -- 記号（サブスタイル: `pointtype` (pt), `pointsize`(ps)）
* `steps` (s) -- 階段状

線種ごとにさらにサブスタイルを指定することができます。
例えば、記号でプロットする場合（`with points` (p) を指定した場合）は、オプションとして **`pointtype (pt)`** と **`pointsize (ps)`** を指定することができます。

例えば、記号のサイズを 3 にする場合は、`plot` コマンドのオプションとして **`with p ps 3`**（省略形での指定）を指定します。
線種や記号の種類にどんなものが指定できるかは、**`help with`** コマンドで調べることができます。


ポイント（記号）のスタイルを変更する
----

{{< code lang="gnuplot" title="Syntax" >}}
plot 'test.dat' with linespoints pointtype <pointType>
plot 'test.dat' with linespoints pt <pointType>
{{< /code >}}

線のスタイルとして、ポイントを伴う **`linespoints`** などを指定した場合は、さらに、サブスタイルとして **`pointtype (pt)`** を指定できます。

{{< code lang="gnuplot" title="例: ポイントのタイプを 5 に変更" >}}
plot 'test.dat' with linespoints pointtype 5
{{< /code >}}

