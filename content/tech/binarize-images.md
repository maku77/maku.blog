---
title: "画像の2値化の閾値を求める方法いろいろ"
url: "/p/uyp4geh"
date: "2001-05-25"
tags: ["画像処理"]
---

* <b>平均法</b>（平均を取るだけ）← 使えない
* <b>Pタイル法</b>（物体・背景の面積が分かっている場合）← 自動ではない
* <b>モード法</b>（極小を求める）← グラフを滑らかにする必要がある
* <b>判別分析法</b>（クラス間分散が最大となるところで２値化する）← Good
