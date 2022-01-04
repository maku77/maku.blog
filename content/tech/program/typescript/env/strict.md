---
title: "TypeScriptの環境/設定: 厳格な型チェックを有効にする (strict)"
linkTitle: "厳格な型チェックを有効にする (strict)"
url: "/p/7b9432m"
date: "2020-05-08"
tags: ["TypeScript"]
---

厳格モードに関するオプション
----

TypeScript の設定ファイル (`tsconfig.json`) には、厳格な型チェックを有効にするための __`strict`__ オプションが用意されています。

| プロパティ名 | デフォルト値 | 説明 |
| :--: | :--: | ---- |
| [strict](https://www.typescriptlang.org/v2/ja/tsconfig#strict) | `false` | Enable all strict type checking options. <br>コンパイル時の様々な厳格な型チェック機能を有効にします。 |

{{< code lang="js" title="tsconfig.json の記述例" >}}
{
  "include": [
    "src/**/*"
  ],
  "compilerOptions": {
    "target": "ES2015",
    "module": "commonjs",
    "allowJs": true,
    "outDir": "./build",
    "strict": true   /* Enable all strict type-checking options. */
  }
}
{{< /code >}}

上記の例では、`tsconfig.json` で `strict` オプションを有効にしていますが、`tsc` コマンドのオプションで __`--strict`__ と直接指定する方法もあります。

{{< code >}}
$ tsc --strict
{{< /code >}}

実は、`strict` オプションは、下記のようなオプション群をまとめて `true` にするためのオプションです。

| プロパティ名 | デフォルト値 | 説明 |
| ---- | :--: | ---- |
| noImplicitAny | `false` | Raise error on expressions and declarations with an implied `any` type. |
| noImplicitThis | `false` | Raise error on `this` expressions with an implied `any` type. |
| alwaysStrict | `false` | Parse in strict mode and emit "use strict" for each source file. |
| strictBindCallApply | `false` | Enable strict `bind`, `call`, and `apply` methods on functions. |
| strictNullChecks | `false` | Enable strict null checks. |
| strictFunctionTypes | `false` | Enable strict checking of function types. |
| strictPropertyInitialization | `false` | Enable strict checking of property initialization in classes. |

TypeScript 3.8 時点では、`strict` オプションを有効にすることで、これらのオプション群がまとめて有効化されますが、将来の TypeScript (tsc) バージョンでは、これらのオプション群は、そのとき推奨される組み合わせに変更されるとされています。
つまり、`strict` オプションを有効にしておくと、TypeScript のバージョンを上げたときに、新しいエラーが表示されるようになる可能性があります。

いずれにしても、 __`strict` オプションは常に有効にして `tsc` ビルドする__ ことで、TypeScript コードの型安全性を高めておくことをお勧めします。


少しずつ strict 対応していく方法
----

`strict` オプションはデフォルトでは `false` になっているため、 __プロジェクトが巨大化してから `strict` オプションを有効化すると、大量のエラーが表示される可能性__ があります。
大量のエラーを一気に直さないといけないので大変です。

そのような場合は、下記のように、個別の strict 系オプションを 1 つずつ有効にして対応していきましょう。

{{< code lang="js" title="tsconfig.json（抜粋）" >}}
{
  "compilerOptions": {
    /* 簡単に対応できそうなところから有効化していく */
    "noImplicitAny": false,
    "noImplicitThis": true,
    "alwaysStrict": true,
    "strictBindCallApply": true,
    "strictNullChecks": false,
    "strictFunctionTypes": false,
    "strictPropertyInitialization": false
  }
}
{{< /code >}}

JavaScript から乗り換えたコードの場合、下記のような `noImplicitAny` によるエラーがたくさん出ると思います。
これは、関数のパラメータの型指定が足りないというエラーですが、これを一気に対応するのは大変なので後回しにするのがよいかもしれません。

{{< code title="noImplicitAny によるエラーの例" >}}
src/main.ts:27:56 - error TS7006: Parameter 'res' implicitly has an 'any' type.
{{< /code >}}

次のように、`strict` オプションで全ての型チェックをまとめて有効化しておいて、個々の警告を無効にするという指定もできます。

{{< code lang="js" title="tsconfig.json（抜粋）" >}}
{
  "compilerOptions": {
    /* ... */
    "strict": true,  /* 全ての型チェックを有効化 */
    "noImplicitAny": false  /* 個別に無効化 */
  }
}
{{< /code >}}

個別の strict 系オプションをすべて `true` にして状態で警告が出なくなったら、下記のように書き換えて strict 対応完了です。

{{< code lang="js" title="tsconfig.json（抜粋）" >}}
{
  "compilerOptions": {
    /* ... */
    "strict": true
  }
}
{{< /code >}}


こっそり少しずつ strict 対応する方法
----

`strict` オプションによる型チェックでエラーが発生すると、デフォルトでは JavaScript (.js) ファイルの出力が停止されるので、設定ファイル (`tsconfig.json`) で `strict` オプションを有効化するのは厳しいということがあるかもしれません。
そのような場合は、次のように `tsc` コマンドの __`--strict`__ オプションで、一時的に厳格な型チェックを有効化することができます。

{{< code >}}
$ tsc --strict
{{< /code >}}

これでエラーを確認しながら少しずつ TypeScript コードを修正していき、エラーがなくなった時点で `tsconfig.json` の `strict` オプションを有効化するというのがよいでしょう。

ちなみに、型チェックなどでエラーが発生しても JavaScript (.js) ファイルの出力を停止しないというオプション (`--noEmitOnError false`) がありますが、ちょっと危険な香りがするのであまり使わない方がよいと思います。

