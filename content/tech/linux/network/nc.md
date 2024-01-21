---
title: "Linuxコマンド: nc コマンドの使い方"
url: "p/awowke9/"
date: "2024-01-20"
tags: ["linux"]
draft: true
---

ポートスキャン
----

```console
$ nc -z
```


nc による通信確認
----

```console
$ socat tcp-listen:4444,fork,reuseaddr exec:"echo hello"
$ nc localhost 4444
```

- socat = SOCket Cat

