---
title: "TV規格: ISDB(ARIB)メモ"
url: "/p/h52ih57"
linkTitle: "ISDB(ARIB)"
date: "2008-07-14"
lastmod: "2019-05-23"
tags: ["ARIB", "TV"]
---

自動表示メッセージ (Auto Message) <!-- 2019-05-23 -->
----

左下に表示される B-CAS 関連のメッセージ枠などに関する仕様です。
詳しくは「**ARIB TR-B14 5.10 自動表示メッセージ表示**」を参照

- EMM共通メッセージセクションの自動表示消去種別というもので表示の ON/OFF を行う
    - 0x00 消去可: ユーザ操作で表示 OFF してもよい
    - 0x01 消去不可: 出しっぱにしなきゃダメ
    - 0x02 表示消去: 今消すべし
- 映像といっしょに EPG（番組表）などが表示されている場合に、自動表示メッセージを表示するかどうかは商品仕様による（表示 OFF にしているものがほとんど）。
- 表示枠は視聴を邪魔しないように**半透明などにするのが望ましい**（とは言っても、視聴の邪魔をして有料契約させる意図がある）。
- 文字色は商品仕様によるが、無彩色にして必要以上に目立たせないのが望ましい。
- **録画された番組を再生する場合にも、TS データに基づいて表示する**。契約情報などは、その時装着されている IC カードに基づいて判断すること。
- デコードに関するメッセージ、例えば IC カード抜け（デスクランブルできない）などのメッセージは優先して表示してよい。IC カードを装着してデコードできる状態になってから（映像が見えてから）自動表示メッセージを改めて表示すれば OK。


Network ID、TS ID、Service ID などのユニーク性に関して <!-- 2018-06-12 -->
----

### 『ＢＳ/広帯域ＣＳデジタル放送運用規定 ARIB TR-B15　6.5版（第二分冊） 5.3 識別子の運用』より。

- <b>network_id</b>: BS デジタル放送に対して1 個割り当て。日本国内でユニーク。
- <b>transport_stream_id</b>: 各TS に対し割り当て。ネットワーク内でユニーク。
- <b>service_id(=program_number)</b>: 各編成チャンネルに対し割り当て。ネットワーク内でユニーク。
- <b>event_id</b>: 各イベントに対し割り当て。service 内でユニーク。
- <b>broadcaster_id</b>ブロードキャスタに対して割り当て。オリジナルネットワーク内でユニーク。
- <b>series_id</b>: 番組のシリーズに対して割り当て。ブロードキャスタ内の同一メディ
アタイプに属するサービス群内でユニーク。

### 8.2.1 event_id の再使用について（時間方向の一意性）より

> 終了時刻を過ぎれば、そのイベントに付与された event_id の値は、EIT から消え、一定時間後、別のイベントに対し同じ値を付与することが可能となる。
> しかし、終了時刻から 24 時間以内は別のイベントに対し同じ値を付与し EIT に記述してはならない。


枝番表示について <!-- 2016-06-06 -->
----

- http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v4_1-1p3-1.pdf#page=213

枝番
: 他地域（他県）の放送を受信する場合、その地域の放送局のチャンネル 番号と自地域の放送局のチャンネル番号が重複する場合があり、この時 は、３桁チャンネル番号も同じになる。受信機では、これらの放送局を 区別するために、3 桁に加えてもう 1 桁、番号を付ける。この番号を「枝番」と呼び、自地域の放送局には、末尾に「0」が、他地域の放送局に は「1」以降の番号が付けられる（例: 自地域 0210、他地域 0211 など）。


ジャンル情報に関する情報 <!-- 2009-09-07 -->
----

- ARIB TR-B14 3.5版（第二分冊）4-341 第4部付録
    - [付録A] 放送開始当初のジャンルコード表（content_nibble）
- TR-B15: BS/広帯域CSデジタル放送運用規定（第2分冊）
    - [付録A] 放送開始当初のジャンルコード表
        - 地デジ/BS/CS 共通のジャンル
