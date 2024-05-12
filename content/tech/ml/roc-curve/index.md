---
title: "機械学習メモ: 分類タスクの評価 ─ ROC カーブと AUC"
url: "p/ha99p76/"
date: "2024-04-29"
tags: ["ml", "数学"]
useMath: true
---

ROC カーブとは
----

__ROC (Receiver Operating Characteristic) カーブ（曲線）__ は、二値分類モデルの性能を評価するためのグラフです。
陽性と陰性を判断するための閾値を変換させたときに、__真陽性率 (TPR: True Positive Rate)__ と __偽陽性率 (FPR: False Positive Rate)__ がどのように変化するかプロットしたものです。

{{< reference >}}
- TP/TN/FP/FN などの用語について → [混同行列と関連指標](/p/9dveh3m/)
{{< /reference >}}


真陽性率 (TPR) と 偽陽性率 (FPR)
----

真陽性率（＝再現率、Recall）は、陽性（正例）サンプルのうち、正しく陽性と予測できたものが何％あったかを示すもので、高いほど（1 に近いほど）よいです。

$$
  TPR \left( 真陽性率 \right) = \frac{TP}{TP + FN}
$$

偽陽性率は、陰性（負例）サンプルのうち、誤って陽性と予測してしまったものが何％あったかを示すもので、低いほど（0 に近いほど）よいです。

$$
  FPR \left( 偽陽性率 \right) = \frac{FP}{FP + TN}
$$

これらはトレードオフの関係にあり、陽性あるいは陰性と判断するための閾値を変化させたときに次のような感じで変化します。

{{< image w="250" src="img-001.drawio.svg" title="一般的な ROC 曲線" >}}

このとき、どの閾値がよいかは一概には言えなくて、真陽性率を犠牲にしてでも偽陽性率を低くしたいのであれば閾値 A を選べばよいし、真陽性率をできるだけ高めたいのであれば閾値 C を選びます。
バランスよく選びたいのであれば閾値 B を選ぶことになります。


AUC (Area Under the Curve)
----

二値分類モデルの性能を示すための指標として、__AUC (Area Under the Curve)__ があります。
AUC は、ROC カーブの下の部分面積であり、大きいほど（1 に近いほど）よい性能だということを表します。
前述の通り、二値分類モデルで使用する閾値は、どの値がベストかは一概に決められないのですが、AUC を使えば、二値分類モデルの全体的な性能を客観的に示すことができます。

真陽性率 (TPR) は高いほどよく、偽陽性率は低いほどよいので、ROC カーブは左上に寄った形の方がよいといえます。
図で表現するとこんな感じです。

{{< image w="550" src="img-002.drawio.svg" title="AUC による分類モデルの性能評価" >}}


ROC カーブのプロット例
----

10 個のサンプルに関して、陽性 (+) と陰性 (-) を予測する分類モデルを考えてみます。
このモデルは、入力サンプルに対して、予測値を 0.0〜1.0 の確率で出力します。
1.0 に近いほど、陽性の確率が高いと予測していることになります。

<style>
  .local-table th, .local-table td {
    padding: 0.2em 0.4em;
    text-align: center;
  }
  .local-table .minus {
    background: #cef;
  }
  .local-table .plus {
    background: #fdd;
  }
</style>

<table class="local-table">
  <tr><th>本来の値</th><th>モデルの予測値（確率）</th></tr>
  <tr><td class="minus">−</td><td>0.0</td></tr>
  <tr><td class="minus">−</td><td>0.1</td></tr>
  <tr><td class="minus">−</td><td>0.2</td></tr>
  <tr><td class="minus">−</td><td>0.3</td></tr>
  <tr><td class="plus">＋</td><td>0.4</td></tr>
  <tr><td class="minus">−</td><td>0.5</td></tr>
  <tr><td class="plus">＋</td><td>0.6</td></tr>
  <tr><td class="minus">−</td><td>0.7</td></tr>
  <tr><td class="plus">＋</td><td>0.8</td></tr>
  <tr><td class="plus">＋</td><td>0.9</td></tr>
</table>

確率の値（0.0〜1.0）では、分類モデルが実際にどちらに分類したかが決まらないので、何らかの閾値をもとに陽性と陰性を分類することになります。
例えば、閾値を 0.5 に設定した場合は、0.5 以上の確率であったサンプルを陽性と判断します。
上記の予測値（確率）のもとで、閾値を少しずつ変化させた場合の TPR と FPR の変化は次のようになります。

| 閾値 | 真陽性率 (TPR) | 偽陽性率 (FPR) |
| :--: | :--: | :--: |
| 1.0 | 0/4=0.00 | 0/6=0.00 |
| 0.9 | 1/4=0.25 | 0/6=0.00 |
| 0.8 | 2/4=0.50 | 0/6=0.00 |
| 0.7 | 2/4=0.50 | 1/6=0.16 |
| 0.6 | 3/4=0.75 | 1/6=0.16 |
| 0.5 | 3/4=0.75 | 2/6=0.33 |
| 0.4 | 4/4=1.00 | 2/6=0.33 |
| 0.3 | 4/4=1.00 | 3/6=0.50 |
| 0.2 | 4/4=1.00 | 4/6=0.66 |
| 0.1 | 4/4=1.00 | 5/6=0.83 |
| 0.0 | 4/4=1.00 | 6/6=1.00 |

これを ROC カーブとしてプロットすると次のようになります。

{{< image src="img-roc.svg" title="ROC カーブの具体例" >}}

{{< accordion title="上記グラフを出力する Python コード" >}}
{{< code lang="python" >}}
import matplotlib.pyplot as plt

tpr = [0, 0.25, 0.5, 0.5, 0.75, 0.75, 1, 1, 1, 1, 1]
fpr = [0, 0, 0, 0.16, 0.16, 0.33, 0.33, 0.5, 0.6, 0.83, 1]

fig, ax = plt.subplots(figsize=(6, 4), layout="tight")
ax.set_xlabel("FPR (False Positive Rate)")
ax.set_ylabel("TPR (True Positive Rate)")
ax.plot(fpr, tpr, marker="o", color="deeppink")

fig.savefig("img-roc.svg")
plt.show()
{{< /code >}}
{{< /accordion >}}


AUC の注意点
----

陽性と陰性のサンプル数に極端な偏りがある場合（不均衡なデータセットの場合）、AUC の値が指標として適切でないことがあります。
このようなケースでは、F1 スコアや MCC などの別の指標を使うことを検討してください。

- 参考: [機械学習メモ: 混同行列と関連指標](/p/9dveh3m/)

