---
title: "機械学習メモ: 多クラス分類の評価 ─ mean-f1, macro-f1, micro-f1"
url: "p/oegkg89/"
date: "2024-05-12"
tags: ["ml"]
useMath: true
---

多クラス分類用の F1 スコア
----

2 クラスの分類タスク（二値分類）の代表的な評価指標として [F1 スコア](/p/9dveh3m/#f1-score) がありますが、多クラスの分類タスクの評価指標としては、これを拡張した次のような指標を使います。
F1 スコアの平均をとったりするのですが、どのような単位で F1 スコアを計算するかが微妙に異なります。

| 評価指標 | 説明 |
| ---- | ---- |
| <span style="white-space: nowrap"><strong>mean-F1</strong></span> | レコードごとに F1 スコアを求め、それらの平均をとります。 |
| <span style="white-space: nowrap"><strong>macro-F1</strong></span> | クラスごとに F1 スコアを求め、それらの平均をとります。 |
| <span style="white-space: nowrap"><strong>micro-F1</strong></span> | 各レコードの各クラスに対する予測（陽性 or 陰性）が TP/TN/FP/FN のいずれであるかを求め、その混同行列をもとに F1 スコアを計算します。 |

これらの指標は、1 つのレコードに対して複数のラベルをつけるマルチラベル分類においてよく用いられます。
下記はマルチラベル分類のタスクにおいて、各種 F1 スコアを求める実装例です。
scikit-learn の __`f1_score()`__ 関数の __`average`__ 引数でどのロジックを用いるかを指定できます。

{{< code lang="python" title="scikit-learn でマルチラベル分類の F1 スコアを求める" >}}
from sklearn.metrics import f1_score

# サンプルデータ
y_true = [[0, 1, 1],  # レコード1の真のラベル (2, 3)
          [1, 1, 1]]  # レコード2の真のラベル (1, 2, 3)
y_pred = [[0, 1, 0],  # レコード1の予測されたラベル (2)
          [1, 0, 1]]  # レコード2の予測されたラベル (1, 3)

print(" mean-F1:", f1_score(y_true, y_pred, average="samples"))  # 0.733
print("macro-F1:", f1_score(y_true, y_pred, average="macro"))    # 0.777
print("micro-F1:", f1_score(y_true, y_pred, average="micro"))    # 0.750
{{< /code >}}


手作業で計算してみる
----

それぞれのロジックをちゃんと理解するために、上記コード中のサンプルデータに関して手作業で計算してみます。

### mean-F1 の計算

mean-F1 は、レコードごとに F1 値を求めてから、それらの F1 値を平均します。

1. レコード 1 に着目して F1 値を求めます
   - 真のラベルは (2, 3)
   - 予測ラベルは (2)
   - TP=1、TN=1、FP=0、FN=1 となるので、\\( F_1 = \frac{2 \cdot TP}{2 \cdot TP + FN + FP} = \frac{2 \cdot 1}{2 \cdot 1 + 1 + 0} = \frac{2}{3} \\)
2. レコード 2 に着目して F1 値を求めます
   - 真のラベルは (1, 2, 3)
   - 予測ラベルは (1, 3)
   - TP=2、TN=0、FP=0、FN=1 となるので、\\( F_1 = \frac{2 \cdot TP}{2 \cdot TP + FN + FP} = \frac{2 \cdot 2}{2 \cdot 2 + 1 + 0} = \frac{4}{5} \\)

上記の \\( F_1 \\) 値の平均をとればよいので、

$$
\operatorname{mean-F_1} = \frac{1}{2} \cdot \left( \frac{2}{3} + \frac{4}{5} \right) = \frac{11}{15} = 0.733 \cdots
$$

### macro-F1 の計算

macro-F1 は、クラス（ラベル）ごとに F1 値を求めてから、それらの F1 値を平均します。

1. クラス 1 に着目して F1 値を求めます
   - TP=1、TN=1、FP=0、FN=0 なので、\\( F_1 = \frac{2 \cdot TP}{2 \cdot TP + FN + FP} = \frac{2 \cdot 1}{2 \cdot 1 + 0 + 0} = \frac{2}{2} \\)
2. クラス 2 に着目して F1 値を求めます
   - TP=1、TN=0、FP=0、FN=1 なので、\\( F_1 = \frac{2 \cdot TP}{2 \cdot TP + FN + FP} = \frac{2 \cdot 1}{2 \cdot 1 + 1 + 0} = \frac{2}{3} \\)
3. クラス 3 に着目して F1 値を求めます
   - TP=1、TN=0、FP=0、FN=1 なので、\\( F_1 = \frac{2}{3} \\) （同上）

上記の \\( F_1 \\) 値の平均をとればよいので、

$$
\operatorname{macro-F_1} = \frac{1}{3} \cdot \left( \frac{2}{2} + \frac{2}{3} + \frac{2}{3} \right) = \frac{7}{9} = 0.777 \cdots
$$

### micro-F1 の計算

micro-F1 は、各レコードの中で各クラスの予測値が TP/TN/FP/FN のいずれかであるかを判別し、それらの数をもとに \\( F_1 \\) を計算します。

例えば、レコード 1 のクラス 1 の予測値は陰性 (negative) つまり、レコード 1 はクラス 1 ではないと予測していますが、これは正しく陰性 (negative) と予測できているので TN (true negative) となります。
同様に、すべてのレコードとクラスの組み合わせに関して TP/TN/FP/FN のいずれかであるかを一覧にすると次のようになります。

| | クラス 1 | クラス 2 | クラス 3 |
| ---- | :--: | :--: | :--: |
| レコード 1 | TN | TP | FN |
| レコード 2 | TP | FN | TP |

micro-F1 は、この TP/TN/FP/FN の数を用いて \\( F_1 \\) 値を求めます。

$$
\operatorname{macro-F_1} = \frac{2 \cdot TP}{2 \cdot TP + FN + FP} = \frac{2 \cdot 3}{2 \cdot 3 + 2 + 0} = \frac{3}{4} = 0.75
$$

これで、手作業でも mean-F1、macro-F1、micro-F1 を求められました！

٩(๑❛ᴗ❛๑)۶ わーぃ

