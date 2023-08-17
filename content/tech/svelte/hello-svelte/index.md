---
title: "Svelte を始める (SvelteKit で Hello World）"
url: "p/qmy6cdh/"
date: "2023-08-17"
tags: ["Svelte"]
---

Svelte とは
----

[Svelte](https://svelte.dev/) は、コンパイラベースの Web アプリケーションフレームワークです。
特殊なテンプレート言語を使ってコンポーネントを実装することで、高度に最適化された HTML/JavaScript/CSS を出力することができます。
React.js アプリなどは、クライアントサイド JavaScript から共通のライブラリを呼び出す形で動作しますが、Svelte はコンパイル後の最適化された JavaScript のみで動作するようになっており、最終的な成果物が非常にコンパクトになります。
独自のテンプレート構文を学ぶ必要はありますが、ユーザー操作に反応するコンポーネントを、少ないコードで記述できるよう工夫されています。

{{% note title="ライバル？の Astro との違い" %}}
Svelte よりも後発のコンパイラベースの Web アプリフレームワークに Astro があります。
Astro は、ビルド時にほとんどの JavaScript コードを実行して、その時点で静的な HTML ファイルを生成するというアプローチをとっています。
最終的にデプロイするのは、HTML (+CSS) だけになるので、初期表示が爆速になるという考え方です。
一方、Svelte はあくまで最適化された JavaScript コードはクライアントサイドで動かすという考え方です。

完全に静的なサイトであれば Astro を使い、インタラクティブなサイトであれば Svelte を使う、という使い分けがよさそうです。

- 参考: [Astro 関連メモ](/p/669r7r2/)
{{% /note %}}


Svelte + SvelteKit プロジェクトの作成
----

Svelte のプロジェクトは次のように __`npm create svelte`__ コマンドで簡単に作成できます。
ウィザード形式の質問に答えていくだけで、TypeScript や ESLint、Prettier の導入まで一気にやってくれます。

{{< code lang="console" title="Svelte プロジェクト (myapp) の作成" >}}
$ npm create svelte@latest myapp
...（省略）...
┌  Welcome to SvelteKit!
│
◇  Which Svelte app template?
│  Skeleton project
│
◇  Add type checking with TypeScript?
│  Yes, using TypeScript syntax
│
◇  Select additional options (use arrow keys/space bar)
│  Add ESLint for code linting, Add Prettier for code formatting
│
└  Your project is ready!
...（省略）...
{{< /code >}}

{{% note title="Svelte と SvelteKit" %}}
上記のコマンドは、実際には [SvelteKit](https://kit.svelte.jp/) というアプリケーションフレームワークを利用する雛形プロジェクトを生成します。
Svelte と SvelteKit の関係は、React.js と Next.js の関係のようなものです。
Svelte はコンポーネントのコンパイラーですが、これだけでは Web アプリを構成することはできません。
SvelteKit を使うことで、ファイルベースのルーティングや、サーバーサイドレンダリング、Vite によるバンドリングなどを行えるようになります。
{{% /note %}}

Svelte プロジェクト（`myapp` ディレクトリ）を作成したら、次のように依存モジュールをインストールし、開発サーバーを立ち上げることができます。

```console
$ cd myapp
$ npm install  # 依存モジュールをインストール
$ npm run dev -- --open  # サーバーを起動してブラウザで開く
```

開発用サーバーは、デフォルトで `5173` 番ポートで待ち受けます。

{{< image w="500" border="true" src="img-001.png" title="Svelte で Hello World" >}}


SvelteKit のルーティング
----

SvelteKit はファイルベースのルーティングの仕組みを採用しており、__`src/routes`__ 以下に __`+page.svelte`__ という名前のファイルを配置することで、ユーザーがアクセス可能な URL が生成されます。
この特殊なファイル名は SvelteKit のルールなので受け入れるしかありません（逆に、Next.js や Astro のように、ページ用の `pages` ディレクトリは存在しません）。
`+page.svelte` ファイルを配置するディレクトリによって、次のように URL が対応づけられます。
基本的には、ページごとに専用のディレクトリを作るというスタンスで、これにより、そのページを構成するためのスクリプト（`+page.server.ts` など）を同じディレクトリでまとめて管理できるようになっています。

| 作成する `+page.svelte` ファイル | 公開される URL |
| ---- | ---- |
| <code>src/routes<strong>/</strong>+page.svelte</code> | __`/`__ |
| <code>src/routes<strong>/about/</strong>+page.svelte</code> | __`/about`__ |
| <code>src/routes<strong>/foo/bar/</strong>+page.svelte` | __`/foo/bar`__ |
| <code>src/routes<strong>/blog/[slug]/</strong>+page.svelte` | __`/blog/[slug]`__（`[slug]` は任意のパス） |

例えば、雛形として生成される `+page.svelte` ファイルの内容は次のようになっています。
`.svelte` という拡張子からも分かるように、これが Svelte コンポーネントです。

{{< code lang="html" title="src/routes/+page.svelte" >}}
<h1>Welcome to SvelteKit</h1>
<p>
	Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a>
	to read the documentation
</p>
{{< /code >}}

現状は HTML の断片的なコードしか含まれていませんが、Svelte コンポーネント独自の構文を使ってコーディングすることができます。
上記の Svelte コンポーネントの内容がそのままレンダリングされるのではなく、実際の表示内容は、__`src/app.html`__ というテンプレートファイルに上記のコンテンツを組み込んだものになります。

{{< code lang="html" title="src/app.html（ページテンプレート）" >}}
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<link rel="icon" href="%sveltekit.assets%/favicon.png" />
		<meta name="viewport" content="width=device-width" />
		%sveltekit.head%
	</head>
	<body data-sveltekit-preload-data="hover">
		<h1>hello svelte</h1>
		<div style="display: contents">%sveltekit.body%</div>
	</body>
</html>
{{< /code >}}

テンプレート内の __`%...%`__ という部分は、SvelteKit によるビルド時に次のように置き換えられます。

- __`%sveltekit.assets%`__ ... アセット用のディレクトリパスに置換されます。
- __`%sveltekit.head%`__ ... Web サイトをビルドした結果、必要な `<link>` 要素や `<script>` 要素が配置されます。また、Svelte コンポーネント内で `<svelte:head>` 要素として追加した内容もここに展開されます。
- __`%sveltekit.body%`__ ... ページのメインコンテンツが配置されます。つまり、`+page.svelte` に記述したマークアップコンテンツです。


ページを追加してみる
----

プロジェクトの初期状態ではトップページ (`src/routes/+page.svelte`) しか存在しないので、試しに次のようなページコンポーネントを追加してみます。

{{< code lang="html" title="src/routes/hello/+page.svelte" >}}
<h1>Hello</h1>
<a href="/">Home</a>
{{< /code >}}

すると、前述のルーティングルール通り、`http://localhost:5173/hello` という URL でアクセスできるページが生成されます。
SvelteKit でサイト内の別ページへリンクするときは、上記のように通常の `<a>` 要素を使用します。


Svelte コンポーネントを作る
----

`.svelte` 拡張子の付いたファイルは Svelte コンポーネントとみなされ、このファイルをコンパイルすることによって、HTML/CSS/JS ファイルが生成されます。
Svelte コンポーネントは、別の Svelte コンポーネントからインポートして使用することができます。

Svelte コンポーネントは、次の 3 つのパートで構成されます（必要なものだけ記述できます）。

1. スクリプト ... TypeScript (JavaScript) コード
2. マークアップ ... 表示する HTML 要素
3. スタイル ... 上のマークアップ用の CSS 定義

{{< code lang="html" title="Svelte コンポーネントの基本構成" >}}
<script lang="ts">
	// 1. ここに JS/TS で処理したいコードを記述します。
	//    TypeScript を使いたいときは上記のように lang="ts" を付けます。
</script>

<!--
	2. ここに出力したいマークアップ要素を記述します。
	   通常の HTML だけではなく Svelte 独自の制御構文を使用できます。
-->

<style>
	/*
	 * 3. ここに CSS スタイルを記述します。
	 *    デフォルトではこのコンポーネントにのみ適用されます。
	 */
</style>
{{< /code >}}

例えば、下記は `Counter` コンポーネントの実装例で、現在のカウント値を増加させるボタンを備えています。
`Counter.svelte` ファイルは `src` ディレクトリ以下のどこに配置してもよいのですが、__`src/lib`__ ディレクトリ以下に配置すると、別のファイルからインポートするときに __`$lib/Counter.svelte`__ というパスで参照できて便利です。

{{< code lang="html" title="src/lib/Counter.svelte" >}}
<script lang="ts">
	let count: number = 0;

	function increment() {
		count += 1;
	}
</script>

<span class="counter">
	<b>現在の値: {count}</b>
	<button on:click={increment}>増やす</button>
</span>

<style>
	.counter {
		display: inline-block;
		padding: 0.5em 1em;
		background: steelblue;
	}
	b {
		color: white;
	}
</style>
{{< /code >}}

スクリプト内で定義した変数 (`counter`) や関数は、マークアップ内で __`{ count }`__ のような埋め込みスクリプトとして参照できます。
`button` 要素には、次のような Svelte 独自のプロパティ __`on:click`__ でイベントハンドラ（`increment` 関数）を呼び出すように設定しています。

```html
<button on:click={increment}>増やす</button>
```

`increment` 関数の中では、次のように `count` 変数の値を更新しているのですが、Svelte ではこのように変数の値を `=` 演算子で再代入することで、その変数を参照している部分が再描画される仕組みになっています。

```ts
function increment() {
	count += 1;
}
```

Web アプリの世界では、このようにデータと連動させてレンダリングを行う仕組みを「リアクティブ」と呼んでいますが、Svelte は React.js などと比べて簡単にリアクティブな実装を行えるようになっています（React.js では `useState` フックを使用して同様の仕組みを実現しています）。

この `Counter.svelte` コンポーネントを利用するには、別のコンポーネントから次のようにインポートします。

{{< code lang="html" title="src/routes/hello/+page.svelte" >}}
<script lang="ts">
	import Counter from '$lib/Counter.svelte';
</script>

<h1>Hello</h1>
<Counter />
{{< /code >}}

{{< image w="500" border="true" src="img-002.png" title="Counter コンポーネントを埋め込んだページ" >}}

`hello` ページに `Counter` コンポーネントを埋め込むことができました。
当然ですが、`Counter` コンポーネントは別のコンポーネントから何度でも使いまわすことができ、インスタンスごとに別々の `count` 変数を保持します（`count` 値は共有されません）。


Svelte コンポーネントの props 定義
----

最後に、Svelte コンポーネントにプロパティ (props) を渡せるようにしてみます。
次のように、Svelte コンポーネント内で変数を定義するときに __`export`__ を付けるだけで、このコンポーネントを利用する側から値をセットできるようになります。

{{< code lang="html" title="src/lib/Counter.svelte" hl_lines="2" >}}
<script lang="ts">
	export let count: number = 0;
	function increment() {
		count += 1;
	}
</script>

<!-- 以下は同じなので省略 -->
{{< /code >}}

`count` 変数の初期値を外からセットするには、次のようにコンポーネントの `count` プロパティ (prop) として指定します。

{{< code lang="html" title="src/routes/hello/+page.svelte" hl_lines="6" >}}
<script lang="ts">
	import Counter from '$lib/Counter.svelte';
</script>

<h1>Hello</h1>
<Counter count={100} />
{{< /code >}}

ちなみに、`count` 変数にはデフォルト値 (`0`) を指定してあるので、`<Counter />` のようにプロパティを省略して使うことも可能です。

これで、Svelte と SvelteKit の基本的な使い方が分かりました。
いろんな決まりごとを覚えなきゃいけないフレームワークですが、使いこなせば React.js や Next.js よりも手軽に Web アプリを作れるかもしれません。

٩(๑❛ᴗ❛๑)۶ わーぃ

