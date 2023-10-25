---
title: "SvelteKit の load 関数でデータのロード処理を定義する"
url: "p/wqt76qw/"
date: "2023-10-24"
tags: ["Svelte"]
draft: true
---

load 関数
----

SvelteKit には、__`+pages.ts`__ や __`+pages.server.ts`__ といった、特殊なルートファイルにデータのロード処理を定義する仕組みが用意されています。
これをうまく利用すると、ページコンポーネント (`+page.svelte`) の UI 実装から、データ処理をきれいに分離することができます。

データのロード処理は、決められた名前のルートファイルに、__`load`__ という名前の関数で定義します。
`load` 関数の戻り値は、ページコンポーネント内から __`data`__ という変数で参照することができます。

{{< image src="img-001.drawio.svg" title="data 変数と load 関数の関係" >}}

`load` 関数は `src/routes` ディレクトリ以下に配置する次のようなファイルに定義することができ、それぞれ呼び出されるタイミング（サーバーサイド or クライアントサイド）や、適用範囲が異なっています。

| ファイル名 | クライアントサイド | サーバーサイド | 注釈 |
| ---- | :--: | :--: | ---- |
| `+page.js(.ts)` | ✅ | ✅※ | ※ `export const ssr = false` と記述した場合はクライアントサイドでのみ呼び出されます。 |
| `+page.server.js(.ts)` | ─ | ✅ | サーバーからしかアクセスできない外部 API にアクセスする場合はこれを使います。 |
| `+layout.js(.ts)` | ✅ | ✅※ | `export const ssr = false` と記述した場合はクライアントサイドでのみ呼び出されます。 |
| `+layout.server.js(.ts)` | ─ | ✅ | サーバーからしかアクセスできない外部 API にアクセスする場合はこれを使います。 |

基本的には、`+page.js` と `+page.server.js` が単一のページコンポーネント (`+page.svelte`) 用で、`+layout.js` と `+layout.server.js` がレイアウトファイル (`+layout.svelte`) および、そのレイアウトが適用されるページコンポーネント用のファイルだと考えれば OK です。


具体例
----

ここでは、TypeScript を使って実装する場合の例を紹介します。

### 固定のデータ

まずは `load()` 関数で固定のデータを返す簡単な例です。
Visual Studio Code などを使っていれば、ページコンポーネント内の `data` 変数の型を推測して補完入力を行ってくれます。

{{< code lang="ts" title="src/routes/+page.ts" >}}
import type { PageLoad } from './$types';

// load 関数の戻り値の型（必ずしも明示的に定義する必要はない）
type Data = {
	message: string;
};

// この関数の戻り値をページコンポーネント (+page.svelte) の data 変数として参照できる
export const load: PageLoad<Data> = () => {
	return { message: 'Hello' };
};
{{< /code >}}

{{< code lang="html" title="src/routes/+page.svelte" >}}
<script lang="ts">
	export let data; // 型は勝手に推測される
</script>

<p>{data.message}</p>
{{< /code >}}

その他
----

- 1 つのページコンポーネント (`+pages.svelte`) に対して 4 種類のファイルの `load()` 関数が呼び出される可能性があります。
  その場合、`data` 変数の内容は、各 `load()` 関数の戻り値のオブジェクトがマージされたものになります。

