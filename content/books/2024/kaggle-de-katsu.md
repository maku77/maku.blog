---
title: "読書メモ『Kaggleで勝つ データ分析の技術』"
linkTitle: "『Kaggleで勝つ データ分析の技術』"
url: "p/a7p6b2s/"
date: "2024-04-01"
tags: ["読書", "ml"]
draft: true
---

{{< amazon
  itemId="B07YTDBC3Z"
  title="Kaggleで勝つ データ分析の技術"
  author="門脇大輔、阪田隆司、保坂桂佑、平松雄司"
  publisher="技術評論社"
  imageUrl="https://m.media-amazon.com/images/I/71kBM0KZSAL._SY522_.jpg"
>}}

ちょっとだけ古めの本だけど、Kaggle 本の中でも評価の高いデータ分析技術本です。
たしかによくまとまってて読みやすいですね。

以下まとめメモメモ。


## 1. 分析コンペとは

### 分析コンペって何？

#### コンペとは

1. 特徴量（変数、説明変数）を入力として
2. 目的変数を予測する

学習データ（提供されるデータ）には上記の 1 と 2 が含まれ、テストデータには 1 だけが含まれる。

- 学習データのイメージ（入力とその答えがある）
  - 123, 135, 74, 12, 301, 634 → 15
  - 423, 562, 10, 44, 125, 988 → 31
  - 904, 111, 64, 15, 877, 502 → 23
- テストデータのイメージ（入力しかない）
  - 335, 218, 66, 40, 226, 999 → ？
  - 590, 449, 59, 20, 633, 490 → ？
  - 771, 703, 12, 30, 550, 300 → ？

#### LB: Leaderboard

- 参加者はコンペ期間中にテストデータを使って予測した値を提出する。
- システム側で **テストデータの一部** だけを使ってスコアが計算され、**Public Leaderboard** として現在の順位が公開される。
- コンペ期間が終了すると、テストデータの残りの部分を使ってスコアが計算され、**Private Leaderboard** として最終的な順位が発表される。

#### Shake up

Public LB (Leader Board) と Private LB の順位が大きく入れ替わること。

#### チームマージ

- 他の参加者にリクエストを出してチームを組むこと。
- コンペの終盤に多くのチームマージが行われることがある。
- チームの人数上限は 5 名。
- マージするチーム同士の合計提出回数は「1日の上限 x コンペ日数」を超えてはいけない。毎日上限まで予測値を Submit していると、チームマージできなくなるので注意。

### 分析コンペのプラットフォーム

#### 主なプラットフォーム

- Kaggle (worldwide)
  - 最も有名
  - 世界中の企業、省庁、研究機関がコンペを開催
- SIGNATE（日本）
  - 日本語
  - 日本国内の企業、省庁、研究機関がコンペを開催
- TopCoder（worldwide）
  - プログラミングコンテストのプラットフォームだが、分析コンペも開催されている

#### カーネルコンペ

- 通常のコンペ ... 予測値を提出する。
- カーネルコンペ ... Kernel (Notebook) に記述したコードを提出する。
  - Kernel 上で学習と予測の両方を実行するタイプと、予測だけを実行すればよいタイプがある（後者はモデルのバイナリをアップロードできるようになっている）。

### 分析コンペに参加してから終わるまで

1. **`Join Competition`** ボタンからコンペに参加する
1. 規約に同意する
   - 1 日の Submit 数やチームメンバー数の上限。
   - **Private Sharing**（チームメンバー意外へのコード共有）はダメ。Kaggle の Discussion での共有は OK。
   - 外部データの使用可否はコンペにより異なる。
1. データをダウンロードする
1. 予測値を作成する
   1. ダウンロードしたデータを使ってモデルを作る
   2. テストデータに対する予測値を求める
1. 予測値を提出する
   - 提出サンプル（主に CSV 形式）と同じ形式の提出用ファイルを作成して Submit。１日の提出数上限に注意。
1. Public Leaderboard をチェックする
   - テストデータの一部を使ってスコア計算した結果の順位が公開される。
