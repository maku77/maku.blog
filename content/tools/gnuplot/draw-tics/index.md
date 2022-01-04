---
title: "gnuplot: 軸の目盛り表示を調整する"
url: "/p/ezq5wy8"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 304
---

軸の目盛りの間隔、開始座標、終了座標を設定する
----

{{< code lang="gnuplot" title="Syntax" >}}
set xtics <start>, <incr>, <end>
set x2tics <start>, <incr>, <end>
set ytics <start>, <incr>, <end>
set y2tics <start>, <incr>, <end>
set ztics <start>, <incr>, <end>
{{< /code >}}

`start` と `end` を省略して、`incr`（ステップ数）だけを指定することもできます。

{{< code lang="gnuplot" title="例: x 軸の目盛りを 1 刻みで 0～5 の間に表示（ついでにグリッドも ON）" >}}
set xtics 0, 1, 5
set grid
plot [-10:10] x
{{< /code >}}

{{< image src="20040103-xtics.gif" >}}

あ、こうすると x=0 には目盛りが表示されないみたい。。。
x=0 にも表示したかったら -1, 1, 5 にしないとダメですね。


軸の目盛りを消す
----

{{< code lang="gnuplot" title="Syntax" >}}
set noxtics
set nox2tics
set noytics
set noy2tics
set noztics
{{< /code >}}

図形の形だけを示したい場合などは、軸の目盛りと、凡例を両方とも消してプロットします。

{{< code lang="gnuplot" title="例: 図形の形だけを示す" >}}
set noxtics
set noytics
set nokey
plot [0:2*pi] sin(x)
{{< /code >}}

{{< image src="20040103-notics.gif" >}}


任意の位置の目盛りにラベルを表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set xtics {align} ({"label"} pos, {"label"} pos ...)
{{< /code >}}

{{< code lang="gnuplot" title="例" >}}
set xtics ("Sun" 10, "Mon" 20, "Tue" 30, "Wed" 40, "Thu" 50, "Fri" 60, "Sat" 70)
{{< /code >}}

