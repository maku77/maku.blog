---
title: "機械学習メモ: 分類タスクの評価 ─ 混同行列と 2 クラス分類の基本的な評価指標 (accuracy, error rate, recall, precision, f1-score, fβ-score)"
url: "p/9dveh3m/"
date: "2024-04-21"
tags: ["ml", "数学"]
useMath: true
---

混同行列 (confusion matrix) とは
----

__混同行列 (confusion matrix)__ は、機械学習やパターン認識などの分野で使われる評価手法のひとつで、主に分類問題の性能を評価するために使用されます。
混同行列は、実際のデータが属するクラスと、モデル（分類器）が予測したクラスを比較することで、分類器の性能を可視化するのに役立ちます。

{{< image src="img-001.drawio.svg" title="混同行列" >}}

混同行列は、予測モデルが予測したデータを下記の 4 種類に分けてカウントします。
陽性・陰性というのは、例えば、「病気の有無」や「本物・偽物」といった分類を示します。

- <span style="color: #009933;">真陽性 (TP: True Positives)</span> ... 陽性データのうち、モデルが<span style="color: #009933;">正しく陽性</span>と予測した数。
- <span style="color: #009933">真陰性 (TN: True Negatives)</span> ... 陰性データのうち、モデルが<span style="color: #009933;">正しく陰性</span>と予測した数。
- <span style="color: #cc0066">偽陽性 (FP: False Positives)</span> ... 陰性データのうち、モデルが<span style="color: #cc0066">誤って陽性</span>と予測した数。
- <span style="color: #cc0066">偽陰性 (FN: False Negatives)</span> ... 陽性データのうち、モデルが<span style="color: #cc0066">誤って陰性</span>と予測した数。

一般的には二値分類に使われますが、多クラスの分類でも利用されます。
混同行列をベースにして、分類問題の様々な評価指標を計算できます。

