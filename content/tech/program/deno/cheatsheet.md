---
title: "Deno チートシート"
url: "p/wdjoxhq/"
date: "2024-01-03"
tags: ["cheatsheet", "Deno"]
draft: true
---

ディレクトリが存在しない場合だけ作成する
----

```ts
import { ensureDir } from "https://deno.land/std/fs/mod.ts";

await ensureDir('./path/to/dir');
```


パスを結合する
----

```ts
import { join } from "https://deno.land/std/path/join.ts";

const fullPath = join(dirPath, filename);
```

