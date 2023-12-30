---
title: "機械学習／ディープラーニングの雑多メモ"
url: "p/jf68azp/"
date: "2023-12-03"
tags: ["ml"]
draft: true
---

画像認識モデル
----

- __AlexNet__ ... 画像認識の初期の画期的なネットワーク
- __ResNet (Residual Network)__ ... 2015 年の ImageNet のコンテストに優勝したネットワーク


XGBoost / LightGBM
----

- https://xgboost.readthedocs.io/
- https://lightgbm.readthedocs.io/
- https://catboost.ai/

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

