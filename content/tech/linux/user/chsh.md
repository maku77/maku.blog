---
title: "Linuxメモ: ユーザーのデフォルトシェルを変更する (chsh)"
linkTitle: "ユーザーのデフォルトシェルを変更する (chsh)"
url: "p/cqaq7n4/"
date: "2022-08-05"
tags: ["Linux"]
---

ユーザーが Linux にログインしたときに起動するデフォルトシェルは、__`chsh`__ (change login shell) コマンドで変更することができます。

{{< code lang="console" title="カレントユーザーのログインシェルを /bin/bash に変更する" >}}
$ chsh -s /bin/bash
{{< /code >}}

`-s` オプションではシェルのフルパスを指定する必要があることに注意してください。
例えば、Bash を使いたければ、`bash` ではなく `/bin/bash` と入力します。
`-s` オプションを省略すると、インタラクティブにシェルのパスを入力できます。

{{% reference %}}
- [Linux のユーザーを管理する (useradd, userdel, passwd)](/p/7m5k3hx/)
  - ユーザーを追加する (`useradd`) ときに、ついでにログインシェルを指定できます。
{{% /reference %}}

現在のシステムで選択可能なシェルの一覧は、__`/etc/shells`__ ファイルで確認できます。

{{< code lang="shell" title="/etc/shells（Ubuntu 22.04 の場合）" >}}
# /etc/shells: valid login shells
/bin/sh
/bin/bash
/usr/bin/bash
/bin/rbash
/usr/bin/rbash
/usr/bin/sh
/bin/dash
/usr/bin/dash
/usr/bin/tmux
/usr/bin/screen
{{< /code >}}

ユーザーごとのログインシェルが現在どう設定されているかは、__`/etc/passwd`__ ファイルで確認できます。

{{< code lang="console" >}}
$ cat /etc/passwd
...（省略）...
maku:x:1000:1000::/home/maku:/bin/bash
{{< /code >}}

