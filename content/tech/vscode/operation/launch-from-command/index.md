---
title: "VS Code を code コマンドで起動できるようにする"
url: "/p/f5iv9kx"
date: "2020-06-10"
tags: ["VS Code"]
weight: 100
---

Windows のコマンドプロンプトや、macOS のターミナルから Visual Studio Code を開くには、__`code`__ コマンドを使用します。

{{< code >}}
$ code .           # ディレクトリを開く（通常はこれを使う）
$ code sample.txt  # ファイルを開く
{{< /code >}}

`code` コマンドが見つからない (command not found) と言われる場合は、VS Code から次のように実行することで実行できるようになります。

1. `Cmd + Shift + P` (`Ctrl + Shift + P`) でコマンドパレットを開く
2. `shell` と入力し、__`Shell Command: install 'code' command in PATH`__ を選択する

macOS の場合は、下記のコマンドへのシンボリックリンクとして、`/usr/local/bin/code` というファイルが作成されるようです。

{{< code >}}
/Applications/Visual Studio Code.app/Contents/Resources/app/bin/code
{{< /code >}}

