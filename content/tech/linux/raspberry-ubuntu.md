---
title: "Raspberry Pi に Ubuntu 20.04 を入れて WiFi 接続できるようにする"
url: "/p/n9kv7gq"
date: "2022-01-20"
tags: ["RaspberryPi"]
---

ひょんなことから、Ubuntu Server の入った Raspberry Pi 4 が必要になったので、セットアップ手順をメモしておきます。
なんか昔より簡単にインストールできるようになっててビックリしました。


何をするか？
----

ここでは、Raspberry Pi 4 Model B に Ubuntu 20.04 をインストールして、Wi-Fi 経由の SSH ログインができるところまでセットアップします。
なお、有線 LAN では一度も接続せずに Wi-Fi 接続できるようにします。
ただし、初期設定のために HDMI でのディスプレイ出力は必要になります。

- 準備するもの
    1. Raspberry Pi 4 （たぶん別のバージョンでもOK）
    2. SD カードに OS をインストールするための PC
    3. HDMI ケーブルとディスプレイと USB キーボード


SD カードに OS をインストール
----

公式の [Raspberry Pi Imager](https://www.raspberrypi.com/software/) というソフトウェアを使うと、ものすごく簡単に SD カードに各種 OS をインストールできます。
Raspberry Pi Imager を起動したら、次のようにポチポチやっていくだけでインストール完了です。

1. Operating System の項目で、`Other general purpose OS` → `Ubuntu` → `Ubuntu Server 20.04 LTS (64-bit server OS)` のように選択
2. Storage の項目で、書き込み対象の SD カードを選択
3. WRITE ボタンを押してしばらく待つ

あとは、この SD カードを Raspberry Pi に挿入して電源を入れれば Ubuntu OS が起動します。
お手軽すぎる。。。


Wi-Fi（無線LAN）に接続する
----

Raspberry Pi を HDMI ケーブルでディスプレイに接続して電源を入れます。
Ubuntu が起動するので、初期ユーザー __`ubuntu`__、初期パスワード __`ubuntu`__ でログインします。

```
ubuntu login: ubuntu
Password: ******
```

初回ログイン時にパスワードの変更を促されるので、適当なパスワードを設定します。

Ubuntu のネットワーク設定は [Netplan の仕組み](/p/7q3dnx8) で行われるため、そのための YAML ファイルを作成します。

```console
$ sudo vim /etc/netplan/99-custom.yaml
```

{{< code lang="yaml" title="/etc/netplan/99-custom.yaml" >}}
network:
  version: 2
  wifis:
    wlan0:
      dhcp4: true
      access-points:
        "<SSID>":
          password: "********"
{{< /code >}}

設定ファイル中の SSID とパスワードの部分は、実際に使用している Wi-Fi アクセスポイントのものに置き換えてください。
YAML ファイルを作成したら、次のようにしてネットワーク設定を反映します。

```console
$ sudo netplan apply
```

数秒待てば Wi-Fi 接続できるようになっているはずなので、`apt` で各種パッケージを更新しておきます。

```console
$ sudo apt update
$ sudo apt upgrade
...しばらく待つ...
```

大量のパッケージが更新されるので、ここでいったん再起動しておくとよいかもです。

```console
$ sudo shutdown -r now
```


SSH 接続できるようにする
----

Raspberry Pi 上で SSH サーバーを起動しておけば、ノート PC などからリモート接続できるようになります。

次のようにして、`apt` で SSH サーバー (__`openssh-server`__) をインストールして起動します。
ついでに IP アドレス確認用の `ifconfig` コマンドなどを使えるように、__`net-tools`__ パッケージもインストールしておきます。

```console
$ sudo apt install openssh-server
$ sudo apt install net-tools
$ sudo systemctl start ssh  # SSHサーバーを起動
```

SSH サーバーが起動したら、`ifconfig` コマンドあるいは `ip a` コマンドで IP アドレスを確認します。

```console
$ ifconfig
```

あとは、この IP アドレスに対して、ノート PC などから SSH 接続できれば成功です。
macOS などであれば、`ssh` コマンドは最初からインストールされています。

```console
$ ssh ubuntu@192.168.1.14
```

この時点で Raspberry Pi をリモート操作できるようになったので、ディスプレイは必要なくなります。


固定 IP アドレスにしておく
----

前述のネットワーク設定では DHCP で IP アドレスを割り当てるようにしましたが、SSH 接続での使用を前提とするなら、固定の IP アドレスを割り当てておいた方がよさそうです。
Netplan の設定ファイルで固定 IP アドレスを指定する場合、DHCP を無効 (`dhcp4: false`) にし、代わりに次のような項目を設定します。

- IP アドレス (`addresses`)
- DNS サーバー (`nameservers`)
- デフォルトゲートウェイ (`routes`)

次の例では、固定 IP アドレスとして 192.168.1.20、DNS サーバーとデフォルトゲートウェイとして 192.168.1.1 を指定しています。
念の為、セカンダリ DNS に Google Public DNS (8.8.8.8) を指定してあります。

{{< code lang="yaml" title="/etc/netplan/99-custom.yaml" >}}
network:
  version: 2
  wifis:
    wlan0:
      dhcp4: no
      access-points:
        "<SSID>":
          password: "********"
      addresses: [192.168.1.20/24]
      nameservers:
        addresses: [192.168.1.1, 8.8.8.8]
      routes:
        - to: default
          via: 192.168.1.1
{{< /code >}}

次のようにネットワーク設定を反映して完了です。

```console
$ sudo netplan apply
```

IP アドレスを変更した場合は、ここで SSH 接続が切れるので、新しい IP アドレスで接続しなおします。

```console
$ ssh ubuntu@192.168.1.20
```

