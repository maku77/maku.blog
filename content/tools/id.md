---
title: "ランダムID生成"
url: "/p/3nx9is3"
date: "2018-04-21"
tags: ["ツール"]
---

<form style="color:white; background:#c50; padding:1rem; text-align:center;">
  <label>
    文字数:
    <input id="digits" type="number" value="7" style="width:3em">
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
  var candidate = document.getElementById('candidate').value;
  var digits = document.getElementById('digits').value;
  var result = '';
  for (var i = 0; i < digits; ++i) {
    var index = Math.floor(Math.random() * candidate.length);
    result += candidate.charAt(index);
  }
  document.getElementById('result').innerText = result;
}
window.onload = generate();
</script>

指定した文字数のランダムな ID を生成します。
数字の1（いち）とアルファベット小文字のl（エル）、数字の0（ぜろ）とアルファベット大文字のO（オー）は区別しにくいので、デフォルトでは含めないようにしています。

