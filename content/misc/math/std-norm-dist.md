---
title: "統計ツール: 標準正規分布の上側確率"
url: "p/3ukihbq/"
date: "2024-09-21"
tags: ["数学"]
useMath: true
---

<style>
  #table-u3confr {
    font-size: 10pt;
    line-height: 0.8;
    max-height: 400px; /* テーブル全体の高さを制限 */
    overflow-y: auto; /* 必要に応じて縦スクロールバーを表示 */
    border: solid 1px #ccc;
  }
  #table-u3confr thead {
    position: sticky;
    top: 0;
  }
  #table-u3confr th {
    border: none;
  }
  #table-u3confr .highlight {
    background: #f38 !important;
    color: white;
  }
  #table-u3confr td:hover {
    background: #fce;
  }
</style>

標準正規分布（z 分布）の上側確率の一覧表です。
統計の勉強用に作りました。
統計の教科書の巻末によく載っているやつです。
表の各セル（z 値）をクリックするとグラフの塗りつぶし領域が更新されます。

<figure style="text-align: center;">
  <svg id="svg-aji6bf9" width="600" viewBox="0 0 600 200" style="max-width: 100%;"></svg>
  <figcaption>図: 標準正規分布の上側確率 \( P(z \geq \) <span id="zval-f6cnepx"></span>\( ) = \) <span id="prob-q6z9rjv" style="color: #f38; font-weight: bolder;"></span></figcaption>
</figure>

<table id="table-u3confr" style="margin: 1em auto">
  <thead>
    <tr>
      <th>z 値</th>
      <th>.00</th><th>.01</th><th>.02</th><th>.03</th><th>.04</th>
      <th>.05</th><th>.06</th><th>.07</th><th>.08</th><th>.09</th>
    </tr>
  </thead>
  <tbody id="table-body-xonih2w"></tbody>
</table>

標準正規分布は、平均 \\( \mu = 0 \\)、分散 \\( \sigma^2 = 1 \\) の正規分布 \\( N(0, 1) \\) です。
正規分布のグラフが左右対称であることと合計面積が 1 であることを利用すると、次のようにいろいろな部分の面積（累積確率）を計算できます。

- 下側確率は上側確率と同じ値になります。
  例えば、\\( P(z \leq -1.96) = P(z \geq 1.96) = 0.025 \\) です。
- 塗りつぶされていない部分の面積は、1 から上側確率をマイナスすることで求められます。
  例えば、\\( P(z \lt 1.96) = 1 - P(z \geq 1.96) = 1 - 0.025 = 0.975 \\) と計算できます。
- 範囲指定の確率も求められます。
  例えば、\\( P(-1 \lt z \lt 2) = 1 - P(z \geq 1) - P(z \geq 2)\\) です。

つまり、すべて上側確率 \\( P(z \geq ●) \\) の値を利用して計算できるんですね。

٩(๑❛ᴗ❛๑)۶ 便利っ

<script type="module">
import * as d3 from "https://cdn.jsdelivr.net/npm/d3@7/+esm";

// 初期表示時の z 値
const DEFAULT_ZVAL = 1.96;
const VIEWBOX_WIDTH = 600;  // TODO: できれば動的に取得したい
const VIEWBOX_HEIGHT = 200;  // TODO: できれば動的に取得したい

// SVGの設定
const svg = d3.select("#svg-aji6bf9");
const margin = {top: 20, right: 22, bottom: 30, left: 30};
//const width = +svg.attr("width") - margin.left - margin.right;
const width = VIEWBOX_WIDTH - margin.left - margin.right;
//const height = +svg.attr("height") - margin.top - margin.bottom;
const height = VIEWBOX_HEIGHT - margin.top - margin.bottom;
const g = svg.append("g").attr("transform", `translate(${margin.left},${margin.top})`);

// スケールの設定
const x = d3.scaleLinear().rangeRound([0, width]);
const y = d3.scaleLinear().rangeRound([height, 0]);

// 標準正規分布の関数
function normalPdf(x) {
  return Math.exp(-(x * x) / 2) / Math.sqrt(2 * Math.PI);
}

// データの生成とドメイン設定
const data = d3.range(-4, 4.01, 0.01).map(x => ({x: x, y: normalPdf(x)}));
x.domain(d3.extent(data, d => d.x));
y.domain([0, d3.max(data, d => d.y) + 0.001]);

// 上位部分の塗り潰し用グループ（後ろに表示するために先に追加しておく）
const upperArea = g.append("g");

