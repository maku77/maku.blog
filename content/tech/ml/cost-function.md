---
title: "機械学習メモ: 評価関数（コスト関数）のまとめ"
url: "p/psotadn/"
tags: ["ml", "数学"]
date: "2014-05-06"
useMath: true
---

平均絶対誤差 (MAE: Mean Absolute Error)
----

$$
  \text{MAE} = \frac{1}{n} \sum_{i=1}^{n} |y_i - \hat{y}_i|
$$

<div style="text-align: right">
  \( y_i \) : 実際の値<br/>
  \( \hat{y_i} \) : 予測値
</div>

{{< code lang="python" title="NumPy での実装" >}}
mae = np.mean(np.abs(actual_values - predicted_values))
{{< /code >}}


平均二乗誤差 (MSE: Mean Squared Error)
----

$$
  \operatorname{MSE}=\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2
$$

<div style="text-align: right">
  \( y_i \) : 実際の値<br/>
  \( \hat{y_i} \) : 予測値
</div>

予測値とのずれの二乗をすべて足して平均をとったもの。


平均二乗誤差平方根 (RMSE: Root Mean Squared Error) <!-- 2017-04-11 -->
----

$$
  \operatorname{RMSE}=\sqrt{\operatorname{MSE}}=\sqrt{\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2}
$$

<div style="text-align: right">
  \( y_i \) : 実際の値<br/>
  \( \hat{y_i} \) : 予測値
</div>

平均二乗誤差 (MSE: Mean Squared Error) の平方根。
実測値と予測値がどれほど異なっているかを表します。

{{% note title="標準偏差" %}}
標準偏差も RMSE と似ていますが、予測値の代わりに平均値を使って計算します。
RMSE は「実測値と予測値のずれ」に着目し、標準偏差は「実測値と（実測値の）平均値のずれ」に着目します。

$$
  \operatorname{標準偏差(\sigma)}=\sqrt{\frac{1}{n} \sum\_{i=1}^{n}(x_i - \bar{x})^2}=\sqrt{\frac{1}{n} \sum\_{i=1}^{n}x_i^2 - \bar{x}^2}
$$

<div style="text-align: right">
  \( x_i \) : 実測値<br/>
  \( \bar{x} \) : 実測値の平均値
</div>
{{% /note %}}

{{% private %}}
二乗平均平方根 (RMS: Root Mean Square) は各データの二乗をすべて足して平均をとり、さらに平方根をとったもの。

$$
  \operatorname{RMS}[x] = \sqrt{\frac{1}{n}\sum_{i=1}^n \left( x_i \right)^2}
$$

これは評価関数ではないけど、一応メモ。
{{% /private %}}

