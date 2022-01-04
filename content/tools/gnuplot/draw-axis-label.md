---
title: "gnuplot: 座標軸にラベルを表示する"
url: "/p/b3wwe4b"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 302
---

座標軸にラベルを表示する
----

{{< code title="Syntax" >}}
set xlabel "<label>" [x [, y]]
set ylabel "<label>" [x [, y]]
set zlabel "<label>" [x [, y]]
set x2label "<label>" [x [, y]]
set y2label "<label>" [x [, y]]
{{< /code >}}

{{< code lang="gnuplot" title="例: x軸の名前を「経過時間(ms)」にする" >}}
set xlabel "経過時間(ms)"
{{< /code >}}


好きな位置にラベルを表示する
----

{{< code lang="gnuplot" >}}
set label "<label>" x, y
{{< /code >}}


y 軸のラベルを回転させて表示する
----

{{< code lang="gnuplot" >}}
set ylabel="\rotate=270 ylabel"
{{< /code >}}

※拡張版 gnuplot のみ対応。

