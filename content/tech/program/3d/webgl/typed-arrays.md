---
title: "WebGL で使う Typed Array"
url: "/p/twmzpfj"
date: "2019-09-11"
tags: ["3D", "JavaScript", "WebGL"]
---

Typed Array とは
----

JavaScript の配列型には、汎用型の `Array` の他にも、`Int32Array` のような、特定の型の値だけしか格納できない配列型が用意されています。
このような配列型のことを **Typed Array** と呼びます。
イメージとしては、C 言語の int 配列や float 配列に近いです。

例えば、WebGL で頂点属性（座標や色）の配列を用意するようなケースでは、その中の要素の型はすべて float 型に統一されていたりするので、汎用型の `Array` ではなく、`Float32Array` を使用することでメモリ効率がよくなります。

Typed Array には下記のようなものが用意されています。

| 型名 | バイト数 | 値の範囲 | 説明（対応するC言語の型） |
| ---- | ---- | ---- | ---- |
| Int8Array | 1 | -128 ～ 127 | 8-bit sined integer (int8_t) |
| Uint8Array | 1 | 0 ～ 255 | 8-bit unsigned integer (uint8_t) |
| Int16Array | 2 | -32768 ～ 32767 | 16-bit signed integer (int16_t) |
| Uint16Array | 2 | 0 ～ 65535 | 16-bit unsigned integer (uint16_t) |
| Int32Array | 4 | -2147483648 ～ 2147483647 | 32-bit signed integer (int32_t) |
| Uint32Array | 4 | 0 ～ 4294967295 | 32-bit unsinged integer (uint32_t) |
| Float32Array | 4 | 1.2x10-38 ～ 3.4x1038 | 32-bit floating point number (float) |
| Float64Array | 8 | 5.0x10-324 ～ 1.8x10308 | 64-bit floating point number (double) |


Typed Array の使い方
----

Typed Array インスタンスは、型名をコンストラクタとして呼び出すことで生成することができます。
配列リテラルの形で初期値を指定することもできるし、サイズだけ指定してすべての要素が 0 に初期化された Typed Array を作成することもできます。

{{< code lang="js" >}}
const arr2 = new Int32Array([0, 1, 2]);  // 初期値を指定
const arr1 = new Int32Array(3);  // デフォルト値の3要素 = [0, 0, 0]
{{< /code >}}

Typed Array は通常の配列と同様に使用できますが、`push()` や `pop()` のようなサイズ変更を伴う操作はできないようになっています（このあたりも C 言語の配列に近いです）。
その分、効率を重視しているということですね。

{{< code lang="js" >}}
const arr = new Int32Array(3);
arr[0] = 777;  // 既存要素の値を変えることは可能
console.log(arr);  //=> [777, 0, 0]
arr.push(888);  // Error!
{{< /code >}}

Typed Array の要素数は `length` プロパティ、要素 1 つあたりのサイズ（バイト数）は **`BYTES_PER_ELEMENT`** で取得することができます。
また、全要素が占めるバイト数 (`length * BYTES_PER_ELEMENT`) は、`byteLength` プロパティで取得することができます。

{{< code lang="js" >}}
const arr = new Float32Array([0.0, 1.0, 2.0]);
console.log(arr.length);  //=> 3
console.log(arr.BYTES_PER_ELEMENT);  //=> 4
console.log(arr.byteLength);  //=> 12
{{< /code >}}

