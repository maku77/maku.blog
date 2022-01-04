---
title: "TypeScript: 2つの変数の値をスワップする"
linkTitle: "2つの変数の値をスワップする"
url: "/p/k8kw8it"
date: "2020-06-03"
tags: ["TypeScript"]
---

TypeScript 独自の構文ではありませんが、ECMAScript 2015 (ES6) で導入された __分割代入 (Destructuring assignment) 構文__ を使用すると、2 つ以上の変数を簡単に入れ替えることができます。

{{< code lang="typescript" >}}
let a = 1;
let b = 2;
[a, b] = [b, a];
{{< /code >}}

- 参考: [配列の分割代入で複数の値を同時に代入する (Array destructuring) ｜ まくまくJavaScriptノート](https://maku77.github.io/js/array/destructuring.html)

