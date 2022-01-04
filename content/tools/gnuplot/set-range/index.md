---
title: "gnuplot: グラフの描画範囲を指定する"
url: "/p/afkbjc6"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 307
---

描画する線の範囲を指定する
----

{{< code lang="gnuplot" title="Syntax" >}}
plot [x_min : x_max] [y_min : y_max] <expression>                   # 二次元の場合
splot [x_min : x_max] [y_min : y_max] [z_min : z_max] <expression>  # 三次元の場合
set xrange [min:max]
set yrange [min:max]
set zrange [min:max]
{{< /code >}}

{{< code lang="gnuplot" title="例: [-2π, 2π] の sin(x) のグラフを表示" >}}
plot [-2*pi:2*pi] sin(x)
{{< /code >}}

{{< image src="20040103-range.gif" >}}

開始座標だけを指定したり、終了座標だけを指定したりすることもできます。

{{< code lang="gnuplot" title="例: x軸の範囲を 0 からにする" >}}
plot [0:] sin(x)  # set xrange [0:] ; plot sin(x) と同じ
{{< /code >}}

{{< code lang="gnuplot" title="例: x軸の範囲を π までにする" >}}
plot [:pi] cos(x)  # set xrange [:pi] ; plot cos(x) と同じ
{{< /code >}}

{{< code lang="gnuplot" title="例: y軸の範囲を [-π,π] にする" >}}
plot [] [-pi:pi] sin(x**2)  # set yrange [-pi:pi] ; plot sin(x**2) と同じ
{{< /code >}}


プロットのオフセットを設定
----

{{< code lang="gnuplot" title="Syntax" >}}
set offset <left>, <right>, <top>, <bottom>  # マージン設定
show offset                                  # マージン表示
{{< /code >}}

{{< code lang="gnuplot" title="例: オフセットを設定" >}}
set offset 2, 2, 1, 1
set grid
plot [-2*pi:2*pi] cos(x)
{{< /code >}}

{{< image src="20040103-offset.gif" >}}


グラフのマージンを設定する
----

{{< code lang="gnuplot" title="Syntax" >}}
set lmargin left    # 左の余白を指定（文字数で指定）
set rmargin right   # 右の余白を指定（文字数で指定）
set tmargin top     # 上の余白を指定（文字数で指定）
set bmargin bottom  # 下の余白を指定（文字数で指定）
{{< /code >}}

グラフの外形だけを表示したい場合などは、タイトルやラベルなどを表示するためのマージンは必要ないので、次のようにすると余計な空白をなくすことができます。

{{< code lang="gnuplot" title="例: マージンを 0 にする" >}}
set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set nokey
plot [0:2*pi] sin(x)**2
{{< /code >}}

{{< image src="20040103-zero_margin1.gif" >}}

あれれ、右と上の枠がなくなってしまいました（＞＿＜ こんな場合はしょうがないので、`rmargin` と `tmargin` を 1 にして次のようにします。

{{< code lang="gnuplot" title="例: できるだけマージンを小さくする" >}}
set lmargin 0
set rmargin 1
set tmargin 1
set bmargin 0
set nokey
plot [0:2*pi] sin(x)**2
{{< /code >}}

{{< image src="20040103-zero_margin2.gif" >}}

目盛りの表示内容が 2 行以上に渡る場合などは、`bmargin` を少し広げてやらないと、表示が切れてしまうことがあります。

{{< code lang="gnuplot" title="例: bmargin を少し広げる" >}}
set xtics ( \
    "5sec\n10sec\n15sec"   5, \
    "10sec\n15sec\n20sec" 10, \
    "15sec\n20sec\n25sec" 15)
set bmargin 4
plot "-" with linespoints
5 10
10 12
15 17M
E
{{< /code >}}

{{< image src="20040103-bmargin.gif" >}}