- TR-B15: BS/広帯域CSデジタル放送運用規定（第4分冊）
    - [付録A] 放送開始当初のジャンルコード表
        - CS では、content_nibble_level_1="0xE", content_nibble_level_2="0x1" に、user_nibble フィールドにて拡張ジャンルを表現いている。


EIT[p/f/sch] に関する情報 <!-- 2014-07-04 -->
----
- ARIB TR-B14 第4分冊: B.8 EIT (p.4-396)


番組セルの表示方法 <!-- 2014-07-04 -->
----
- ARIB TR-B14 第4分冊: 19 イベント編成変更
- ARIB TR-B14 第4分冊: 19.6.5 番組割り込み(2)
- EPG の表示仕様については以下を参照
    - ARIB TR-B14 6.6 EPG

EPG のチャンネルの並び順（07DTG の UI 仕様より）
----
- BS/CS デジタルの場合には service_id 昇順。
- 地上デジタルの場合には「ARIB TR-B14 第二編 6.6.3 番組表および番組リスト」
    - 「JEITA 地上デジタルテレビジョン放送受信機 受信機動作ガイドライン 4 EPG への放送局提示順に関するガイドライン」に準拠する。並び変えたチャンネルは、左から右に配置する。


デジタル放送に関するアクセス制御方式 <!-- 2009-05-26 -->
----

- ARIB STD-B25
- CAS-R -- 受信時の制御方式である限定受信方式。暗号化（スクランブル）されたコンテンツを受信時に復号し、リアルタイムでコンテンツを視聴することを可能とするもの。
- CAS-P -- 再生時の制御方式である限定再生方式。サーバー型放送受信機の蓄積装置に暗号化されたままの状態で蓄積したコンテンツを、再生時に復号し、サービス事業者より許諾された利用条件に従って視聴、利用することを可能とするもの。


チャンネルロゴに関して <!-- 2008-07-14 -->
----

### チャンネルロゴのサイズ

~~~
HD ラージ        36x64  ｽｸｴｱﾋﾟｸｾﾙ      9 : 16  0x05  1152
HD スモール      27x48  ｽｸｴｱﾋﾟｸｾﾙ      9 : 16  0x02  972
SD4:3 ラージ     36x72  1.118 : 1      9 : 16  0x03  1296
SD4:3 スモール   24x48  1.118 : 1      9 : 16  0x00  864
SD16:9 ラージ    36x54  1.118 : 1.333  9 : 16  0x04  972
SD16:9 スモール  24x36  1.118 : 1.333  9 : 16  0x01  648
~~~

参考: 『地上デジタルテレビジョン放送運用規定』ARIB TR-B14 3.5版（第一分冊）(1/2) のページ 1-6 表4-1 送出するロゴマークのサイズパターン（ロゴタイプ）

### ロゴデータのダウンロード <!-- 2010-01-15 -->

チャンネルロゴのダウンロードに関して運用規定を抜粋。

#### ARIB TR-B14_3.5版（第一分冊）(1/2)

> 4 ダウンロードの用途と前提
>
> (3) ロゴデータの更新
> 受信機の設置場所にて受信できる放送局のロゴデータ更新を行う。受信機は出荷時の初期状態においてロゴデータを持っていないと想定される。各放送局は表示させたいサービスのロゴデータを自らの放送波によって受信機へダウンロードする。

> 5 ダウンロード伝送ガイドライン -- 5.1.1 ダウンロードコンテンツ
>
> ロゴのうち、PNGデータとして送出されるロゴデータはCDT（Common Data Table）を用いて配信される。また簡易ロゴに用いる文字（最大英数記号5文字）はSDT内に配置するロゴ伝送記述子に記載する。

> 5 ダウンロード伝送ガイドライン -- 5.1.2 告知情報
>
> CDTを用いてロゴが配信されている間は、SDTサービスループ内のそのロゴを参照するサービスにロゴ伝送記述子が配置され、ロゴ配信が検知可能であるとともに、CDTへのポインティングがなされる。

> 6.2 CDT方式の受信機ガイドライン -- 6.2.1 メモリ規定
>
> (1) ロゴデータ用のメモリ領域として必要となるメモリ量を確保すること。送られてくる 6 種類のロゴデータのうち、どれを取得するかは商品企画とする。

