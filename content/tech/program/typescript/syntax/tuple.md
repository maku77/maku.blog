---
title: "TypeScriptの型: タプルを定義する (Tuple types)"
linkTitle: "タプルを定義する (Tuple types)"
url: "/p/6uzqf4q"
tags: ["TypeScript"]
weight: 10
---

タプルの基本
----

TypeScript のタプル型は、複数の値を保持することのできる型です。
`[]` 記号を使うところも配列によく似ていますが、それぞれの位置の要素の型を明示しておくことができます。

{{< code lang="ts" title="文字列と数値のタプルを定義する例" >}}
let vote: [string, number]
vote = ['red', 100];     // OK
vote = ['green', 200];   // OK
vote = ['yellow', 300];  // OK
vote = ['AAA', 'BBB'];   // Error
{{< /code >}}

上記の例では、2 つの値を保持するタプル型変数を定義していますが、3 つ以上の値を保持するタプルを定義することもできます。
配列と同様、タプルの各要素にはインデックスでアクセスできます。

{{< code lang="ts" >}}
let data: [string, number, number]
data = ['maku', 100, 5];

// インデックスで各要素を参照
console.log(data[0]);  //=> maku
console.log(data[1]);  //=> 100
console.log(data[2]);  //=> 5

// ループで各要素を取り出す
for (const x of data) {
  console.log(x);
}
{{< /code >}}


タプルによる多値関数
----

タプルを関数の戻り値として使用すると、複数の値を返す「多値関数」として扱うことができます。

{{< code lang="ts" >}}
function getResult(): [number, string] {
  return [404, 'Not Found'];
}

const result = getResult();
console.log(result);  //=> [404, 'Not Found']
{{< /code >}}

次のようにすると、返されたタプルを複数の変数に分解して受け取ることができます。

{{< code lang="ts" >}}
const [code, message] = getResult();
console.log(code);  //=> 404
console.log(message);  //=> Not Found
{{< /code >}}

ただ、タプル型は各位置の値が何を示しているかが不明瞭になってしまうので、できればインタフェースを使って、明確にプロパティを定義した方がよいでしょう。

{{< code lang="ts" >}}
interface HttpStatus {
  code: number;
  message: string;
}

function getResult(): HttpStatus {
  return { code: 404, message: 'Not Found' };
}

const result: HttpStatus = getResult();
console.log(result.code);  //=> 404
console.log(result.message);  //=> Not Found
{{< /code >}}

{{< reference >}}
- [TypeScript: 配列を定義する (Array)](/p/ttwoetd)
{{< /reference >}}

