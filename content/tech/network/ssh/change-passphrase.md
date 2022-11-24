---
title: "SSH キーの管理: SSH キーのパスワードを変更する (ssh-keygen -p)"
url: "p/as2ahpw/"
date: "2022-11-24"
tags: ["SSH"]
---

既存の SSH キー（秘密鍵）のパスワード（パスフレーズ）を変更するには、__`ssh-keygen -p`__ コマンドを使用します。

```
$ ssh-keygen -p [-a rounds] [-f keyfile] [-m format] [-N new_passphrase] [-P old_passphrase]
```

変更する SSH キーのファイルパス (`-f`) や、旧パスワード (`-P`)、新パスワード (`-N`) をオプションで指定することもできますが、次のようにオプションなしで実行すれば、対話的に変更内容を入力していけます。

```console
$ ssh-keygen -p
Enter file in which the key is (/Users/maku/.ssh/id_rsa):
Enter old passphrase: ********
Key has comment 'maku@macbook.local'
Enter new passphrase (empty for no passphrase): ********
Enter same passphrase again: ********
Your identification has been saved with the new passphrase.
```

パスワードはオプションで指定すると入力内容が見えてしまうので、上記のように対話的に入力した方がよいでしょう。

