---
title: "gnuplot: 2次元プロットの基本"
url: "/p/ffyw55u"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 102
---

関数をプロットする
----

{{< code lang="gnuplot" title="Syntax" >}}
plot <expression> [with <line_style>]
{{< /code >}}

* line_style 一覧
    * `lines` -- 線だけ (関数をプロットする時のデフォルト)
    * `points` -- 記号だけ (ファイルのデータをプロット時のデフォルト)
    * `linespoints` -- 線と点

{{< code lang="gnuplot" title="例: y = 0.5x + 1" >}}
plot 0.5*x + 1
{{< /code >}}

{{< image src="20040103-plot1.gif" >}}


複数の関数をプロットする
----

{{< code lang="gnuplot" title="Syntax" >}}
plot <expression> [, <expression>...]
{{< /code >}}

複数のグラフを重ねたい場合は、 `plot` の後ろにカンマ (`,`) で区切って関数を並べます。
一行が長くなってしまう場合は `\` の次に改行すれば複数行に渡ってコマンドを書くことができます。


最後に実行した plot を再び実行する
----

{{< code lang="gnuplot" title="Syntax"  >}}
replot [<expression>]
{{< /code >}}

`replot` と入力すると、最後にプロットしたグラフを再表示します。
`replot` の引数に別の関数を指定すれと、 最後に実行した `plot` の出力に重ねてグラフを表示することができます。
連続して `replot` を実行すればどんどんグラフが重なっていきます。
`replot` は主に環境設定を変更した際に使用します。

{{< code lang="gnuplot" title="例: sin(x) と cos(x) と tan(x) を重ねたグラフを描画"  >}}
plot sin(x)
replot cos(x)
replot tan(x)
{{< /code >}}

次のようにしても同じ

{{< code lang="gnuplot" >}}
plot sin(x), cos(x), tan(x)
{{< /code >}}


ファイルのデータをプロットする
----

{{< code lang="gnuplot" title="Syntax" >}}
plot '<filename>' [with <line_style>]
{{< /code >}}

{{< code lang="gnuplot" title="例: ファイルからデータを読み込んで表示"  >}}
plot 'sample.dat' with linespoints
{{< /code >}}

{{< code tilte="sample.dat" >}}
# x    y
10     412.4
20     234.2
40     301.3
{{< /code >}}

1行に1つの点を表す座標を書きます。
データは空白かタブで区切ります。
`#` 以降その行はコメントと見なされます。
**データの間に空行を入れると、空行で分けられた点と点はプロットした際に線で結ばれなくなります。**

{{< image src="20040103-plot2.gif" >}}


ファイル内の複数のデータをプロットする
----

{{< code lang="gnuplot" >}}
plot "test.dat" using 1:2 with lines,\
     "test.dat" using 1:3 with lines,\
     "test.dat" using 1:4 with lines
{{< /code >}}

{{< code title="test.dat" >}}
# x  y1  y2  y3
  1   0   0   0
  2  14  19  24
  3  50  60  70
{{< /code >}}

x 座標を共有する、複数のデータをまとめて上のようなフォーマットで記述することができます。
`using 1:2` は 1 列目のデータ (x) と、 2 列目のデータ (y1) を使用することを示しています。
つまり、上の例では 3 本の線が引かれることになります。

{{< image src="20040103-plot3.gif" >}}


プロットデータを直接入力する
----

{{< code title="Syntax" >}}
plot '-' [with line_style]
[x1] y1
[x2] y2
[x3] y3
  :
e
{{< /code >}}

x 座標を省略すると、0, 1, 2 ... が指定されたと見なされます。

{{< code title="例" >}}
plot '-' with linespoints
0 0
1 12
2 34
3 20
e
{{< /code >}}

{{< image src="20040103-plot4.gif" >}}

