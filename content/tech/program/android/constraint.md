---
title: "Android開発: ContraintLayout で配置されたビューの実際のサイズを取得する"
linkTitle: "ContraintLayout で配置されたビューの実際のサイズを取得する"
url: "/p/khqpnjf"
date: "2020-07-27"
tags: ["Android"]
---

ConstraintLayout で配置したビューは、周囲のビューとの位置関係で位置やサイズなどが決まるため、実際に内部的なレイアウトが終了するまでは、ビューのサイズ情報などが取得できなかったりします（0 になります）。

__`View.doOnLayout()`__ などで、レイアウト完了するのを待ってからサイズを取得すればうまくいきます。
これは、`TransitionManager` などでレイアウトを切り替えた直後なども同様です。

{{< code lang="kotlin" >}}
// レイアウト変更＆アニメーション開始
TransitionManager.beginDelayedTransition(constraintLayout)
constraintSet.applyTo(constraintLayout)

// この直後にレイアウト情報取得してもうまく取得できない
val w = myView.width
val h = myView.height

myView.doOnLayout {
    // このタイミングであればうまく取得できる
    val w = it.width
    val h = it.height
}
{{< /code >}}