1. 最終予測値を選ぶ
   - コンペの終了前に、最終評価に使う予測値を 2 つ選んで **`Use for Final Score`** にチェックを入れる。
   - 自分で選ばないと、Public Leaderboard の中でスコアが高いものが選ばれる。
1. Private Leaderboard をチェックする
   - 分析コンペが終了したら Private LB で最終順位を確認する。
   - 多くの場合は終了と同時に発表されるが、発表までに時間がかかるものもある。

### 分析コンペに参加する意義

- 賞金、称号、ランキング
- データ分析の経験
- データサイエンティストとの繋がり
- 就業機会

### 上位を目指すためのポイント

#### 探索的データ分析 (EDA: Exploratory Data Analysis)

- まず優先すべきはデータの理解
- 可視化手法
  - 棒グラフ、箱ひげ図、バイオリンプロット、散布図、折れ線グラフ
  - ヒートマップ、ヒストグラム
  - Q-Q プロット
  - **t-SNE**、**UMAP**
- テーブルデータのコンペでは、よいデータを作れたかどうかで順位がきまる

## 2. タスクと評価指標

### 分析コンペにおけるタスクの種類

タスクの種類ごとに評価指標がいろいろある。

- 回帰タスク → **RMSE**、**MAE**
- 分類タスク
  - 二値分類
    - ラベル (0 or 1) で予測 → **F1-score**
    - 各クラスの確率で予測 → **logless**、**AUC**
  - 多クラス分類
    - マルチクラス分類（予測値はひとつのラベル） → **multi-class logloss**
    - マルチラベル分類（予測値は1つ以上のラベル） → **mean-F1**、**macro-F1**
- レコメンデーション
  - 予測値に順位をつける場合 → **MAP@K**
- その他のタスク
  - 物体検出 (object detection)（矩形領域: bounding box で推定）
  - セグメンテーション (segmentation)（ピクセル単位で推定）

### 評価指標 (evaluation metrics)

評価指標は、学習させたモデルの性能やその予測値の良し悪しを測るための指標。

#### 回帰における評価指標

- **RMSE**: Root Mean Squared Error（平均平方二乗誤差）
  - MAE と比べて **外れ値の影響を受けやすい**
  - scikit-learn の metrics モジュールの **`mean_squared_error`** 関数を使う
- **RMSLE**: Root Mean Squared Logarithmic Error
  - 目的変数の対数をとった値を新たな目的変数として RMSE を最小化すれば RMSLE を最小化することになる。
  - 大きな値の影響が強くなってしまう場合などに用いられる
  - scikit-learn の metrics モジュールの **`mean_squared_log_error`** 関数を使う
- **MAE**: Mean Absolute Error
  - **外れ値の影響を低減して** 評価するときに最適
  - scikit-learn の metrics モジュールの **`mean_absolute_error`** 関数を使う
- **決定係数** (R<sup>2</sup>)
  - 回帰分析の当てはまりの良さを示す（1 に近いほど精度の高い予想ができている）
  - 決定係数を最大化するのは、RSME を最小化するのと同じ意味
  - scikit-learn の metrics モジュールの **`r2_score`** 関数を使う

#### 二値分類における評価指標（正例か負例かを予測する場合）

- **混同行列** (Confusion Matrix)
  - TP: True Positive（真陽性） ... 予測値を正例として、その予測が正しいとき
  - TN: True Negative（真陰性） ... 予測値を負例として、その予測が正しいとき
  - FP: False Positive（偽陽性） ... 予測値を正例として、その予測が誤りのとき
  - FN: False Negative（偽陰性） ... 予測値を負例として、その予測が誤りのとき
  - 完全な予測を行ったモデルの混同行列では、TP と TN のみに値が入る
- **正答率** (Accuracy) と **誤答率** (Error Rate)
  - Accuracy = (TP + TN) / (TP + TN + FP + FN)
  - ErrorRate = 1 - Accuracy
  - scikit-learn の metrics モジュールの **`accuracy_score`** 関数を使う
