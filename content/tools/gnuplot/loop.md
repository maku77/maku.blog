---
title: "gnuplot: スクリプト内での有限ループの書き方"
url: "/p/92s2tau"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 405
---

ループ処理の回数を有限にするには、1 つのファイルでループ終了判定用変数を設定し、その中からループ処理を行うファイルを呼び出します。

{{< code lang="gnuplot" title="loop_test.gp" >}}
i = 0
call 'sub.gp'
{{< /code >}}

{{< code lang="gnuplot" title="sub.gp" >}}
i = i + 1
print i
if (i < 10) reread
{{< /code >}}

上の 2 つのファイルを用意し、 次のようにすると、1～10 までを表示します。

{{< code lang="gnuplot" >}}
call 'loop_test.gp'
{{< /code >}}

