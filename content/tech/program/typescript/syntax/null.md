---
title: "TypeScript で undefined/null をうまく扱う (nullish coalescing (??), optional chaining (?.))"
url: "/p/5oyaju5"
date: "2021-06-17"
tags: ["TypeScript"]
---

Null 合体演算子 (??)
----

ES2020 で [Null 合体演算子 (Nullish Coalescing Operator)](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing_operator) の __`??`__ が導入されました。

{{< note title="nullish とは？" >}}
JavaScript において nullish とは、「`undefined` あるいは `null`」を示します。
coalescing には、「癒合、合体」という意味があります。
よって、nullish coalescing は、「null っぽかったら合体させるよ」という意味になります。
{{< /note >}}

`??` 演算子を使うと、ある変数の値が `undefined`（あるいは `null`）だったときの代替となる値を指定することができます。
つまり、

{{< code >}}
A ?? B
{{< /code >}}

は、次のように記述するのと同等です。

{{< code >}}
A != undefined ? A : B
{{< /code >}}

下記は、`??` 演算子の振る舞いの一覧です。
`??` 演算子はあくまで `undefined` と `null` だけを判別するものであって、左側に偽となる値（`false` や空文字）をおいた場合は、その値がそのまま使われることに注意してください。

{{< code lang="ts" >}}
// 右側の値が（代わりに）使われるパターン
const a = undefined ?? 'default'  //=> 'default'
const b = null ?? 'default'       //=> 'default'

// 左側の値が（そのまま）使われるパターン
const c = '' ?? 'default'     //=> ''
const d = 'aaa' ?? 'default'  //=> 'aaa'
const e = 1 ?? 100            //=> 1
const f = 0 ?? 200            //=> 0
const g = true ?? 'default'   //=> true
const h = false ?? 'default'  //=> false
const i = [] ?? [1, 2, 3]     //=> []
{{< /code >}}

Nullish Coaescing Operator は、省略可能なプロパティを持つオブジェクトを参照するときに便利です。
次の `dumpPoint` 関数は、渡された `Enemy` オブジェクトの `point` プロパティの値をしますが、この値が未設定の場合はデフォルト値の 100 を表示します。

{{< code lang="ts" >}}
type Enemy = {
  name: string
  point?: number  // このプロパティ値は省略可能
}

function dumpPoint(enemy: Enemy) {
  console.log(enemy.point ?? 100)
}

dumpPoint({ name: 'Enemy1', point: 0 })   //=> 0
dumpPoint({ name: 'Enemy2', point: 50 })  //=> 50
dumpPoint({ name: 'Enemy3' })             //=> 100
{{< /code >}}

従来はこのような処理のために短絡演算子 (`||`) がよく使われていましたが、下記のように偽値となる値（0 や `false`）がうまく扱えないという問題がありました。
今後は、`??` を使えば OK です。

{{< code lang="ts" title="よくない例" >}}
console.log(enemy.point || 100)  // point=0 の時に 100 になってしまう
{{< /code >}}

デフォルト値に関しては、次のように分割代入時に設定する方法も健在です。
用途によって使い分けましょう。

{{< code lang="ts" >}}
function dumpPoint(enemy: Enemy) {
  const { name, point = 100 } = enemy
  console.log(name)
  console.log(point)
}
{{< /code >}}


オプショナルチェイニング演算子 (?.)
----

ES2020 では、[オプショナルチェイニング演算子 (Optional Chaining Operator)](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Operators/Optional_chaining) の __`?.`__ も導入されています。
これは、`undefined`（あるいは `null`）であるかもしれないオブジェクトのプロパティを参照するときに使用します。

{{< code >}}
A?.foo()
{{< /code >}}

とすると、オブジェクト `A` が `undefined`（あるいは `null`）の場合に `undefined` になり、それ以外の場合に `A` のプロパティ `foo` が参照されます。
つまり、上記は下記と同様です。

{{< code lang="ts" >}}
A != undefined ? A.foo() : undefined
{{< /code >}}

下記は Optional Chaining の具体的な使用例です。
`User` オブジェクトは入れ子の形で `Address` オブジェクトを保持することができますが、そのための `address` プロパティは省略可能なものとして定義されています。
このように、入れ子になったオブジェクトのプロパティを参照するケースで、Optional Chaining は威力を発揮します。

{{< code lang="ts" >}}
type Address = {
  country: string
  city: string
}

type User = {
  name: string
  address?: Address
}

function dumpCity(user: User) {
  // ここでオプショナルチェイニングを使用 (｀•ω•´)
  const city = user.address?.city ?? 'UNKNOWN_CITY'
  console.log(city)
}

dumpCity({ name: 'User1', address: { country: 'Japan', city: 'Tokyo' } })
dumpCity({ name: 'User2' })
{{< /code >}}

{{< code title="実行結果" >}}
Tokyo
UNKNOWN_CITY
{{< /code >}}

ポイントは次の部分で、Optional Chaining (`?.`) と、前述の Nullish Coalescing (`??`) を組み合わせて使っています。

{{< code lang="ts" >}}
const city = user.address?.city ?? 'UNKNOWN_CITY'
{{< /code >}}

`user.address` に `Address` オブジェクトが設定されている場合は `city` プロパティを参照し、`user.address` が設定されていない場合（`undefined` のとき）は、代わりに `UNKNOWN_CITY` という値を使うようにしています。
単純に `user.address.city` と繋げて書くと、`user.address` が `undefined` のときに不正な参照になってしまいます。

このイディオムは、オブジェクトの入れ子構造がもっと深い場合でもうまく機能します。

{{< code lang="ts" >}}
// いずれかのプロパティが undefined になった時点で NO_NAME になる
const name = obj.aaa?.bbb?.ccc?.name ?? 'NO_NAME'
{{< /code >}}

また、ある関数型プロパティが設定されている場合だけ呼び出したいときは次のように記述できます。

{{< code lang="ts" >}}
// fix プロパティに関数がセットされている場合のみ呼び出す
sentence.fix?.()
{{< /code >}}

