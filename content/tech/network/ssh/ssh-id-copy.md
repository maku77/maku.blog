---
title: "SSH キーの管理: ssh-id-copy で SSH の公開鍵をリモートホストに登録する"
url: "p/2mzbmw8/"
date: "2022-02-17"
tags: ["SSH"]
---

ssh-id-copy とは
----

SSH で公開鍵認証方式を使ってリモートホストに接続するには、リモートホスト側の `~/.ssh/authorized_keys` ファイルに公開鍵を書き込んでおく必要がありますが、[ssh-copy-id](https://manpages.ubuntu.com/manpages/en/man1/ssh-copy-id.1.html) コマンドを使うと、この作業を一撃で済ますことができます。
`ssh-copy-id` コマンドは Linux 環境であれば標準でインストールされているはずです。

__前提条件として、接続先ホストにパスワード認証で SSH 接続できる状態__ にしておく必要があります。
公開鍵の内容をコピーする段階では、まだ SSH キーを使った接続ができないからです。
例えば、次のような感じで、接続先ホストに登録されている「ユーザー名」と「ユーザーパスワード」のペアで接続できるようになっていれば OK です。

{{< code lang="console" title="パスワード認証で接続できるか確認" >}}
$ ssh maku@192.168.1.20
maku@192.168.1.20's password: （接続先ホストのユーザー maku のパスワードを入力）
{{< /code >}}

本記事の作業が完了すれば、SSH 接続時に「ユーザーパスワード」を送信する必要はなくなり、代わりに「SSH 秘密鍵」を使って接続できるようになります。
SSH 秘密鍵のパスフレーズを入力する必要があるかもしれませんが、このパスフレーズがネットワーク上に流れることはないので安全です。


（必要があれば）鍵ファイルの作成 (ssh-keygen)
----

接続元のマシンに次のような秘密鍵＆公開鍵のペアが存在しないときは、[ssh-keygen コマンドなどで作成](/p/ftducs9/)しておきます。

- `~/.ssh/id_rsa` ... 秘密鍵
- `~/.ssh/id_rsa.pub` ... 公開鍵（これをリモートホストに登録します）

{{< code lang="console" title="秘密鍵と公開鍵を生成する" >}}
$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/Users/maku/.ssh/id_rsa): （このパスでよければ Enter）
Enter passphrase (empty for no passphrase): （鍵のパスワードを入力）
Enter same passphrase again: （パスワードを再入力）
Your identification has been saved in /Users/maku/.ssh/id_rsa.
Your public key has been saved in /Users/maku/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:hmOfShQhPstmuGyB3qj0wpdosKXD82ibJs+7Jsb+wSl maku@makumac.local
The key's randomart image is:
+---[RSA 3072]----+
|                 |
|                 |
|o.* * .          |
|O O O = S        |
|o . . .          |
|.@ o * . .       |
|oB * B o         |
| *E+o+           |
|++oOOo           |
+----[SHA256]-----+
{{< /code >}}


リモートホストに公開鍵を登録する (ssh-copy-id)
----

公開鍵の準備ができたら、`ssh-copy-id` コマンドでリモートホストの `~/.ssh/authorized_keys` に書き込みます。
最初は、__`-n`__ オプションをつけて __dry-run 実行__ してみるのがよいです。
実際には実行されませんが、どの公開鍵が登録されるかを確認できます。

{{< code lang="console" title="dry-run で処理内容を確認" >}}
$ ssh-copy-id -n user@192.168.1.20
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/Users/maku/.ssh/id_rsa.pub"
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s),
to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed --
if you are prompted now it is to install the new keys
=-=-=-=-=-=-=-=
Would have added the following key(s):

ssh-rsa AAAAB3NzaCn21Bq...(省略)...Qu4cIuQFG92hxMqU= maku@makumac.local
=-=-=-=-=-=-=-=
{{< /code >}}

公開鍵ファイルは、デフォルトで __`~/.ssh/id*.pub`__ というファイル名で検索されて使用されますが、`-i` オプションで明示することもできます。

```console
$ ssh-copy-id -n -i ~/.ssh/id_rsa_XXX.pub user@192.168.1.20
```

登録内容に問題なさそうであれば、今度は `-n` (dry-run) オプションを外して実際に実行します。
ここでは、まだパスワード認証が使われるので、リモートホスト側のユーザーのパスワードを入力する必要があります。

```console
$ ssh-copy-id user@192.168.1.20
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/Users/maku/.ssh/id_rsa.pub"
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s),
to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed --
if you are prompted now it is to install the new keys
user@192.168.1.20's password: （リモートホストのユーザーのパスワードを入力）

Number of key(s) added: 1

Now try logging into the machine, with: "ssh 'user@192.168.1.20'"
and check to make sure that only the key(s) you wanted were added.
```

これで、`ssh` コマンド実行時に SSH 鍵を使って（公開鍵認証方式で） 接続するようになります。

```console
$ ssh user@192.168.1.20
Enter passphrase for key '/Users/maku/.ssh/id_rsa': （SSH鍵のパスワードを入力）
```

`ssh-copy-id` で `-i` オプションを使って標準と異なる名前の公開鍵を登録した場合は、`ssh` で接続するときも `-i` オプションを使って対となる秘密鍵を指定します（参考: [SSH の接続先ごとにキーを使い分ける](/p/szajt4d/)）。

```console
$ ssh -i ~/.ssh/id_rsa_XXX user@192.168.1.20
```

SSH 鍵のパスワード入力に失敗すると、従来のリモートホストのパスワード認証が実行されますが、この振る舞いは後述のように無効化することができます。


パスワード認証を無効にする
----

SSH 鍵でリモートホストに接続できるようになったら、パスワード認証による接続は無効にしておくと安全です。
次のように `vim` や `nano` エディタで SSH デーモンの設定ファイルを開き、

```console
$ sudo vim /etc/ssh/sshd_config
```

下記の行を修正して保存します。

```
PasswordAuthentication yes
↓
PasswordAuthentication no
```

あとは、SSH デーモンを再起動して反映します。

```console
$ sudo systemctl restart ssh
```


（おまけ）ssh-copy-id を使わずに authorized_keys に登録する場合
----

何らかの理由で `ssh-copy-id` コマンドが使えない場合は、次のようにして `ssh` コマンド経由で公開鍵を登録することができます。

```console
$ cat ~/.ssh/id_rsa.pub | ssh user@192.168.1.20 "cat >> ~/.ssh/authorized_keys"
```

こちらでも簡単に登録できますが、`~/.ssh` ディレクトリが存在しない場合は先に作成しておくなどの対応が必要になってくるので、できれば `ssh-copy-id` コマンドを使った方がよいです。

{{< code lang="console" title="接続先ホストに .ssh ディレクトリがない場合の事前準備" >}}
$ mkdir -m 700 ~/.ssh  # ディレクトリがなければ作成しておく
$ touch ~/.ssh/authorized_keys
$ chmod 600 ~/.ssh/authorized_keys
{{< /code >}}

