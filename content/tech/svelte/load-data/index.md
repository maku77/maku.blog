---
title: "SvelteKit の load 関数でデータのロード処理を定義する"
url: "p/wqt76qw/"
date: "2023-10-31"
tags: ["Svelte"]
---

load 関数の基本
----

SvelteKit には、__`+pages.ts`__ や __`+pages.server.ts`__ といった、特殊なルートファイルにデータのロード処理を定義する仕組みが用意されています。
これをうまく利用すると、ページコンポーネント (`+page.svelte`) の UI 実装からデータ取得処理をきれいに分離することができ、データの取得タイミングを細かく制御することができます。

データのロード処理は、決められた名前のルートファイルに、__`load`__ という名前の関数として定義する必要があります（`Promise` を返す `async` 関数として定義することも可能です）。
`load` 関数の戻り値は、ページコンポーネント内から __`data`__ という変数で参照することができます。
この対応付けは SvelteKit のルールであり、慣れるしかありません（逆にこの作法を知らないとコードを読めません）。

{{< image src="img-001.drawio.svg" title="data 変数と load 関数の関係" >}}

`load` 関数は、ユーザー操作によってインタラクティブに情報を取得するためのものというよりは、あくまでページの初期表示内容を取得するためのものだと考えるとよいです。

`load` 関数は、`src/routes` ディレクトリ以下に配置する次のようなファイルに定義することができ、それぞれ呼び出されるタイミング（サーバーサイド __(SSR)__ or クライアントサイド __(CSR)__）や、適用範囲が異なっています。

| ファイル名 | CSR | SSR | 注釈 | 適用範囲 |
| ---- | :--: | :--: | ---- | ---- |
| `+page.js(.ts)` | ✅ | ✅※ | ※ `export const ssr = false` と記述した場合はクライアントサイドでのみ呼び出されます。 | 同一ディレクトリの `+page.svelte` |
| `+page.server.js(.ts)` | ─ | ✅ | サーバーからしかアクセスできない外部 API にアクセスする場合はこれを使います。 | 同一ディレクトリの `+page.svelte` |
| `+layout.js(.ts)` | ✅ | ✅※ | `export const ssr = false` と記述した場合はクライアントサイドでのみ呼び出されます。 | `+layout.svelte` とそれが適用されるファイル |
| `+layout.server.js(.ts)` | ─ | ✅ | サーバーからしかアクセスできない外部 API にアクセスする場合はこれを使います。 | `+layout.svelte` とそれが適用されるファイル |

基本的には、下記の方針でファイルを使い分ければよいです。

- 単一のページコンポーネント (`+page.svelte`) で使う `data` を提供する `load` 関数を実装したい
  - サーバーサイドだけで実行したい → __`+page.server.js(.ts)`__
  - クライアントサイドだけで実行したい → __`+page.js(.ts)`__ + __`export const ssr = false`__
  - 両サイドで実行しても OK なら → __`+page.js(.ts)`__
- レイアウト (`+layout.svelte`) とレイアウトの適用範囲で使う `data` を提供する `load` 関数を実装したい
  - サーバーサイドだけで実行したい → __`+layout.server.js(.ts)`__
  - クライアントサイドだけで実行したい → __`+layout.js(.ts)`__ + __`export const ssr = false`__
  - 両サイドで実行しても OK なら → __`+layout.js(.ts)`__

{{< mermaid title="どのファイルで load 関数を実装すればよいか" >}}
graph TB
    Start([start])-->Branch1{単一のページ用？}
    Branch1--Yes<br/>（単一のページに適用）-->Branch2{サーバーサイド<br/>のみで実行？}
    Branch2--Yes-->End2_Yes[<strong><code>+page.server.js/.ts</code></strong>]
    Branch2--"No"-->Branch3{クライアントサイド<br/>のみで実行？}
    Branch3--Yes-->End3_Yes["<strong><code>+page.js/.ts</code><br/>(<code>export const ssr = false</code>)</strong>"]
    Branch3--"No"-->End3_No["<strong><code>+page.js/.ts</code></strong>"]

    Branch1--No<br/>（レイアウト全体に適用）-->Branch4{サーバーサイド<br/>のみで実行？}
    Branch4--Yes-->End4_Yes[<strong><code>+layout.server.js/.ts</code></strong>]
    Branch4--"No"-->Branch5{クライアントサイド<br/>のみで実行？}
    Branch5--Yes-->End5_Yes["<strong><code>+layout.js/.ts</code><br/>(<code>export const ssr = false</code>)</strong>"]
    Branch5--"No"-->End5_No["<strong><code>+layout.js/.ts</code></strong>"]
{{< /mermaid >}}


具体例
----

TypeScript を使って実装する場合の例をいくつか紹介します。

### 固定のデータを返す

まずは `load()` 関数で固定のデータを返す簡単な例です。
`load` 関数の返すオブジェクトの型情報が、`data` 変数の型情報に自動的に反映されます。
TypeScript の型情報としては、__`./$types`__ という SvelteKit の特殊パスでインポートできる __`PageLoad`__ と __`PageData`__ を使用します。

{{< code lang="ts" title="src/routes/+page.ts" >}}
import type { PageLoad } from './$types';

// load 関数の戻り値の型（明示的に定義する必要はありませんが、
// 戻り値に必ず含めなければいけないプロパティを事前に定義できます）
type Data = {
	message: string;
};

// この関数の戻り値をページコンポーネント (+page.svelte) の data 変数として参照できます
export const load: PageLoad<Data> = () => {
	return { message: 'Hello' };
};
{{< /code >}}

