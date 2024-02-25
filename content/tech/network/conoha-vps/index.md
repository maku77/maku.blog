---
title: "ConoHa VPS を借りて独自ドメインでアクセスできるようにする"
url: "p/n5emu3a/"
permalink: "p/n5emu3a/"
date: "2022-06-27"
tags: ["VPS"]
---

何をするか？
----

{{< mm/ad/conoha-vps-text "ConoHa の VPS" >}} を借りたので、そのときの設定手順をメモしておきます。
大体こんな感じのことをしています。

1. {{< mm/ad/conoha-vps-text "ConoHa VPS を契約" >}} して Ubuntu 22.04 を起動
2. {{< ad/onamae-domain-text "お名前.com で独自ドメインを取得" >}}
3. 独自ドメインで ConoHa VPS にアクセスできるようにする（IPv6 も有効化）

ConoHa VPS はリーズナブルかつ API でのサーバー管理ができたりして、徐々に人気が高まってきてるみたいです。
安いプランであれば月額数百円で借りることができます。
Docker コンテナやデータベース (RDB) をホストするサーバーが欲しかったのですが、AWS や Azure はプライベートで使うには高すぎるので、今回は ConoHa VPS を採用しました。

VPS にはグローバル IP アドレスが割り当てられるので独自ドメインは必須ではないですが、やはりドメイン名でアクセスできた方がよいので、いつも通り {{< ad/onamae-domain-text "お名前.com" >}} でドメイン取得しました。
ここでは、`example.com` というドメインを取得したものとして説明しています。


ConoHa のアカウントを作って VPS を契約
----

何はともあれ、まずは ConoHa のアカウントを作って {{< mm/ad/conoha-vps-text "ConoHa VPS" >}} を借ります。
ポチポチやってけば終わります。

{{< image border="true" src="img-001.png" title="ConoHa VPS での OS の選択" >}}

VPS の初期 OS として、__Ubuntu 22.04__ を選択しました。
上記は選択できる OS の一例ですが、他にもいろいろあります。


独自ドメインを取得する
----

ConoHa VPS に割り当てる独自ドメインを取得しておきます。
ドメインはどこで取得してもいいんですが、あまりいろいろな会社に散らばると面倒なので、わたしの場合はお名前.com に統一しています。
ConoHa でもドメイン取得できるので、そちらで取得しても大丈夫です。

- {{< ad/onamae-domain-text "ドメイン取るなら - お名前.com" >}}

ここでは、`example.com` というドメインを取得したとして説明していきます。
毎度のことなんですが、ドメイン名を考えるのが一番時間がかかりますねｗ
ドメインの更新料金は 1 年ごとに数千円程度ですが、1 年目は数百円だったりします。


DNS サーバーを設定する
----

独自ドメインを取得したら、そのドメイン名で ConoHa VPS のサーバーにアクセスできるようにします。
つまり、`example.com` から `160.xxx.xxx.xxx` のような IP アドレスを引けるようにします。

- 参考: [ドメイン管理と DNS 管理の違いを理解する](/p/bwamwgp/)

### ネームサーバーを変更する

お名前.com でドメインを取得すると、そのドメインの IP アドレスは次のような DNS サーバーで管理するよう初期設定されています。

- `dns1.onamae.com` / `dns2.onamae.com`

この状態で、Web ブラウザから独自ドメインにアクセス (`http://example.com`) すると、お名前.com の「ようこそ」的な Web ページが表示されるようになっています。
このままではダメなので、DNS サーバーを変更して、その DNS サーバーで正しい IP アドレスを関連づけてやります。
お名前.com が用意している次のような DNS サーバーを使うこともできますが、

- `01.dnsv.jp` / `02.dnsv.jp`

今回はせっかくなので、ConoHa が提供している DNS サーバーを使ってみます。
お名前.com Navi にログインして、次のように使用する DNS サーバーを変更します。

{{< image border="true" src="img-002.png" title="お名前.com のネームサーバー変更" >}}

1. `ネームサーバーの設定` → `ネームサーバーの設定` と選択する
2. 取得したドメイン名 (`example.com`) を選択する
3. `他のネームサーバーを利用` を選択して、次のような ConoHa 側の DNS サーバーを登録する
   - __`ns-a1.conoha.io`__
   - __`ns-a2.conoha.io`__
   - __`ns-a3.conoha.io`__

### DNS に IP アドレス情報を設定する

