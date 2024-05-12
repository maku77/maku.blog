---
title: "機械学習メモ: 分類タスクの評価 ─ balanced accuracy"
url: "p/g6v9puz/"
date: "2024-05-12"
tags: ["ml"]
useMath: true
---

機械学習の分類タスクにおける評価指標のひとつとして __balanced accuracy__ があります（日本語だと加重平均正答率とかかな）。
balanced accuracy は、各クラスのデータ数に偏りがある場合に正答率 (accuracy) の計算に補正を加えます。


balanced accuracy の計算方法
----

通常の正解率 (accuracy) の計算は次のようなシンプルなものです。

$$
\operatorname{accuracy} = \frac{正解数 (r)}{データ数 (n)}
$$

例えば、10 個のデータのうち 8 個の予測に成功した場合は、accuracy は 0.8 (80%) になりますが、blanced accuracy の場合は、各クラスに属するデータの数の逆数を重みとした加重平均を取ります。

$$
\operatorname{balanced accuracy} = \frac{1}{N} \sum_{i=1}^{N} \frac{r_i}{n_i}
$$

- \\( N \\) ... クラス数
- \\( r_i \\) ... クラス \\( i \\) に属すると正しく予測できたデータ数
- \\( n_i \\) ... クラス \\( i \\) の（実際の）データ数

要するに、もともとたくさんのデータがあるクラスに属すると予測して正解したとしても、accuracy はあまり上がらないようにするということです。
逆に、少ししかデータがないクラスに属すると予測して正解すれば、accuracy は大きく上がります。
感覚としては、各クラスごとに個別に accuracy を求めてから平均するという感じです。


balanced accuracy の例
----

例として、7 個のデータを 3 クラスに分類する問題を考えてみます。

{{< image w="500" src="img-001.drawio.svg" title="blanced accuracy の例題" >}}

クラス 1 に属すると予測したデータの内 2 個が正解、クラス 2 に属すると予測したデータの内 1 個が正解だった場合の計算例です。

$$
\begin{align}
\operatorname{balanced accuracy}
  &= \frac{1}{N} \sum_{i=1}^{N} \frac{r_i}{n_i} \\\\
  &= \frac{1}{3} \left( \frac{2}{4} + \frac{1}{1} + \frac{0}{2} \right) \\\\
  &= 0.5 \text{ (50%)}
\end{align}
$$

実際に分類確率を出力するモデルを使ってクラス分類を行う場合は、このデータ数の重み \\( 1/n \\) を加味した上で、どのクラスに分類するかを判断します。
例えば、モデルの出力として、クラス 1 の確率が 0.3、クラス 3 の確率が 0.2 という値が得られたとします。
このとき、単純にこの確率 \\( (0.3 \gt 0.2) \\) だけを見てクラス 1 に属するデータであると予測するのではなく、データ数の重みで補正した \\( 0.3 \times \frac{1}{4} = 0.075 \\)、\\( 0.2 \times \frac{1}{2} = 0.1 \\) という値を使って、クラス 3 に属すると予測します。


参考
----

- [sklearn.metrics.balanced_accuracy_score — scikit-learn documentation](https://scikit-learn.org/stable/modules/generated/sklearn.metrics.balanced_accuracy_score.html)
  ```python
  >>> from sklearn.metrics import balanced_accuracy_score
  >>> y_true = [0, 1, 0, 0, 1, 0]
  >>> y_pred = [0, 1, 0, 0, 0, 1]
  >>> balanced_accuracy_score(y_true, y_pred) # 1/2 * (1/2 + 3/4)
  0.625
  ```
- [Balanced Accuracy Score - Kaggle](https://www.kaggle.com/code/metric/balanced-accuracy-score)

