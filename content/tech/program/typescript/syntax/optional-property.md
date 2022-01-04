---
title: "TypeScriptの型: インタフェースのプロパティをオプショナルにする"
linkTitle: "インタフェースのプロパティをオプショナルにする"
url: "/p/rpfr5fr"
date: "2020-04-28"
tags: ["TypeScript"]
weight: 51
---

オプショナルプロパティの定義
----

TypeScript でインタフェースを定義するとき、プロパティ名の末尾に __`?`__ を付けると、そのプロパティをオプショナルプロパティとすることができます（関数のオプショナルパラメータと同様です）。

例えば、次の `User` インタフェースの `age` プロパティはオプショナルになっているため、`User` オブジェクトを生成するときに `age` プロパティの値を省略することができます。

{{< code lang="ts" >}}
interface User {
  name: string;
  age?: number;  // age プロパティはオプショナル
}

const u1: User = { name: 'Maku', age: 5 };  // OK
const u2: User = { name: 'Maku' };  // OK
console.log(u1.age);  //=> 5
console.log(u2.age);  //=> undefined
{{< /code >}}

インタフェースを定義せずに、変数の定義時に型情報を指定する場合も同様です。

{{< code lang="ts" >}}
let user: { name: string, age?: number };
user = { name: 'Maku', age: 5 };  // OK
user = { name: 'Maku' };  // OK
user = { age: 5 };  // Error
{{< /code >}}

オプショナルなプロパティを定義する代わりに、特定の値（例えば `-1`）を、値がないことを示すために使用するという方法もありますが、プロパティ自体格納せずに `undefined` としておく方が分かりやすいでしょう。


オプショナルプロパティが指定されたか調べる
----

オブジェクトが、あるプロパティを保持しているかどうかを調べるには、次のように __`in`__ や __`typeof`__ を使って確認します。

{{< code lang="ts" >}}
if ('age' in user) {
  // age プロパティが存在する
}

if (typeof user.age === 'undefined') {
  // age プロパティが存在しない
}
{{< /code >}}


使用例
----

次の `getUser` 関数は、`name` プロパティと `age` プロパティを持つ `User` オブジェクトを返す関数ですが、場合によっては `age` プロパティを持たないオブジェクトを返すことを示しています。

{{< code lang="ts" >}}
interface User {
  name: string;
  age?: number;  // age プロパティはオプショナル
}

function getUser(withAge: boolean): User {
  return withAge ? {name: 'Maku', age: 5} : {name: 'Maku'};
}

// main
const user = getUser(true);
console.log(`name = ${user.name}`);
if ('age' in user) {
  console.log(`age = ${user.age}`)
}
{{< /code >}}

