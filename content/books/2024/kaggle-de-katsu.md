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

- [多クラス分類の blanced accuracy](/p/g6v9puz/) における最適化
  - 1 割しかないクラスを正しく予測することは、2 割あるクラスを正しく予測することの 2 倍の価値があるので、「確率 × クラスの割合の逆数」が最大となるクラスに分類するのがよい。
- mean-F1 における閾値の最適化
  - mean-F1 ... レコードごとの F1 スコアの平均
  - レコード（ここではオーダー）ごとに閾値を変えないと評価指標を最適できない。
- quadratic weighted kappa における閾値の最適化
  - 1〜8 のレーティングを付けるマルチクラス分類では、クラスに順序関係があるので回帰として解くことも分離として解くこともできる。
  - quadratic weighted kappa という評価指標では、__連続値で予測値を出力したあとにクラス間の閾値を最適化（後処理）__ するアプローチが効果的。
    1. 回帰モデルや分類モデルで各クラスの確率の加重平均による予測値を出力する（各クラスの値を i、確率を p<sub>i</sub> としたときに、Σip<sub>i</sub> を予測値とする）
    2. どの値を境にクラス分けをするかの区切り位置を最適化する
  - ようするに、モデルによって求めた予測値を単純に四捨五入してクラス分けするのではなく、後処理で閾値を決める。
  - イメージとしては、このように等間隔に区切るのではなく、
    <table>
      <tr>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
      </tr>
    </table>
    それぞれうまく分類できる区切り位置を見つける。
    <table>
      <tr>
        <th style="width:10px;border-color:black;"></th>
        <th style="width:30px;border-color:black;"></th>
        <th style="width:25px;border-color:black;"></th>
        <th style="width:40px;border-color:black;"></th>
        <th style="width:5px;border-color:black;"></th>
        <th style="width:50px;border-color:black;"></th>
      </tr>
    </table>
  - `scipy.optimize` の `minimize` 関数で Nelder-Mead 法などを用いる。
- カスタム目的関数による MAE の最適化
  - MAE: Mean Absolute Error（平均絶対誤差）は、勾配が不連続で二階微分値が微分可能な点で 0 になってしまうため、xgboost などで目的関数としてセットできない（学習できない）。
  - そこで、MAE のグラフ形状に近く、かつ勾配が連続になる代替の関数として、__Fair__ や __Psuedo-Huber__ などの関数をカスタム目的関数としてい使用する。
- MCC の PR-AUC による近似
  - MCC は閾値の最適化が必要で、閾値にセンシティブなので、モデル選択の指標としては不安定。
  - Kaggle の「Bosch Production Line Performance」では、正例に比べて負例のデータ数が非常に多く、AUC も扱いづらかった。
  - 正例が非常に多い場合、MCC の式は Precision と Recall の幾何平均 √(precision × recall) で近似することができる。長丁場のコンペではこういった工夫をして、大局的に良い方向にモデル選択を行うのが重要。
  - シンプルにモデル選択のための評価指標を logloss として進めるのも有力（それぞれの予測確率の改善がスコア上昇につながるので）。

### リーク (data leakage)

2 種類のリークがある。

- 分析コンペの設計の問題
  - 予測に有用な情報が漏れて使えるようになってしまうもの。
  - 分析という観点では本質的ではないが、リークがあるときは積極的に見つけざるを得ない。
  - 例
    - テストデータが学習データに入っている
    - テストデータの特徴量に予測対象の値が入っている
    - 将来の情報が過去のデータに入っている
    - 第三者のデータに有用な情報が含まれている
    - etc.
  - 実際にあった例
    - Google Analytics 関連の予測コンペで、テストデータが Google Analytics のデモアカウントに含まれていた。
    - 各種特徴量を比較すると、テストデータと同じユーザーが学習データに含まれていることを判別できた。
    - ID に時間情報として扱えるデータが含まれていた。
- モデル作成時の技術的な問題
  - バリデーションの枠組みを誤ってバリデーションスコアが不当に高くなってしまうもの。
  - これは、参加者が特徴量の作成やバリデーションを行う際に注意するもの。
  - 例えば時系列データを扱う場合に、同時刻の似たレコードの目的変数をほぼそのまま予測値としてしまうと、バリデーションでだけ高いスコアが出る。


## 3. 特徴量の作成

### GBDT (Gradient Boosting Decision Tree)

