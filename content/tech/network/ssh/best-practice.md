---
title: "sshd 設定のベストプラクティス"
url: "p/42cmu5d/"
date: "2022-07-17"
tags: ["SSH"]
---

これは何？
----

{{% private %}}
`man sshd_config` でマニュアル表示可能。
{{% /private %}}

SSH をできるだけ安全に使うための sshd（OpenSSH サーバー）の設定方法です。

sshd の設定は、__`/etc/ssh/sshd_config`__ ファイルで行います。
`sudo` などで設定ファイルを編集後、sshd サービスにリロード要求することで反映できます。

{{< code lang="console" title="典型的な sshd の設定変更手順" >}}
$ sudo vim /etc/ssh/sshd_config  # エディタで設定を変更
$ sudo systemctl reload ssh      # sshd を再起動
{{< /code >}}

- 参考: [SSH サーバー (sshd) の起動・停止・再起動方法まとめ](/p/9gs3cmu/)

`sshd` の再起動や設定のリロードを行っても、作業中の SSH 接続が切れることはありません。
設定変更後は、別のターミナルから接続できることを確認してから、作業用の SSH 接続をクローズするようにしましょう。


root ユーザーでの接続を拒否する (PermitRootLogin)
----

__`PermitRootLogin`__ を `no` に設定することで、このサーバーには `root` ユーザーで SSH 接続することができなくなります（`ssh root@example.com` できなくなる）。

{{< code title="/etc/ssh/sshd_config" >}}
PermitRootLogin no
{{< /code >}}

管理者権限での作業が必要な場合は、一般ユーザーでの接続後に `sudo` 実行することになります。
`root` ユーザーで直接 SSH 接続できるようになっていると、悪意のあるユーザーによるパスワード総当たり攻撃を許してしまうことになります。
後述の `PasswordAuthentication` 設定でパスワードによる認証を拒否している場合でも、`root` ユーザーでの直接接続は避けるべきです。
なぜなら、いきなり `root` ユーザーで接続してしまうと、実際に誰が `root` 接続して作業したのかをログから判別できなくなってしまうからです。
一般ユーザーでログインして `sudo` することを強制しておけば、誰が管理者権限でコマンド実行したのかをログから確認することができます (`sudo grep COMMAND /var/log/auth.log`)。

`PermitRootLogin` には下記のような値を設定できます。

| `PermitRootLogin` の値 | パスワードで root 接続 | SSH キーで root 接続 |
| ---- | :--: | :--: |
| `yes` | ● | ● |
| __`no`__ | - | - |
| `prohibit-password` (default) | - | ● |
| `forced-commands-only` | - | ●<br>キーに command 指定が必要 |

`PermitRootLogin` のデフォルト値は `prohibit-password` になっており、SSH キーを利用したログインのみが許可されています。
レンタルサーバー (VPS) などでは、ユーザーに初期設定してもらう都合上、`PermitRootLogin` が `yes` になっていたりするので要注意です。


接続可能なユーザーを制限する (AllowUsers)
----

デフォルトではすべてのユーザー名で SSH 接続できるようになっていますが、SSH 接続するユーザーがあらかじめわかっている場合は、__`AllowUsers`__ でユーザー名を列挙しておきます。

{{< code title="/etc/ssh/sshd_config" >}}
AllowUsers  maku ansible backup
{{< /code >}}

複数のユーザー ID を指定する場合は、上記のようにスペース区切りで列挙します。
この例では、`maku`、`ansible`、`backup` というユーザー名のみで SSH 接続を許可しています。


最大トライ数を減らす (MaxAuthTries)
----

__`MaxAuthTries`__ は、SSH クライアントからの接続が何回失敗したら、リトライを打ち切るかを指定します。

{{< code title="/etc/ssh/sshd_config" >}}
MaxAuthTries 1
{{< /code >}}

