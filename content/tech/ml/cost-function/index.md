---
title: "機械学習メモ: 回帰タスクの評価 ─ MAE, MSE, RMSE, MSLE, RMSLE, MAPE, 決定係数"
url: "p/psotadn/"
tags: ["ml", "数学"]
date: "2014-05-06"
useMath: true
---

機械学習における回帰モデルは、目的変数となる数値を予測するためのモデルです。
よって、回帰モデルの精度は、真の値と予測値のズレをもとに計算されます（ズレが小さいほど精度がよい）。
その計算方法としては、単純にズレの平均値を求める方法（平均絶対誤差: MAE）や、二乗値や対数値を使う方法などいろいろあります。


平均絶対誤差 (MAE: Mean Absolute Error)
----

__平均絶対誤差 (MAE: Mean Absolute Error)__ は、実測値と予測値の「誤差の絶対値」をもとに算出します。

$$
  \text{MAE} = \frac{1}{n} \sum_{i=1}^{n} |y_i - \hat{y}_i|
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

二乗誤差に比べると、外れ値の影響が小さいのが特徴すす。

{{< code lang="python" title="自力で計算する方法" >}}
import numpy as np
import pandas as pd

y_true = pd.Series([1.0, 2.0, 3.0, 4.0, 5.0])  # 実測値
y_pred = pd.Series([0.8, 2.4, 2.5, 3.8, 7.2])  # 予測値

print(np.mean(np.abs(y_true - y_pred)))  #=> 0.7
{{< /code >}}

{{< code lang="python" title="scikit-learn の mean_absolute_error 関数を使う方法" >}}
import numpy as np
import pandas as pd
from sklearn.metrics import mean_absolute_error

y_true = pd.Series([1.0, 2.0, 3.0, 4.0, 5.0])  # 実測値
y_pred = pd.Series([0.8, 2.4, 2.5, 3.8, 7.2])  # 予測値

print(mean_absolute_error(y_true, y_pred))  #=> 0.7
{{< /code >}}


平均二乗誤差 (MSE: Mean Squared Error)
----

__平均二乗誤差 (MSE: Mean Squared Error)__ は、実測値と予測値のずれの二乗を足し合わせて平均をとったものです。

$$
  \operatorname{MSE}=\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

二乗しているので外れ値の影響が大きくなります。

{{< code lang="python" title="scikit-learn の mean_squared_error 関数を使う方法" >}}
import numpy as np
import pandas as pd
from sklearn.metrics import mean_squared_error

y_true = pd.Series([1.0, 2.0, 3.0, 4.0, 5.0])
y_pred = pd.Series([0.8, 2.4, 2.5, 3.8, 7.2])

print(mean_squared_error(y_true, y_pred))  #=> 1.066
{{< /code >}}


平均二乗誤差平方根 (RMSE: Root Mean Squared Error) <!-- 2017-04-11 -->
----

__平均二乗誤差平方根 (RMSE: Root Mean Squared Error)__ は、平均二乗誤差 (MSE: Mean Squared Error) の平方根です。

$$
  \operatorname{RMSE}=\sqrt{\operatorname{MSE}}=\sqrt{\frac{1}{n}\sum_{i=1}^n(y_i-\hat{y_i})^2}
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

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

scikit-learn には直接 RMSE を求める関数はありませんが、`mean_squared_error()` で MSE を求めて平方根 (`np.sqrt()`) を取れば RMSE になります。

{{< code lang="python" >}}
import numpy as np
import pandas as pd
from sklearn.metrics import mean_squared_error

y_true = pd.Series([1.0, 2.0, 3.0, 4.0, 5.0])
y_pred = pd.Series([0.8, 2.4, 2.5, 3.8, 7.2])

print(np.sqrt(mean_squared_error(y_true, y_pred)))  #=> 1.0324
{{< /code >}}


{{% note title="平均二乗平方根 (RMS: Root Mean Square)" %}}
__平均二乗平方根 (RMS: Root Mean Square)__ は各データの二乗をすべて足して平均をとり、さらに平方根をとったものです。

$$
  \operatorname{RMS}[x] = \sqrt{\frac{1}{n}\sum_{i=1}^n \left( x_i \right)^2}
$$

これは誤差を調べるための評価関数ではないけど、たまに出てくるので一応メモ。
{{% /note %}}


対数平均二乗誤差 (MSLE: Mean Squared Logrithnmic Error)
----

__対数平均二乗誤差 (MSLE: Mean Squared Logrithnmic Error)__ は、実測値の対数と予測値の対数の誤差の二乗を足し合わせて平均をとったものです。

$$
\operatorname{MSLE} = \frac{1}{n} \sum_{i=0}^{n} \left( \log(1+y_i) - \log(1+\hat{y_i}) \right)^2
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

