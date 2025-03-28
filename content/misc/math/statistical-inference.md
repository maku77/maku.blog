---
title: "数学メモ: 母集団の推測方法のまとめ"
url: "p/y2gu9vz/"
date: "2024-11-13"
tags: ["数学"]
useMath: true
---

推測統計のお勉強メモです。
母集団から抽出した標本の値を使って、母集団の値を推測する方法をまとめます。
例えば、何人かにアンケートを取って、日本人全体（母集団）がどのようになっているかを語るときにこの知識が必要になります。

母平均 \\( \mu \\) の推測
----

母集団から抽出した標本の値を使って、**どのように母平均 \\( \mu \\) を推測するかのパターン** です。
標本サイズが 1 のときはその値を **\\( x \\)**、標本サイズが 2 以上の場合は標本平均を **\\( \bar{x} \\)** と表現しています。

| | 母分散 \\( \sigma^2 \\) が既知のとき | 母分散 \\( \sigma^2 \\) が未知のとき |
| ---- | ---- | ---- |
| 標本サイズ \\( n=1 \\) のとき | （**母集団が正規分布** に従うことが前提）1 つの値 \\( x \\) と母分散 \\( \sigma^2 \\) を使って区間推定する。これはつまり、母集団の分布に従って値を取り出しているだけ。 $$ z = \frac{x - \mu}{\sigma} $$ と標準化する（平均を引いて、標準偏差で割る）と、標準化変量 \\( z \\) が **標準正規分布** に従うことを利用して、母平均 \\( \mu \\) を **区間推定** できる。 | 母平均 \\( \mu \\) は、取り出した唯一の値 \\( x \\) と等しいだろうと **点推定** するしかない。何％くらい確からしいということも語れない。 |
| 標本サイズ \\( n \geq 2 \\) のとき | （**母集団が正規分布** に従うことが前提）標本平均 \\( \bar{x} \\) と母分散 \\( \sigma^2 \\) を使って区間推定する。標本平均 \\( \bar{x} \\) の分散の理論値は \\( \frac{\sigma^2}{n} \\) になるので、 $$ z = \frac{\bar{x} - \mu}{\frac{\sigma}{\sqrt{n}}} $$ と標準化すると、標準化変量 \\( z \\) が **標準正規分布** に従うことを利用して、母平均 \\( \mu \\) を **区間推定** できる。 | （**母集団が正規分布** に従うことが前提）母分散 \\( \sigma^2 \\) の代わりに、**不偏分散** \\( \hat{\sigma}^2 = \frac{\sum{(x_i - \bar{x})^2}}{n - 1} \\) を使って区間推定する。 $$ t = \frac{\bar{x} - \mu}{\frac{\hat{\sigma}}{\sqrt{n}}} $$ と標準化すると、\\( t \\) が **t 分布** に従うことを利用して、母平均 \\( \mu \\) を **区間推定** できる。 |
| 標本サイズ \\( n \geq 30 \\) のとき | （**中心極限定理** により **母集団の分布に依存しない**）上記と同様に母平均 \\( \mu \\) の推定を行える。$$ z = \frac{\bar{x} - \mu}{\frac{\sigma}{\sqrt{n}}} $$ 標本サイズ \\( n \\) が大きくなると、標本平均 \\( \bar{x} \\) は収束していき、最終的に母平均 \\( \mu \\) と一致する。 | （**中心極限定理** により **母集団の分布に依存しない**）母分散 \\( \sigma^2 \\) の代わりに、標本分散 \\( s^2 =  \frac{\sum{(x_i - \bar{x})^2}}{n} \\) あるいは不偏分散 \\( \hat{\sigma}^2 = \frac{\sum{(x_i - \bar{x})^2}}{n - 1} \\) を使って区間推定する。どちらもほぼ同じ値になるのでどっちでもよい。 $$ z = \frac{\bar{x} - \mu}{\frac{s}{\sqrt{n}}} $$ と標準化すると、標準化変量 \\( z \\) が **標準正規分布** に従うことを利用して、母平均 \\( \mu \\) を **区間推定** できる。 |
| 標本サイズ \\( n=全数 \\) のとき | 「母平均 \\( \mu \\) = 標本平均 \\( \bar{x} \\)」 は自明。 | 同左。 |

ポイント:

- 標本平均 \\( \bar{x} \\) の値が母平均と等しいであろうと推測するのは **点推定** です。ここに **分散の情報** を加えることで、**確率を使った区間推定** を行えるようになります。
- 標本平均 \\( \bar{x} \\) の分散の理論値は \\( \sigma^2 / n \\) です。ちなみに、この理論値は母誤差分散 \\( \sigma_\bar{x}^2 \\) と呼びますが、たぶん覚えなくて大丈夫（参考: [用語集](/p/n597k8j/)）。要するに標本サイズ \\( n \\) が大きくなるほど、その標本の平均値 \\( \bar{x} \\) のぶれ幅は小さくなり、より精度の高い区間推定ができるようになるということです。
- **母集団の分布が分からないとき** は、標本平均 \\( \bar{x} \\) が正規分布に従うという前提も置けなくなります。この場合は、標本サイズ \\( n \\) を十分に大きくする（目安は \\( n \geq 30 \\)）ことで、標本平均 \\( \bar{x} \\) の分布（標本分布）が正規分布に従うようになるという性質（**中心極限定理**）を利用します。
- 母分散 \\( \sigma^2 \\) が分からないときは（これが一般的だけど）、標本から計算可能な **不偏分散 \\( \hat{\sigma}^2 \\)** を母分散 \\( \sigma^2 \\) の代わりに使います。その場合は、標準正規分布ではなく **t 分布** の上側確率（両側確率）を使って区間推定することに注意してください。ただし、標本サイズ \\( n \\) が十分に大きいときは、上記の通り中心極限定理が使えるため、t 分布よりシンプルな **標準正規分布** を使って区間推定できます。

{{% private %}}
- 確率変数 X と Y の和の分散
  - \\( V(X + Y) = V[X] + V[Y] + 2Cov(X, Y) \\)
  - \\( \sigma^2_{x_1 + x_2 + \cdots + x_n} = n \sigma^2 \\)
- 確率変数 X と Y の差の分散
  - \\( V(X + Y) = V[X] + V[Y] - 2Cov(X, Y) \\)
- 母分散 \\( \sigma^2 \\) の区間推定には \\( \chi^2 \\) 分布を使う。
{{% /private %}}