例えば、上記のように 1 を指定しておくと、パスワード入力に 1 度でも失敗すると接続拒否されるようになります（リトライのプロンプトが表示されません）。

```console
$ ssh maku@examle.com
maku@example.com's password: （間違ったパスワードを入力する）
Received disconnect from xxxx:xxxx:xxxx:xxx:xxx:xxx:xx:xx port 22:2: Too many authentication failures
```

`MaxAuthTries` のデフォルト値は 6 ですが、SSH クライアント側の設定 (`/etc/ssh/ssh_config`) の `NumberOfPasswordPrompts` がデフォルトで 3 になっているため、通常はパスワード入力を 3 回失敗すると `ssh` コマンドは終了します。


ポート番号を 22 以外に変更する (Port)
----

当然ですが sshd が攻撃対象にされる場合は、22 番ポートが真っ先に狙われます。
次のように、sshd のポート番号を変更することで、攻撃対象から外されやすくなります。

{{< code title="/etc/ssh/sshd_config" >}}
Port 1943
{{< /code >}}

sshd のポート番号を変更した場合は、ファイアウォール（パケットフィルタ）の設定も忘れずに変更しておく必要があります。

- 参考: [各種 Linux のファイアウォール設定ツール](/p/ceow5cj/)

設定変更後は、`ssh` コマンド（および内部的に `ssh` を利用するコマンド）を実行する際に、明示的なポート番号の指定が必要になります。

```console
$ ssh -p 1943 maku@example.com
```

あるいは、特定のホストに SSH 接続するときのポート番号を設定ファイルで指定しておくこともできます。

{{< code title="~/.ssh/config" >}}
Host example.com
    HostName examle.com
    Port 1943
{{< /code >}}

いずれにしても、ポート番号を変更するといろいろなコマンドの使い勝手が悪くなるので、この設定は余裕があればやっておけばよいと思います。
10022 や 20022 など、末尾のポート番号を 22 にしておくと分かりやすいのですが、これではポート番号を変更する効果がほとんどないので、変更するなら特定しにくいポート番号にすべきです。


ログイン成功までの猶予期間を短くする (LoginGraceTime)
----

__`LoginGraceTime`__ は、`ssh` コマンドを実行してからログインするまでの猶予時間です。

{{< code title="/etc/ssh/sshd_config" >}}
LoginGraceTime 15s
{{< /code >}}

典型的にはパスワード入力に成功するまでの猶予期間です。
デフォルトは `2m`（120 秒）ですが、もう少し短くしてもよさそうです。
上記の例では `15s` と設定しているので、15 秒以内にログインに成功しないと、正しいパスワードで接続しようとしても接続拒否されます。

```console
$ ssh maku@examle.com
maku@example.com's password: （15 秒以内に認証しないと拒否される）
Connection closed by xxxx:xxxx:xxxx:xxx:xxx:xxx:xx:xx port 22
```

ちなみに `LoginGraceTime` を 0 に設定するとタイムアウトしなくなりますが、そのような設定はすべきではありません。


パスワード認証を無効にする (PasswordAuthentication, ChallengeResponseAuthentication)
----

[SSH キーによる接続設定](/p/2mzbmw8/) が終わったら、パスワードによる接続を無効にするとより安全になります。

{{< code title="/etc/ssh/sshd_config" >}}
PasswordAuthentication no           # パスワードによる接続を拒否
ChallengeResponseAuthentication no  # これもパスワード認証の一貫なので拒否
PubkeyAuthentication yes  # これはデフォルトで yes なのでそのままでも OK
{{< /code >}}

SSH 公開鍵の登録をする前にこの設定をしてしまうと、その後 SSH 接続できなくなってしまうので、設定変更後は別のターミナルで SSH 接続できることを確認してから、設定用のターミナルを閉じてください。
万が一、SSH 接続できなくなってしまった場合は、サーバーに直接モニタとキーボードを繋いで設定するか、VPS であれば管理用の Web コンソールなどから設定し直す必要があります。