- **適合率** (Precision) と **再現率** (Recall)
  - Precision（適合率） = TP / (TP + FP)
    - 陽性と予測したもののうち、正しく陽性と予測できた割合
    - ▼陽性適合率 (PPV: Positive Predict Value) とも
  - Recall（再現率） = TP / (TP + FN)
    - すべての陽性データのうち、正しく陽性と予測できた割合
    - ▼感度 (sensitivity) とも
  - 0 からの 1 の値をとり、1 に近いほど良いスコア
  - Precision と Recall はトレードオフの関係
  - 誤検知を少なくしたいときは Precision を重視するとよい（間違えて陽性と判断すると困るケース）
  - 正例の見逃しを避けたいときは Recall を重視するとよい（陽性を見逃しては困るケース：例として病気の検出）
  - scikit-learn の metrics モジュールの **`precision_score`**、**`recall_score`** 関数を使う
- **F1-score（F値）** と **Fβ-score**
  - 二値分類のモデルの精度を測るための評価指標
  - F1-score は Precision と Recall の調和平均
  - F1-score は実務でもよく使われる
  - 二値分類タスクで正しく分類できたか (accuracy) だけではなく、A（陽）と B（陰）のどちらに分類するかが大きく意味を持つケースなどで使う（例えば、病気かどうかの判断など）
  - F1 = 2TP / (2TP + FP + FN)
  - Fβ-score は F1-score から Recall と Precision のバランスを係数βで調整したもの
  - Fβ = (1 + β<sup>2</sup>)・Recall・Precision / (Recall + β<sup>2</sup>Presision)
  - scikit-learn の metrics モジュールの **`f1_score`**、**`fbeta_score`** 関数を使う
- **MCC**: Matthews Correlation Coefficient
  - 不均衡なデータに対するモデルの性能を評価するのに使える
  - -1 から +1 の値を取り、+1 は完璧な予測、0 はランダムな予測、-1 は完全に反対の予測を示す
  - F1-score は正例と負例の数が反転すると値が変わってしまうが、MCC は同じ値になる

#### 二値分類における評価指標（正例である確率を予測する場合）

- **logloss** (**cross entropy**)
  - 分類タスクでの代表的な評価指標
  - logloss = -Σ(y<sub>i</sub> log p<sub>i</sub> + (1-y<sub>i</sub>)log(1-p<sub>i</sub>)) / N
  - logloss は低い方がよい
  - scikit-learn の metrics モジュールの **`log_loss`** 関数を使う
- **AUC**: Area Under the ROC Curve
  - ROC: Receiver Operating Characteristic Curve が描く曲線をもとに計算する（ROC 曲線の下部の面積）
  - scikit-learn の metrics モジュールの **`roc_auc_score`**

#### 多クラス分類における評価指標

- **multi-class accuracy**
  - scikit-learn の metrics モジュールの **`accuracy_score`** 関数を使う
- **multi-class logloss**
  - scikit-learn の metrics モジュールの **`log_loss`** 関数を使う（二値分類のときと引数の配列の形を変える）
- **mean-F1**、**macro-F1**、**micro-F1**
  - F1-score を多クラス分類に拡張したもので、マルチラベル分類で用いられる
- **quadratic weighted kappa**
  - マルチクラス分類でクラス間に順序関係がある場合（例: 映画の 1～5 のレーティング）

#### レコメンデーションにおける評価指標

- **MAP@K**: Mean Average Precision @K
  - 各レコードが 1 つまたは複数のクラスに属しているときに、属している可能性が高いと予測する順に K 個のクラスを予測値とする

### 評価指標と目的関数

- **Objective function（目的関数）**
  - モデルの学習において最適化される関数。
  - 学習では目的関数の値を最小化するように係数などを更新していく。
  - 目的関数は **微分可能でなければいけない**。
  - 主な目的関数
    - 回帰タスク ... RMSE
    - 分類タスク ... logloss
- 評価指標と目的関数が一致していない場合、そのモデルは評価指標に対して最適化されていない可能性がある。

### 評価指標の最適化

### 評価指標の最適化の例

### リーク (data leakage)


{{< amazon
  itemId="B07YTDBC3Z"
  title="Kaggleで勝つ データ分析の技術"
  author="門脇大輔、阪田隆司、保坂桂佑、平松雄司"
  publisher="技術評論社"
  imageUrl="https://m.media-amazon.com/images/I/71kBM0KZSAL._SY522_.jpg"
>}}

