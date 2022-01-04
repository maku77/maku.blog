---
title: "Linuxコマンド: ファイルの MIME メディアタイプを調べる (file)"
linkTitle: "ファイルの MIME メディアタイプを調べる (file)"
url: "/p/6i7dvxj"
date: "2011-06-20"
tags: ["Linux"]
---

`file` コマンドに `-i` オプションを付けて実行すると、そのファイルの MIME タイプを調べることができます。

{{< code >}}
$ file -i sample.txt
sample.txt: text/plain; charset=us-ascii
{{< /code >}}

