---
title: "ランダム ID 生成ツール"
url: "/p/3nx9is3"
date: "2018-04-21"
tags: ["ツール"]
---

<form style="color:white; background:#c50; padding:1rem; text-align:center;">
  <label>
    文字数:
    <input id="digits" type="number" value="7" style="width:3.5em">
  </label>
  <label>
    文字候補:
    <input id="candidate" type="text" value="23456789abcdefghijkmnopqrstuvwxyz" required style="width:20em"></input>
  </label>
  <div id="result" style="border-radius:1rem; letter-spacing:0.1em; text-align: center; background:#fff0e0; color:#c50; font-size: 2.5rem; font-weight: bolder; margin: 1rem 1rem 0 1rem; padding: 0.7em;"></div>
  <button onclick="generate(); return false;" style="margin-top:1rem; font-size:1.5em;">再生成</button>
</form>

<script>
function generate() {
  // 最大桁数を 100 文字までに制限
  const $digitsElem = document.getElementById('digits');
  const digits = Math.min(100, $digitsElem.value);
  $digitsElem.value = digits;

  // ランダム文字列を生成
  const candidate = document.getElementById('candidate').value;
  let result = '';
  for (let i = 0; i < digits; ++i) {
    let index = Math.floor(Math.random() * candidate.length);
    result += candidate.charAt(index);
  }

  // 結果表示
  document.getElementById('result').innerText = result;
}
window.onload = generate();
</script>

指定した文字数のランダムな ID を生成します。
数字の `1`（いち）とアルファベット小文字の `l`（エル）、数字の `0`（ぜろ）とアルファベット大文字の `O`（オー）は区別しにくいので、デフォルトでは含めないようにしています。
このような UX 上の工夫は、さまざまな標準プロトコルで推奨されています。
例えば、下記は RFC 8628 におけるユーザーコード表示の例です。

{{% accordion title="参考: RFC 8628 - OAuth 2.0 Device Authorization Grant の例"%}}
It is RECOMMENDED to avoid character sets that contain two or more characters that can easily be confused with each other, like __"0" and "O" or "1", "l" and "I"__. Furthermore, to the extent practical, when a character set contains a character that may be confused with characters outside the character set, a character outside the set MAY be substituted with the one in the character set with which it is commonly confused; for example, "O" may be substituted for "0" when using the numerical 0-9 character set.
{{% /accordion %}}

