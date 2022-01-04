---
title: "数学メモ: 平均二乗誤差など"
url: "/p/psotadn"
tags: ["数学"]
date: "2014-05-06"
use_math: true
---

二乗平均平方根 (RMS: Root Mean Square)
----

$$\operatorname{RMS}[x] = \sqrt{\frac{1}{n}\sum_{i=1}^n \left( x_i \right)^2}$$

各データの二乗をすべて足して平均をとり、さらに平方根をとったもの。

平均二乗平方根じゃなくて二乗平均平方根って言うんだね。


平均二乗誤差 (MSE: Mean Squared Error)
----

$$\operatorname{MSE}=\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2$$

* $y_i$ : 実際の値
* $\hat{y_i}$ : 予測値

予測値とのずれの二乗をすべて足して平均をとったもの。


平均二乗誤差平方根 (RMSE: Root Mean Squared Error) <!-- 2017-04-11 -->
----

$$\operatorname{RMSE}=\sqrt{\operatorname{MSE}}=\sqrt{\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2}$$

* $y_i$ : 実際の値
* $\hat{y_i}$ : 予測値

平均二乗誤差 (MSE: Mean Squared Error) の平方根。
実測値と予測値がどれほど異なっているかを表す。


### 標準偏差

標準偏差も基本的には RMSE と同じで、予測値の代わりに平均値を使っているだけ。

* <b>RMSE</b>: 実測値と予測値のずれ
* <b>標準偏差</b>: 実測値と（実測値の）平均値のずれ

$$\operatorname{標準偏差(\sigma)}=\sqrt{\frac{1}{n} \sum\_{i=1}^{n}(x_i - \bar{x})^2}=\sqrt{\frac{1}{n} \sum\_{i=1}^{n}x_i^2 - \bar{x}^2}$$

* $x_i$ : 実測値
* $\bar{x}$ : 実測値の平均値