- この種のライブラリとしては **`xgboost`**、**`lightgbm`** が有名。
- 決定木をベースとしたモデルで、分岐の繰り返しによって変数間の相互作用を反映するため、非線形な関係性も表現できる。
- 数値の絶対値的な大き差ではなく、大小関係のみを考慮するため、データのスケーリングは不要。
- 欠損値をそのまま扱える。one-hot エンコーディングではなく、label エンコーディングを使っても、分岐の繰り返しによって各カテゴリの影響を反映してくれる（特徴量を無闇に増やさなくて済む）。
- 一方、ニューラルネットの場合
  - 数値の大きさが影響する。
  - 欠損値は何らかの数値に置き換えが必要。
  - 前の層の出力を結合する計算によって、変数間の相互作用を反映する。

### 決定木の気持ちになって考える

- モデルから読み取りづらい情報を追加で与えてやる。
  - 例: 別テーブルと結合してデータを関連づける。
  - 例: 平均購入価格が重要そうであれば、購入額と購入数から新しい特徴量として作ってやる。

### 欠損値の扱い

- GBDT 系のライブラリであれば、欠損値はそのまま使うのが基本だが、明示的に補完した方が精度が向上することもある。
- 欠損値をもとに、新たな特徴量を作成するも有効。
- 欠損値のあるカラムを除外するのは得策ではない。
- scikit-learn のランダムフォレストなどでは欠損値をそのまま扱えないが、欠損値として通常取り得ない値（-9999など）を代入することで分岐処理をうまく動かせることがある。
- 欠損値を代表値で埋める方法
  - 平均値、中央値、対数変換してから平均をとる。
  - 別のカテゴリ変数でグループ分けしてから平均をとる。その際に、グループ内のデータ数が少なくなってしまう場合は、**Bayesian Average** という平均値計算を使うとよい。
- **欠損値を他の変数から予測する**
  - 欠損値を補完したい変数を目的変数としたモデルを作って学習する → 予測値で補完する。この際の学習には、テストデータを使ってよい。
- **欠損値から新たな特徴量を作る**
  - 欠損しているかどうかを示す二値変数
  - レコードごとの欠損数
  - 欠損の組み合わせによりパターン分類する
- pandas で欠損値の表現を指定して読み込む方法
  ```python
  # na_values で指定したものをすべて np.nan 化
  train = pd.read_csv("train.csv", na_values=["", "NA", -1, 9999])
  ```
  変数ごとに欠損値の表現が異なる場合
  ```python
  # 例: col1 列だけ欠損値が -1 で表現されているとき
  data["col1"] = data["col1"].replace(-1, np.nan)
  ```

### 数値変数の変換

以下の変換は、線形回帰やロジスティック回帰などの線形モデルや、ニューラルネットで有効。
GBDT などの決定木ベースのモデルでは効果がないことに注意。

- **標準化 (standardization)**
  - 平均を 0、標準偏差を 1 にする。
  - `sklearn.preprocessing.StandardScaler` を使う。
  - `StandardScaler` のフィット時には、テストデータを合わせて (`pd.concat`) 使ってもよいが、そこまで大きな差異は生じない。
- **Min-Max スケーリング**
  - 変数のとる範囲を特定の区間（通常は0〜1）に押し込める。
  - `sklearn.preprocessing.MinMaxScaler` を使える。
  - 平均が 0 にならない、外れ値の影響を受けやすいなどのデメリットあり。
  - 画素値（0〜255）などには適用しやすい。
- **非線形変換**
  - 対数をとる (`x2 = np.log(x)`)。
  - 1 を加えて対数をとる (`x2 = np.log1p(x)`)。0 が値として含まれているときはこっちを使う。
  - 絶対値の対数をとってから符号を戻す。
    ```python
    x2 = np.sign(x) * np.og(np.abs(x))
    ```
  - Box-Cox 変換、Yeo-Johnson 変換
    - 正規分布に近づくように変換してくれる。
  - その他
    - generalized log transformation
    - 絶対値／平方根／n乗する／四捨五入
    - 正かどうかで二値化／100円未満の端数を抽出
- **clipping**
  - 外れ値を1％タイルや99％タイルにクリップ
  - NumPy の `clip()` 関数を使う。
- **binning**
  - 区間ごとにグループ分けする。
  - pandas の `cut()` 関数、NumPy の `digitize()` 関数を使う。
