---
title: "macOS で昔ながらの ftp コマンドを使用できるようにする"
url: "p/4fsvwuc/"
date: "2018-02-19"
tags: ["mac"]
---

Mac High Sierra からは、`ftp` コマンドは廃止され、よりセキュアな `sftp` コマンドしか使用することができません。
`sftp` コマンドでファイル転送するには、サーバ側で sftp プロトコルに対応している必要があります。

下記のように `tnftp` をインストールすれば、昔ながらの `ftp` コマンドを使用できるようになります。

```console
$ brew install tnftp
```

```console
$ ftp user@ftp.example.com
```

ちなみに、次のようにすれば、FTP コマンドを列挙したスクリプトを実行することができます（`-n` オプションで、ログインプロンプトの表示を抑制する必要があります）。

```console
$ ftp -n < upload.ftp
```

{{< code title="upload.ftp" >}}
open ftp.example.com
user username password
bin
prompt
cd hoge
put sample.txt
quit
{{< /code >}}

