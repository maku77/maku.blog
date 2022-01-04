---
title: "gnuplot: 棒グラフのプロット (with boxes)"
url: "/p/67swums"
date: "2004-01-18"
tags: ["gnuplot"]
weight: 103
---

棒グラフをプロット
----

棒グラフをプロットしたい時は、ラインスタイルに **`boxes`** を指定します。
範囲とボックスの幅を指定しない場合は、隣のボックスとくっつくように表示されます。

{{< code lang="gnuplot" title="例: 棒グラフのプロット" >}}
plot 'test.dat' with boxes
{{< /code >}}

{{< code title="test.dat" >}}
1 10
2 30
3 25
4 40
5 20
{{< /code >}}

{{< image src="20040418-boxes1.gif" >}}

範囲を指定しないと、上のように見にくいグラフになってしまうので、棒グラフを表示する場合は範囲指定を行うか、マージンなどを設定するとよいです。

{{< code lang="gnuplot" title="例: 範囲を指定してプロット" >}}
plot [0:6] [0:50] 'test.dat' with boxes
{{< /code >}}

{{< code lang="gnuplot" title="例: マージンを設定してプロット" >}}
set yrange [0:]
set offset 1, 1, 1, 0  # left, right, top, bottom の順 (後ろの方は省略できる)
plot 'test.dat' with boxes
{{< /code >}}

{{< image src="20040418-boxes2.gif" >}}


ボックスの幅を指定する
----

{{< code lang="gnuplot" title="Syntax" >}}
set boxwidth <width>   # ボックスの幅を指定
set boxwidth           # ボックスの幅を自動調整に戻す
{{< /code >}}

{{< code lang="gnuplot" title="例: ボックスの幅を 0.7 にする" >}}
set boxwidth 0.7
plot [0:6] [0:50] 'test.dat' with boxes
{{< /code >}}

{{< image src="20040418-boxes3.gif" >}}

さらに、`with lines` で線を引いてやれば度数分布多角形を表示することができます。

{{< code lang="gnuplot" title="例: 度数分布多角形を表示 (続けて実行すること)" >}}
replot 'test.dat' with lines
{{< /code >}}

{{< image src="20040418-boxes4.gif" >}}


関数の棒グラフ表示
----

関数を棒グラフ表示することもできます （あんまり使い道ないかもしれないけど^^）。

{{< code lang="gnuplot" title="例: sin(x) の棒グラフ表示" >}}
plot [-pi:pi] sin(x) with boxes
{{< /code >}}

{{< image src="20040418-sin1.gif" >}}

関数の棒グラフのボックス幅を変更する場合は、次のようにしてサンプル数を変更してやります（サンプル数のデフォルトは 100）。
ボックス間の隙間を開けることはできないみたいです（たぶん）。

{{< code lang="gnuplot" title="例: サンプル数を 20 にした場合" >}}
set samples 20
plot [-pi:pi] sin(x) with boxes
{{< /code >}}

{{< image src="20040418-sin2.gif" >}}

