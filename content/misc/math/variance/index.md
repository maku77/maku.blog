---
title: "数学メモ: 共分散と相関係数"
url: "p/huzv74o/"
date: "2024-05-01"
tags: ["数学"]
useMath: true
---

共分散 (covariance) とは
----

__共分散 (covariance)__ は 2 変数で構成されたデータの関係性を示すもので、各変数の偏差を掛け合わせて平均を取ることで求められます。
\\( X \\) と \\( Y \\) の共分散は、__\\( Cov[X, Y] \\)__、__\\( \sigma(X, Y) \\)__、__\\( \sigma_{XY} \\)__ のように表現されます。

$$
Cov[X, Y] = \frac{1}{n} \sum_{i=1}^{n} (x_i - \bar{x})(y_i - \bar{y})
$$

- \\( n \\) ... サンプル数
- \\( x_i \\), \\( y_i \\) ... 各サンプルの値
- \\( \bar{x} \\), \\( \bar{y} \\) ... \\( X \\) の平均値、\\( Y \\) の平均値

共分散の値には次のような意味があります。

- 共分散が正の値 ... 2 つの変数が同じ方向に増減する（一方の変数が大きくなるとき、もう一方も大きくなる）
- 共分散が負の値 ... 2 つの変数が逆の方向に増減する（一方の変数が大きくなるとき、もう一方は小さくなる）
- 共分散が 0 に近い ... 2 つの変数にほとんど関連はない（\\( X \\) と \(( Y \\) は無相関）


期待値を用いた表現
----

\\( X \\) と \\( Y \\) が具体的なデータではなく、ある確率分布に従った確率変数だとみなすと、共分散の式は次のように期待値 \\( \mathbb{E} \\) を用いて表現することができます。

$$
Cov[X,Y] = \mathbb{E} [ (X - \mathbb{E}[X])(Y - \mathbb{E}[Y]) ]
$$

ここで、\\( \mathbb{E}[X] \\) は確率変数 \\( X \\) の期待値ですが、これは集合 \\( X \\) の平均値のようなものだと考えればよいです。
そうすると、この式は前述の式と同じことを表現していることがわかります。


共分散の式の変形
----

共分散は次のように変換した式を使って計算することもできます。
まずは、具体的なサンプル集合があるときの式から。

$$
\begin{align}
  Cov[X, Y] &= \frac{1}{n} \sum_{i=1}^{n} (x_i - \bar{x})(y_i - \bar{y}) \\\\
    &= \frac{1}{n} \sum (x_i y_i - x_i \bar{y} - y_i \bar{x} + \bar{x} \bar{y}) \\\\
    &= \frac{1}{n} \sum x_i y_i - \frac{1}{n} \sum x_i \bar{y} - \frac{1}{n} \sum y_i \bar{x} + \frac{1}{n} \sum \bar{x} \bar{y} \\\\
    &= \frac{1}{n} \sum x_i y_i - \bar{x} \bar{y} - \bar{y} \bar{x} + \frac{1}{n} n \bar{x} \bar{y} \\\\
    &= \color{red}{\frac{1}{n} \sum x_i y_i - \bar{x} \bar{y}}
\end{align}
$$

次に確率分布に従った確率変数を使った場合の式の変換です。
\\( \mathbb{E}[X] \\) や \\( \mathbb{E}[Y] \\) は定数（≒平均値）とみなすことができるので、外側の \\( \mathbb{E} \\) の外に出すことができるのがポイントです。

$$
\begin{align}
  Cov[X,Y] &= \mathbb{E}[ (X - \mathbb{E}[X])(Y - \mathbb{E}[Y])] \\\\
    &= \mathbb{E}[XY - X \mathbb{E}[Y] - Y \mathbb{E}[X] + \mathbb{E}[X] \mathbb{E}[Y]] \\\\
    &= \mathbb{E}[XY] - \mathbb{E}[X \mathbb{E}[Y]] - \mathbb{E}[Y \mathbb{E}[X]] + \mathbb{E}[\mathbb{E}[X] \mathbb{E}[Y]] \\\\
    &= \mathbb{E}[XY] - \mathbb{E}[Y] \mathbb{E}[X] - \mathbb{E}[X] \mathbb{E}[Y] + \mathbb{E}[X] \mathbb{E}[Y] \\\\
    &= \color{red}{\mathbb{E}[XY] - \mathbb{E}[X] \mathbb{E}[Y]}
\end{align}
$$


具体例
----

具体的な例として、国語と数学のテストの点数の共分散を求めてみます。

| | 国語の点数 (\\(x_i\\)) | 数学の点数 (\\(y_i\\)) |
| ---- | :--: | :--: |
| 生徒 1 | 75 | 65 |
| 生徒 2 | 83 | 72 |
| 生徒 3 | 90 | 75 |
| 生徒 4 | 72 | 64 |

- 国語の平均点 \\(\bar{x}\\) = (75 + 83 + 90 + 72) / 4 = 80
- 数学の平均点 \\(\bar{y}\\) = (65 + 72 + 75 + 64) / 4 = 69

$$
\begin{align}
  Cov[X,Y] &= \frac{1}{n} \sum_{i=1}^{n} (x_i - \bar{x})(y_i - \bar{y}) \\\\
    &= \frac{1}{4} \left( (75 - 80)(65 - 69) + (83 - 80)(72 - 69) + (90 - 80)(75 - 69) + (72 - 80)(64 - 69) \right) \\\\
    &= \frac{1}{4} (-5 \cdot -4 + 3 \cdot 3 + 10 \cdot 6 + -8 \cdot -5) \\\\
    &= \frac{1}{4} (20 + 9 + 60 + 40) \\\\
    &= 32.25
\end{align}
$$

共分散は 32.25 と求められました。
もうひとつの式でも計算してみます。

$$
\begin{align}
  Cov[X, Y] &= \frac{1}{n} \sum x_i y_i - \bar{x} \bar{y} \\\\
    &= \frac{1}{4} (75 \cdot 65 + 83 \cdot 72 + 90 \cdot 75 + 72 \cdot 64) - 80 \cdot 69 \\\\
    &= 32.25
\end{align}
$$

ちゃんと同じ値になりました。

٩(๑❛ᴗ❛๑)۶ わーぃ


相関係数
----

共分散の欠点として、元のデータのスケールがそのまま染み出してしまうので、その値から相関の度合いを判断しずらいという点があげられます。
また、異なるスケールを持つ 2 つの集合の共分散を比較することに意味はありません。
例えば、100 点満点のテストと 990 点満点のテストの共分散の値をそのまま比較することはできません。

このスケールの問題を解決したのが __相関係数 (correlation coefficient)__ です。
相関係数は、-1 から 1 の値を取るように標準化されているため、相関の強さを直感的に判断できます。
相関係数の値は次のような意味を持ちます。

- 相関係数が 1 に近い ... 正の相関（一方の値が増えると、もう一方の値も増える傾向）
- 相関係数が -1 に近い ... 負の相関（一方の値が増えると、もう一方の値は減る傾向）
- 相関係数が 0 に近い ... 相関なし

相関係数は、共分散を各標準偏差の積で割ることで求められます。
相関係数は、__\\( \rho \\)（ロー）__ や __\\( r \\)__ などの記号で表現するのが一般的です。

$$
\begin{align}
  \rho_{XY} &= \frac{Cov[X, Y]}{\sigma_X \sigma_Y} \\\\
    &= \frac{\frac{1}{n} \sum (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\frac{1}{n} \sum (x_i - \bar{x})^2 } \sqrt{\frac{1}{n} \sum (y_i - \bar{y})^2 } } \\\\
    &= \frac{\sum (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\sum (x_i - \bar{x})^2 \cdot \sum (y_i - \bar{y})^2 }} \\\\
\end{align}
$$

{{% note title="ρ と r の使い分け" %}}
相関係数を表す記号 \\( \rho \\) と \\( r \\) は、次のように使い分けることが多いようです。

- \\( \rho \\) ... 母集団（データ全体）の相関係数
- \\( r \\) ... 標本（取得できたデータ）の相関係数
{{% /note %}}


共分散と分散の関係
----

分散 \\( \rho^2 \\) は次のような式で計算できますが、

$$
\rho^2 = \frac{1}{n} \sum (x_i - \bar{x})^2
$$

これは、共分散の式を 1 つのデータに適用したものと考えることができます。

$$
Cov[X, X] = \frac{1}{n} \sum (x_i - \bar{x})(x_i - \bar{x})
$$

共分散は負の値になることがありますが、分散は必ず 0 以上の値になります。


（おまけ）Python で共分散、相関係数を求める
----

Python の数値計算ライブラリである NumPy (np) や Pandas (pd) を使うと、簡単に共分散や相関係数を求めることができます。

{{< code lang="python" title="NumPy (np) の場合" >}}
import numpy as np

# 2 つのサンプル配列を作成
x = np.array([1, 2, 3, 4, 5])
y = np.array([5, 4, 3, 2, 1])

# 共分散を計算
covariance = np.cov(x, y)[0, 1]
print("共分散:", covariance)  # -2.5

# 相関係数を計算
correlation_coefficient = np.corrcoef(x, y)[0, 1]
print("相関係数:", correlation_coefficient)  # -0.99999
{{< /code >}}

{{< code lang="python" title="Pandas (pd) の場合" >}}
import pandas as pd

# 2 つのデータフレームを作成
df = pd.DataFrame({"x": [1, 2, 3, 4, 5], "y": [5, 4, 3, 2, 1]})

# 共分散を計算
covariance = df["x"].cov(df["y"])
print("共分散:", covariance)  # -2.5

# 相関係数を計算
correlation_coefficient = df["x"].corr(df["y"])
print("相関係数:", correlation_coefficient)  # -0.99999
{{< /code >}}

