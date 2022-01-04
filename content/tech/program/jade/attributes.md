---
title: "Jadeの構文: クラス属性と ID 属性を設定する"
url: "/p/n6cwnk7"
date: "2013-12-30"
tags: ["Jade"]
weight: 202
---

Jade のテンプレートファイルでは、CSS セレクタと同じ表現で、HTML タグのクラス属性、ID 属性を指定することができます。

{{< code >}}
p.class1 AAA  #=> <p class="class1">AAA</p>
span#id1 BBB  #=> <span id="id1">BBB</span>
{{< /code >}}

もちろんクラス属性値は、複数指定可能です。

{{< code >}}
p.class1.class2.class3 AAA  #=> <p class="class1 class2 class3">AAA</p>
{{< /code >}}

クラス属性、ID 属性を指定するときに、タグ名を省略すると、デフォルトで `div` タグとみなされます。

{{< code >}}
.class1 AAA  #=> <div class="class1">AAA</div>
#id1 BBB     #=> <div id="id1">BBB</div>
{{< /code >}}

クラス属性と ID 属性は、どちらを先に指定しても構いません。

{{< code >}}
.class1#id1.class2  #=> <div id="id1" class="class1 class2">AAA</div>
{{< /code >}}

