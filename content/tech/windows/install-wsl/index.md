---
title: "WSL (Windows Subsystem for Linux) をインストールする"
url: "/p/sorrvcb"
date: "2020-04-15"
description: "かつて Windows 上で Linux ぽい環境を構築したい場合は Cygwin を使っていましたが、Windows 10 では WSL を使ってほぼ Linux な環境を簡単に立ち上げることができます。Windows 上で tmux などのターミナルを使いたいときにも WSL を利用できます。"
tags: ["Windows", "WSL"]
image: "img-001.png"
---

{{< image w="600" src="img-001.png" >}}

WSL 本体のインストール
----

{{< image w="400" src="img-002.png" >}}

まず、コントロールパネルから `Windows の機能の有効化または無効化` を起動し、__`Windows Subsystem for Linux`__ にチェックを入れて WSL 本体をインストールします。


Linux ディストリビューションのインストール
----

### Microsoft ストアからのインストール

{{< image w="600" src="img-003.png" >}}

WSL 本体のインストールができたら、 [Microsoft ストア](https://www.microsoft.com/ja-jp/store/apps) から Ubuntu や [Debian](https://www.microsoft.com/ja-jp/p/debian/9msvkqc78pk6) などのディストリビューションをインストールします。
ここでは、Ubuntu よりもサイズの小さい、Debian GNU Linux をインストールしてみました（Debian も `apt` で各種コマンドを追加インストールしていくことができます）。

インストールが完了すると、Windows メニューから Debian などのターミナルを起動できるようになります。

{{< image w="600" src="img-001.png" >}}

### マニュアルインストール

会社の PC などで、Windows ストアの利用が制限されている場合は、下記サイトからディストリビューションをダウンロードして手動インストールすることができます。

- [Linux 用 Windows サブシステム (WSL) ディストリビューションを手動でダウンロードする | Microsoft Docs](https://docs.microsoft.com/ja-jp/windows/wsl/install-manual)

