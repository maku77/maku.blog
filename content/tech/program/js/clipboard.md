---
title: "JavaScript で任意のテキストをクリップボードにコピーする"
url: "/p/buk5i2o"
date: "2020-11-20"
lastmod: "2020-11-26"
tags: ["JavaScript"]
---

copyToClipboard 関数
----

次の `copyToClipboard` 関数を使うと、引数で指定したテキストを OS のクリップボードにコピーすることができます。

{{< code lang="javascript" >}}
function copyToClipboard(text){
  // テキストコピー用の一時要素を作成
  const pre = document.createElement('pre');

  // テキストを選択可能にしてテキストセット
  pre.style.webkitUserSelect = 'auto';
  pre.style.userSelect = 'auto';
  pre.textContent = text;

  // 要素を追加、選択してクリップボードにコピー
  document.body.appendChild(pre);
  document.getSelection().selectAllChildren(pre);
  const result = document.execCommand('copy');

  // 要素を削除
  document.body.removeChild(pre);

  return result;
}
{{< /code >}}

JavaScript からクリップボードにテキストをコピーするときは、任意の HTML 要素のテキストを選択して、`document.execCommand('copy')` を実行するという流れになります。
そのため、上記の関数では、テキスト選択用の一時的な `pre` 要素を作成しています。


使用例
----

例えば次のようにすると、ボタンを押したときにクリップボードにテキストをコピーできます。

<button id="copy">クリップボードにコピー</button> ← 実際に動作します

<script>
function copyToClipboard(text){
  const pre = document.createElement('pre');
  pre.style.webkitUserSelect = 'auto';
  pre.style.userSelect = 'auto';
  pre.textContent = text;
  document.body.appendChild(pre);
  document.getSelection().selectAllChildren(pre);
  const result = document.execCommand('copy');
  document.body.removeChild(pre);
  return result;
}
window.addEventListener('DOMContentLoaded', () => {
  document.getElementById('copy').addEventListener('click', () => {
    copyToClipboard('こんにちは！\nテキストがコピーされたよ！');
  });
});
</script>

{{< code lang="html" title="sample.html" >}}
<button id="copy">クリップボードにコピー</button>

<script>
window.addEventListener('DOMContentLoaded', () => {
  document.getElementById('copy').addEventListener('click', () => {
    copyToClipboard('こんにちは！\nテキストがコピーされたよ！');
  });
});
</script>
{{< /code >}}