- [正答率 (accuracy) と誤答率 (error rate)](#accuracy)
- [再現率 (recall) と 適合率 (precision)](#recall-precision)
- [F1 スコア、Fβ スコア](#f1-score)
- マシューズ相関係数 (MCC)

ここで、正答率 (accuracy) というのは、単純に「予測が当たった数／データ数」という計算で求められるものです。
例えば、10 個のデータを分類して、5 個のデータを正しく分類できれば、正答率 (accuracy) は50％です。

でも、実世界の分類問題はこんな簡単に評価できるものではなかったりします。
典型的なのが疾患の有無の判断です。
実際に 100 人中 5 人が疾患を持っている場合、誰も疾患を持っていないと予測すれば、その予測モデルの精度は95％になります。
でもそんな予測モデルは役に立ちませんよね？
そうではなく、いかに間違いなく疾患を持っている人を見つけられるかが重要なはずです。
そこで必要になってくるのが、再現率、適合率、F1 スコアといった評価指標です。


正答率 (accuracy) と誤答率 (error rate) {#accuracy}
----

### 正答率 (accuracy)

{{< image src="img-accuracy.drawio.svg" title="正答率 (accuracy)" >}}

正答率 (accuracy) はとても簡単で、予測モデルの答えが正しかった割合を示しています。
ここでは、陽性と陰性のどちらと予測したのかの区別はあまりありません。

$$
\begin{align}
{\rm accuracy} &= \frac{正しく予測したデータ数}{データ数} \\\\
  &= \frac{TP + TN}{TP + TN + FP + FN}
\end{align}
$$

正答率 (accuracy) が高いからといって、そのモデルが優れているとは限りません。
例えば、元データのほとんどが陽性 (positive) である場合、すべて陽性 (positive) と予測してしまえば、正答率 (accuracy) はほぼ 100% になります。

### 誤答率 (error rate)

誤答率 (error rate) は逆に、予測モデルの答えが誤りだった割合です。

{{< image src="img-error-rate.drawio.svg" title="誤答率 (error rate)" >}}

$$
\begin{align}
{\rm error\ rate} &= 1 - {\rm accuracy} \\\\
  &= \frac{誤って予測したデータ数}{データ数} \\\\
  &= \frac{FP + FN}{TP + TN + FP + FN}
\end{align}
$$

正答率 (accuracy) と誤答率 (error rate) はシンプルな指標ですが、前述の通り、これらの指標は陽性と陰性のどちらのクラスに分類したかを意識していません。
ただ予測したクラスが正しかったどうかの割合を見ているだけです（だから混同行列を作るまでもない）。


再現率 (recall) と 適合率 (precision) {#recall-precision}
----

### 再現率 (recall)

再現率 (recall) は、__実際に陽性であるものデータだけに着目して__、その中でどれだけを陽性と予測できたかの割合です。

{{< image src="img-recall.drawio.svg" title="再現率 (recall)" >}}

$$
{\rm recall} = \frac{TP}{TP + FN}
$$

病気の判断など、__陽性の見落としを避けたいケース__ では、再現率 (recall) が重視されます。

- 例: 100 人の患者のうち、10 人が癌（陽性）を患っている場合
  - 予測モデルは、がん患者 10 人中 8 人を正しく陽性と判断した（2 人は間違えて陰性と判断した）
  - \\( recall = \frac{8}{8 + 2} = 0.8 \\)
  - これは20％のがんを見逃してしまったということ

### 適合率 (precision)

適合率 (precision) は、__陽性だと予測したデータだけに着目して__、その中で実際に陽性だった割合です。

{{< image src="img-precision.drawio.svg" title="適合率 (precision)" >}}

$$
{\rm precision} = \frac{TP}{TP + FP}
$$

__間違えて陽性と判断すると不都合がある（誤判定を避けたい）ケース__ では、適合率 (precision) が重視されます。

- 例: 100 通のメールのうち、5 通がスパムメール（陽性）の場合
  - 予測モデルは、7 通をスパムと判断したが、実際にスパムだったのはそのうち 4 通だった（3 通は間違えてスパムと判断してしまった）
  - \\( precision = \frac{4}{4 + 3} = 0.571 \\)
  - これは43％のメールを間違えてスパムフィルターにひっかけてしまったということ


F1スコア (f1 score) とFβスコア (fβ score) {#f1-score}
----

### F1 スコア (f1 score)

F1 スコア (f1 score) は、再現率 (recall) と適合率 (precision) の両方を考慮した評価指標です。
具体的には、再現率 (recall) と適合率 (precision) の[調和平均](/p/4fz2tti/)をとったものです。

{{< image src="img-f1-score.drawio.svg" title="F1 スコア (f1 score)" >}}

$$
\begin{align}
{\rm F}_1
  &= \left( \frac{{\rm recall}^{-1} + {\rm precision}^{-1}}{2} \right)^{-1} \\\\
  &= \frac{2}{{\rm recall}^{-1} + {\rm precision}^{-1}} \\\\
  &= \frac{2}{\frac{1}{\rm recall} + \frac{1}{\rm precision}} \\\\
  &= \frac{2 \cdot {\rm recall} \cdot {\rm precision}}{{\rm recall}+{\rm precision}} \\\\
  &= \frac{2 \cdot TP}{2 \cdot TP + FN + FP}
\end{align}
$$

### Fβ スコア (fβ score)

Fβ スコア (fβ score) は、F1 スコアとほぼ同じですが、再現率 (recall) に \\( \beta^2 \\) の重みを付けたものです。
\\( \beta \gt 1 \\) の場合は再現率 (recall) が重視され、\\( \beta \lt 1 \\) の場合は適合率 (precision) が重視されます。
\\( \beta = 1 \\) の場合は F1 スコアと同じになります。

調和平均の形から 1 ステップずつ変換していくと、Fβ スコアは次のように表現できます。

$$
\begin{align}
{\rm F}_\beta
  &= \left( \frac{\beta^2 \cdot {\rm recall}^{-1} + {\rm precision}^{-1}}{1 + \beta^2} \right)^{-1} \\\\
  &= \frac{1 + \beta^2}{\beta^2 \cdot {\rm recall}^{-1} + {\rm precision}^{-1}} \\\\
  &= \frac{1 + \beta^2}{\frac{\beta^2}{\rm recall} + \frac{1}{\rm precision}} \\\\
  &= \frac{1 + \beta^2}{\frac{\beta^2 \cdot {\rm precision} + {\rm recall}}{{\rm recall} \cdot {\rm precision}}} \\\\
  &= \frac{(1 + \beta^2) \cdot {\rm recall} \cdot {\rm precision}}{{\rm recall} + \beta^2 \cdot {\rm precision}}
\end{align}
$$

F1 スコアも Fβ スコアも再現率 (recall) と適合率 (precision) の両方を考慮した指標ですが、真陰性 (TN: True Negatives) の要素が考慮されていないため、クラスに偏りがある場合の評価指標として問題があるという指摘もあります。
そのような場合は、真陰性 (TN: True Negatives) まで考慮したマシューズ相関係数 (MCC) などを使います。

