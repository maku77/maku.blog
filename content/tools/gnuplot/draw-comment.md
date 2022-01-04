---
title: "gnuplot: グラフ内にコメントを表示する"
url: "/p/adxysxf"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 202
---

コメントを表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set label tag 'text' at [coordinate] x, [coordinate] y [justification]
{{< /code >}}

`tag` にはコメントを識別するための 1 以上の整数を指定します。

オプショナルパラメータの `coordinate` には、どのような座標系でコメント位置を指定するかを設定できます。

* `first` (default) -- x、y 軸の座標系
* `second` -- x2、y2 軸の座標系
* `graph` -- グラフの左下を (0, 0)、右上を (1, 1) とする座標系
* `screen` -- 表示全体の左下 (0, 0)、右上を (1, 1) とする座標系

`justification` パラメータでは、指定した座標のどちら側にコメントを表示するかを設定できます。

* `left` (default) -- 指定した座標の左側
* `right` -- 指定した座標の右側
* `center` -- 指定した座標の中央

{{< code lang="gnuplot" title="例" >}}
set label 1 "Label 1" at 5, 4
set label 2 "Label 2" at 10, 7
{{< /code >}}

