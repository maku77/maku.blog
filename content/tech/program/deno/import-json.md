---
title: "Deno で JSON ファイルを読み込む方法いろいろ"
url: "p/a83hr2b/"
date: "2022-12-28"
tags: ["Deno", "JSON"]
---

Deno で JSON ファイルを読み込んで、JavaScript のオブジェクトとして参照できるようにするには、`import` で直接インポートする方法と、`Deno.readTextFile` で読み込んでから `JSON.parse` する方法があります。
ここでは、それぞれの方法で次のような JSON ファイルを読み込んでみます。

{{< code lang="json" title="books.json" >}}
[
  {"title": "Title 1", "author": "Author 1"},
  {"title": "Title 2", "author": "Author 2"},
  {"title": "Title 3"}
]
{{< /code >}}


JSON モジュールとしてインポートする方法
----

Deno では、__`import`__ 文で JSON ファイルを直接インポートできるようになっています。
JSON ファイルをインポートするときは、[import assertions](https://github.com/tc39/proposal-import-assertions) の機能を使って、JSON ファイルであることを明示する必要があります。

### import 文で静的インポート

{{< code lang="ts" title="main.ts" >}}
import books from "./books.json" assert { type: "json" };

for (const b of books) {
  console.log(`title: ${b.title}, author: ${b.author ?? "Unknown"}`);
}
{{< /code >}}

{{< code lang="console" title="実行例" >}}
$ deno run main.ts
title: Title 1, author: Author 1
title: Title 2, author: Author 2
title: Title 3, author: Unknown
{{< /code >}}

この方法では JSON ファイルが静的に読み込まれるため、次のような恩恵があります。

- JSON データから TypeScript の型情報を推測してくれるので、VS Code などで入力補完が効きます。
- `deno run` でのプログラム実行時に、`--allow-read` を付加する必要がありません。

### import 関数で動的インポート

`import` 文の代わりに `import` 関数を使うと、動的に JSON ファイルを読み込むことができます。

{{< code lang="ts" title="main.ts" >}}
async function loadBooks() {
  const { default: books } = await import("./books.json", {
    assert: { type: "json" },
  });
  return books;
}

const books = await loadBooks();
{{< /code >}}

ただし、動的に読み込む場合は、プログラムの実行時に `--allow-read` フラグが必要です。

```console
$ deno run --allow-read main.ts
```

### URL で JSON ファイルを指定する

Web 上で公開されている JSON ファイルを URL で指定してインポートすることもできます。
次の例では、気象庁が公開している東京 (=130000) の天気予報を取得しています。

{{< code lang="ts" title="main.ts" >}}
import forecast from "https://www.jma.go.jp/bosai/forecast/data/overview_forecast/130000.json" assert { type: "json" };

console.log(forecast.targetArea);
console.log(forecast.reportDatetime);
console.log(forecast.text.split("\n", 1)[0]);
{{< /code >}}

{{< code lang="console" title="実行例" >}}
$ deno run main.ts
東京都
2022-12-28T16:39:00+09:00
　日本付近は、日本の南に中心を持つ高気圧に覆われています。一方、三陸沖には低気圧があって東北東へ進んでいます。
{{< /code >}}

初期状態では TypeScript の型情報が認識されませんが、一度プログラムを実行するか、次のように __`deno cache`__ コマンドで明示的に JSON ファイルをキャッシュすることで補完が効くようになります。

```console
$ deno cache https://www.jma.go.jp/bosai/forecast/data/overview_forecast/130000.json
```


Deno.readTextFile で読み込む方法
----

Deno API の __`Deno.readTextFile`__ で JSON ファイルを通常のテキストファイルとして読み込んでから、__`JSON.parse`__ でパースする方法です。
`JSON.parse` の戻り値は `any` 型なので、TypeScript の型情報は自分で定義する必要があります。

{{< code lang="ts" title="main.ts" >}}
// JSON テキストを JavaScript オブジェクトに変換するときの型情報
type Book = {
  title: string;
  author?: string;
};

const jsonText = await Deno.readTextFile("./books.json");
const books = JSON.parse(jsonText) as Book[];

for (const b of books) {
  console.log(`title: ${b.title}, author: ${b.author ?? "Unknown"}`);
}
{{< /code >}}

動的に JSON ファイルを読み込むので、プログラムの実行時に `--allow-read` フラグの指定が必要です。

{{< code lang="console" title="実行例" >}}
$ deno run --allow-read main.ts
title: Title 1, author: Author 1
title: Title 2, author: Author 2
title: Title 3, author: Unknown
{{< /code >}}

