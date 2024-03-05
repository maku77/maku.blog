---
title: "Svelte 入門: store を使ってコンポーネント間でデータを共有する"
url: "p/gokv285/"
date: "2024-03-05"
tags: ["Svelte"]
draft: true
---

store とは？
----

Svelte コンポーネント間で何らかのデータを共有するには、__`svelte/store`__ モジュールを使って store と呼ばれるオブジェクトを作成します。
簡単に言うと、グローバル変数のようなものです。

Svelte コンポーネント内で定義した変数は、基本的にそのコンポーネントに閉じたローカル変数として扱われるため、コンポーネント間で共有する変数は必ず store として定義する必要があります（親コンポーネントからバインドした変数で連携する方法はありますが）。
これは、別ファイルとして定義した `.js/.ts` ファイルをインポートして使用する場合も同様です。


store の作成例
----

### 読み取り専用の store を作成する

{{< code lang="ts" title="src/lib/store.ts（store を定義する）" >}}
import { readable } from "svelte/store";

export const colors = readable("red,blue,yellow");
{{< /code >}}

この例では、`colors` という名前の store オブジェクトを作成しています。
コンポーネントからこの store オブジェクトを参照するには、__`$colors`__ のように先頭に `$` を付けて参照します。

{{< code lang="ts" title="store を利用する" >}}
import { colors } from "$/lib/store.ts";

// $colors のように参照する
{{< /code >}}

