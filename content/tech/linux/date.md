---
title: "date コマンドの使い方"
url: "/p/ucnv5ck"
date: "2021-02-23"
tags: ["Linux"]
---

Linux や macOS に搭載されている `date` コマンドの使い方のメモです。


日時をフォーマットして出力する
----

{{< code >}}
$ date +'%Y-%m-%d'
2021-02-23
{{< /code >}}

{{< code >}}
$ date +'%Y-%m-%d %H:%M:%S'
2021-02-23 01:08:51
{{< /code >}}


Linux と macOS の date コマンドの違い
----

Linux と macOS では、標準でインストールされている `date` コマンドが次のように異なります。

- Linux ... GUN/Linux 系の `date` コマンド
- macOS ... BSD 系の `date` コマンド

そのため、`-d` オプションと `-v` オプションの使い方に次のような違いがあります。

{{< code title="1 カ月前の日付を調べる" >}}
$ date -d '-1 month'     # GNU/Linux
$ date -d '1 month ago'  # GNU/Linux
$ date -v-1m             # macOS
{{< /code >}}

{{< code title="1 日前の日付を調べる" >}}
$ date -d '-1 day'  # GNU/Linux
$ date -v-1d        # macOS
{{< /code >}}