function updateZValue(zval) {
  // 既存の塗り潰しパスがあったら先に削除
  upperArea.selectAll("path").remove();

  // 上位部分を塗り潰すためのパスを構築
  const fillPath = d3.area()
    .x(d => x(d.x))
    .y0(height)
    .y1(d => y(d.y))
    .curve(d3.curveBasis);
  upperArea.append("path")
    .datum(data.filter(d => (d.x >= zval)))
    .attr("fill", "#f38")
    .attr("d", fillPath);
}

updateZValue(DEFAULT_ZVAL);

// X 軸の描画
g.append("g")
  .attr("transform", `translate(0,${height})`)
  .call(d3.axisBottom(x));

// Y 軸の描画
g.append("g")
  .call(d3.axisLeft(y)
    .tickValues([0, 0.1, 0.2, 0.3, 0.4])
    .tickFormat(d3.format(".1f")));

// 指定された X 位置の縦線を追加
const xGridValues = [-3, -2, -1, 0, 1, 2, 3, 4];
g.selectAll(".x-grid")
  .data(xGridValues)
  .enter()
  .append("line")
  .attr("class", "x-grid")
  .attr("x1", d => x(d))
  .attr("x2", d => x(d))
  .attr("y1", 0)
  .attr("y2", height)
  .attr("stroke", "#ccc")
  .attr("stroke-dasharray", "5,5");

// 曲線の描画（最後に描画）
const line = d3.line()
  .x(d => x(d.x))
  .y(d => y(d.y))
  .curve(d3.curveBasis);
g.append("path")
  .datum(data)
    .attr("fill", "none")
    .attr("stroke", "steelblue")
    .attr("stroke-width", 1.5)
    .attr("d", line);

// x 軸のラベル (z) を表示
g.append("text")
  .attr("x", width)
  .attr("y", height)
  .attr("fill", "#666")
  .attr("font-size", "10pt")
  .attr("transform", `translate(13, 3)`) // 少し右に移動
  .style("text-anchor", "middle")
  .text("z");

// 表のキャプションを更新
function updateCaption(zval, probability) {
  document.getElementById('zval-f6cnepx').innerText = zval;
  document.getElementById('prob-q6z9rjv').innerText = probability;
}

// 浮動小数点数の同値判断
function areAlmostEqual(a, b) {
  return Math.abs(a - b) < 0.00001;
}

// erf 関数の近似を実装（Abramowitz and Stegun の式）
function erf(x) {
  const sign = (x >= 0) ? 1 : -1;
  x = Math.abs(x);

  // Abramowitz and Stegun's approximation
  const a1 =  0.254829592;
  const a2 = -0.284496736;
  const a3 =  1.421413741;
  const a4 = -1.453152027;
  const a5 =  1.061405429;
  const p  =  0.3275911;

  // Calculate the approximation
  const t = 1.0 / (1.0 + p * x);
  const y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * Math.exp(-x * x);

  return sign * y;
}

// 上側確率を求める関数
function upperTailProbability(x) {
  return (1 - erf(x / Math.sqrt(2))) / 2;
}

// テーブルのセルを生成
function generateTable() {
  let lines = '';

  // z 値を 0.00 から 4.00 まで 0.01 刻みで表示
  for (let i = 0; i < 4; i += 0.1) {
    let line = `<tr><th>${i.toFixed(1)}</th>`;
    for (let j = 0; j <= 0.09; j += 0.01) {
      const z = i + j;
      const upperProb = upperTailProbability(z).toFixed(4);
      let clazz = 0;
      if (areAlmostEqual(z, DEFAULT_ZVAL)) {
        clazz = 'highlight';
        updateCaption(z.toFixed(2), upperProb);
      }
      //const clazz = areAlmostEqual(z, DEFAULT_ZVAL) ? 'class="highlight"' : '';
      line += `<td data-zval="${z.toFixed(2)}" class="${clazz}">${upperProb}</td>`;
    }
    lines += line +'</tr>';
  }

  // テーブルに全ての行を追加
  document.getElementById('table-body-xonih2w').innerHTML = lines;

  // 各 <td> にクリックイベントを追加
  const tdElements = document.querySelectorAll('#table-body-xonih2w td');
  tdElements.forEach(td => {
    td.addEventListener('click', function() {
      // Remove highlight from all cells
      tdElements.forEach(cell => cell.classList.remove('highlight'));
      // Add highlight to the clicked cell
      this.classList.add('highlight');
      const zval = this.getAttribute('data-zval');
      const prob = this.textContent;
      updateZValue(zval);
      updateCaption(zval, prob);
      });
    });
}

// ページ読み込み時にテーブルを生成
// （script 要素で type="module" としているので DOM 読み込み後に実行されることは保証されている）
generateTable();
</script>

