---
title: "VS Code で行末の空白（半角スペース）を自動で削除する (files.trimTrailingWhitespace)"
url: "/p/j6jv7it"
date: "2020-07-28"
tags: ["VS Code"]
---

Visual Studio Code で次のように設定しておくと、ファイル保存時に、行末の余計なスペースを自動で削除してくれます。


settings.json で設定する方法
----

{{< code lang="js" title="settings.json" >}}
{
  "files.trimTrailingWhitespace": true,
  // ...
}
{{< /code >}}

Markdown ファイル (`.md`) は、行末の 2 つのスペースが改行の意味を持っていたりします。
そのような場合はスペースが削除されてしまうと都合が悪いので、次のように言語別設定で `markdown` の場合のみ無効化しておきます。

{{< code lang="js" hl_lines="3-5" >}}
{
  "files.trimTrailingWhitespace": true,
  "[markdown]": {
    "files.trimTrailingWhitespace": false
  },
  // ...
}
{{< /code >}}

- 参考: [settings.json について](/p/tfq2cnw)


設定画面で設定する方法
----

{{< image w="600" src="img-001.png" >}}

1. `Ctrl + ,` で設定画面を開く（macOS の場合は `Cmd + ,`）
2. `trim` で検索して __`Trim Trailing Whitespace`__ にチェックを入れる

