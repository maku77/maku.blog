---
title: "SSH キーの管理: SSH キーのコメントを確認する／変更する (ssh-keygen -c)"
url: "p/o3m4j2h/"
date: "2022-11-24"
tags: ["SSH"]
---

SSH 鍵ファイルのコメントを確認する
----

OpenSSH の [`ssh-keygen` コマンド](/p/ftducs9/) で生成した SSH キーペア（秘密鍵＋公開鍵）には、デフォルトで __`ユーザー名@ホスト名`__ という形のコメントが付加されます。
このコメントは、公開鍵ファイル (`~/.ssh/id_XXX.pub`) の末尾を見ると確認できます。

{{< code title="~/.ssh/id_ed25519.pub" >}}
ssh-ed25519 AAA3NzaC1AA...省略...COst+87ciWFY maku@macbook.local
{{< /code >}}

あるいは、__`ssh-keygen -l`__ コマンドで確認することもできます。

{{< code lang="console" title="SSH キーの内容を確認" >}}
$ ssh-keygen -l -f ~/.ssh/id_ed25519
256 SHA256:vd6Ab577wzS6/so1BR1zGP8r7br0smWV4Wklw28Rre0 maku@macbook.local (ED25519)
{{< /code >}}


SSH 鍵ファイルのコメントを設定する
----

SSH キーのコメントは、キーの作成時に __`-C`__ オプションで自由に設定できます。

{{< code lang="console" title="SSH キーの作成時にコメントを設定" >}}
$ ssh-keygen -t ed25519 -C "your_email@example.com"
{{< /code >}}

既存の SSH キーのコメントを修正するには、__`ssh-keygen -c`__ コマンドを使用します。

{{< code lang="console" title="SSH キーのコメントを変更" >}}
$ ssh-keygen -c -C "新しいコメント" -f ~/.ssh/id_ed25519
{{< /code >}}

オプションの大文字と小文字に注意してください。
最初は小文字 (`-c`) で、後ろは大文字 (`-C`) です。