- **順位への変換**
  - pandas の `rank()` 関数を使う方法、NumPy の `argsort()` を 2 回適用する方法がある。
  - さらにレコード数で割ることで 0〜1 の範囲に収められる。
- **RankGauss**
  - ランク化して正規分布にする。
  - ニューラルネットでは通常の正規化よりよい性能を示す。

### カテゴリ変数の変換

「水準」という用語は、カテゴリ変数におけるカテゴリのことを示す。

- **one-hot エンコーディング**
  - 各水準かどうかを示す 0, 1 の二値変数を作る（「ダミー変数」と呼ぶ）。
  - pandas の `get_dummies()` 関数を使う。
  - `sklearn.preprocessing.OneHotEncoder` もある。
  - カテゴリ変数の水準が多すぎて、ダミー変数が増えすぎてしまう場合は、
    - 別のエンコーディング手法を使う。
    - グルーピングして水準の数を減らす。
    - 頻度の少ないカテゴリを「その他」にまとめる。
- **label/oridinal エンコーディング**
  - 各水準を整数のインデックス（例: 0、1、2）に置き換える。
  - 変換後のインデックスは数値の大小に意味がないので、決定木をベースにした手法（GBDT など）でのみ有効。
  - `sklearn.preprocessing.LabelEncoder` を使う。
- **feature hashing**
  - one-hot エンコーディングと同様の考え方だが、生成する特徴量の数を指定することができる。
  - どの変数のフラグを立てるかは、ハッシュ関数を利用して決める。
  - `sklearn.feature_extraction.FeatureHasher` を使う。
- **frequency エンコーディング**
  - 各水準の出現日度でカテゴリ変数を置き換える。
- **target エンコーディング**
  - 各カテゴリの目的変数の平均値に置き換える。
    - 回帰の場合 ... 目的変数の平均をとる。
    - 二値分類の場合 ... 正例を 1、負例を 0 として平均をとる。
    - 多クラス分類の場合 ... クラスの数だけ二値分類があると考えて複数の特徴量を作る。
  - 目的変数のリークを防ぐために、out-of-fold で自身以外の fold で平均値を計算する必要がある。
    fold 数は 4〜10 程度がよい。
    テストデータの変換時には学習データすべてを使って平均をとって OK。
- **embedding**
  - 単語やカテゴリ変数を実数ベクトルに変換する。
  - 自然言語処理 ... Word2Vec、GloVe、fastText
  - ニューラルネット ... embedding layer
  - embedding はニューラルネットに限らず、GBDT や線形モデルでも有効。
- **順序変数**（1、2、3 や A、B、C など）
  - 決定木ベースのモデルでは、1、2、3 などをそのまま入れればよい（大小関係だけが保たれていればよい）。
  - その他のモデルでは、数値変数としてそのまま扱うか、カテゴリ変数として扱うかを考える。

### 日付・時刻

- 学習データとテストデータが時間で分けられているときは、「年」のデータをそのまま使ってはいけない。
- 「月」を周期性のあるデータとして扱うには、線形モデルでは工夫が必要。
  例えば、12 月の次が 1 月ということを表現しなければならない（円形に配置するなど）。
  GBDT などの決定木ベースのモデルでは、そのままつっこんでも大体うまくいく。
- 月初・月末・給料日、曜日・祝日・休日を特別扱いする。
- クリスマスやゴールデンウィーク、ブラックフライデーかどうかの二値変数。
- ある時点からの経過時間。

### 変数の組み合わせ

- **数値変数 × カテゴリ変数**
  - カテゴリごとに統計量（平均や分散）をとる。
- **数値変数 × 数値変数**
  - 加減乗除、余りなど。
- **カテゴリ変数 × カテゴリ変数**
  - 組み合わせて新たなカテゴリ変数を作る。
  - one-hot エンコーディングすると水準が多くなりすぎるので、target エンコーディングを適用するのがよい。
- **行の統計量**
  - レコードごとに複数の変数血の統計をとる。

### 他のテーブルの結合

- pandas の `merge()` メソッドを使う。


## 4. モデルの作成

### モデルとは何か

- バリデーションは通常はクロスバリデーションで行う。
- テストデータの予測をするときは 2 つの方法がある
  - 各フォールドで学習したモデルを使って予測して平均をとる
  - 学習データ全体に対して改めてモデルを学習する
