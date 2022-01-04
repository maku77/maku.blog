---
title: "Jadeの構文: CSS や JavaScript を使用する"
url: "/p/d8cx8a3"
date: "2013-12-29"
tags: ["Jade"]
weight: 205
---

Jade テンプレート内に、直接 CSS や JavaScript のコードを記述するときは、ドット記法によるテキストブロック内に記述します。

{{< code title="test.jade" >}}
style.
  body {
    color: #111;
    background: #ccc;
  }
script.
  var message = 'Hello';
  alert(message);
{{< /code >}}

{{< code title="実行結果" >}}
$ jade --pretty < test.jade
<style>
  body {
    color: #111;
    background: #ccc;
  }
</style>
<script>
  var message = 'Hello';
  alert(message);
</script>
{{< /code >}}

`link` タグや `script` タグによって、外部の CSS や JavaScript コードをロードするには、以下のようにします。

{{< code title="test.jade" >}}
doctype html
html
  head
    meta(charset='UTF-8')
    link(rel='stylesheet', href='main.css')
    title Hello
    script(src='jquery.js')
    script(src='main.js')
  body
    p Hello
{{< /code >}}

{{< code title="実行結果" >}}
$ jade --pretty < test.jade
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="main.css">
    <title>Hello</title>
    <script src="jquery.js"></script>
    <script src="main.js"></script>
  </head>
  <body>
    <p>Hello</p>
  </body>
</html>
{{< /code >}}

