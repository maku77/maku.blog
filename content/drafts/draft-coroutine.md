---
title: "DRAFT: Kotlin コルーチン（GlobalScope をなくそう勉強会）"
url: "/p/5p2cnx8"
date: "2021-05-21"
tags: ["Kotlin"]
draft: true
---

- CoroutineScope()
    - 同一スコープ内のいずれかの（兄弟）コルーチンでエラーが発生した場合、全てのコルーチンの処理が停止する。
- SupervisorScope()
    - 同一スコープ内のいずれかの（兄弟）コルーチンでエラーが発生しても、独立して動き続ける。

起動の仕方。

- Activity / Fragment / View から
    - `lifecycleScope.launch`
- ViewModel から
    - `viewModelScope.launch`

```
lifecycleScope.launch {
    ...ここで...
    coroutineScope()
        // いずれかのコルーチンがエラー/キャンセルされるとまとめて停止する。

    supervisorScope()
        // これらは独立して動き続ける
}
```

