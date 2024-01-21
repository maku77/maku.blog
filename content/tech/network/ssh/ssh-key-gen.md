---
title: "SSH キーの管理: SSH キーを作成する (ssh-keygen)"
url: "p/ftducs9/"
date: "2008-08-13"
lastmod: "2022-11-25"
tags: ["SSH"]
weight: 1
---

SSH (Secure Shell) によるマシン間接続を有効にするには、接続元のマシンで SSH のキーペア（秘密鍵＋公開鍵）を作成し、接続先のマシンに公開鍵を登録する必要があります。
ここでは OpenSSH の `ssh-keygen` コマンドを使用した SSH キーの作成方法を紹介します。

SSH 鍵の作成
----

__`ssh-keygen`__ コマンドをオプションなしで実行すると、SSH キー（秘密鍵＋公開鍵）を作成できます（SSH プロトコル ver.2 用）。
デフォルトでは暗号アルゴリズムとして RSA を使用するキーが生成されますが、__`-t`__ オプションで暗号アルゴリズムを指定することができます。

{{< code lang="console" title="SSH キーペアの生成" >}}
$ ssh-keygen -t ed25519  # Ed25519 鍵を作成する場合
{{< /code >}}

| `-t` オプション | アルゴリズム | 説明 |
| ---- | ---- | ---- |
| `rsa` | RSA 鍵 | 4096 bits の長い鍵長を選べば強度は出るが遅くなる |
| `ecdsa`<br>__★おすすめ2__ | ECDSA 鍵 | 楕円曲線暗号。少ない鍵長で強度あり。ED25519 より普及している |
| `ed25519`<br>__★おすすめ1__ | Ed25519 鍵 | 楕円曲線暗号。少ない鍵長で強度あり。ECDSA より高速 |
| `dsa` | DSA 鍵 | （非推奨）SSH2 の環境で使えるが、1024 bits しか選べないことが多い |
| `rsa1` | RSA1 鍵 | （非推奨）SSH1 のレガシー環境用。強度が弱いので使わない方がよい |


実際に `ssh-keygen` コマンドを実行すると、パスワード (passphrase) の入力を求められるので、秘密鍵を使用する際のパスワードを設定しておきます。
このパスワードは、`ssh` コマンドでリモートホストに接続する際に入力することになります。
パスワード入力時に、何も入力せずに <key>Enter</key> を入力すれば、パスワードなしの SSH キーを生成できますが、通常はパスワードを設定してください。

{{< code lang="console" title="SSH キーを作成してみる（デフォルトの RSA で）" >}}
$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/Users/maku/.ssh/id_rsa): （出力するファイル名）
Created directory '/Users/maku/.ssh'.
Enter passphrase (empty for no passphrase): （秘密鍵のパスワードを入力）
Enter same passphrase again: （秘密鍵のパスワードをもう一度入力）
Your identification has been saved in /Users/maku/.ssh/id_rsa
Your public key has been saved in /Users/maku/.ssh/id_rsa.pub
The key fingerprint is:
SHA256:4qBU6wehiisfj5V686F2cQLCQ3Dws8dRnVG6C02xyxA maku@macbook.local
The key's randomart image is:
+---[RSA 3072]----+
|oo.   E.o+.      |
| o.  . .o+       |
| oo + . +        |
|  +*.+ = o       |
|  +o*.o S        |
|.o + =oo..       |
|o o + ++.        |
|.. *+o..         |
|o.+oo+.          |
+----[SHA256]-----+
{{< /code >}}

例えば、上記のようにデフォルトの RSA 鍵を生成した場合は、次の 2 つのファイルが __`~/.ssh`__ ディレクトリに保存されます（ディレクトリが存在しなければ自動的に作成されます）。

- __`~/.ssh/id_rsa`__ ... 秘密鍵 (private key)
- __`~/.ssh/id_rsa.pub`__ ... 公開鍵 (public key)

公開鍵 (`id_rsa.pub`) は接続先ホストに登録する情報で、秘密鍵 (`id_rsa`) は SSH 接続時にクライアント側で使うファイルです。

{{% note title="パスワードなしの SSH キー" %}}
例外的に、パスワードを設定しない SSH キーを生成する場合もあります。
例えば、GitHub Actions などの CI/CD 環境や、Ansible などの自動設定環境で使用する SSH キーの場合は、対話的にパスワードを入力できないことがあるので、パスワードなしの SSH キーを使ったりします。
ただし、そのように作成した SSH キー（秘密鍵）は、ファイルさえ手に入れれば誰でも使えてしまうため、厳重に管理する必要があります。
{{% /note %}}


接続先への公開鍵の登録
----

SSH のキーペアを作成したら、公開鍵ファイルの内容を接続先ホストの `~/.ssh/authorized_keys` ファイルに登録することで、この PC から SSH 接続が可能になります。
この登録作業は、__`ssh-id-copy`__ コマンドを使うと簡単に済ませることができます（参考: [ssh-id-copy で SSH の公開鍵をリモートホストに登録する](/p/2mzbmw8/)）。

{{< code lang="console" title="接続先に SSH 公開鍵を登録する" >}}
$ ssh-copy-id -i ~/.ssh/id_rsa.pub maku@192.168.1.20
maku@192.168.1.20's password: （接続先ホストのユーザー maku のパスワードを入力）
{{< /code >}}

これで、SSH キーを使った接続が可能になります。

{{< code lang="console" title="SSH キーを使って SSH 接続する" >}}
$ ssh maku@192.168.1.20
Enter passphrase for key '/Users/maku/.ssh/id_rsa': （SSH 秘密鍵のパスフレーズを入力）
{{< /code >}}


どのタイプの SSH キーを作成すべきか？
----

パフォーマンスや安全度を考慮すると、楕円曲線暗号の __Ed25519 アルゴリズム (`-t ed25519`)__、あるいは、__ECDSA アルゴリズム (`-t ecdsa`)__ を使用しておくのがよさそうです（2022 年現在）。
可能であれば、ECDSA よりも高速な Ed25519 を使うのがよいです。

```console
$ ssh-keygen -t ed25519
```

GitHub の公式ドキュメントでも、SSH 鍵の作成方法として、[Ed25519 鍵の作成例](https://docs.github.com/ja/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) が示されています（2022 年時点）。

{{< code lang="console" title="GitHub 推奨の方法" >}}
$ ssh-keygen -t ed25519 -C "your_email@example.com"
{{< /code >}}

昔は、RSA 4096 bits の作成例が示されていましたが（2016 年時点）、現状は上記の方法で作成した方がパフォーマンスがよくなります。

{{< code lang="console" title="古い方法" >}}
$ ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
{{< /code >}}

米国 NIST の提示している暗号化方式としては、2031 年以降は下記のものしか使えなくなるとあります（参考: [米国における暗号技術をめぐる動向（2016年10月）: 図表10](https://www.ipa.go.jp/files/000055177.pdf)）。

- RSA/DSA 3072 bit 以上
- ECDSA 256 bit 以上

