---
title: "scp コマンドでファイルやディレクトリを転送する"
url: "/p/buvv4fz"
date: "2013-11-09"
tags: ["scp", "コマンド"]
---

ここでは、`scp` を使ったファイル転送の方法を示していますが、ディレクトリ単位のファイルコピーやバックアップを行いたい場合は、[rsync](/p/c3s7wyx) コマンドを使用したほうが効率的です。


ファイル送信
----

```
$ scp <localFile> <user>@<host>:<remoteFile>
```

ファイル受信
----

```
$ scp <user>@<host>:<remoteFile> <localFile>
```

{{< code title="例: リモートのホームディレクトリ内のファイルを取得" >}}
$ scp maku@maku.example.com:hello.txt hello.txt
{{< /code >}}

remoteFile の指定は、ホームディレクトリからの相対パスで OK です。


ディレクトリ送信
----

```
$ scp -r <localDir> <user>@<host>:<remoteDir>
```

ディレクトリ受信
----

```
$ scp -r <user>@<host>:<remoteDir> <localDir>
```

{{< code title="例: ホームディレクトリの sample ディレクトリを丸ごとダウンロード" >}}
$ scp -r maku@maku.example.com:sample .
{{< /code >}}

