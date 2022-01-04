---
title: "gnuplot: グラフにタイトルを表示する"
url: "/p/taoir3i"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 301
---

グラフにタイトルを入れる
----

{{< code title="Syntax" >}}
set title "<title>" [<x_offset>] [, <y_offset>]
{{< /code >}}

タイトルはシングルクォーテーション (`'`) で囲んでも構いませんが、改行 (`\n`) を入れる場合はダブルクォーテーション (`"`) で囲む必要があります。
タイトルはデフォルトでは表の上部中央に表示されますが、`x_offset` に移動させる文字数を指定することができます。
タイトルを消したいときは、`set title` を実行します。

{{< code lang="gnuplot" title="例: タイトルを「Normal Distribution (正規分布)」にする" >}}
normal_distribution(x, a, b) = exp(-((x-b)**2)/(2*(a**2))) / sqrt(2*pi)*a
set title "Normal Distribution"
plot normal_distribution(x, 3, 0)
{{< /code >}}

{{< image src="20040103-title.gif" >}}


タイトルを消す
----

{{< code lang="gnuplot" >}}
set title
{{< /code >}}

タイトルを消したい場合は、タイトル文字列を指定せずに `set title` を実行します。


複数行のタイトルを入れる
----

{{< code lang="gnuplot" >}}
set title "これは\n複数行にわたる\nタイトルだよ"
{{< /code >}}

タイトルに改行をいれるには `\n` を使います。
複数行のタイトルを指定する場合は、`'` (シングルクォーテーション) ではなくて `"` (ダブルクォーテーション) で囲む必要があります。

