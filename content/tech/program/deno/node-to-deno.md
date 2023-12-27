---
title: "Node.js と Deno のコーディング方法の違いまとめ"
url: "p/zbaga2z/"
date: "2022-12-21"
changes:
  - 2023-12-26: std/node が削除されたことを追記
tags: ["Deno"]
weight: 2
---

Deno でのコーディングは、Node.js とほぼ同様の書き方で行えますが、いくつか押さえておきたいポイントがあるのでまとめておきます。


Web Platform APIs
----
- Web ブラウザーで使える JavaScript の標準 API、例えば [fetch API](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) や、それに付随する [Request](https://developer.mozilla.org/en-US/docs/Web/API/Request) や [Response](https://developer.mozilla.org/en-US/docs/Web/API/Response) インタフェースをデフォルトで使用することができます。
- これは、Chrome や Firefox などのブラウザ上で動作する JavaScript で使える API と同様の API を使えるということです。もちろん、Deno の実行環境はブラウザではないので、セキュリティモデルなどに微妙に違いがありますが、インタフェースが統一されているので学習コストが低いです。

Web Platform API の一覧は、[こちらを参照](https://deno.land/manual/runtime/web_platform_apis)してください。


Deno APIs / Built-in APIs
----

- JavaScript 標準ではない、組み込みの API (Deno API) は、__`Deno`__ という namespace 以下のメソッド／プロパティとして提供されています。Deno が提供している API ということが一目でわかります。下記は、コマンドライン引数を参照するときの例です。
  ```ts
  Deno.argv[0]     // Deno の場合
  process.argv[0]  // Node の場合
  ```
- Deno API 呼び出し時に発生するエラーは、[Deno.errors](https://deno.land/api?s=Deno.errors) namespace に定義されています。
- Deno API には、まだ安定版 (stable) になっていないものがあり、それらを使う場合は、`deno run` 実行時に __`--unstable`__ オプションを指定します。

Deno namespace の API の一覧は、[こちらを参照](https://deno.land/api?s=Deno)してください。


Standard Library (standard modules)
----

- Deno は `Deno` namespace 以下のグローバルな Deno API 以外にも、様々な標準ライブラリ（std モジュール）を提供しています。
- std モジュールは、使用時に明示的なインポートが必要です（一方で、Deno API は何もインポートせずに呼び出せます）。
- Deno の std モジュールは、[Golang の std ライブラリ](https://pkg.go.dev/std) を参考に実装されています。

Deno の標準モジュールの一覧は、[こちらを参照](https://deno.land/std) してください。


npm モジュール
----

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


deno.land レジストリ
----

Deno ランタイムに組み込まれていないモジュールは、deno.land レジストリで管理されています。
下記モジュールが重要です。

- [deno.land/std](https://deno.land/std)
  - Deno の標準ライブラリ (Deno standard library) です。
  - 使用例:
    ```ts
    import { copy } from "https://deno.land/std@0.168.0/fs/copy.ts";
    ```
- [deno.land/x](https://deno.land/x)
  - 3rd パーティモジュールはここで管理されています。
  - 実際には、コードがある場所にリダイレクトしているだけです。
- <s>[deno.land/std/node](https://deno.land/std/node)</s>
  - <s>従来の Node API をエミュレートするためのモジュールが、標準ライブラリとして提供されています。例えば、[std/node/global.ts](https://deno.land/std/node/global.ts) は、Node のグローバル変数 `process` を提供します。</s>
  - （2023-12 追記）__`node:...`__ 形式によるインポートがサポートされたことにより、Node.js 互換レイヤーである `std/node` ライブラリは削除されました。


mod.ts と deps.ts
----

### mod.ts

モジュールのエントリポイントとなるファイル名には、慣例として __`mod.ts`__ が使用されます。これは Rust にインスパイアされたものです。
逆に、`index.ts` のような名前は避けるべきだとされています。

- 参考: [Style Guide | Manual | Deno](https://deno.land/manual/references/contributing/style_guide)

### deps.ts

アプリケーションがインポートするモジュールを一箇所でまとめて定義しておきたい場合は、__`deps.ts`__ という名前のファイルが使われます。
このファイルで、アプリケーションが使用するメソッドを re-export しておき、他のファイルからはそれをインポートして使用します。
こうすることで、アプリケーション内で使用するモジュールのバージョンを揃えることができます。

{{< code lang="ts" title="deps.ts" >}}
export {
  add,
  multiply,
} from "https://x.nest.land/ramda@0.27.0/source/index.js";
{{< /code >}}

{{< code lang="ts" title="example.ts" >}}
import { add, multiply } from "./deps.ts";
{{< /code >}}

同様に、開発中のみ使用する依存関係は、__`dev_deps.ts`__ という名前のファイルにまとめるのが慣例です。

- 参考: [Manage Dependencies | Manual | Deno](https://deno.land/manual/examples/manage_dependencies)）