{{< code lang="html" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import type { PageData } from './$types';
	export let data: PageData;  // { message: 'Hello' } が格納されている
</script>

<p>{data.message}</p>
{{< /code >}}

このレベルの `load` 関数実装にはほとんど意味はないですが、まずは基本的な使い方ということで。

### ページのパス (URL) によって異なるデータを返す

`src/routes/users/[name]/+pages.svelte` のような、動的ルートに対する `load` 関数を定義することもできます。
この場合、URL 内のプレースホルダー部分（この例では `[name]`）に指定された値は、`load` 関数に渡される `LoadEvent` オブジェクトを使って、__`event.params.name`__ のように参照することができます。

{{< code lang="ts" title="src/routes/users/[name]/+page.ts" >}}
import type { PageLoad } from './$types';

// load 関数が返すオブジェクトに含むべきプロパティの一覧
type Data = { message: string };

// ページ構築用のデータを取得する
export const load: PageLoad<Data> = ({ params }) => {
	return { message: `Hello, ${params.name}!` };
};
{{< /code >}}

ページコンポーネントの実装は、前述のものと同じです。

{{< code lang="html" title="src/routes/users/[name]/+page.svelte" >}}
<script lang="ts">
	import type { PageData } from './$types';
	export let data: PageData;
</script>

<p>{data.message}</p>
{{< /code >}}

これで、`http://localhost:5173/users/maku` という URL でアクセスしたときに `Hello, maku!` と表示されれば成功です。

### 外部データを fetch する

多くの場合、`load` 関数の中では外部のデータを読み込むことになると思います。
例えば、Web API で取得するデータや、サーバー上の JSON ファイルなどです。
このようなデータの取得は非同期処理 (`async`) になるため、`load` 関数自体も `async` 関数として定義できるようになっています。

次の例では、`load` 関数から [GitHub の REST API](https://docs.github.com/ja/rest/users/users?apiVersion=2022-11-28) を呼び出しています。

{{< code lang="ts" title="src/routes/+page.ts" >}}
import type { PageLoad } from './$types';

// 外部から取得する JSON データは any 型になってしまうので型を付けてやると親切
export type User = {
	login: string;
	id: number;
	name: string;
	bio: string;
};

// ページレンダリング用のデータを取得する
export const load: PageLoad = async ({ fetch }) => {
	const res = await fetch('https://api.github.com/users/maku77');
	const user = (await res.json()) as User;
	return { user }; // +page.svelte 側で data.user で参照できる
};
{{< /code >}}

{{< code lang="html" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import type { PageData } from './$types';
	export let data: PageData;
</script>

<ul>
	<li>login: {data.user.login}</li>
	<li>id: {data.user.id}</li>
	<li>name: {data.user.name}</li>
	<li>bio: {data.user.bio}</li>
</ul>
{{< /code >}}

`load` 関数の中では、__`load` 関数に渡される `fetch` 関数を使用する__ ことに注意してください。
`load` 関数はいろいろなタイミングで呼び出されますが、この専用の `fetch` 関数を使うことで、SvelteKit がよい感じの振る舞いになるよう制御してくれます（参考: [Making fetch requests](https://kit.svelte.dev/docs/load#making-fetch-requests)）。


load 関数についての雑多メモ
----

### 特定の URL パス (slug) 以外が指定されたら 404 エラーにする

SvelteKit が提供する __`error`__ 関数を使って作成したオブジェクトを、`load` 関数から `throw` することで、明示的に 404 エラーを発生させることができます。

{{< code lang="ts" title="src/routes/blog/[slug]/+page.ts" hl_lines="1 12" >}}
import { error } from '@sveltejs/kit';
import type { PageLoad } from './$types';

export const load: PageLoad = ({ params }) => {
	if (params.slug === 'hello-world') {
		return {
			title: 'Hello world!',
			content: 'Welcome to our blog. Lorem ipsum dolor sit amet...',
		};
	}

	throw error(404, 'Not found');
};
{{< /code >}}

上記のように実装した場合、`/blog/hello-world` にはアクセスできますが、それ以外の `/blog/aaa` などにアクセスすると 404 エラーページが表示されます。

### 静的サイトとしてビルドするときの振る舞い

[SSG: Static Site Generation により、Svelte アプリを静的サイトとしてビルド](/p/4oudmxy/) する場合、SvelteKit はサイトのビルド時にすべてのページの URL を把握しなければいけません。

例えば、`src/routes/blog/[slug]/+page.server.ts` というルートが定義されている場合、ビルドの時点で、`[slug]` 部分に入ってくる可能性がある値をすべて把握した上でページを生成しなければいけません。
そのために、SvelteKit は各ページに記述されたリンク情報 (`<a href="/blog/foo">` など) を回収し、`[slug]` 位置に相当する値（この例だと `foo`）を把握し、ページを生成します。

ほとんどのケースではこの仕組みだけでうまくいきますが、`<a>` 要素でリンクされていないページを生成したい場合は、特殊な対応が必要です。
次のように、__`entries`__ という名前のジェネレーター関数を `export` することで、どのページからもリンクされていない動的ルートのページを生成できます（参考: [Page options • Docs • SvelteKit](https://kit.svelte.dev/docs/page-options#entries)）。

{{< code lang="ts" title="src/routes/blog/[slug]/+page.server.ts" >}}
import type { EntryGenerator } from './$types';

export const entries: EntryGenerator = () => {
	return [
		{ slug: 'hello-world' },
		{ slug: 'another-blog-post' }
	];
};

export const prerender = true;
{{< /code >}}

