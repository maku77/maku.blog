---
title: "数学メモ: TeX チートシート"
url: "/p/oau2e7h"
date: "2013-04-29"
tags: ["数学", "TeX", "cheatsheet"]
changes:
  - 2023-10-20: 表形式に整理して、分散などの項目を追加。
use_math: true
---

演算子、等号（不等号）
----

### 演算子

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $a+b$ | `a+b` | plus |
| $a-b$ | `a-b` | minus |
| $a \times b$ | `a \times b` | multiplied by, times |
| $a \pm b$ | `a \pm b` | plus or minus |

### 等号・不等号

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $a = b$ | `a = b` | equal to |
| $a \equiv b$ | `a \equiv b` | identical to （定義・合同） |
| $a \neq b$ | `a \neq b` | not equal to |
| $a \lt b$ | `a \lt b` | less than （そのまま `<` でも OK） |
| $a \gt b$ | `a \gt b` | greater than （そのまま `>` でも OK） |
| $a \leq b$ | `a \leq b` | less than or equal to |
| $a \geq b$ | `a \geq b` | greater than or equal to |
| $a \ll b$ | `a \ll b` | much less than |
| $a \gg b$ | `a \gg b` | much greater than |

### 近似

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $a \sim b$ | `a \sim b` | similar |
| $a \simeq b$ | `a \simeq b` | similar equal |
| $a \approx b$ | `a \approx b` | approximately equal |

平均 (Mean)、標準偏差（Standard Deviation）、分散 (Variance)
----

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $\bar{x}$<br/>$\overline{x}$ | `\bar{x}`<br/>`\overline{x}` | サンプル平均 |
| $\mu$ | `\mu` | 母集団平均 |
| $s$ | `s` | サンプル標準偏差 |
| $\sigma$ | `\sigma` | 母集団標準偏差 |
| $s^2$ | `s^2` | サンプル分散 |
| $\sigma^2$ | `\sigma^2` | 母集団分散 |


対数 (Logarithm)
----

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $\log x$ | `\log x` | 対数 |
| $\log_2 x$ | `\log_2 x` | 対数の底が 2 の場合 |
| $\log_{10} x$ | `\log_{10} x` | 常用対数 (common logarithm) |
| $\ln x$ | `\ln x` | 自然対数 (natural logarithm) |

- 10 を底とする対数を __常用対数 (common logarithm)__ と呼びます。
- ネイピア数 $\mathrm{e}$ を底とする対数を __自然対数 (natural logarithm)__ と呼びます。
自然対数はラテン語で logarithmus naturalis なので ln と略されます。


分数、連分数
----

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $y = \frac{a}{b}$ | `y = \frac{a}{b}` | 分数 (fraction) |
| $y = \frac{a-1}{\frac{b+1}{c+2}}$ | `y = \frac{a-1}{\frac{b+1}{c+2}}` | 連分数 (continued fraction) |


論理演算（論理積∩、論理和∪、含意⇒）
----

<table>
<thead>
  <tr><th>表示</th><th>TeX</th><th>説明</th>
</thead>
<tbody>
  <tr>
    <td>$\cup$</td>
    <td><code>\cup</code></td>
    <td>論理和 (logical add, logical sum, OR)</td>
  </tr>
  <tr>
    <td>$\cap$</td>
    <td><code>\cap</code></td>
    <td>論理積 (logical AND, logical multiply)</td>
  </tr>
  <tr>
    <td>$\oplus$</td>
    <td><code>\oplus</code></td>
    <td>排他的論理和 (exclusive OR)</td>
  </tr>
  <tr>
    <td>$A && B$</td>
    <td><code>A && B</code></td>
    <td>条件付き論理積 (conditional AND, short-circuit logical AND)</td>
  </tr>
  <tr>
    <td>$A || B$</td>
    <td><code>A || B</code></td>
    <td>条件付き論理和 (conditional OR, short-circuit logical OR)</td>
  </tr>
  <tr>
    <td>$\Rightarrow$</td>
    <td><code>\Rightarrow</code></td>
    <td>含意 (conditional implication)</td>
  </tr>
</tbody>
</table>


順列、組み合わせ
----

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| ${}_n \mathrm{P} _r$ | `{}_n \mathrm{P} _r` | 順列 (permutation) |
| ${}_n \Pi _r$ | `{}_n \Pi _r` | 重複順列 (repeated permutation) |
| ${}_n \mathrm{C} _r$ | `{}_n \mathrm{C} _r` | 組み合わせ1 (combination) |
| ${n}\choose{r}$ | `{n}\choose{r}` | 組み合わせ2 (combination) ※海外ではこう書くことがあります |
| ${}_n \mathrm{H} _r$ | `{}_n \mathrm{H} _r` | 重複組み合わせ (repeated combination) |


Sum 型記号
----

| 表示 | TeX | 説明 |
| :--: | ---- | ---- |
| $\sum$ | `\sum` | 合計 (sum) |
| $\sum_{k=1}^{n}k$ | `\sum_{k=1}^{n}k` | 合計 (sum) |
| $\sum_{i=1}^{n}x_{i}$ | `\sum_{i=1}^{n}x_{i}` | 合計 (sum) |

等号 align
----

$$
\begin{align}
(a + b)^2 &= (a + b)(a + b) \\\\
          &= a^2 + 2ab + b^2
\end{align}
$$

```tex
\begin{align}
(a + b)^2 &= (a + b)(a + b) \\\\
          &= a^2 + 2ab + b^2
\end{align}
```

