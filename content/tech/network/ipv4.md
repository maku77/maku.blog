---
title: "IPv4 アドレスの解析"
url: "p/4sypaw3/"
date: "2023-12-01"
tags: ["network"]
draft: true
---

<style>
#ipv4-input-w6oyd5f {
  text-align: center;
  width: 20em;
  max-width: 100%;
  font-size: 20pt;
}

#ipv4-display-w6oyd5f {
  display: inline-block;
  border: 2px solid gray;
}

.bit-zero-w6oyd5f {
  display: inline-block;
  font-weight: bold;
  font-size: 14pt;
  width: 1em;
  background: #eee;
  text-align: center;
}

.bit-one-w6oyd5f {
  display: inline-block;
  font-weight: bold;
  font-size: 14pt;
  width: 1em;
  background: #8f9;
  text-align: center;
}

.separator-w6oyd5f {
  display: inline-block;
  font-size: 14pt;
  width: 1px;
  background: gray;
}
</style>

<div style="text-align: center">
  <input id="ipv4-input-w6oyd5f" placeholder="IPv4 アドレスを入力（例: 127.0.0.1）"></input>
</div>
<div>
  <label>二進数表記: <div id="ipv4-display-w6oyd5f"></div></label>
</div>

<script>
function main() {
  const ipv4Input = document.getElementById("ipv4-input-w6oyd5f");
  const ipv4Display = document.getElementById("ipv4-display-w6oyd5f");

  ipv4Input.addEventListener("input", handleInput)

  function handleInput() {
    const ipAddress = ipv4Input.value.trim();

    // IPv4アドレスの各セグメントを取得し、それぞれを2進数に変換
    const segments = ipAddress.split(".");
    if (segments.length > 4) {
      error("ドットで区切られたセグメントが多すぎます");
      return;
    }

    // "001010001001..." のようなバイナリ表現にする
    let binaryRepresentation = "";
    for (const seg of segments) {
      const byteNum = parseInt(seg, 10);
      if (!(0 <= byteNum && byteNum <= 255)) {
        error("各セグメントの値は 0～255 の範囲でなければいけません");
        return;
      }
      const binaryStr = toBinaryString(byteNum)
      binaryRepresentation += binaryStr;
    }

    // 0と1で表示するHTMLを作成
    let html = "";
    for (let i = 0; i < binaryRepresentation.length; i++) {
      const bit = binaryRepresentation.charAt(i);
      const bitClass = (bit === "0") ? "bit-zero-w6oyd5f" : "bit-one-w6oyd5f";
      html += `<span class="${bitClass}">${bit}</span>`;
      if ((i + 1) % 8 == 0) {
        html += '<span class="separator-w6oyd5f">&nbsp;</span>';
      }
    }
    ipv4Display.innerHTML = html;
  }

  function error(message) {
    ipv4Display.innerHTML = `<b>${message}</b>`;
  }

  function toBinaryString(byteNum) {
    return byteNum.toString(2).padStart(8, "0");
  }
}

document.addEventListener("DOMContentLoaded", main);
</script>
