---
title: "Macメモ: AppleScript で変数を定義する"
url: "p/zpg5wns/"
date: "2018-02-18"
tags: ["mac"]
description: "AppleScript で変数に値を代入するには、set あるいは copy を使用します。"
---

AppleScript で変数に値を代入するには、**`set`** あるいは **`copy`** を使用します。

次の例では、`set` を使って、変数 `user` に文字列 `"Maku"` を代入しています。

```
set user to "Maku"
display dialog "Hello " & user
```

`set` の代わりに、`copy` を使用することもできます。

```
copy 33 to myAge
```

