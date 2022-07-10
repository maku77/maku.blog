---
title: "Linuxメモ: Linux のユーザーを管理する (useradd, userdel, passwd)"
url: "p/7m5k3hx/"
date: "2022-07-09"
tags: ["Linux"]
---

Linux をインストールしたばかりの状態では、通常は特権ユーザーの `root` でログインできる状態になっています。
システム管理以外の日常的な作業には、`useradd` コマンドで作成した一般ユーザーを用います。


ユーザーを作成する (useradd)
----

新しいユーザーを作成するには、__`useradd`__ コマンドを使用します。
`-m` オプションを指定することで、ユーザーのホームディレクトリを同時に作成することができます。

{{< code title="ユーザー maku を作成する" >}}
# useradd maku    （ユーザーを作成するだけ）
# useradd -m maku （ホームディレクトリを同時に作成する）
# useradd -m -s /bin/bash maku  （ログインシェルを設定する）
{{< /code >}}

- オプションの意味
  - __`-m`__ ... ユーザーのホームディレクトリを同時に作成する
  - __`-s /bin/bash`__ ... ログインシェルを `/bin/bash` に設定する

`/etc/passwd` ファイルを見ると、ユーザーが追加されていることを確認できます。
デフォルトでは、最初に作成したユーザーのユーザー ID は 1000 になります（その後追加するユーザーは、1001、1002、1003 となります）。

```
# grep maku /etc/passwd
maku:x:1000:1000::/home/maku:/bin/bash
```


ユーザーを削除する (userdel)
----

Linux のユーザーを削除するには、__`userdel`__ コマンドを使用します。
`-r` オプションを指定することで、ユーザーのホームディレクトリを同時に削除することができます。

{{< code title="ユーザー maku とそのホームディレクトリ /home/maku を削除する" >}}
# userdel -r maku
{{< /code >}}


ユーザーのパスワードを設定する (passwd)
----

作成したユーザーで Linux にログインするには、__`passwd`__ コマンドでパスワードを設定しておく必要があります。
root ユーザーが、特定のユーザーのパスワードを設定するには次のようにします。

{{< code title="ユーザー maku のパスワードを設定する" >}}
# passwd maku
New password: （パスワードを入力）
Retype new password: （パスワードを入力）
passwd: password updated successfully
{{< /code >}}

ユーザーにパスワードを設定したら、`ssh` コマンドでローカルホストにログインできるか確認できます。

```
# ssh maku@localhost
maku@localhost's password: （上で設定したパスワードを入力）
```

カレントユーザーのパスワードを変更するには、`passwd` コマンドを引数なしで実行します。

{{< code lang="console" title="カレントユーザーのパスワードを変更する" >}}
$ passwd
Changing password for maku.
Current password: （現在のパスワードを入力する）
New password: （新しいパスワード入力する）
Retype new password: （新しいパスワードを入力する）
passwd: password updated successfully
{{< /code >}}

