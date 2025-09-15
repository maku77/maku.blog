---
title: "macOS のスポットライトのインデックス処理を停止・開始する (mdutil)"
url: "/p/j5gr3dn"
date: "2021-05-30"
tags: ["mac"]
---

macOS の **`mdutil`** コマンドを使用すると、スポットライトのインデックス設定を変更することができます。

{{< code lang="console" title="インデックス処理の現在の設定を調べる" >}}
$ sudo mdutil -a -s
/:
    Indexing enabled.
/System/Volumes/Data:
    Indexing enabled.
/Volumes/SD_card:
    Indexing and searching disabled.
{{< /code >}}

{{< code lang="console" title="インデックス処理を無効化する" >}}
$ sudo mdutil -a -i off
{{< /code >}}

{{< code lang="console" title="インデックス処理を有効化する" >}}
$ sudo mdutil -a -i on
{{< /code >}}

{{< code lang="console" title="インデックスを削除して再生成" >}}
$ sudo mdutil -a -E
{{< /code >}}