目的変数の値が大きくなる可能性があり、大きな値をそのまま使うと影響が大き過ぎる場合に使います。
目的変数の値の範囲が広いときに、その差を比率として扱う効果があります。
下記のように対数の引き算は割り算に変換できるため、値の比率を扱っていることになります。

$$
\log(1+y_i) - \log(1+\hat{y_i}) = \log \frac{1+y_i}{1+\hat{y_i}}
$$

MSLE は、scikit-learn の [mean_squared_log_error 関数](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_squared_log_error.html) で計算できます。

{{% note title="対数をとる前に 1 を足す理由" %}}
実測値や予測値の対数 (log) をとるときは、実際には元の値に 1 を足してから対数をとります。
下記は \\( \log(1 + x) \\) のグラフです。

{{< image w="400" src="img-log1p.svg" >}}

元の値に 1 を足すのは、元の値が 0 のときに、対数がちょうど 0 になるよう対応づけるためです。
1 を足しておかないと、元の値が 0 のときに対数が負の無限大になってしまいます。
\\( \log(1 + x) \\) は、NumPy の __`log1p()`__ 関数で計算できます。
{{% /note %}}


対数平均二乗誤差平方根 (RMSLE: Root Mean Squared Logarithmic Error)
----

__対数平均二乗誤差平方根 (RMSLE: Root Mean Squared Logarithmic Error)__ は、対数平均二乗誤差 (MSLE: Mean Squared Logrithnmic Error) の平方根です。

$$
\begin{align}
  \operatorname{RMSLE} &= \sqrt{\operatorname{MSLE}} \\\\
    &= \sqrt{ \frac{1}{n} \sum_{i=0}^{n} \left( \log(1+y_i) - \log(1+\hat{y_i}) \right)^2 }
\end{align}
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

機械学習のコンペなどで RMSLE を最小化したいときは、目的変数の対数をとったものを新たな目的変数として扱うと計算が楽になります。
その上で、平均二乗誤差平方根 (RMSE: Root Mean Squared Error) を最小化するように学習すると、RMSLE を最小化することになります。

{{< code lang="python" title="RMSLE 計算の実装例" >}}
import numpy as np
from sklearn.metrics import mean_squared_log_error  # MSLE

# MSLE の平方根をとれば RMSLE になる
rmsle = np.sqrt(mean_squared_log_error(y_true, y_pred))

# NumPy だけで計算する場合
# rmsle = np.sqrt(np.mean((np.log1p(y_true) - np.log1p(y_pred)) ** 2))
{{< /code >}}


平均絶対パーセント誤差 (MAPE: Mean Absolute Percentage Error)
----

__平均絶対パーセント誤差 (MAPE: Mean Absolute Percentage Error)__ は、実測値 \\( y_i \\) に対する誤差の割合（相対的な誤差率）を表します。
名前の通り、通常はパーセンテージ (%) で表現します。

$$
\operatorname{MAPE} = \frac{100}{n} \sum_{i=1}^{n} \left| \frac{y_i - \hat{y_i}}{y_i} \right|
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値
</div>

スケールの異なるデータの誤差を比較しやすいという特徴があるため、例えば、価格帯が大きく異なる商品の価格予測をするときなどに利用できます。
実測値 \\( y_i \\) に 0 が含まれるデータには適用できません（0 による除算が発生してしまうため）。


決定係数 (R<sup>2</sup>; Coefficient of Determination)
----

__決定係数 (R<sup>2</sup>)__ は、モデルの予測値の当てはまり度合い（適合性）を表します。
重回帰分析などでよく使われる評価関数です。

$$
R^2 = 1 - \frac{\sum_{i=1}^{n} (y_i - \hat{y_i})^2}{\sum_{i=1}^{n} (y_i - \bar{y_i})^2}
$$

<div style="text-align: right">
  \( y_i \) : 実測値<br/>
  \( \hat{y_i} \) : 予測値<br/>
  \( \bar{y_i} \) : 実測値の平均<br/>
</div>

R<sup>2</sup> の値はデータのばらつき度合い（分散）によって正規化されているため、目的変数のスケールに依存せずにモデルの適合性を評価することができます。
予測の精度が高いほど R<sup>2</sup> の値は 1 に近づきます。
R<sup>2</sup> の最大値は 1（完全に適合）ですが、予測の精度が悪い場合は R<sup>2</sup> の値は負の値になり得ることに注意してください。

{{< code lang="python" title="scikit-learn の r2_score 関数による計算例" >}}
import numpy as np
import pandas as pd
from sklearn.metrics import r2_score

y_true = pd.Series([100, 200, 300, 400, 500])
y_pred = pd.Series([200, 300, 400, 350, 400])

print(r2_score(y_true, y_pred))  #=> 0.575
{{< /code >}}

