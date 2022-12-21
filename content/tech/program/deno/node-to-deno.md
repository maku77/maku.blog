---
title: "Node.js と Deno のコーディング方法の違いまとめ"
url: "p/zbaga2z/"
date: "2022-12-21"
tags: ["Deno"]
---

Deno でのコーディングは、Node.js とほぼ同様の書き方で行えますが、いくつか押さえておきたいポイントがあるのでまとめておきます。

- Web ブラウザーで使える JavaScript の標準 API、例えば [fetch API](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) や、それに付随する [Request](https://developer.mozilla.org/en-US/docs/Web/API/Request) や [Response](https://developer.mozilla.org/en-US/docs/Web/API/Response) インタフェースをデフォルトで使用することができます。
- JavaScript 標準ではない API は、__`Deno.xxx`__ という形の静的メソッド／プロパティで提供されています。Deno が提供している API ということが分かりやすいです。下記は、コマンドライン引数を参照するときの例です。
  - Deno の場合: `Deno.argv[0]`
  - Node の場合: `process.argv[0]`
- npm モジュールは、__npm specifier__ というフォーマット (`npm:xxx`) でインポート可能です。さらに、型定義が DefinitelyTyped で提供されている場合は、コメントで __`@deno-types`__ ディレクティブを使って指定できます。
  ```ts
  // @deno-types="npm:@types/express@^4.17";
  import express from "npm:express@^4.17";
  const app = express();
  ```
- npm モジュールのインポート方法として、[esm.sh](https://esm.sh/) や [Skypack CDN](https://docs.skypack.dev/) といった CDN を利用する方法もあります。
  ```ts
  import React from "https://esm.sh/react@~16.13.0";
  import React from "https://cdn.skypack.dev/react@~16.13.0";
  ```
- Deno 用のモジュールは deno.land レジストリで管理されています。
  - __`deno.land/std/`__ ... Deno の標準ライブラリが提供されています。
    ```ts
    import { copy } from "https://deno.land/std@0.168.0/fs/copy.ts";
    ```
  - __`deno.land/std/node`__ ... 従来の Node API をエミュレートするためのレイヤーです。例えば、[std/node/global.ts](https://deno.land/std/node/global.ts) は、Node のグローバル変数 `process` を提供します。
  - __`deno.land/x/`__ ... 3rd パーティモジュールが提供されています。実際には、コードがある場所にリダイレクトしているだけです。

