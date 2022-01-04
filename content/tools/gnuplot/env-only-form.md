---
title: "gnuplot サンプル: グラフの形だけ表示する環境設定 (only_form.env)"
url: "/p/h2swmzv"
date: "2002-12-05"
tags: ["gnuplot"]
weight: 402
---

{{< code lang="gnuplot" title="lib/only_form.env" >}}
#----------------------------
# グラフの形だけ表示する環境
#----------------------------
# マージンを枠が消えない程度になくす
set lmargin 0
set rmargin 1
set tmargin 1
set bmargin 0
# 目盛りを消す
set noxtics
set noytics
# 軸の名前を消す
set nokey
{{< /code >}}

