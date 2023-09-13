---
title: "D3.js の d3-force でノードの引力・反発力・重力を考慮したレイアウトを行う (2) 応用編"
url: "p/9ujohp6/"
date: "2023-09-14"
tags: ["D3.js"]
draft: true
---

ノードの設定
----

ノードにかかる力をシミュレートするには、__`d3.forceSimulation()`__ 関数でシミュレーションオブジェクトを生成します。
この関数に、ノードの配列を渡すと、自動的に

- `index` ... 0 始まりのインデックス
- `x` ... X 座標
- `y` ... Y 座標
- `x` ... X 軸の移動速度
- `y` ... Y 軸の移動速度

{{% private %}}
// 摩擦係数。大きくすると止まりやすくなる。デフォルトは 0.04?
simulation.velocityDecay(0.01)

// 全ノードが中心に集まるようにする
simulation.force("center", d3.forceCenter(width / 2, height / 2))

// 全ノードが指定した X 座標、Y 座標に集まるようにする
simulation.force("x", d3.forceX(width / 2))
simulation.force("y", d3.forceY(height / 2))
{{% /private %}}

