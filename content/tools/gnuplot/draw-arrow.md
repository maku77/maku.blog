---
title: "gnuplot: グラフ内に矢印や直線を表示する"
url: "/p/xfp42np"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 201
---

矢印・直線を表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set arrow <tag> from <sx> <sy> to <ex> <ey> [[no]head]            # 2次元の場合
set arrow <tag> from <sx> <sy> <sz> to <ex> <ey> <ez> [[no]head]  # 3次元の場合
{{< /code >}}

`nohead` オプションを付けると、矢印でなく直線になります。
`<tag>` は他のコマンドで対象となる矢印を指定するために 1 以上の値を指定します。

※プロットの範囲の中に矢印が収まっていないと変な表示になることがあります。

{{< code lang="gnuplot" title="例: 原点付近に十字を表示" >}}
set arrow 1 from 0, -3 to 0, 3 nohead
set arrow 2 from -3, 0 to 3, 0 nohead
{{< /code >}}


矢印を非表示にする
----

{{< code lang="gnuplot" title="Syntax" >}}
set noarrow <tag>
{{< /code >}}

`<tag>` には、`set arrow` で設定した数値を指定します。

