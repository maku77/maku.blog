---
title: "Deno でテキストファイルを出力する"
url: "p/jkov7xs/"
date: "2023-12-26"
tags: ["Deno"]
---

ファイル出力用の Deno API
----

Deno でファイルへの出力を行うには、次のような File System API を使用します。

| API | 渡せるデータ | 同期 |
| ---- | ---- | ---- |
| [`Deno.writeFile`](https://deno.land/api?s=Deno.writeFile) | バイナリ (`Uint8Array`/`ReadableStream<Uint8Array>`) | |
| [`Deno.writeFileSync`](https://deno.land/api?s=Deno.writeFileSync) |  バイナリ (`Uint8Array`) | ✅ |
| [`Deno.writeTextFile`](https://deno.land/api?s=Deno.writeTextFile) | テキスト (`string`/`ReadableStream<string>`) | |
| [`Deno.writeTextFileSync`](https://deno.land/api?s=Deno.writeTextFileSync) | テキスト (`string`) | ✅ |

前者の 2 つはバイナリデータ (`Uint8Array`) の書き込み用なので、文字列データ (`string`) を出力するには、[TextEncoder#encode()](https://developer.mozilla.org/ja/docs/Web/API/TextEncoder) で UTF-8 形式のバイナリ (`Uint8Array`) に変換してから渡す必要があります。
後者の 2 つは文字列データ (`string`) をそのまま渡せるで、こちらを使うのが簡単です。


テキストファイル出力の例
----

### テキスト出力の基本

下記は、`Hello` という文字列をテキストファイルに出力する Deno プログラムです。

{{< code lang="ts" title="main.ts" >}}
await Deno.writeTextFile("hello.txt", "Hello");
{{< /code >}}

このプログラムを実行するときは、__`--allow-write`__ オプションを付けてファイル出力を許可してやる必要があります。

```console
$ deno run --allow-write main.ts
$ cat hello.txt
Hello
```

### 複数行のテキストを書き込む

複数行のテキストを出力したい場合は、改行文字 (`\n`) を含む文字列を `Deno.writeTextFile()` に渡せば OK です。
必要に応じて、文字列配列を `join("\n")` で結合して渡すとよいです。

{{< code lang="ts" title="main.ts" >}}
// 文字列配列を用意する
const sentences = [
  "風は常に旅人の友である。",
  "星は我々の運命を照らす。",
];
sentences.push("海は無限の可能性を秘めている。");
sentences.push("川は変化と進化を象徴している。");

// ファイルへ出力
await Deno.writeTextFile("sentences.txt", sentences.join("\n"));
{{< /code >}}

```console
$ deno run --allow-write main.ts
$ cat sentences.txt
風は常に旅人の友である。
星は我々の運命を照らす。
海は無限の可能性を秘めている。
川は変化と進化を象徴している。
```

### 追記フラグ (append)

`Deno.writeTextFile()` の `options` パラメーターで __`{ append: true }`__ を指定すると、既存のファイルの末尾に追記する形で出力できます。

{{< code lang="ts" title="main.ts" hl_lines="3" >}}
async function log(message: string) {
  const line = `${new Date().toISOString()}: ${message}\n`;
  await Deno.writeTextFile("log.txt", line, { append: true });
}

await log("この行がファイルの末尾に追加されます。");
{{< /code >}}

このプログラムを呼び出すたびに、`log.txt` に行が追加されていきます。

```console
$ deno run --allow-write main.ts
$ deno run --allow-write main.ts
$ deno run --allow-write main.ts

$ cat log.txt
2023-12-27T07:46:10.940Z: この行がファイルの末尾に追加されます。
2023-12-27T07:46:15.272Z: この行がファイルの末尾に追加されます。
2023-12-27T07:46:19.308Z: この行がファイルの末尾に追加されます。
```
