---
title: "Svelte 入門: 複数のコンポーネントで状態を共有する（$state ルーン）"
url: "p/nhuh2if/"
date: "2025-03-16"
tags: ["Svelte"]
---

$state ルーンのおさらい
----

Svelte 5 では、**`$state`** ルーンを使ってリアクティブな変数を定義することができます。
下記は公式サイトでも最初に出てくる基本的なカウンター変数の実装例です。

{{< code lang="svelte" title="src/lib/components/CounterButton.svelte" >}}
<script lang="ts">
	let count = $state(0);
</script>

<button onclick={() => count++}>
	clicks: {count}
</button>
{{< /code >}}

上記のように `$state` ルーンを使って定義した `count` 変数はリアクティブな変数として扱われ、`count` の値が変更されると、それに連動して画面上の表示も更新されます（`clicks: 0` が `clicks: 1` になる）。
`count` の値を変更したあとに再描画処理を呼び出す、という手続き的なコードを書く必要はありません。


リアクティブな変数のロジックを切り出す
----

`$state` ルーンを使ったロジックは、`.svelte` コンポーネントから分離して別ファイルに記述することができます。
これは Svelte 5 でルーンが導入された理由のひとつです。
`$state` ルーンを使う TypeScript コードの拡張子は、単純な `.ts` ではなく **`.svelte.ts`** にする必要があります（ルーンは特殊なビルド処理が必要なため）。

下記は、リアクティブなカウンター変数のロジックを別ファイルに分離した例です。
`$state` ルーンだけでなく、`$effect` ルーンを使ってリアクティブ変数の変化に反応させることもできます。
ここでは、`count` 変数の値が変更されるたびにコンソールにログを出力するようにしています。

{{< code lang="svelte" title="src/lib/counter.svelte.ts" >}}
export function createCounter() {
	let count = $state(0);

	$effect(() => {
		console.log(`The count is now ${count}`);
	});

	return {
		get count() { return count; },
		increment: () => { count += 1; }
	};
}
{{< /code >}}

この `createCounter()` 関数は、任意のコンポーネントから以下のように使用します。

{{< code lang="svelte" title="src/lib/components/CounterButton.svelte" >}}
<script lang="ts">
	import { createCounter } from '$lib/counter.svelte';  // .ts は付けない
	let counter = createCounter();  // リアクティブなプロパティを持つオブジェクトを生成
</script>

<button onclick={() => counter.increment()}>
	clicks: {counter.count}
</button>
{{< /code >}}

`counter` オブジェクトの `increment()` メソッドを呼び出すと、内部で保持しているリアクティブなカウンター変数 (`count`) の値がインクリメントされるため、それを参照している UI も自動的に更新されます。


リアクティブな変数の値を複数コンポーネントで共有する
----

`.svelte.ts` ファイル内でグローバルなリアクティブ変数を定義し、それを `export` することで、複数のコンポーネントでその変数を共有することができます。
前述の例ではリアクティブ変数の「ロジック」を使いまわせるようにしましたが、今回は、**リアクティブ変数の「値」を共有します**。

{{< code lang="ts" title="src/lib/counter.svelte.ts" >}}
export const counter = $state({ count: 0 });
{{< /code >}}

複数のコンポーネントでリアクティブ変数の値を共有したいときは、**`$state()` に渡す初期値はオブジェクトの形で渡す必要がある** ことに注意してください。

```ts
export const counter = $state(0);  // NG
```

上記のようにプリミティブな値で初期化してしまうと、利用側のコンポーネントでの再代入処理（`counter++` など）が、別の変数への代入として扱われてしまうため、リアクティブな振る舞いが失われてしまうようです（複数コンポーネント間で値が共有されない）。

上記のように `export` した `counter` 変数は、以下のように複数のコンポーネントから参照します。

{{< code lang="svelte" title="src/lib/components/CounterButton1.svelte" >}}
<script lang="ts">
	import { counter } from '$lib/counter.svelte';
</script>

<button onclick={() => counter.count++}>
	Button1: {counter.count}
</button>
{{< /code >}}

{{< code lang="svelte" title="src/lib/components/CounterButton2.svelte（ほぼ同じ実装）" >}}
<script lang="ts">
	import { counter } from '$lib/counter.svelte';
</script>

<button onclick={() => counter.count++}>
	Button2: {counter.count}
</button>
{{< /code >}}

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import CounterButton1 from '$lib/components/CounterButton1.svelte';
	import CounterButton2 from '$lib/components/CounterButton2.svelte';
</script>

<CounterButton1 />
<CounterButton2 />
{{< /code >}}

2 つのコンポーネントから参照している `counter` 変数は、実際には同一のオブジェクトを指しています。
そのため、そのプロパティであるリアクティブ変数 `counter.count` の値が変更されると、2 つのコンポーネントの UI が同時に更新されます。

Svelte 4 の頃は、このような処理は Store の仕組み (`svelte/store`) を使って実装する必要がありましたが、Svelte 5 では `$state` ルーンを使って、よりシンプルに記述できるようになりました。


（おまけ）クラス内でリアクティブ変数を使う
----

クラスのプロパティとしてリアクティブ変数を使うこともできます。
下記の例では、`Counter` クラスの `count` プロパティをリアクティブにしています。

{{< code lang="ts" title="src/lib/counter.svelte.ts" >}}
export class Counter {
	count: number = $state(0);
}

export const counter = new Counter();
{{< /code >}}

さらに、上記モジュールではグローバル変数として作成した `counter` オブジェクトを `export` しているため、複数のコンポーネントで `counter.count` の値を共有できます。

{{< code lang="svelte" title="src/lib/components/CounterButton.svelte" >}}
<script lang="ts">
	import { counter } from '$lib/counter.svelte';
</script>

<button onclick={() => counter.count++}>
	clicks: {counter.count}
</button>
{{< /code >}}

この仕組みはとても強力で、リアクティブな変数をカプセル化して、クラス内のロジックを構築していくことができます。

{{< code lang="ts" title="src/lib/counter.svelte.ts" >}}
export class Counter {
	#count: number = $state(0);  // Private field

	get count(): number {
		return this.#count;
	}

	increment(): void {
		this.#count += 1;
	}
}

export const counter = new Counter();
{{</ code >}}

{{< code lang="svelte" title="src/lib/components/CounterButton.svelte" >}}
<script lang="ts">
	import { counter } from '$lib/counter.svelte';
</script>

<button onclick={() => counter.increment()}>
	clicks: {counter.count}
</button>
{{< /code >}}

٩(๑❛ᴗ❛๑)۶ 超便利っ


応用例
----

- [Svelte 実装例: $state ルーンによる状態変数を localStorage/sessionStorage と連動させる](/p/tpjvh3q/)

