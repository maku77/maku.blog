---
title: "TypeScriptの型: 配列を定義する (Array)"
linkTitle: "配列を定義する (Array)"
url: "/p/ttwoetd"
date: "2020-01-22"
tags: ["TypeScript"]
weight: 10
---

TypeScript で配列を定義するには、次のいずれかの方法を使います。

{{< code lang="typescript" title="文字列配列を作成する" >}}
const strArr: string[] = [];
const strArr: Array<string> = [];
{{< /code >}}

ここでは初期値として空の配列 `[]` を代入しています。

`const` で変数を作成していますが、JavaScript の `const` は変数自体への代入を制限するだけなので、配列の内容を変更することは可能です。

{{< code lang="typescript" >}}
const nums: number[] = [];
nums.push(100);
nums.push(200);
nums.push(300);

for (const x of nums) {
    console.log(x);
}
{{< /code >}}

{{< code title="実行結果" >}}
100
200
300
{{< /code >}}

{{< reference >}}
- [TypeScript: タプルを定義する (Tuple types)](/p/6uzqf4q)
{{< /reference >}}