> 6.2.2 動作規定 -- 6.2.2.1 受信機能
>
> 以下の手順で新しいロゴデータの受信／蓄積／表示を行う。
> (1) SDT 内サービスループにCDT 伝送方式1 のロゴ伝送記述子が配置されているのを検出する。
> (2) logo_id とlogo_version 値が現在受信機内に保存されているものと異なる場合、ロゴ伝送記述子に記載のdownload_data_id をサブテーブルとし、かつ、受信機で使用する logo_type のセクションのCDT を受信する。
> (3) ...

⇒ つまり、SDT 内に「ロゴ配信しますよ」フラグが立っているタイミングでロゴデータが受信されます。

#### ARIB STD-B21_4.8版 デジタル放送用受信装置（望ましい仕様）

> 12.2.2.2 ダウンロードコンテンツ・セクション伝送方式
>
> ダウンロードコンテンツをセクション形式で伝送する場合はCDT（Common Data Table）を用いる。
> (1) 全受信機共通データテーブル（CDT）（Common Data Table）
CDT は、そのテーブルを受信する全ての受信機を対象として、不揮発性メモリに格納すべき共通データをセクション形式で伝送するために用いる。地上デジタルテレビジョン放送では、事業者のサービスロゴデータはCDT 内のdata_module_byte にロゴデータを配置して伝送する。ロゴデータフォーマットは「付属(ダウンロード機能)」のAppendix.A を参照のこと。

> 3 用語 [#tb268b1d]
>
> CDT: Common Data Table：全受信機共通データテーブル。事業者ロゴマークなど、受信機で共通に必要なデータを伝送する。


番組の最短時間長、最大時間長について
----

- ARIB TR-B14_3.5版（第二分冊）
    - C.5 番組表
    - C.5.6 番組表で扱う情報種

より抜粋

> 番組開始予定時刻と終了予定時刻
>
> EITのstart_time , durationフィールドを利用する。イベント時間長の最大値は48時間である。よってイベント終了時刻は最大二日後の日付になる可能性もある。情報提示を行う際には注意が必要である。また最小時間は1分であるがやむを得ぬ場合には1分未満のイベントが定義されることがあること、開始時刻が毎正分に設定されるとは限らないことにも注意が必要である。サマータイム導入時にはTOTで提供されるローカルタイムオフセット記述子を利用しオフセット時刻を足した時刻で提示すると良い。

１分未満を正式に許可しているのか、どっちなのかはっきりして欲しい。。。


規格資料リンク <!-- 2009-09-28 -->
----

### 放送分野の標準規格 (STD)
http://www.arib.or.jp/tyosakenkyu/kikaku_hoso/hoso_kikaku_number.html

- STD-B10: デジタル放送に使用する番組配列情報
    - 概要: http://www.arib.or.jp/tyosakenkyu/kikaku_hoso/hoso_std-b010.html
    - PDF: http://www.arib.or.jp/english/html/overview/doc/2-STD-B10v5_3.pdf

### 放送分野の技術資料 (TR)
http://www.arib.or.jp/tyosakenkyu/kikaku_hoso/hoso_gijutsu_number.html

- TR-B14: 地上デジタルテレビジョン放送運用規定（全5分冊）
    - 概要: http://www.arib.or.jp/tyosakenkyu/kikaku_hoso/hoso_tr-b014.html
    - 第1分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v5_6-1p5.pdf
    - 第2分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v5_6-2p5.pdf
    - 第3分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v5_6-3p5.pdf
    - 第4分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v5_6-4p5.pdf
    - 第5分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B14v5_6-5p5.pdf
- TR-B15: BS/広帯域CSデジタル放送運用規定（全4分冊）
    - 第1分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B15v6_5-1p4.pdf
    - 第2分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B15v6_5-2p4.pdf
    - 第3分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B15v6_5-3p4.pdf
    - 第4分冊: http://www.arib.or.jp/english/html/overview/doc/4-TR-B15v6_5-4p4.pdf

