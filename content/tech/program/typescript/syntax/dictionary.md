---
title: "TypeScriptの型: 辞書型を定義する (Dictionary)"
linkTitle: "辞書型を定義する (Dictionary)"
url: "/p/x3ocp9a"
date: "2020-02-04"
tags: ["TypeScript"]
weight: 10
---

辞書オブジェクトを作成する
----

JavaScript の配列は、もともと「キー＆値」を扱う連想配列として使用することができますが、TypeScript のタイプアノテーションを使うと、**キーの型と値の型** を明示した辞書 (Dictionary) オブジェクトを作成することができます。

次の例では、文字列のキーと、数値の値を持つ辞書オブジェクトを作成しています。

{{< code lang="ts" >}}
// 辞書オブジェクトを作成する
const userAges: { [name: string]: number } = {};

// 辞書オブジェクトを使用する
userAges['maku'] = 14;
userAges['hemu'] = 6;
console.log(userAges['maku']);  //=> 14
console.log(userAges['hemu']);  //=> 6
{{< /code >}}

間違った型のキーや値を格納しようとするとエラーになります。

{{< code lang="ts" >}}
userAges[14] = 'maku';  // Error!!
{{< /code >}}


辞書型のインタフェースを定義する
----

### 単純な辞書

上記の例では、辞書オブジェクトを生成するときに同時にその型を指定していましたが、あらかじめ辞書型を表すインタフェースだけを定義しておくこともできます。
次の `UserAges` インタフェースは、文字列型のキーと数値型の値を持つ、よくある辞書の定義例です。

{{< code lang="typescript" >}}
// 辞書オブジェクトを作成する
interface UserAges {
  [name: string]: number;
}

// 使用例
const userAges: UserAges = {};
userAges['maku'] = 14;
console.log(userAges['maku']);  //=> 14
{{< /code >}}

### 任意のオブジェクトを値にする辞書

次の `UserDictionary` インタフェースは、文字列型のキーと `User` 型の値を持つ辞書型を表しています。

{{< code lang="ts" title="UserDictionary インタフェースの定義" >}}
interface User {
  name: string;
  age: number;
}

interface UserDictionary {
  [id: string]: User;
}
{{< /code >}}

`UserDictionary` インタフェースは次のように使用します。

{{< code lang="ts" title="UserDictionary インタフェースの使用例" >}}
// 辞書オブジェクトの作成
const dic: UserDictionary = {};
dic['maku'] = { name: 'Makkuma', age: 14 };
dic['hemu'] = { name: 'Hemumu', age: 6 };

// 辞書の参照
const maku = dic['maku'];
console.log(maku.name);  //=> Makkuma
console.log(maku.age);  //=> 14
{{< /code >}}


辞書からキーを削除する
----

辞書のキー（とその値）を削除するには、`delete` キーワードを使用します。

{{< code lang="ts" >}}
delete dic['maku'];  // 辞書から maku のエントリを削除
console.log(Object.keys(dic).length);  //=> 1（キーのサイズが減ってる）
{{< /code >}}


辞書をループ処理する
----

辞書内の要素をループ処理するには、例えば次のようにキーをループ処理します。

{{< code lang="ts" >}}
Object.keys(dic).forEach(key => {
  const value = dic[key];
  console.log(value.name);
  console.log(value.age);
});
{{< /code >}}

ES 2017 以降であれば、**`Object.entries()`** 関数を使って、キーと値のペアを取り出しながらループ処理できます。

{{< code lang="ts" title="ES 2017 以降の辞書ループ" >}}
Object.entries(dic).forEach(([key, value]) => {
  console.log(key);
  console.log(value);
});
{{< /code >}}

ちなみに、`tsc` コマンドなどで TypeScript コードをトランスパイルしている場合、ES 2017 の機能を使うには、`compilerOptions.target` の項目で `ES2017` 以上を指定しておく必要があります。

{{< code lang="json" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "outDir": "./build",
    "allowJs": true,
    "target": "ES2017",
    "module": "commonjs",
    "strict": true
  },
  "include": [
    "./src/**/*"
  ]
}
{{< /code >}}


なぜ辞書オブジェクトの型定義は分かりにくいのか？
----

{{< code lang="ts" >}}
{ [name: string]: number }
{{< /code >}}

という辞書オブジェクトの型定義は、「次のように簡潔に記述できないの？」と思うかもしれません。

{{< code lang="ts" >}}
{ string: number }
{{< /code >}}

しかし、後者の定義は辞書型を示すものではなく、`string` という名前のプロパティを持つオブジェクト型を定義するものになってしまいます。
使い方も、次のようにまったく異なります。

{{< code lang="ts" >}}
// これは複数の「key＆value」を格納できる辞書オブジェクト
const dic: { [name: string]: number } = {};
dic['Maku'] = 5;
dic['Hemu'] = 10;
dic['Puni'] = 15;

// これは単一のオブジェクトを格納する変数
let data: { string: number };
data = { string: 100 };
{{< /code >}}

TypeScript の辞書オブジェクトの型定義は、最初は面食らうかもしれませんが、 __型定義の中に `[]` が出てきたら、それは辞書オブジェクトのキーの型__ なのだということが分かっていれば、コードもスラスラと読めるようになります。

