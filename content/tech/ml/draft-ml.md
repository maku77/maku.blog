---
title: "機械学習／ディープラーニングの雑多メモ"
url: "p/jf68azp/"
date: "2023-12-03"
tags: ["ml"]
draft: true
---

データ分割・検証
----

- データセットを学習用データと検証用データに分割するための関数は、[sklearn.model_selection](https://scikit-learn.org/stable/modules/classes.html#module-sklearn.model_selection) パッケージにまとまっています。

- KFold（K-分割交差検証）
- StratifiedKFold (Stratified K-Fold cross-validator)（層状 K 分割）
  - [sklearn.model_selection.StratifiedKFold](https://scikit-learn.org/stable/modules/generated/sklearn.model_selection.StratifiedKFold.html#sklearn.model_selection.StratifiedKFold)
  - 正解ラベルの数に偏りがあるときに使います。例えば、陽性と陰性の正解ラベル比が 1:9 の場合、うまくデータ分割 (train, test) しないと、test データが陰性ばかりになってしまう可能性があります。このようなケースでは　StratifiedKFold を使うことで、分割後のデータ (train, test) にも、同じ比率の目的変数が含まれるように分割してくれます。


画像認識モデル
----

- __AlexNet__ ... 画像認識の初期の画期的なネットワーク
- __ResNet (Residual Network)__ ... 2015 年の ImageNet のコンテストに優勝したネットワーク


物体検出
----

物体検出（領域検出＋クラス分類）の分野では、ほとんどの場合は __CNN: Convolutional Neural Networks（畳み込みニューラルネットワーク）__ が採用されます。
これは、プーリング層による位置普遍性を利用するためです。

### R-CNN (Regional CNN)

R-CNN は、物体検出の初期のディープラーニング手法の一つです。
領域探索アルゴリズムと CNN による画像認識アルゴリズムから構成されています。
R-CNN における物体検出の手順は次の通りです。

1. 画像内で物体が存在する可能性のある領域の候補（ROI）を見つける（約 2000 箇所）
1. 各領域の特徴量を抽出するために CNN を適用する
1. 各領域をクラス分類する

R-CNN は約 2000 個の認識領域ごとに CNN を実行するため演算量が多く、次のような欠点があります。

- 学習に時間がかかる
- 大量のメモリを使用する

R-CNN の改良版として下記のようなものがあります。

- Fast R-CNN
- Faster R-CNN

これらの改良版では、ROI プーリングや RPN（Region Proposal Network）などの新しい技術が導入され、物体検出の速度と効率が向上しました。
Fast R-CNN では、ROI プーリングを導入することで CNN の計算を効率的に行い、Faster R-CNN では RPN を用いて高速な領域提案を実現しました。
これにより、従来の R-CNN よりも効率的な物体検出が可能となりました。

### Faster R-CNN

Faster R-CNN は、2015 年に Microsoft によって発表された、R-CNN 物体検出アルゴリズムの改良版です。
Faster R-CNN の学習は、次の 2 段階のステップで行われます。

1. 物体が存在する領域か背景かを区別する学習
1. 各領域がどのような物体であるかを識別する学習

最初のステップでは、RPN（Region Proposal Network）と呼ばれる CNN を使用します。
候補となる物体領域を提案するネットワークを使用して、画像内の領域を提案します。
従来のアルゴリズムでは、画像内の物体の候補領域を見つけるために Selection Search が使われていましたが、RPN はこれをディープラーニングを用いた手法 (CNN) に置き換え、高速で正確な領域提案を行います。
その後の識別段階では、候補領域から実際の物体のクラスと位置を推定するためのネットワークが利用されます。

### YOLO (You Only Look Once)

YOLO は You Only Look Once（一度見るだけでいい）の略で、その名の通り、画像全体を一度処理するだけで物体検出を行うことができます。
一度の CNN で「領域候補検出」と「クラス分類」を行うので高速です。
YOLO では、入力画像を正方形にリサイズして、さらに細かい正方形に分割して処理します。

一般的な物体検出アルゴリズムは、多段階の処理（例えば、領域提案と分類）に分割して実行しますが、YOLO はこの多段階の処理を 1 つのニューラルネットワークで同時に行います。
そのため YOLO は高速で、リアルタイム処理に適しています。
しかし、複雑な形状の物体や、小さな物体の検出精度は他の手法よりも劣ることがあります。

### SSD (Single Shot MultiBox Detector)

SSD は、異なるスケールの特徴マップを使用して、画像内の異なるサイズの物体を検出するためのアーキテクチャです。
SSD を使った物体検出タスクの基本的な流れは以下のようになります。

1. 教師データのダウンロード: 物体検出のためのラベル付きデータセットを取得します。一般的には、PASCAL VOC や COCO などのデータセットが使用されます。
1. 前処理: データの前処理を行います。これには、画像のリサイズ、正規化、データ拡張（augmentation）などが含まれます。これにより、モデルの学習がより効果的になります。
1. データセットの作成: 前処理されたデータを学習や検証用のデータセットに分割し、適切な形式に整えます。
1. DataLoader の作成: データのバッチ処理とシャッフルを行いながら、学習用のデータセットをモデルに供給するための DataLoader を作成します。
1. SSD モデルの作成: SSD のモデルを定義しますが、多くの場合、訓練済みの重みを使用します。
1. 損失関数の定義と最適化手法を設定: SSD では、検出されたボックスの位置とクラスを同時に予測するため、位置損失とクラス分類損失を考慮した損失関数を定義します。また、最適化手法（例: Adam、SGD など）を選択し、学習率などのハイパーパラメータを設定します。
1. 学習と検証: モデルの学習を実行し、検証データを使用してモデルの性能を評価します。
1. テストデータの推論: 学習したモデルを使用して、未知のテストデータに対して物体検出を行います。画像内の物体を検出し、それらの位置とクラスを予測します。

### DETR (End-to-End Object Detection with Transformers)

DETR は Transformer モデルを用いた初の物体検出モデルです。
2020 年 5 月に Facebook の研究チームが論文を発表しました。
DETR は次の 3 つの主要な要素で構成されています。

バックボーン (Backbone)
: 画像の特徴量をエンコードするための CNN です。
通常、畳み込みニューラルネットワーク (CNN) が用いられ、画像から特徴マップを抽出します。
これらの特徴マップは後段の処理で利用されます。

トランスフォーマー (Transformer)
: CNN によって得られた特徴マップを受け取り、物体の位置やクラスの情報を表すベクトルに変換し、予め定められた物体数の予測を行います。
Transformer の自己注意機構 (self-attention mechanism) を利用して、各物体の位置や種類を同時に推論します。

デコーダ (Decoder, FFN)
: Transformer からの出力を受け取り、物体の位置座標とクラスラベルにデコードするためのネットワークです。
このデコーダは、Transformer から得られた情報を元に最終的な物体検出を行います。

DETR は従来の物体検出手法とは異なり、領域提案（region proposals）や NMS (Non-Maximum Suppression)（重複するボックスの削除）を使用せずに、Transformer を介して直接物体を検出する点で革新的です。
これにより、従来の手法と比べてより単純かつ効率的に物体を検出することが可能となりました。
DETR が End-to-End と呼ばれているのは、複数の段階を持つ従来の手法とは異なり、Transformer と Decoder からなる単一のネットワークを使って画像全体から直接的に物体を検出できるからです。

### 処理の流れの違い

- R-CNN / Fast R-CNN
  - 入力 → 領域検出 → Deep Learning → 出力
- YOLO / SSD / Faster R-CNN
  - 入力 → Deep Learning → 出力


XGBoost / LightGBM / CatBoost
----

勾配ブースティング系のパッケージ。

### XGBoost

- Website: https://xgboost.readthedocs.io/
- 2014 年にリリース
- 決定木アルゴリズムに基づいた勾配ブースティング (Gradient Boosting)

### LightGBM (Light Gradient Boosting Machine)

- Website: https://lightgbm.readthedocs.io/
- 2016 年にリリース
- マイクロソフト社がスポンサー
- 決定木アルゴリズムに基づいた勾配ブースティング (Gradient Boosting) で、大量の決定木を作成しながら学習を進める
- 特徴量の標準化が不要（決定木ベースの手法なので）
- モデルの訓練が高速
- 欠損値を含む入力を受け付けてくれる
- カテゴリ変数を扱ってくれる
- ハイパーパラメーターを調整しないと過学習（オーバーフィッティング）しやすい

### CatBoost

- Website: https://catboost.ai/

### 比較

> XGBoost（XGBClassifier）とLightGBM（LGBMClassifier）の基本的な違いを表形式でまとめたものです。これらのモデルは、機械学習で広く使用される勾配ブースティングアルゴリズムです。

| 特徴 | XGBoost<br/>(XGBClassifier) | LightGBM<br/>(LGBMClassifier) | CatBoost<br/>(CatBoostClassifier) |
| ---- | ---- | ---- | ---- |
| 勾配ブースティングの種類 | Gradient Boosting Trees | Gradient Boosting Trees | Ordered Boosting （順序ブースティング） |
| カテゴリカル変数 | 数値エンコードが必要 | そのまま使用可能 | そのまま使用可能 |
| スケーリング | スケーリングが必要 | 不要 | 不要 |
| 分散処理 | 単一ノードでの学習 | 分散処理で高速学習 | 分散処理で高速学習 |
| メモリ使用量 | 大きい | メモリ最適化により、大規模データに適応 | メモリ最適化により、大規模データに適応 |
| データサイズ | 小～中規模 | 大規模 | 大規模 |
| 木の成長方法 | 深さ優先探索 (depth-wise) | 葉ノード優先探索 (leaf-wise)、高速な学習を実現 | 自動的な特徴選択機能を持つ |
| 過学習の抑制 | ハイパーパラメータで調整 | ハイパーパラメータで調整 | デフォルトで過学習を抑制するための機能あり |
| 設定なしでの使用 | △ | デフォルト設定でも良好な結果 | デフォルト設定でも良好な結果 |

> CatBoostは、Gradient Boostingの一種であり、カテゴリカル特徴量に特に焦点を当てた高性能な機械学習モデルです。CatBoostは、カテゴリカル変数のエンコーディングや特徴量のスケーリングを行う必要がないことが特徴です。
> これらのモデルは、データや問題に応じて適切な選択が必要です。例えば、XGBoostは一般的に小～中規模のデータセットでうまく機能しますが、LightGBMは大規模なデータセットに特に適しており、高速な学習と予測が可能です。

最初のステップでは、 Faster R-CNN における最初の段階では、RPN（Region Proposal Network）と呼ばれるCNNが使われます。この段階では、候補となる物体領域を提案するネットワークを使用して、画像内の領域を提案しますPNは、従来の手法であるSelection Searchの代わりに、ディープラーニングを適用したものです。Selection Searchは画像内の物体の候補領域を見つけるための従来のアルゴリズムでしたが、RPNはこれをCNNベースのアプローチに置き換え、高速で正確な領域提案を行います。


その他
----

- 画像のアノテーション付けツール
  - labelImg
  - Cloud Annotations
  - VoTT
  - LabelBox
- 物体検知モデルの性能評価
  - mAP (mean Average Precision) ... 各物体の検出制度の平均値。
- 線形回帰アルゴリズムの 3 つの予測モデル
  - 単回帰 ... 特徴量は 1 つ。特徴量が x として、ax + b のような一次関数として表現される。
  - 重回帰 ... 特徴量は複数。ax<sub>1</sub> + bx<sub>2</sub> + cx<sub>3</sub> + d のような式で表現される。
  - 多項式回帰 ... ax<sub>1</sub> + bx<sub>1</sub><sup>2</sup> + cx<sub>2</sub> + dx<sub>2</sub><sup>2</sup> + ... のような式で表現される（特徴量 x<sub>i</sub> の n 乗という項が出てくる）。説明変数と目的変数の関係が線形ではなく、より複雑な曲線や曲面を表現できます。


特徴量の作成
----

### 交互作用特徴量 (Polynomial Features)

```python
from sklearn.preprocessing import PolynomialFeatures

df2 = PolynomialFeatures(include_bias=False).fit_transform(df1)
```


評価指標／評価関数
----

- 交差エントロピー誤差 (CE: Cross Entropy)
  - 分類問題のように決まったラベルの推測を行う場合に使用します。