次に、[ConoHa 側のコントールパネル](https://manage.conoha.jp/Dashboard) で DNS サーバーの設定をします。
ここで、ドメイン名と IP アドレスを関連付けます。
まず、コントロールパネルから VPS サーバー情報を開いて、割り当てられた IPv4 アドレス（`160.xxx.xxx.xxx` など）を確認しておきます。

- [ConoHa コントロールパネル - サーバーリスト](https://manage.conoha.jp/Service/)

また、そこで IPv6 として割り当て可能なアドレスのリストも確認できるので、一番上のアドレス（`2400:8500:1801:XXX:XXX:XXX:XX:XX` など）とゲートウェイのアドレスを控えておきます。
VPS に最初から 17 個の IPv6 アドレスが割り当てられているように見えますが、それは罠で、自分で VPS に設定しないといけません（後述）。
IP アドレスがわかったら、DNS の設定画面を開いて、A レコード（IPv4 アドレス）と AAAA レコード（IPv6 アドレス）として登録します。

- [ConoHa コントロールパネル - サーバーリスト](https://manage.conoha.jp/DNS/)

{{< image border="true" src="img-003.png" title="DNS レコードの追加" >}}

1. 取得したドメイン (`example.com`) を追加
2. 編集ボタンを押して、A レコードとして IPv4 アドレスを追加します。
   - 例: 名称: `@`, TTL: `3600`, VALUE: `160.xxx.xxx.xxx`
3. IPv6 アドレスも割り当てたい場合は、AAAA レコードも追加します。
   - 例: 名称: `@`, TTL: `3600`, VALUE: `2400:8500:1801:XXX:XXX:XXX:XX:XX`

以上の設定を終えて 10 分くらい待つと、`ping examle.com` が通るようになります。
ただし、デフォルトでは VPS に IPv6 のアドレスは割り当てられていないので、VPS の設定を変更して IPv6 を有効化します。


VPS に IPv6 アドレスを設定する
----

ConoHa VPS に IPv6 のアドレスを割り当てる方法として、下記のマニュアルが用意されていますが、これは CentOS 用のものみたいです（2022年6月現在）。
Ubuntu では Netplan でネットワーク設定するので、そのやり方をメモっておきます。

- 参考: [ConoHa - ご利用ガイド IPv6 を VPS へ設定する](https://support.conoha.jp/v/setipv6/?btn_id=v-port-sidebar_v-setipv6)
- 参考: [Linuxメモ: Netplan で Ubuntu のネットワーク設定を行う](/p/7q3dnx8/)

以下のファイル編集作業は、次のように SSH 接続するか、ConoHa コントロールパネルからコンソールを開いて行います。

```console
$ ssh root@example.com
```

ConoHa VPS の Ubuntu 22.04 の初期設定では、Netplan の設定ファイルとして下記のファイルだけが入っています。
IPv4 は DHCP が有効になっていますが、うまく固定のアドレスが割り当てられているようです。

{{< code lang="yaml" title="vi /etc/netplan/10-gmovps.yaml" >}}
network:
    ethernets:
        eth0:
            addresses: []
            dhcp4: true
            dhcp6: false
            accept-ra: false
            optional: true
    version: 2
{{< /code >}}

このファイルを書き換えてもよいのですが、IPv6 設定用のファイルを別に用意することにします。

{{< code lang="yaml" title="vi /etc/netplan/99-custom.yaml" >}}
network:
    ethernets:
        eth0:
            addresses:
            - "2400:8500:XXXX:XXX:XXX:XXX:XX:XX/64"  # IPv6 アドレスを設定
            dhcp6: false
            accept-ra: false
            optional: true
            routes:
            - to: default
              via: "2400:8500:XXXX:XXX::1"  # デフォルトゲートウェイを設定
    version: 2
{{< /code >}}

あとは、次のように Netplan の設定を反映すれば、IPv6 の設定完了です。

```console
# netplan apply
```

[ip コマンド](/p/p7q7n4i/) で IPv6 アドレスやデフォルトゲートウェイが正しく割り当てられているか確認しておきます。

{{< code lang="console" title="IPv6 アドレスの確認" >}}
$ ip a show dev eth0
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 02:02:a0:fb:XX:XX brd ff:ff:ff:ff:ff:ff
    altname enp0s3
    altname ens3
    inet 160.XXX.XXX.XXX/23 metric 100 brd 160.XXX.XXX.255 scope global dynamic eth0
       valid_lft 5183492sec preferred_lft 5183492sec
    inet6 2400:8500:XXXX:XXX:XXX:XXX:XX:XX/64 scope global
       valid_lft forever preferred_lft forever
    inet6 fe80::2:a0ff:fefb:330a/64 scope link
       valid_lft forever preferred_lft forever
{{< /code >}}

{{< code lang="console" title="デフォルトゲートウェイの確認" >}}
$ ip -6 route
::1 dev lo proto kernel metric 256 pref medium
2400:8500:XXXX:XXX::/64 dev eth0 proto kernel metric 256 pref medium
fe80::/64 dev eth0 proto kernel metric 256 pref medium
default via 2400:8500:XXXX:XXX::1 dev eth0 proto static metric 1024 pref medium
{{< /code >}}

IPv6 で外部サーバー (google.com) に `ping6` できることも確認しておきます。

```console
$ ping6 -c 3 -I eth0 google.com
PING google.com(nrt12s30-in-x0e.1e100.net (2404:6800:4004:812::200e)) from 2400:8500:XXXX:XXX:XXX:XXX:XX:XX eth0: 56 data bytes
64 bytes from nrt12s30-in-x0e.1e100.net (2404:6800:4004:812::200e): icmp_seq=1 ttl=56 time=1.03 ms
64 bytes from nrt12s30-in-x0e.1e100.net (2404:6800:4004:812::200e): icmp_seq=2 ttl=56 time=0.838 ms
64 bytes from nrt12s30-in-x0e.1e100.net (2404:6800:4004:812::200e): icmp_seq=3 ttl=56 time=0.833 ms
```

逆に、手元の PC から ConoHa VPS に `ping6` できることも確認しておきます。

```console
$ ping6 example.com
...（省略）...
```

これで、独自ドメインで ConoHa VPS にアクセスできるようになりました！


ConoHa VPS 広告
----

最後に、お得な広告を貼っておきます（＾＾

- __ConoHa VPS__ の申し込みはこちらから → {{< mm/ad/conoha-vps-banner >}}
- お名前.com での __ドメイン取得__ はこちらから → {{< ad/onamae-domain-banner >}}

