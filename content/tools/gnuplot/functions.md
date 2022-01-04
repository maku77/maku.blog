---
title: "gnuplot サンプル: いろんなグラフ用の関数 (functions.gp)"
url: "/p/e2h3dg4"
date: "2002-12-06"
tags: ["gnuplot"]
weight: 404
---

{{< code lang="gnuplot" title="lib/functions.gp" >}}
#
# Gabor のマザー・ウェーブレット
#
# 使用例:
#  plot [-20:20] gabor_mother(x, 8)
#
gabor_mother(x,sigma) = (1/(2*sqrt(pi)*sigma)) * exp(-x**2/sigma**2) * cos(x)

#
# メキシカン・ハット
#
mexican_hat(x) = (1-2*x**2)*exp(-x**2)

#
# 正規分布
#
#              1
# f(x) = ----------- e ^(- (x-μ)^2 / 2σ^2)
#         √(2π)σ
#
#  σ … 散らばり具合  -- a
#  μ … 平均値        -- b
#
#
# 使用例：
#  plot normal_distribution(x, 3, 0)
#
normal_distribution(x, a, b) = exp(-((x-b)**2)/(2*(a**2))) / sqrt(2*pi)*a

# シグモイド関数
sigm(x) = 1 / (1 + exp(-x))
{{< /code >}}

