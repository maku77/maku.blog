---
title: "Svelte 入門: SvelteKit を使ったプロジェクトのディレクトリ構成／コーディングルールなど"
url: "p/69urn22/"
date: "2023-11-01"
tags: ["Svelte"]
---

Svelte/SvelteKit を使用したプロジェクトのディレクトリ構成やファイル名などのルールのまとめです。
これはひとつの案なので、具体的なルールはプロジェクトごとに決めてください。

SvelteKit のようなファイルベースのルーティングを採用したフレームワークでは、プロジェクトの構成は大体決まったものになるのですが、それでも細かい部分では独自のルールを採用することが多いと思います。
プロジェクトごとに定義したルールは、GitHub リポジトリの `README.md` あたりに記述しておくとよいです。


ディレクトリ構成
----

下記のプロジェクト構造は、SvelteKit 本家の [Project structure • Docs • SvelteKit](https://kit.svelte.dev/docs/project-structure) からの抜粋です。

```
my-project/
├ src/
│ ├ lib/
│ │ ├ server/
│ │ │ └ [your server-only lib files]
│ │ └ [your lib files]
│ ├ params/
│ │ └ [your param matchers]
│ ├ routes/
│ │ └ [your routes]
│ ├ app.html
│ ├ error.html
│ ├ hooks.client.js
│ ├ hooks.server.js
│ └ service-worker.js
├ static/
│ └ [your static assets]
├ tests/
│ └ [your tests]
├ package.json
├ svelte.config.js
├ tsconfig.json
└ vite.config.js
```

基本的にはこのようなディレクトリ構造になりますが、`src/lib` ディレクトリ以下の構成や、各種ファイル名のルールなどは、プロジェクトごとに定義しておいた方がよいでしょう。

### src/lib/server ディレクトリ

ここにはサーバーサイドで実行しなければいけない関数を定義したライブラリファイルを格納します。
例えば、`src/lib/server/search.ts` という名前のファイルを置いた場合、`+page.server.ts` などのコードから次のようにインポートできます。

{{< code lang="ts" title="+page.server.ts" >}}
import { searchBooks } from '$lib/server/search';
{{< /code >}}

この `server` というディレクトリ名はただの慣例ではなく、SvelteKit が特別な扱いをします。
このパスに置かれたファイルを、`+page.svelte` などクライアントサイドで実行される可能性のあるコードからインポートしようとすると、SvelteKit が警告してくれます。
SSR (Server Side Rendering) を使用するサイトでは、このディレクトリをうまく活用してください。

### src/lib/components ディレクトリ

複数個所で使用できる汎用的な Svelte コンポーネントを格納するために、__`components`__ というディレクトリを用意する例はよくあります。
例えば、`src/lib/components/Avatar.svelte` という名前のファイルを置いた場合、`+page.svelte` などの Svelte コンポーネントから次のようにインポートできます。

{{< code lang="html" title="+page.svelte" >}}
<script>
	import Avatar from '$lib/components/Avatar.svelte';
</script>
{{< /code >}}

逆に、特定のページ (`+page.svelte`) でしか使用しないような Svelte コンポーネントは、そのファイルと同じ階層（`src/routes` 以下）に置いてしまえばよいです。
こうすることで、`src/lib` ディレクトリが汎用性のないファイルで溢れかえってしまうことを防げます。
`src/routes` ディレクトリはもともとページごとに階層化されているので、コンポーネントの依存関係も分かりやすいです。

```
src/
└ routes/
　 └ books/
　 　 ├ +page.svelte
　 　 └ BookTable.svelte （上の +page.svelte でしか使わないコンポーネント）
```

### src/lib/assets ディレクトリ

ビルド時に静的にインポートして使用するリソースファイル（JSON や画像など）がたくさんある場合、__`assets`__ のような専用のディレクトリを作っておくとよいかもしれません（参考: [Svelte 応用: JSON ファイルをインポートして使う](/p/pzrza7m/)）。

似たような用途の `static` ディレクトリがありますが、`static` ディレクトリはあくまでホスティングサーバーにそのままアップロードするファイル（`favicon.png` など）を格納するためのディレクトリであることに注意してください。
JavaScript/TypeScript から明示的にインポートするようなリソースは、`src` ディレクトリ階層に配置する必要があります。

あと、Svelte コンポーネントと同様ですが、汎用性のないリソースファイルに関しては、それを使用する `+page.svelte` と同じディレクトリに置いてしまえばよいです。


命名規則
----

- ディレクトリ名: __`kebab-case`__ （例: `src/routes/user-settings/`)
  - ただし、基本は 1 単語でシンプルな名前を付けるのが望ましいです。
- Svelte コンポーネントのファイル名: __`CamelCase`__ （例: `src/routes/search/FilterPanel.svelte`）
  - ただし、SvelteKit が定めるルートファイル（`+page.svelte` など）は例外です。
- その他のファイル名: __`kebab-case`__ （例: `string-utils.ts`、`app.css`、`b1-map.json`）
  - ただし、外から取得したデータファイルなどをそのままの名前で使いたい場合は例外とします。


サイト全体のスタイル定義
----

Svelte コンポーネント内の `<style>` 要素では、スコープを限定したスタイル定義を行うことができますが、サイト全体に適用する CSS や、複数コンポーネントで共有する CSS などの置き場所を定義しておくとよいかもしれません。

- 例: `src/lib/theme/global.css` ... サイト全体に適用するスタイル（テーマ設定）

`src/routes` ディレクトリ以下の `+layout.svelte` で次のようにインポートしておけば、すべてのコンポーネントにスタイルを適用できます。

{{< code lang="html" title="src/routes/+layout.svelte" >}}
<style>
	/* src/lib/theme/global.css を読み込む */
	@import '$lib/theme/global.css';
</style>
{{< /code >}}

もちろん、`+layout.svelte` の `<style>` 要素の中で直接定義しちゃうのもありです。
その場合は、`body` 要素などの上位要素のスタイル定義を行うときに、`:global()` というスコープ指定が必要なことに注意してください。

{{< code lang="html" title="src/routes/+layout.svelte" >}}
<style>
	:global(body) {
		margin: 0;
		padding: 0;
		background: white;
	}

	h1 {
		font-size: 2.5rem;
	}
</style>
{{< /code >}}


まとめ
----

```
my-project/
├ src/
│ ├ lib/
│ │ ├ assets/
│ │ │ └ コードからインポートして使用する汎用的なリソースファイル （例: config.json）
│ │ ├ components/
│ │ │ └ 汎用的な Svelte コンポーネント（例: Avatar.svelte）
│ │ ├ server/
│ │ │ └ サーバーサイドでの実行が必要な汎用的なライブラリ（例: search.ts）
│ │ ├ theme/
│ │ │ └ サイト全体あるいは複数のコンポーネントに適用する CSS （例: global.css）
│ │ └ クライアントサイドでも実行可能な汎用的なライブラリ（例: string-utils.ts）
│ ├ params/
│ │ └ 動的ルートのカスタマイズ用の param matchers 定義
│ ├ routes/
│ │ └ foo-bar/
│ │ 　 ├ +page.svelte
│ │ 　 └ PuniPuni.svelte （このルートでのみ使う Svelte コンポーネント）
│ ├ ...
├ static/
│ └ そのままデプロイするファイル（例: favicon.png）
├ tests/
│ └ ユニットテストコード
...
```

