---
title: "レバーレスコントローラー HAUTE BOARD B16 (Haute42) の設定メモ"
url: "p/2fc7xwn/"
date: "2026-01-28"
tags: ["ゲーム"]
---

[Guilty Gear Strive で使い始めた](/p/9xjirjr/)超コンパクトな Haute42 のレバーレスコントローラー{{< amazon-search keyword="HAUTE BOARD B16" >}}の設定メモです。
レバーレス面白いですね。今のところ飽きる気配がありません。

{{< image src="img-board.jpg" title="HAUTE BOARD B16（現在の外観）" >}}

キースイッチの交換、キーキャップの交換、消音リングの導入など、色々カスタマイズして遊んでいます。

Start ボタンを押しながら PC と USB ケーブルで接続すると、[http://192.168.7.1](http://192.168.7.1) で設定用の Web UI を開けるようになります。
カッコイイ。
これは GP2040-CE というオープンソースのファームウェアが提供している機能で、HAUTE BOARD B16 もこれを採用しています（参考: [GP2040-CE Web Configurator](https://gp2040-ce.info/web-configurator/))。
Haute42 系のコントローラーは多数発売されていますが、ほとんど同じように設定できるはずです。


Pin Mapping 設定
----

下記のページを開くと、ボタン割り当てを変更できます。

- [http://192.168.7.1/pin-mapping](http://192.168.7.1/pin-mapping)
- あるいはメニューから `Configuration` → `Pin Mapping`

HAUTE BOARD B16 の物理スイッチには次のように固定の **PIN 番号** が割り当てられており、ここに任意のボタンを割り当てていきます。

{{< image src="img-layout1.drawio.svg" title="HAUTE BOARD B16 の PIN 番号（変更不可）" >}}

ボタンの割り当ては、下記図の青文字で示されているような **仮想ボタン名** で指定します。
さらに仮想ボタンは、起動時の入力モードによってそれぞれのゲーム機用のボタンにマッピングされます。
例えば、`B1` ボタンを押しながら USB ケーブルを接続すると Nintendo Switch モードで起動し、各ボタンが下記の赤文字で示されているような Nintendo Switch 用のボタンにマッピングされます。

{{< image src="img-layout2.drawio.svg" title="HAUTE BOARD B16 のデフォルトのボタン割り当て" >}}

デフォルトでは仮想ボタンの `L3`/`Up` が 2 箇所に割り当てられているので、ここは好みに応じてカスタマイズしてしまうのがよさそうです。
設定画面で変更するのは、あくまで青文字の仮想ボタンの割り当てで、例えば次のように変更します。

{{< image src="img-layout-custom.drawio.svg" title="ボタン配置の変更" >}}

- PIN26: `L3` → `L2`（Switch の `ZL` ボタンに相当）
- PIN27: `Up` → `S1`（Switch の `-` ボタンに相当。通称 SELECT ボタン）

こんな感じで、プルダウンメニューから選択するだけです。簡単ですね。

{{< image w="450" src="img-pin-mapping.png" title="Pin Mapping 設定画面の例" >}}


SOCD Cleaning モードの設定
----

レバーレスコントローラーでは、Up/Down キーや Left/Right キーを同時に押すことができるため、これらの同時押しをどのように扱うかを設定する必要があります。
このような反対方向のボタンの同時押し状態を **SOCD (Simultaneous Opposite Cardinal Directions)** と呼び、その状態になったときの処理を SOCD クリーニングと呼びます。

| SOCD Cleaning モード | 説明 | 設定方法 |
| ---- | ---- | ---- |
| **SOCD Neutral（推奨/Default）**| Up+Down → ニュートラル<br>Left+Right → ニュートラル | Home(A1) + Start(S2) + Down |
| SOCD Up Priority | Up+Down → Up<br>Left+Right → ニュートラル | Home(A1) + Start(S2) + Up |
| SOCD Last Win | Up+Down → 後勝ち<br>Left+Right → 後勝ち | Home(A1) + Start(S2) + Left |

SOCD クリーニングの設定は、ゲーム中に上記のキーを同時押しすることで変更できます。
電源を OFF/ON しても保存されているので、最初に一度だけ設定すれば OK です。
多くの大会で採用されている Neutral を選んでおくのが無難です。

Web UI の設定ページからも変更できます。

- [http://192.168.7.1/settings](http://192.168.7.1/settings)
- あるいはメニューから `Settings` を選択 → `SOCD Cleaning Mode`


Input モードの設定
----

前述の通り、HAUTE BOARD B16 のボタン設定は仮想ボタンで行い、実際のゲーム機にどのような信号が送られるかは起動時の **入力モード** によって決まります。
下記のボタンを押しながら USB ケーブルを接続することで、入力モードを切り替えられます（実はこのボタンの位置もカスタマイズ可能）。
この設定も本体に保存されるので、同じゲーム機で使う場合は、次回からは USB ケーブルを接続するだけで OK です。

{{< image src="img-layout-input-mode.drawio.svg" title="このボタンを押しながらゲーム機に接続" >}}

さらに、Web UI の設定ページからは、メガドライブ・ミニのような小型のレトロゲーム機向けのモードも選択できます。
何でもできますね。

- Mini/Classic Console Modes
  - Sega Genesis/MegaDrive Mini（メガドライブ・ミニ）
  - NEOGEO Mini（ネオジオ・ミニ）
  - PC Engine/TurboGrafx-16 Mini（PCエンジン・ミニ）
  - EGRET II Mini（イーグレットII・ミニ）
  - ASTROCITY Mini（アストロシティ・ミニ）
  - PlayStation Classic（プレイステーション・クラシック）


おまけ: Guilty Gear Strive のキー配置
----

{{< image src="img-layout-ggst.drawio.svg" title="Guilty Gear Strive 用のボタン割り当て（一例）" >}}

フォルトレスディフェンス (FD) のボタンが重複していますが、Switch のボタン割り当ての限界であきらめています。
サイクバーストは、`K` + `D` の同時押しでがんばります。
Steam や PS 版だったらもっと柔軟にキー設定できるんだろうなぁ。

