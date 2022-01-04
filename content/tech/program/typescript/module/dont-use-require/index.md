---
title: "TypeScriptのモジュールのインポートには import を使う"
linkTitle: "モジュールのインポートには import を使う"
url: "/p/emdtiio"
date: "2020-02-04"
lastmod: "2020-04-22"
tags: ["JavaScript", "TypeScript"]
weight: 202
---

require ではなく import を使う
----

TypeScript で型情報付きのモジュールをインポートするときは、[ECMAScript 2015 の module 構文](https://www.ecma-international.org/ecma-262/6.0/#table-40) で定義されている __`import`__ を使うようにすると、VSCode などで型情報を使った補完がうまく効くようにになります。

{{< code lang="ts" title="これを使う" >}}
// Good
import * as mod from 'mod';
{{< /code >}}

下記のように、Node.js で一般的に使用されていた CommonJS 形式の `require` を使ってもインポートできますが、`any` 型の変数を定義していることになるため、型情報を用いた補完が効きません。

{{< code lang="ts" title="これは使わない" >}}
// NG
const mod = require('mod');
{{< /code >}}

**TypeScript でモジュールをインポートするときには、できるだけ `import` を使う** ようにしましょう。


インポートの例（関数ベースのモジュール）
----

Node.js のコアモジュール（`os` や `path` など）は、もともと TypeScript 用に作成されたものではありませんが、TypeScript 用の型情報が `@types/node` モジュールとして提供されています。

{{< code title="Node.js コアモジュールの型情報をインストール" >}}
$ npm install --save-dev @types/node
{{< /code >}}

これで、Node.js のコアモジュールを下記のように型情報付きでインポートできるようになります。
これは、複数の関数を `export` する TypeScript モジュールをインポートする方法のよい例です。

{{< code lang="ts" title="よい例: 型として認識される" >}}
import * as path from 'path';  // 全ての関数をインポート
import { join } from 'path';   // 個別の関数をインポート
{{< /code >}}

これで、VSCode などの IDE で次のように補完が聞くようになります。

{{< image border="true" src="img-001.png" >}}

逆に、次のようにインポートしてしまうと、`any` 型の `path` 変数が定義されているものとみなされ、補完機能がうまく働きません。

{{< code lang="ts" title="悪い例: 型として認識されない" >}}
const path = require('path');
{{< /code >}}


インポートの例（クラスのインポート）
----

モジュールが `export` しているクラスを個々にインポートする場合も同様に `import` を使用します。

{{< code lang="ts" title="よい例: 型として認識される" >}}
import { QnAMaker } from 'botbuilder-ai';
{{< /code >}}

{{< code lang="ts" title="悪い例: 型として認識されない" >}}
const { QnAMaker } = require('botbuilder-ai');
{{< /code >}}

前者のように、`import` キーワードを使ってインポートすれば、`QnAMaker` は型情報だとみなされるため、TypeScript の型安全性をいかしたコーディングを行うことができます。

{{< code lang="ts" >}}
class Faq {
  qnaMaker: QnAMaker;  // QnAMaker 型のプロパティを定義
  // ...
}
{{< /code >}}

