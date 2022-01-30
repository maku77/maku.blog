---
title: "Linuxメモ: Netplan で Ubuntu のネットワーク設定を行う"
url: "/p/7q3dnx8"
date: "2022-01-20"
tags: ["Linux", "Ubuntu"]
---

Netplan とは
----

[Netplan](https://netplan.io) は Linux のネットワーク設定を簡潔な YAML ファイルで行う仕組みで、Ubuntu 18.04 以降（正確には 17 以降）で採用されています。
昔の Ubuntu/Debian で使用されていた `/etc/network/interfaces` という設定ファイルはもう使われていません。

実際のネットワーク管理は、バックエンドで動作する NetworkManager や systemd-networkd といったネットワークデーモンが行うのですが、Netplan は YAML 設定ファイルの内容を適切な形に変換して、これらのネットワークデーモンに渡してくれます。
つまり、バックエンドでどのようなネットワークデーモンが動いているかを意識せずに、統一されたわかりやすい YAML ファイルでネットワーク設定を行うことができます。

Netplan は次のように振る舞います。

1. システム起動時に Netplan が設定ファイル (`/etc/netplan/*.yaml`) を読み込んで、各ネットワークデーモン用の設定値を `/run` 以下へ書き出す
2. 各ネットワークデーモンが渡された情報に基づいてネットワーク設定を行う

{{< image w="300" title="netplan.io のサイトより" src="img-001.svg" >}}

バックエンドのネットワークデーモンとして何を使うかは、設定ファイルの `renderer` 部分で指定できるようになっていますが、デフォルトでは次のように動作します。

- Wifi や 無線 WAN/LTE の設定 ... NetworkManager に渡される
- それ以外のネットワーク（有線LANなど）の設定 ... networkd に渡される

よって、通常はどのネットワークデーモンを使うかを明示する必要はありません。


設定ファイルの読み込まれる順序
----

Netplan はすべての __`/etc/netplan/*.yaml`__ ファイルを読み込みます。
複数のファイルが存在する場合は、ファイル名のアルファベット順に読み込まれ、後に読み込まれたファイルの設定値が優先的に使用されます。
例えば、

- /etc/netplan/50-cloud-init.yaml
- /etc/netplan/99-custom.yaml

というファイルがある場合、`50-cloud-init.yaml` で設定された値は `99-custom.yaml` の設定値によって上書きされます。

ちなみに、Linux のインストール方法によって、最初から `50-cloud-init.yaml` といった設定ファイルが存在することがありますが、これらのファイルを編集してはいけません。
`50-cloud-init.yaml` は別のモジュール（この場合は cloud-init）が生成しているファイルであり、このファイルを編集しても、将来的に上書きされてしまう可能性があります。
そのため、ネットワーク設定をカスタマイズするときは、それらのファイルよりも後に読み込まれる `99-custom.yaml` といった別名の設定ファイルを作成します（名前は何でも OK です）。


設定例
----

Netplan の公式サイトで設定例や、各項目の意味を参照できます。

- [https://netplan.io/examples/ ... 設定例](https://netplan.io/examples/)
- [https://netplan.io/reference/ ... 各項目の意味](https://netplan.io/reference/)

例えば、次のような感じで設定します。

### 有線 LAN の eth0 に DHCP でアドレス割り当て

{{< code lang="yaml" title="/etc/netplan/99-custom.yaml" >}}
network:
  version: 2
  ethernets:
    eth0:
      dhcp4: true
      optional: true
{{< /code >}}

ルートプロパティの `network` と、その直下の `version` の指定はお決まりです。
有線 LAN などの設定は `ethernets` の下に記述していきます。

### Wi-Fi 接続の wlan0 に固定 IP アドレスを割り当て

{{< code lang="yaml" title="/etc/netplan/99-custom.yaml" >}}
network:
  version: 2
  wifis:
    wlan0:
      dhcp4: false
      access-points:
        "YOUR-SSID":
          password: "********"
      addresses: [192.168.1.20/24]
      nameservers:
        addresses: [192.168.1.1, 8.8.8.8]
      routes:
        - to: default
          via: 192.168.1.1
{{< /code >}}

この例では、無線 LAN で 192.168.1.20 という固定 IP アドレスを使用するよう設定しています。
DNS サーバーと、デフォルトゲートウェイには 192.168.1.1 を指定しています。

### 設定を反映する

YAML ファイルを編集したら、次のように実行してネットワーク設定を反映できます。

```console
$ sudo netplan apply
```

NetworkManager や networkd に設定が反映されるまで数秒かかります。

