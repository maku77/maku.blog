---
title: "Jadeの構文: タブとスペースを混ぜない"
url: "/p/2sdyex5"
date: "2013-12-30"
tags: ["Jade"]
weight: 201
---

Jade では、インデントとしてスペースとタブのどちらでも使用できますが、テンプレートファイル (`.jade`) 内では、どちらか一方に統一する必要があります。
スペースとタブが混ざっていると、変換時に Invalid indentation のエラーが発生します。

{{< code title="エラーになる例" >}}
$ jade test.jade

/usr/local/lib/node_modules/jade/lib/runtime.js:200
  throw err;
        ^
Error: test.jade:4
    2| div
    3|      b Maku
  > 4|     b Moja
    5|
    6|

Invalid indentation, you can use tabs or spaces but not both
{{< /code >}}

