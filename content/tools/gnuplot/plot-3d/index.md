---
title: "gnuplot: 三次元プロットの基本"
url: "/p/awbbari"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 104
---

三次元グラフの表示
----

{{< code title="Syntax" >}}
splot <expression>
{{< /code >}}

{{< code lang="gnuplot" title="例: z = x2 + 10y" >}}
splot x**2 + 10*y
{{< /code >}}

{{< image src="20040103-splot.gif" >}}


メッシュの細かさを指定する
----

{{< code title="Syntax" >}}
set isosample <x_rate>, <y_rate>
{{< /code >}}

デフォルトの細かさは 10, 10 です。

{{< code lang="gnuplot" title="例: メッシュの細かさを 20, 20 にする" >}}
set isosample 20, 20
{{< /code >}}


等高線を表示する
----

{{< code title="Syntax" >}}
set contour
{{< /code >}}

{{< code lang="gnuplot" title="例: z = -x2-y2 の等高線を表示" >}}
set contour
splot -x**2 - y**2
{{< /code >}}

{{< image src="20040103-contour.gif" >}}

