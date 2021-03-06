---
title: "Jadeの構文: テキストブロックでテキストノードを生成する"
url: "/p/wwao94j"
date: "2013-12-29"
tags: ["Jade"]
weight: 204
---

Jade のテンプレートファイルでは、次のように要素名の後ろに続けてテキストを記述することで、__要素内にテキストノードを作成__ することができます。

```css
p Hello World
```

テキストの内容が長くなり、複数行に分けて記述したいときは、__バー記法 (bar notation) やドット記法 (dot notation) を使用した「テキストブロック」__ を作成します。

{{< code title="バー記法によるテキストブロックの例" >}}
p
  | long long long long long
  | long long long long long
  | long long long description.
{{< /code >}}

{{< code title="ドット記法によるテキストブロックの例" >}}
p.
  long long long long long
  long long long long long
  long long long description.
{{< /code >}}

__ドットは、必ずタグ名の直後にスペースを入れずに記述__ してください。
スペースを入れると、ドットがテキスト内容として認識されてしまいます。

{{< code lang="html" title="出力" >}}
<p>long long long long long
long long long long long
long long long description.</p>
{{< /code >}}

これらの記法は、ソースコードなど、改行やインデントが重要なテキストを `pre` 要素として出力する際に便利です。

```
pre.
  #include <iostream>
  int main() {
      return 0;
  }
```

バー記法 (bar notation) によるテキストブロックは、テキストノードの前後に同じ階層で別の要素がある場合にも使用します。

```
div
  span AAA
  | BBB
  span CCC
```

