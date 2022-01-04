---
title: "gnuplot: サンプル数を上げて強引に塗りつぶす（積分領域の表示）"
url: "/p/fyjpvb2"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 204
---

{{< code lang="gnuplot" title="強引な塗りつぶし表示" >}}
set samples 1000
plot [-pi:pi] sin(x) with impulse
{{< /code >}}

{{< image src="20040103-force_fill.gif" >}}

（追記: 2013-05-02）最近の gnuplot だと、こんな強引なことしなくても、`plot` するときに **`with filledcurve`** の指定をするだけで、塗りつぶし表示ができるみたいです。
しかも、凡例の表示もそれっぽく代わってくれます。

* 参考
    * http://gnuplot.sourceforge.net/demo_cvs/fillcrvs.html
    * http://gnuplot.sourceforge.net/demo_cvs/transparent.html

