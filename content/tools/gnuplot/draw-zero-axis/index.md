---
title: "gnuplot: グラフに補助線を表示する"
url: "/p/556wrds"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 305
---

X=0, Y=0 の中心線 (axis) を表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set [no]zeroaxis     # x=0, x2=0, y=0, y2=0 軸の補助線を表示・非表示
set [no]xzeroaxis    # x=0  軸の補助線を表示・非表示
set [no]x2zeroaxis   # x2=0 軸の補助線を表示・非表示
set [no]yzeroaxis    # y=0  軸の補助線を表示・非表示
set [no]y2zeroaxis   # y2=0 軸の補助線を表示・非表示
show zeroaxis        # 補助線の表示状態を確認する
{{< /code >}}

X=0, Y=0 の中心線はデフォルトでは非表示です。

{{< code lang="gnuplot" title="例: x, y の中心線を表示" >}}
set zeroaxis
plot 1/(1+exp(-x)) - 0.5
{{< /code >}}

{{< image src="20040103-axis.gif" >}}


目盛りごとに補助線（グリッド）を表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set [no]grid   # グリッドの表示・非表示
show grid      # グリッドの表示状態を確認
{{< /code >}}

{{< code lang="gnuplot" title="例: グリッドを表示" >}}
set grid
plot log(x)
{{< /code >}}

{{< image src="20040103-grid.gif" >}}

