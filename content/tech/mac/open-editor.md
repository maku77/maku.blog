---
title: "macOS のコマンドラインからデフォルトエディタでファイルを開く (open -t)"
url: "p/p2mvive/"
date: "2018-02-18"
tags: ["mac"]
---

次のようにすると、`.txt` ファイルに関連づけられたエディタでファイルを開くことができます。

```console
$ open sample.txt
```

ただし、任意の拡張子のファイルをデフォルトのテキストエディタで開く場合は、**`-t`** オプションを指定する必要があります。

```console
$ open -t sample.xxx
```

任意のアプリケーションでファイルを開くには、**`-a`** オプションでアプリケーションを指定します。

```console
$ open -a /Applications/MacVim.app sample.txt
```

