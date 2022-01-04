---
title: "エミュレーター: RetroArch のメモ"
linkTitle: "RetroArch"
url: "/p/n9nybnx"
date: "2020-08-14"
tags: ["エミュレーター"]
---

設定ファイルの場所
----

キーコンフィグなどの設定ファイルは下記のディレクトリに格納されています。

{{< code title="macos の場合" >}}
./Users/ユーザー名/Library/Application Support/RetroArch
{{< /code >}}

キー設定を間違えて、にっちもさっちもいかなくなってしまった場合は、この中の

- config/retroarch.cfg

というファイルを削除して RetroArch を再起動すれば OK です。

スキャンした ROM のリストや、セーブファイルなどは下記のディレクトリに格納されています。

{{< code title="macos の場合" >}}
./Users/ユーザー名/Documents/RetroArch
{{< /code >}}

