---
title: "gnuplot サンプル: 関数のある点から x軸、y軸に向かって線を引く (cross_line.gp)"
url: "/p/arfdp4b"
date: "2002-12-04"
tags: ["gnuplot"]
weight: 403
---

{{< code lang="gnuplot" title="lib/cross_line.gp" >}}
#
# 関数のある点から x軸、y軸に向かって線を引きます。
# f(x) に対象の関数を指定し、x 座標を指定します。
#
# 使用例:
#  f(x) = sin(x)
#  call 'lib/cross_x.gp' 'pi/4'
#  plot [-pi:pi] f(x)
#
set arrow from $0, 0 to $0, f($0) nohead lt 3 lw 3
set arrow from 0, f($0) to $0, f($0) nohead lt 3 lw 3
set zeroaxis
{{< /code >}}

