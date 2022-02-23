---
title: "Linuxメモ: Bluetooth 関連コマンドのメモ (hciconfig, bluetoothctl)"
url: "/p/ter3doz"
date: "2022-02-20"
tags: ["Bluetooth", "Linux"]
---

Bluetooth デバイスの情報を表示する (hciconfig)
----

__`hciconfig`__ は、Bluetooth デバイスの情報表示や設定を行うためのコマンドです。
HCI プロトコルで Bluetooth コントローラーと通信します。
`hciconfig` コマンドが見つからない場合は、__`bluez`__ パッケージをインストールします。

{{< code lang="console" title="Ubuntu への bluez パッケージのインストール" >}}
$ apt update && apt install -y bluez
{{< /code >}}

Bluetooth のデバイス名には `hci0` や `hci1` といった名前が付けられ、`hciconfig` コマンドでそれらの一覧を表示することができます。
次の実行例は、Raspberry Pi 4 に USB Bluetooth ドングルを接続した状態で `hciconfig` コマンドを実行したときの結果です（アドレスは一部伏せてます）。

```console
$ hciconfig
hci1:   Type: Primary  Bus: USB
        BD Address: 00:E0:4C:XX:XX:XX  ACL MTU: 1021:6  SCO MTU: 255:12
        UP RUNNING
        RX bytes:2186 acl:0 sco:0 events:105 errors:0
        TX bytes:12322 acl:0 sco:0 commands:105 errors:0

hci0:   Type: Primary  Bus: UART
        BD Address: DC:A6:32:XX:XX:XX  ACL MTU: 1021:8  SCO MTU: 64:1
        UP RUNNING
        RX bytes:1514 acl:0 sco:0 events:90 errors:0
        TX bytes:2061 acl:0 sco:0 commands:90 errors:0
```

USB ドングルの Bluetooth デバイスが `hci1` (Bus: USB)、Raspberry Pi 4 組み込みの Bluetooth デバイスが `hci0` (Bus: UART) として認識されているのが分かります。
`hciconfig` コマンドに `-a (--all)` オプションをつけて実行すると、詳細情報を表示できます。

```console
$ hciconfig -a
hci1:   Type: Primary  Bus: USB
        BD Address: 00:E0:4C:XX:XX:XX  ACL MTU: 1021:6  SCO MTU: 255:12
        UP RUNNING
        RX bytes:1909 acl:0 sco:0 events:102 errors:0
        TX bytes:12313 acl:0 sco:0 commands:102 errors:0
        Features: 0xff 0xff 0xff 0xfe 0xdb 0xfd 0x7b 0x87
        Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
        Link policy: RSWITCH HOLD SNIFF PARK
        Link mode: SLAVE ACCEPT
        Name: 'RTK_BT_5.0'
        Class: 0x000000
        Service Classes: Unspecified
        Device Class: Miscellaneous,
        HCI Version: 5.1 (0xa)  Revision: 0x97b
        LMP Version: 5.1 (0xa)  Subversion: 0xec43
        Manufacturer: Realtek Semiconductor Corporation (93)

hci0:   Type: Primary  Bus: UART
        BD Address: DC:A6:32:XX:XX:XX  ACL MTU: 1021:8  SCO MTU: 64:1
        UP RUNNING
        RX bytes:1514 acl:0 sco:0 events:90 errors:0
        TX bytes:2061 acl:0 sco:0 commands:90 errors:0
        Features: 0xbf 0xfe 0xcf 0xfe 0xdb 0xff 0x7b 0x87
        Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
        Link policy: RSWITCH SNIFF
        Link mode: SLAVE ACCEPT
        Name: 'ubuntu'
        Class: 0x000000
        Service Classes: Unspecified
        Device Class: Miscellaneous,
        HCI Version: 5.0 (0x9)  Revision: 0x156
        LMP Version: 5.0 (0x9)  Subversion: 0x6119
        Manufacturer: Cypress Semiconductor Corporation (305)
```


Bluetooth デバイスをスキャンする (bluetoothctl)
----

{{% private %}}
- ここ詳しい: https://zenn.dev/noraworld/articles/bluetoothctl-commands
{{% /private %}}

__`bluetoothctl`__ コマンドを使うと、Bluetooth デバイスの検出やペアリングを行うことができます。

`bluetoothctl` を起動して、その中で __`scan on`__ サブコマンドを実行すると、周辺の Bluetooth デバイスを検出できます。
`exit` コマンドで終了できます。

