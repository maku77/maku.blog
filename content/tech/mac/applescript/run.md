---
title: "Macメモ: AppleScript を実行する"
url: "p/6je96q9/"
date: "2018-02-18"
tags: ["mac"]
weight: 1
description: "AppleScript を使用すると Mac で実行したい一連の処理をスクリプト化することができます。Windows のバッチファイルのようなものです。"
---

macOS の AppleScript を使用すると、実行したい一連の処理をスクリプト化することができます。
Windows のバッチファイルのようなものです。
AppleScript を実行するには、**`osascript`** コマンドを使用します。


ワンライナーで実行する
----

次のようにして、**`-e`** オプションで実行する処理を直接記述することができます。

{{< code lang="console" title="例: ダイアログでメッセージを表示する" >}}
$ osascript -e 'display dialog "Hello"'
{{< /code >}}


スクリプトファイルを実行する
----

長いスクリプトを作成するときは、次のようにスクリプトファイルとして保存しておくのがよいでしょう。

{{< code lang="sh" title="sample.osa（スクリプトファイル）" >}}
#!/usr/bin/env osascript

display dialog "Hello"
{{< /code >}}

スクリプトファイルを実行するときは、ファイル名をそのままパラメーターとして指定します。

```console
$ osascript sample.osa
```

上記のスクリプト例のように 1 行目にシェバング (`#!`) を記述した上で実行権限を付けておけば、そのまま実行ファイルとして実行することができます。

```console
$ chmod +x sample.osa
$ ./sample.osa
```