- オーバーフィッティング ... 学習データだけスコアが良い。モデルが複雑すぎる
- アンダーフィッティング ... 学習データでも（それ以外でも）スコアが悪い。学習不足。
- 正則化 (regularization)
  - 学習時にモデルが複雑な場合に罰則を課す。
  - 罰則が上回るほど予測に寄与する場合のみモデルが複雑になるため、過学習を抑えることができる。
- アーリーストッピング
  - GBDT やニューラルネットなどのライブラリが備えている機能。
    学習時にバリデーションスコアが上がらなくなったら学習を打ち切る。
  - イテレーションごとに学習データのスコアは通常良くなり続けるが、バリデーションのスコアはある程度で改善が止まる（それ以上やると過学習になる）。
- バギング (bootstrap aggregating)
  - __同じ種類のモデルを並列に複数作成__ し、それぞれの予測値の平均などをとる。
  - 各モデルでデータをランダムに抽出して使ったり、学習用の乱数シードを変えることで汎化性能を高める。
  - ランダムフォレストはバギングを利用している。
- ブースティング (boosting)
  - __複数のモデルを直列に組み合わせる__。
  - GBDT（勾配ブースティング）はブースティングを利用している。

### GBDT の主なライブラリ

- xgboost ... 2014 年公開
  - 目的関数を変えると色々なタスクに対応できる。
    - 回帰 ... 二乗誤差
    - 二値分類 ... logloss
    - マルチクラス分類 ... multi-class logloss
- lightgbm ... 2016 年公開
  - xgboost より高速なので現在よく使われている。
- catboost ... 2017 年公開
  - カテゴリ変数の扱いに工夫あり。
  - 遅いのでそれほど使われていない。

### ニューラルネットの主なライブラリ

- Keras/TensorFlow
- PyTorch

### 線形モデル

- 単体では精度が低いので、GBDT やニューラルネットにはほぼ勝てない。
- アンサンブルの 1 つのモデルや、スタッキングの最終層などに適用するのが主な使い方。
- Lasso ... L1 正則化を行う線形モデル
- Ridge ... L2 正規化を行う線形モデル
- 分類タスクに使うときはロジスティック回帰モデルを使う。
  ロジスティック回帰では、線形回帰で求めた値にシグモイド関数を適用し、0〜1 の確率を求める。
- 線形モデルの主なライブラリ
  - scikit-learn の `linear_model` モジュール
    - 分類タスク ... `LogisticRegression`
    - 回帰タスク ... `Ridge`

### その他のモデル

- その他のモデルはアンサンブルのモデルの 1 つとして使うのが基本。
- k 近傍法 (kNN: k-Nearest Neighbor algorithm)
  - scikit-learn の `neighbor` モジュールの `KNeighborsClassifier`、`KNeighborsRegressor` を使う。
- ランダムフォレスト (RF: Random Forest)
  - scikit-learn の `ensemble` モジュールの `RandomForestClassifier`、`RandomForestRegressor` を使う。
- ERT: Extremely Randomized Trees
  - scikit-learn の `ensemble` モジュールの `ExtraTreesClassifier`、`ExtraTreesRegressor` を使う。
- RGF:Regularized Greedy Forest
  - Regularized Greedy Forest (`rgf_python`) パッケージがある。
- FFM: Field-aware Factorization Machines
  - `libffm` パッケージや `xlearn` パッケージがある。
  - FM: Factorization Machines を発展させたモデル。
  - レコメンド系のタスクと相性がよい。

### その他のテクニック

- __疑似ラベリング (pseudo labeling)__
  - テストデータから導き出した予測値を目的変数の値とみなし、学習データに加えて再度学習する手法。
  - この予測値のことを疑似ラベル (pseudo label) と呼ぶ。
- フォルダ構成について
  - データサイエンスプロジェクトのディレクトリ構成をどうするか。
  - Patterns for Research in Machine Learning を参考に。


## 5. モデルの評価

## 6. モデルのチューニング　

## 7. アンサンブル


{{< amazon
  itemId="B07YTDBC3Z"
  title="Kaggleで勝つ データ分析の技術"
  author="門脇大輔、阪田隆司、保坂桂佑、平松雄司"
  publisher="技術評論社"
  imageUrl="https://m.media-amazon.com/images/I/71kBM0KZSAL._SY522_.jpg"
>}}
