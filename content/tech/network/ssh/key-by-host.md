---
title: "SSH キーの管理: SSH の接続先ごとにキーを使い分ける (~/.ssh/config)"
url: "p/szajt4d/"
date: "2022-07-17"
tags: ["SSH"]
---

何をするか？
----

SSH 鍵を使って複数の SSH サーバーに接続する場合、1 つの SSH 鍵ファイルを使い回すこともできますが、場合によっては SSH サーバーごと（SSH ユーザーごと）に鍵ファイルを使い分けたいこともあるかもしれません。
ここでは、SSH サーバーごとの SSH キーを用意する方法を説明します。


鍵ファイルを作成する
----

例えば、{{< mm/ad/conoha-vps-text "ConoHa VPS" >}} に `maku` というユーザーで SSH 接続するための SSH キーを作成するとします。
一般的に SSH のキーペアは `~/.ssh` ディレクトリ以下に保存しますが、次のような感じで接続先ごとにディレクトリ (`<接続先>-<ユーザー>`) を作ると管理しやすくなります（ファイル名で表現することもできます）。

- `~/.ssh/conoha-maku/id_rsa` ... 秘密鍵
- `~/.ssh/conoha-maku/id_rsa.pub` ... 公開鍵

```console
# SSH キーペア用のディレクトリを作成
$ mkdir -p -m 0700 ~/.ssh/conoha-maku

# SSH キーペアを作成
$ ssh-keygen -t rsa -f ~/.ssh/conoha-maku/id_rsa
```


公開鍵を SSH サーバーに登録
----

SSH キーで SSH 接続するには、接続先の SSH サーバーの `~/.ssh/authorized_keys` に公開鍵を登録しておく必要があります。
この作業は、`ssh-copy-id` コマンドを使うと簡単に済ませられます（参考: [ssh-id-copy で SSH の公開鍵をリモートホストに登録する](/p/2mzbmw8/)）。

```console
$ ssh-copy-id -i ~/.ssh/conoha-maku/id_rsa.pub maku@xxx.xxx.xxx.xxx
```

`xxx.xxx.xxx.xxx` は接続先のアドレスを示しています。
これで、次のように SSH キーで接続できるようになります。

```console
$ ssh -i ~/.ssh/conoha-maku/id_rsa maku@xxx.xxx.xxx.xxx
```


接続先ごとに使用する SSH キーを切り替える
----

ここまでの設定で、SSH キーを使った接続はできるようになっているのですが、毎回秘密鍵のパスやユーザー名などを入力するのは面倒です。
このような場合は、__`~/.ssh/config`__ ファイルを作って、エイリアス名を使って SSH 接続できるようにしておくと便利です（Windows の場合は __`%USERPROFILE%\.ssh\config`__ です）。

{{< code title="~/.ssh/config" >}}
Host conoha-maku
    Hostname xxx.xxx.xxx.xxx
    User maku
    Port 22
    IdentityFile ~/.ssh/conoha-maku/id_rsa
{{< /code >}}

上記のように設定しておくと、次のようなコマンドで簡単に SSH 接続できるようになります。

```console
$ ssh conoha-maku
```

あとは、同様の手順を接続先の SSH サーバーごとに繰り返せば OK です。
`~/.ssh/config` の設定方法の詳細は、`man ssh_config` で確認できます。

{{% note title="おすすめのエイリアス名" %}}
`Host` 行のエイリアス名には任意の名前を設定できますが、__`<ホスト名>-<ユーザー名>`__ のような感じにしておくと分かりやすいです。
ここでは `conoha-maku` という名前を付けました。

次のように、スペース区切りで複数のエイリアス名を設定しておくこともできます。

```
Host conoha-maku conoha
```
{{% /note %}}

{{% note title="Dropbox で SSH キーを共有" %}}
セキュリティ的に微妙かもしれませんが、Dropbox などで SSH キーを管理しておくと、いろんな環境から SSH 接続できるようになるので便利です。

```
IdentityFile ~/Dropbox/share/appdata/ssh/conoha-maku/id_rsa
```
{{% /note %}}


（おまけ）シェルの alias じゃダメなの？
----

bash の `alias` などで、次のように定義しておけば同じことが簡単に実現できるのでは？と思うかもしれません。

{{< code lang="bash" title="~/.bash_profile" >}}
alias ssh-conoha='ssh -i ~/.ssh/conoha-maku/id_rsa maku@xxx.xxx.xxx.xxx'
{{< /code >}}

これでもよいのですが、`~/.ssh/config` で接続設定をしておけば、内部的に SSH を使ったツールでもその設定を利用できます。
例えば、下記の `config` ファイルは GitHub 用の接続設定です（GitHub に SSH 接続するときのユーザー名は必ず `git` になることに注意）。

{{< code title="~/.ssh/config" >}}
Host github-maku77
    HostName github.com
    User git
    IdentityFile ~/.ssh/github-maku77/id_rsa
{{< /code >}}

この設定を、Git の設定ファイルから次のように参照することができます。

{{< code lang="ini" title="example-repo/.git/config（抜粋）" >}}
[remote "origin"]
    url = github-maku77:maku77/example-repo.git
{{< /code >}}

