---
title: "TypeScriptの型: 環境変数 (process.env) 用の型情報を定義する"
linkTitle: "環境変数 (process.env) 用の型情報を定義する"
url: "/p/r8iry9g"
date: "2020-10-12"
tags: ["JavaScript", "TypeScript"]
weight: 301
---

Node.js で環境変数を参照するためのオブジェクト `process.env` の型情報は、`@types/node` で次のように文字列型ディクショナリ `Dict<string>` として定義されています。

{{< code lang="ts" title="node_modules/@types/node/process.d.ts（抜粋）" >}}
declare module "process" {
  global {
    var process: NodeJS.Process;
    namespace NodeJS {
      interface ProcessEnv extends Dict<string> {}
      //...
    }
  }
  export = process;
}
{{< /code >}}

この型定義は `@types/node` モジュールをインストールするだけで簡単に使用できるようになるのですが、具体的なキー名（環境変数名）が定義されていないので、Visual Studio Code などで `process.env` 以降の入力補完が効きません。

__VS Code 上で環境変数名を補完入力__ できるようにするには、次のような型定義ファイル (`globals.d.ts`) をソースツリーのルートに作成します。

{{< code lang="ts" title="globals.d.ts" >}}
declare namespace NodeJS {
  // 環境変数名の定義
  interface ProcessEnv {
    /** 現在の Node.js 実行環境 */
    readonly NODE_ENV: 'development' | 'production' | 'test';

    /** GitHub アクセストークン */
    readonly MYAPP_GITHUB_TOKEN: string;
  }
}
{{< /code >}}

環境変数 `NODE_ENV` の値は、決められた 3 種類の値 (development/production/test) のいずれかであると定義しているので、それ以外の値と比較しようとしたときに警告してくれます。

{{< code lang="ts" >}}
if (process.env.NODE_ENV === 'hoge') {

// 警告メッセージ ts(2367)
// This condition will always return 'false' since the types
// '"development" | "production" | "test"' and '"hoge"' have no overlap.
{{< /code >}}

また、上記の型定義のように TSDoc/JSDoc ドキュメンテーションコメントを記述しておけば、VS Code で入力補完するときにドキュメントを表示してくれます。

ちなみに、このような型情報の追加が可能なのは、TypeScript の `interface` 定義が後付けで拡張できる (open-ended) という性質を持っているからです。

