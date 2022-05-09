---
title: "WinSCP をコマンドラインから利用して2つのディレクトリを同期する"
url: "/p/eqrmt6x"
date: "2018-09-21"
tags: ["scp", "Windows"]
description: "Linux で2つのディレクトリを同期するには rsync コマンドが便利ですが、Windows ではフリーソフトの WinSCP を使用すると同様のことを行うことができます。"
---

WinSCP のインストール
----

WinSCP の本体は下記のサイトからダウンロードできます。

- [WinSCP :: Official Site :: Download](https://winscp.net/eng/download.php)

ここでは、コマンドラインから `winscp.exe` を使用しますので、インストーラを使ってインストールするときに、**カスタムインストール**を選択し、**インストールディレクトリを環境変数(%PATH%)に追加**を選択してください。


winscp コマンドでリモートホストへディレクトリをコピーする
----

ローカルディレクトリの内容を、リモートにあるディレクトリへ同期させるには、次のように `winscp.exe` の `/synchronize` オプションを使用します。

```
winscp [mysession] /synchronize [local_directory] [remote_directory]
```

例えば、`example.com` というホストに、ユーザ名 `user` で接続してローカルの `website` ディレクトリ内のファイルを `user` のホームディレクトリの `public_html` ディレクトリに同期させるには次のようにします。

{{< code title="例: ローカルの website をリモートの public_html へ" >}}
winscp user@example.com /synchronize "C:\Users\maku\website" "/user/maku/public_html"
{{< /code >}}


参考
----

- [WinSCP のドキュメント (Command-line Options)](https://winscp.net/eng/docs/commandline)

