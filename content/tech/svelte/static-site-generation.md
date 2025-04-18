---
title: "Svelte 入門: Svelte アプリを完全な静的サイトとしてビルドする (SSG: Static Site Generation)"
url: "p/4oudmxy/"
date: "2023-09-05"
tags: ["Svelte"]
---

{{% private %}}
- [Static site generation (adapter-static) - SvelteKit](https://kit.svelte.jp/docs/adapter-static)
{{% /private %}}

何をするか？
----

Svelte/SvelteKit を使ったプロジェクト全体を、完全な静的リソースの形にビルドする __SSG (Static Site Genration)__ 機能の使い方について説明します。
AWS の S3 ストレージのようなファイルの配信機能しか持たないサービスで Web サイトをホスティングする場合、SSG によるビルドが必要になります。

ちなみに、Cloudflare Pages や Vercel のようなモダンなホスティングサービス上で Svelte アプリをビルドする場合は、このような設定は一切行わずにデプロイすることができます（ゼロコンフィグ）。
できればそういったサービスを使うことを推奨しますが、ここでは何らかの事情でそういった便利なサービスを使用できないケースを想定しています（会社のポリシーで S3 しか使えないなど）。


SvelteKit の SSG 設定
----

### adapter-static のインストール

SvelteKit の __`@sveltejs/adapter-static`__ というモジュールが SSG 機能を提供しているので、まずはこのモジュールをインストールします。

{{< code lang="console" title="adapter-static のインストール" >}}
$ npm install --save-dev @sveltejs/adapter-static
{{< /code >}}

### adapter-static の有効化

ビルド設定ファイル (__`svelte.config.js`__) で、`adapter-static` を使用するように設定します。
プロジェクトの初期状態では、次のような感じで `adapter-auto` というモジュールが使われるように設定されていると思います。

{{< code lang="js" title="svelte.config.js（変更前）" hl_lines="1" >}}
import adapter from '@sveltejs/adapter-auto';
import { vitePreprocess } from '@sveltejs/kit/vite';

/** @type {import('@sveltejs/kit').Config} */
const config = {
	preprocess: vitePreprocess(),
	kit: {
		adapter: adapter()
	}
};

export default config;
{{< /code >}}

`adapter-auto` は、Vercel や Cloudflare Pages などのサービス上でビルドするときに使えるモジュールですが、SSG するときには使えないので、ここを `adapter-static` に置き換えてやります。

{{< code lang="js" title="svelte.config.js（変更後）" hl_lines="1" >}}
import adapter from '@sveltejs/adapter-static';
import { vitePreprocess } from '@sveltejs/kit/vite';

/** @type {import('@sveltejs/kit').Config} */
const config = {
	preprocess: vitePreprocess(),
	kit: {
		adapter: adapter()
	}
};

export default config;
{{< /code >}}

### 各ページのレンダリング設定

SSG によるビルドを行うためには、さらに各ページ (routes) でプリレンダリングを行う想定であることを宣言しておく必要があります。
これは、__`prerender`__ という公開変数に `true` をセットするだけでよいのですが、次のような __`+layout.js`__ ファイルを作成して、全ページにまとめて適用してしまうのが簡単です。

{{< code lang="js" title="src/routes/+layout.js" hl_lines="2" >}}
// SSG (Static Site Generation) のためプリレンダリングを有効化
export const prerender = true;

// すべてのページを index.html ファイルとして出力する
export const trailingSlash = 'always';
{{< /code >}}

上記の例では、ついでに __`trailingSlash`__ の設定も追加しています。
この設定により、各ページの HTML は `hoge.html` ではなく、`hoge/index.html` という形で出力されるようになり、URL の末尾に `.html` を付けずに Web ページにアクセスできるようになります（ホスティング環境によっては、この設定が不要な場合もあります）。


SSG によるビルド
----

上記の設定が終わったら、あとは __`npm run build`__ (`vite build`) でビルドを実行するだけです。

{{< code lang="console" title="Svelte アプリのビルド" >}}
$ npm run build
{{< /code >}}

うまくいくと、__`build`__ ディレクトリに HTML ファイルなどが生成されます。
これらのファイルを S3 などにデプロイすれば OK です。

```
build/
  +-- _app/
  +-- favicon.png
  +-- index.html
  +-- page1/index.html
  +-- page2/index.html
```

ローカルでテストサーバーを立てて表示を確認したいときは、次のようにします。

{{< code lang="console" title="ビルドされたサイトの表示確認" >}}
$ npm run preview -- --open
{{< /code >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ


動的ルートでの SSG ビルド
----

SSG で Svelte アプリを静的サイトとしてビルドする場合、SvelteKit はサイトのビルド時にすべてのページの URL を把握しなければいけません。
例えば、`src/routes/blog/[slug]/+page.svelte` というルートが定義されている場合、ビルドの時点で、`[slug]` 部分に入ってくる可能性がある値をすべて把握した上でページを生成しなければいけません。
そのために、SvelteKit は各ページに記述されたリンク情報 (`<a href="/blog/foo">` など) を回収し、`[slug]` 位置に相当する値（この例だと `foo`）を把握し、ページを生成します。

ほとんどのケースではこの仕組みだけでうまくいきますが、`<a>` 要素でリンクされていないページを生成したい場合は、特殊な対応が必要です。
次のように、__`entries`__ という名前のエントリージェネレーター関数を `export` することで、どのページからもリンクされていない動的ルートのページを生成できます（参考: [Page options • Docs • SvelteKit](https://kit.svelte.dev/docs/page-options#entries)）。

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

外部データを利用するために内部で非同期関数を呼び出す場合は、エントリージェネレーター関数を `async` 関数として定義します。

