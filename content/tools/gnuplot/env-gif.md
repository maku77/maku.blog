---
title: "gnuplot サンプル: ホームページ用の GIF を出力する環境設定ファイル (output_gif.env)"
url: "/p/idkxws2"
date: "2002-12-04"
tags: ["gnuplot"]
weight: 401
---

{{< code lang="gnuplot" title="lib/output_gif.env" >}}
#
# 使用例: output.gif というファイル名で出力する設定を行う場合
#
#     gnuplot> call 'lib/hpgif.gp' 'output.gif'
#
set terminal gif size 320, 240 transparent xffffff
#set terminal gif size 320, 240
#set samples 200
set output '$0'
set zeroaxis
{{< /code >}}

