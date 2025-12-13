---
title: "ドラクエ3で種狩り自動化"
url: "p/w3vv3uc/"
date: "2024-12-21"
tags: ["日記"]
draft: true
---

- 用意するもの
  - Raspberry Pi 3 Model B
  - USB Bluetooth アダプター (Realtek RTL8761B)
- 使うソフトウェア
  - [NXBT](https://github.com/Brikwerk/nxbt/) ... 本体


Raspberry Pi に OS をインストール
----

Raspberry Pi に Raspberry Pi OS (64-bit) をインストールします。
公式の [Imager](https://www.raspberrypi.com/software/) を使ってポチポチやれば Micro SD カードにインストールできます。
設定画面で、ユーザーや Wi-Fi の情報を入力しておくと、起動直後に Wi-Fi で SSH 接続できる状態になるので便利です。


Bluetooth アダプターを認識させる
----

Raspberry Pi に USB Bluetooth アダプターを接続して、認識されたことを確認しておきます。
**`lsusb`** コマンドで、認識されている USB デバイスの一覧を確認できます。

{{< code lang="console" hl_lines="2" >}}
$ lsusb
Bus 001 Device 008: ID 0bda:8771 Realtek Semiconductor Corp. Bluetooth Radio
Bus 001 Device 005: ID 0424:7800 Microchip Technology, Inc. (formerly SMSC)
Bus 001 Device 003: ID 0424:2514 Microchip Technology, Inc. (formerly SMSC) USB 2.0 Hub
Bus 001 Device 002: ID 0424:2514 Microchip Technology, Inc. (formerly SMSC) USB 2.0 Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
{{< /code >}}

次に、その USB Bluetooth アダプターが実際に Bluetooth デバイスとして動作しているかを **`hciconfig`** コマンドで確認します。

{{< code lang="console" hl_lines="4" >}}
$ hciconfig -a
hci1:	Type: Primary  Bus: USB
	BD Address: 00:E0:4C:72:F9:03  ACL MTU: 1021:6  SCO MTU: 255:12
	DOWN
	RX bytes:1537 acl:0 sco:0 events:162 errors:0
	TX bytes:31106 acl:0 sco:0 commands:162 errors:0
	Features: 0xff 0xff 0xff 0xfe 0xdb 0xfd 0x7b 0x87
	Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
	Link policy: RSWITCH HOLD SNIFF PARK
	Link mode: PERIPHERAL ACCEPT

hci0:	Type: Primary  Bus: UART
	BD Address: B8:27:EB:A0:04:72  ACL MTU: 1021:8  SCO MTU: 64:1
	UP RUNNING
	RX bytes:4762 acl:0 sco:0 events:439 errors:0
	TX bytes:68184 acl:0 sco:0 commands:439 errors:0
	Features: 0xbf 0xfe 0xcf 0xfe 0xdb 0xff 0x7b 0x87
	Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
	Link policy: RSWITCH SNIFF
	Link mode: PERIPHERAL ACCEPT
	Name: 'raspberrypi'
	Class: 0x6c0000
	Service Classes: Rendering, Capturing, Audio, Telephony
	Device Class: Miscellaneous,
	HCI Version: 5.0 (0x9)  Revision: 0x17e
	LMP Version: 5.0 (0x9)  Subversion: 0x6119
	Manufacturer: Cypress Semiconductor (305)
{{< /code >}}

USB Bluetooth アダプターは `hci1` として認識されていますが、状態が `DOWN` になっているので起動します。

```console
$ sudo hciconfig hci1 up
Can't init device hci1: Operation not possible due to RF-kill (132)
```

RF-kill (電波を出さない状態) によってブロックされているようなので、Bluetooth デバイスのブロック状態を確認します。

{{< code hl_lines="9" >}}
maku@raspberrypi:~ $ sudo rfkill list
0: hci0: Bluetooth
	Soft blocked: no
	Hard blocked: no
1: phy0: Wireless LAN
	Soft blocked: no
	Hard blocked: no
2: hci1: Bluetooth
	Soft blocked: yes
	Hard blocked: no
{{< /code >}}

`hci1` はソフトブロックされているようです。
次のようにしてアンブロックします。

```
$ sudo rfkill unblock bluetooth
```

これで、次のように `UP RUNNING` 状態になります。
`sudo hciconfig hci1 up` しなくても自動で立ち上がるようです。

{{< code hl_liknes="4" >}}
$ hciconfig
hci1:	Type: Primary  Bus: USB
	BD Address: 00:E0:4C:72:F9:03  ACL MTU: 1021:6  SCO MTU: 255:12
	UP RUNNING
	RX bytes:2240 acl:0 sco:0 events:208 errors:0
	TX bytes:31844 acl:0 sco:0 commands:208 errors:0

hci0:	Type: Primary  Bus: UART
	BD Address: B8:27:EB:A0:04:72  ACL MTU: 1021:8  SCO MTU: 64:1
	UP RUNNING 
	RX bytes:5322 acl:0 sco:0 events:445 errors:0
	TX bytes:68208 acl:0 sco:0 commands:445 errors:0
{{< /code >}}


NXBT のインストールと Switch との接続確認
----

`nxbt` コマンドを `pip` でインストールしようとすると、`dbus-python` がインストールできないというエラーになるので、事前に `libdbus-glib-1-dev` をインストールしておきます。
あと最新の Python 3 では、グローバルにパッケージインストールしようとすると怒られるので、venv 環境にインストールします。

```console
$ sudo apt-get install pkg-config  # これないと dbus-python がインストールできない
$ sudo apt-get install libdbus-glib-1-dev  # これないと dbus-python がインストールできない
$ mkdir ~/nxbt
$ cd ~/nxbt
$ python3 -m venv venv
$ . venv/bin/activate
$ pip install nxbt  # 時間かかる
```

`sudo nxbt test` コマンドを使って、Switch と接続できるかテストできます。
Switch のコントローラーのメニューから「持ちかた／順番を変更する」を選択した状態にしてコマンドを実行すると、自動的にコントローラーとして認識されて前の画面に戻ります。

```
(venv) $ sudo /home/maku/nxbt/venv/bin/nxbt test
[1] Attempting to initialize NXBT...
Successfully initialized NXBT.

[2] Checking for Bluetooth adapter availability...
2 Bluetooth adapter(s) available.
Adapters: ['/org/bluez/hci0', '/org/bluez/hci1']

[3] Please turn on your Switch and navigate to the 'Change Grip/Order menu.'
Press Enter to continue...
Creating a controller with the first Bluetooth adapter...
Successfully created a controller.

[4] Waiting for controller to connect with the Switch...
Connection timeout is 120 seconds for this test script.
Successfully connected.

[5] Attempting to exit the 'Change Grip/Order Menu'...
Controller successfully exited the menu.

All tests passed.
```

`sudo` で `nxbt` を実行するときに毎回フルパスで入力するのは面倒なので、シェルスクリプトにしておきます。

{{< code lang="bash" title="nxbt" >}}
#!/bin/bash
sudo env "PATH=$PATH" nxbt $@
{{< /code >}}

これで上記コマンドは `./nxbt text` で実行できるようになります（`venv` に入っていることが前提）。

{{% private %}}
★(2025-12-07)既存のRaspberry Pi 環境構築ができていれば、電源を入れて `./nxbt test` からやればよい。**
{{% /private %}}


webapp モードで起動してブラウザから操作
----

接続確認が済んだら、今度は webapp モードで接続して、Web ブラウザから Switch を操作してみます。
先ほどと同様に、コントローラーのメニューから「持ちかた／順番を変更する」を選択した状態にしてコマンドを実行します。

```
$ ./nxbt webapp
(186042) wsgi starting up on http://0.0.0.0:8000
```

{{% note title="jinja2 のエラー" %}}
webapp モードで起動しようとしたときに、`jinja2` まわりのエラーがでたときは、ライブラリのバージョン依存問題が発生しています。
今回の環境では下記のようにバージョンを指定してインストールすることで解決しました。

```
pip install jinja2==3.0.3 itsdangerous==2.0.1 Werkzeug==2.0.2 eventlet==0.33.0 dnspython==2.2.1
```
{{% /note %}}

webapp モードで起動したら、LAN 内の PC から Raspberry Pi のアドレス（例: `192.168.1.8:8000`）を指定してブラウザで開きます。



マクロ
----

### DQ3 くちぶえ→戦闘（種狩り）

- 実行前の準備
  1. フィールドに出る
  2. X → 右 → 下 → A → A で「くちぶえ」を実行できる状態にしておく

```
LOOP 10000
  X 0.1S
  DPAD_RIGHT 0.1S
  DPAD_DOWN 0.1S
  A 0.1S
  0.5S
  A 0.1S
  LOOP 55
    A 0.1S
    0.2S
```

### DQ2 穴掘り用（竜王の城:スタミナの種、すばやさの種、ラックの種）

- 実行前の準備
  - トヘロス役に（ムーンブルク王女）にしあわせの帽子を被せる
  - 「R+上」で「トヘロス」を設定（たまに実行する。しのびあしではダメ）
  - 「R+下」で「あなほり」を設定
  - 竜王の城 B4F/B5F の間の階段前。地下4階から地下5階に降りた場所にしておく。
- やってること
  - 「R+上」で「トヘロス」を実行
  - 以下繰り返し
    - 「R+下」で「あなほり」を5回実行
    - 少し上に移動（フロア移動）
    - 「R+下」で「あなほり」を5回実行
    - 少し下に移動（フロア移動）

```
LOOP 100
  R 0.1S
  R DPAD_UP 0.1S
  0.1S
  B 0.1S
  0.1S

  LOOP 5
    LOOP 5
      R 0.1S
      R DPAD_DOWN 0.1S
      0.1S
      B 0.1S
      0.1S
      B 0.1S
      0.1S

    DPAD_UP 2S
    1.3S

    LOOP 5
      R 0.1S
      R DPAD_DOWN 0.1S
      0.1S
      B 0.1S
      0.1S
      B 0.1S
      0.1S

    DPAD_DOWN 2S
    1.3S
```

### DQ2 穴掘り用（ハーゴンの神殿:ちからの種）

- 実行前の準備
  - トヘロス役に（ムーンブルク王女）にしあわせの帽子を被せる
  - 「R+上」で「トヘロス」を設定（たまに実行する） ★しのびあしではダメ！
  - 「R+下」で「あなほり」を設定
  - ハーゴンの神殿 2F/3F の間の階段前。3階に上がった場所にしておく。
- やってること
  - 「R+上」で「トヘロス」を実行
  - 以下繰り返し
    - 「R+下」で「あなほり」を5回実行
    - 少し下に移動（フロア移動2Fへ）
    - 「R+下」で「あなほり」を5回実行
    - 少し右に移動（フロア移動3Fへ）

```
LOOP 1000
  R 0.05S
  R DPAD_UP 0.05S
  0.1S
  B 0.05S
  0.1S

  LOOP 10
    LOOP 5
      R 0.05S
      R DPAD_DOWN 0.05S
      0.1S
      B 0.05S
      0.1S
      B 0.05S
      0.1S

    DPAD_DOWN 1S
    1.8S

    LOOP 5
      R 0.05S
      R DPAD_DOWN 0.05S
      0.1S
      B 0.05S
      0.1S
      B 0.05S
      0.1S

    DPAD_RIGHT 1.2S
    1.8S
```

