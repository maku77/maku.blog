---
title: "gnuplot: 凡例（ラインの名前）を表示する"
url: "/p/zsdpkyg"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 303
---

凡例の名前を変える
----

{{< code lang="gnuplot" title="Syntax" >}}
plot <expression> title '<線の名前>'
{{< /code >}}

{{< code lang="gnuplot" title="例" >}}
plot 0.5*x title "my line"
{{< /code >}}

`title` オプションは、`with` オプションよりも前に指定する必要があります。

{{< image src="20040103-legend1.gif" >}}


凡例を消す
----

{{< code lang="gnuplot" >}}
set nokey
{{< /code >}}

全ての凡例を消すには `set nokey` を実行します。
線ごとに凡例を消すには `plot` のオプションで `notitle` を指定します。

{{< code lang="gnuplot" title="例: 特定のラインだけ凡例を消す" >}}
plot 0.5*x notitle, -0.5*x
{{< /code >}}

{{< image src="20040103-legend2.gif" >}}


凡例と線種の表示順を逆にする
----

{{< code lang="gnuplot" >}}
set key reverse
{{< /code >}}


凡例を四角で囲む
----

{{< code lang="gnuplot" >}}
set key box
{{< /code >}}

四角を消したくなった場合は `nobox` をセットします。

{{< code lang="gnuplot" >}}
set key nobox
{{< /code >}}


凡例の行間を変える
----

{{< code lang="gnuplot" >}}
set key spacing <倍率>
{{< /code >}}


凡例に表示する線の長さを変える
----

{{< code lang="gnuplot" >}}
set key samplen <長さ>
{{< /code >}}

線の長さは文字数で指定します。
例えば 10 と指定した場合は 10 文字分の長さになります。


凡例の表示位置を指定する
----

{{< code lang="gnuplot" title="Syntax" >}}
set key <y 座標>, <y 座標>
set key left | right | top | bottom
{{< /code >}}

{{< code lang="gnuplot" title="例: 凡例を (100, 100) に表示" >}}
set key 100,100
{{< /code >}}

{{< code lang="gnuplot" title="例: 凡例を右下に表示" >}}
set key right bottom
{{< /code >}}