```console
$ bluetoothctl
Agent registered
[CHG] Controller DC:A6:32:XX:XX:XX Pairable: yes
[bluetooth]# scan on
Discovery started
[CHG] Controller DC:A6:32:XX:XX:XX Discovering: yes
[NEW] Device 7B:D4:F8:XX:XX:XX iPhone
[NEW] Device A4:83:E7:XX:XX:XX BRAVIA
[NEW] Device 27:7D:7C:XX:XX:XX 27-7D-7C-XX-XX-XX
[NEW] Device 52:9A:7B:XX:XX:XX 52-9A-7B-XX-XX-XX
...
[bluetooth]# exit
```

次のように直接サブコマンド (`scan on`) を指定することも可能です。

```console
$ bluetoothctl scan on
```

`discoverable on` サブコマンドを実行すると、他のデバイスから Bluetooth デバイスを発見できるようになります。
`discoverable-timeout <SECONDS>` で指定した秒数（デフォルトは 180 秒）が経過すると、自動的に `discoverable off` されるので注意してください。

```
$ bluetoothctl discoverable on
Changing discoverable on succeeded
```


USB Bluetooth 5.0 ドングル (Realtek RTL8761B) を認識させる
----

Ubuntu Server 20.04 に Bluetooth 5.0 対応の USB ドングル (Realtek RTL8761B) を接続してもうまく認識されませんでした。
`hciconfig` コマンドを実行すると、下記のように `hci1` というデバイス名は出てくるけど、`BD Address` は認識されておらず、ステータスも `DOWN` 状態。

```config
$ hciconfig -a
hci1:   Type: Primary  Bus: USB
        BD Address: 00:00:00:00:00:00  ACL MTU: 0:0  SCO MTU: 0:0
        DOWN
        RX bytes:42 acl:0 sco:0 events:4 errors:0
        TX bytes:12 acl:0 sco:0 commands:4 errors:0
        Features: 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
        Packet type: DM1 DH1 HV1
        Link policy:
        Link mode: SLAVE ACCEPT
```

`hciconfig up` しようとすると、ファイルが見つからないというエラー。

```console
$ sudo hciconfig hci1 up
Can't init device hci1: No such file or directory (2)
```

`dmesg` でカーネルログを参照すると、どうも Realtek のファームウェアファイルが足りていないみたいです。

```console
$ dmesg | grep hci1
...
[ 5013.848992] Bluetooth: hci1: RTL: firmware file rtl_bt/rtl8761b_fw.bin not found
```

`rtl8761b_fw.bin` ファイルの取得方法は、下記に記述されていました。

- 参考: [Realtek RTL8761B - LinuxReviews](https://linuxreviews.org/Realtek_RTL8761B)
    - ダウンロードするファイル ... [rtl8761b_config](https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_config) / [rtl8761b_fw](https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_fw)

まず、上記のファイルをカレントディレクトリにダウンロードします。

```console
$ curl -O https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_config
$ curl -O https://raw.githubusercontent.com/Realtek-OpenSource/android_hardware_realtek/rtk1395/bt/rtkbt/Firmware/BT/rtl8761b_fw
```

ファイル名の末尾に __`.bin`__ という拡張子がついていないので、リネームしてから `/lib/firmware/rtl_bt` ディレクトリにコピーします。

```console
$ mv rtl8761b_config rtl8761b_config.bin
$ mv rtl8761b_fw rtl8761b_fw.bin
$ sudo cp rtl8761b_config.bin /lib/firmware/rtl_bt
$ sudo cp rtl8761b_fw.bin /lib/firmware/rtl_bt
```

これで、`hciconfig up` できるようになります。

```console
$ sudo hciconfig hci1 up
```

BD Address もちゃんと認識されて、`UP RUNNING` 状態になりました。めでたしめでたし。

```console
$ hciconfig -a hci1
hci1:   Type: Primary  Bus: USB
        BD Address: 00:E0:4C:XX:XX:XX  ACL MTU: 1021:6  SCO MTU: 255:12
        UP RUNNING
        RX bytes:1909 acl:0 sco:0 events:102 errors:0
        TX bytes:12313 acl:0 sco:0 commands:102 errors:0
        Features: 0xff 0xff 0xff 0xfe 0xdb 0xfd 0x7b 0x87
        Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
        Link policy: RSWITCH HOLD SNIFF PARK
        Link mode: SLAVE ACCEPT
        Name: 'RTK_BT_5.0'
        Class: 0x000000
        Service Classes: Unspecified
        Device Class: Miscellaneous,
        HCI Version: 5.1 (0xa)  Revision: 0x97b
        LMP Version: 5.1 (0xa)  Subversion: 0xec43
        Manufacturer: Realtek Semiconductor Corporation (93)
```

