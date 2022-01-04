---
title: "TV規格: DVBメモ"
linkTitle: "DVB"
url: "/p/oi66eim"
tags: ["DVB", "TV"]
---

DVB について
----
<!-- 2008-07-02 -->

DVB (Digital Video Broadcasting) はヨーロッパのデジタル放送規格。標準化団体名でもある。DVB には以下のような種類がある。

- DVB-T (Terrestrial) -- 地上デジタル放送の規格
- DVB-S (Satellite) -- 衛星放送の規格
- DVB-C (Cable) -- CATV の規格

ISDB (Integrated Services Digital Broadcasting) は日本、ブラジル向けのデジタル放送規格。ちなみに ARIB (Association of Radio Industries and Broadcast): 社団法人電波産業会というのは通信・放送分野の研究、標準化などを行う団体名であって規格の名前ではない。ISDB には以下のような種類がある。

- ISDB-T (Terrestrial) -- 地上デジタル放送の規格
- ISDB-S (Satellite) -- 衛星放送の規格
- ISDB-TSB (Terrestrial for Sound Broadcasting) -- 地上デジタルラジオ放送の規格


DVB の資料 <!-- 2008-07-02 -->
----

EPG に関することを知りたかったら、DVB SI 系の資料を読むと良い。まずは、DVB の Web サイトにある、DVB SI の 2 冊の Bluebook (PDF) から読み始めると楽（150ページくらい）。Bluebook はメアド登録しなくてもダウンロードできる。

- [DVB - Digital Video Broadcasting](http://www.dvb.org/)
    - [Standards & Technology] → [Standards & BlueBooks] → [DVB SI](http://www.dvb.org/technology/standards/)

日本、ブラジル向けのデジタル放送規格 ISDB も DVB をもとに考えられているので、DVB の資料を先に読んでおくと理解しやすい。


service, programme, event の違い <!-- 2008-07-03 -->
----

- service -- DVB で定義
- event, programme -- MPEG2 で定義

broadcaster （放送局）が 1 つの TV チャンネルとして垂れ流している一連の放送を service という。つまり TV チャンネルと考えてよい。service は映像、音声、字幕などの Components (ES: Elementary Stream) をまとめたものとして構成されている。

programme は、MPEG-2 のストリームを分けるための概念で、TV のチャンネルの考え方に近い。ただし、MPEG-2 では TV に特化したものは定義していないので、DVB で TV のチャンネルに相当する service というものを定義している。service には TV のチャンネル名などが含まれる。

service 自体には時間の概念はなく、延々と続く映像や音声の ES を垂れ流しているだけ。ここに、時間の概念を入れて、1 つの番組として、開始時刻や長さを定義したものが event。複数の event を集めたものを service と考えるよりは、1 つの service を event という概念で区切ったものが番組であると考える方が正しい。

1 つの transponder（あるいは channel）で複数の service が multiplex （多重化）されて、同時に配信されている。※ここでいう channel は TV のチャンネルではなく、チャネル。ケーブル TV や地上波での 1 つの配信システムを表している。


Network ID について <!-- 2011-07-22 -->
----

- DVB Services - identifiers
    - http://www.dvbservices.com/identifiers/network_id
    - http://www.dvbservices.com/identifiers/original_network_id
- ここが分かりやすい
    - http://www.interactivetvweb.org/tutorials/dtv_intro/dtv_intro

階層構造はこんなイメージ。

- Network（Cable とか Satellite とかのシステムを表す。1つ以上の Transport stream を流せる）
    - Transport stream (MPEG-2 のストリーム。複数の Service を含む）
        - Service (TV channel)
            - Event (TV show)
                - Elementary stream


TS パケット <!-- 2008-07-03 -->
----

MPEG-2 の TS (Transport Stream) は、TS パケットという単位でいろんなデータが多重化されて配信されている（複数のチャンネルの映像や音声、字幕、番組情報、時刻データなど）。TS パケットは 188byte 固定長。

 TS パケット (188byte) = TS ヘッダ (4byte) + ペイロード (184byte)

TS パケットは、Audio/Video/Teletext などのデータを構成する PES (Packetized Elementary Stream) の一部であったり、SI (Service Information) や BML データを構成する section data の一部であったりする。

ある TS パケットが、これらのどのデータの一部であるかを判別するために、TS ヘッダの中に 13bit の PID (PacketID) が入っている。デコーダはこの PID を見て、TS ストリームを多重分離 (demultiplex) することができる。例えば、SI の PAT 情報を構成する TS パケットの場合、PID は 0x0000 が入っている。


SI (Service Information) tables <!-- 2008-07-03 -->
----

DVB の SI (Service Information) は、ISO/IEC 13818-1 (MPEG-2) で定義されている PSI のテーブルに加え、service （チャンネル）情報や event （番組）情報を示す各種テーブルで構成されている。

### PSI (Program Specific Information) のテーブル

PSI は、受信機が多重化された multiplex データ（MPEG-2 TS ストリーム）を demultiplex するための情報で、MPEG-2 にて定義されている。
※複数の service を 1 つの channel、transponder で多重送信することを multiplex という。

PSI には下記の 4 つのテーブルがある。

- PAT: Program Association Table
    - MPEG-2 の TS ストリームで伝送されるプログラムを管理する情報。各 service（チャンネル）における PMT の PID (PacketID) を示す（正確に言えば、PMT の一部のデータである TS パケットの PID）。
    - その TS で流している service の数だけ PID (PacketID) を含んでいる（つまり PMT もチャンネルの数だけある）。
    - デコーダは、この PAT の情報を起点として各 service（チャンネル）ごとの PMT パケットの PID を知るので、PAT の PID (PacketID) は 0x0000 になっている。
- NIT: Network Information Table
    - MPEG-2 の TS ストリームを配信するのに使用されている physical network の情報を示す。
    - MPEG-2 の PSI では NIT の構造は定義していないので、各放送方式などで独自に定義する必要がある。
- PMT: Program Map Table
    - 各 service を構成する ES: Elementary Stream（映像、音声ストリームなど）の位置を示す。チャンネルの数だけ PMT が存在する。
    - その service（チャンネル）が Video、Audio、Teletext の 3 つの ES で構成されているなら、PMT の中に PID (PacketID) が 3 つ含まれる。
    - PMT データ自体の TS パケットの PID は、PAT 内で指定されるので、PAT や CAT パケットの PID とは違って固定値ではない。
- CAT: Conditional Access Table
    - CA システムに関する情報。EMM stream (Entitlement Management Message) の位置を示す。EMM はそれぞれの CA システムに特化した情報であり、例えば日本のデジタル T V では、B-CAS カードの契約情報や暗号解除するための鍵情報などが含まれている。EMM は B-CAS カードごとに個別に送信され、情報はカード内に保存される。
    - PAT と同じく、TS パケットにおいて最初の段階で取得しないといけない情報なので、CAT には固定の PID (PacketID) 0x0001 が割り当てられている。

### DVB 独自のテーブル

DVB の SI データには、MPEG-2 で定義されている PSI の 4 つのテーブルの他に、以下の 9 種類のテーブルが含まれている。これらのテーブルは、チャンネル情報、番組情報、時刻情報などを示している。

- BAT: Bouquet Association Table
    - bouquet （ブーケ）情報を示す。bouquet を構成する service のリストなど。異なるネットワークで送信された service も 1 つの bouquet にまとめられ得る。
- SDT: Service Description Table
    - チャンネル情報（service 情報）。チャンネル名、放送局の識別子など。
- EIT: Event Information Table
    - 番組情報（event 情報）。番組名、番組の開始時刻、番組長など。SI のテーブルのうち、この EIT だけはスクランブルがかかることがある。
- RST: Running Status Table
    - 番組の現在の進行状況。
- TDT: Time and Date Table
    - 現在時刻の情報。
- TOT: Time Offset Table
    - ローカルタイム（現地時間）に関する情報。
- ST: Stuffing Table
    - 現在のセクションを無効にする。データ境界を合わせるために使用。
- SIT: Selection Information Table
- DIT: Discontinuity Information Table


SED と EED
----

* 参考資料（規格）
    - https://www.dvb.org/standards
    - DVB SI [DVB BlueBook A038] Specification for Service Information (SI) in DVB system
        - https://www.dvb.org/resources/public/standards/a038_dvb-si_spec.pdf
- DVB SI [DVB BlueBook A005] Guidelines on implementation and usage of Service Information (SI)
    - https://www.dvb.org/resources/public/standards/a005_dvb-si_impl_guide.pdf

### SED と EED の定義

SED (Short Event Descript) と EED (Extended Event Descriptor) は、SI データの EIT-section で流される (EIT: Event Information Table) の一種です。
SED には「番組名とその短い説明」が入り、EED はそれを補足する形で、「出演者: ○○○」「あらすじ: ○○○」みたいな、いわゆる Key and Value のような形で詳細説明が入ってます。

以下、DVB BlueBook A005 より

> 4.2.4.10 Short event descriptor
>
> This descriptor is used to transmit the name and a short text description for an event. A language code is transmitted in order to indicate in which language the title and the text are written. Transmission of this descriptor is mandatory, unless there is a time_shifted_event_descriptor, in which case the descriptor is not allowed. This descriptor is allowed more than once in the EIT event loop for different languages. Thus it is not allowed to have more than one short_event_descriptor with the same language code.
>
> 4.2.4.5 Extended event descriptor
>
> This descriptor is used to transmit a bigger amount of textual information for an event than is possible with the short_event_descriptor. The information in extended event descriptors supplements that given in a short event descriptor. A language code is transmitted in order to indicate in which language the text is written. More than one extended_event_descriptor is allowed in the EIT event loop, for transmitting more data than one descriptor can contain (255 bytes excluding header), and for different languages. The last_descriptor field specifies the number of the last extended_event_descriptor for a specific language. If there is a time_shifted_event_descriptor, this descriptor is not allowed. Transmission of this descriptor is optional.


### SED と EED のテーブルのフォーマット

以下、DVB BlueBook A038 より。

> 6.2.37 Short event descriptor
>
> ```
> short_event_descriptor(){
>     descriptor_tag
>     descriptor_length
>     ISO_639_language_code
>     event_name_length
>     for (i=0;i<event_name_length;i++){
>         event_name_char
>     }
>     text_length
>     for (i=0;i<text_length;i++){
>         text_char
>     }
> }
> ```
>
> 6.2.15 Extended event descripto
>
> ```
> extended_event_descriptor(){
>     descriptor_tag
>     descriptor_length
>     descriptor_number
>     last_descriptor_number
>     ISO_639_language_code
>     length_of_items
>     for ( i=0;i<N;i++){
>         item_description_length
>         for (j=0;j<N;j++){
>             item_description_char
>         }
>         item_length
>         for (j=0;j<N;j++){
>             item_char
>         }
>     }
>     text_length
>     for (i=0;i<N;i++){
>         text_char
>     }
> }
> ```


MPEG-2 TS を多重分離する流れの概要 <!-- 2008-07-03 -->
----

### ES（映像、音声など）を多重分離する流れ

1. PAT (PID=0x0000) の TS パケットを受信
2. （TS パケットを結合して PAT を復元）
3. PAT の内容を見て、PMT の PID を取得（チャンネル数だけある）
4. PMT の内容を見て、各 ES の PID を取得（ES の数だけある）
5. ES の TS パケットを結合して再生

### EIT（番組情報テーブル）を多重分離する流れ

1. EIT (PID=0x0012) の TS パケットを受信。
2. （TS パケットを結合して EIT を復元）
3. EIT の内容がそのチャンネルの番組情報。

他の SI のテーブル、例えば SDT (PID=0x0011)、RST (PID=0x0013)、TDT/TOT (PID=0x0014) なども同様に多重分離される。

### DVB 標準一覧 <!-- 2009-05-18 -->

{{< image src="dvb-std.png" title="出典: 映像情報メディア学会誌 Vol.60, No. 5 (2006)" >}}


ディジタル地上波マルチプレックスの例 <!-- 2009-05-18 -->
----

{{< image src="dvb-multiplex.png" title="出典: 映像情報メディア学会誌 Vol.60, No. 5 (2006)" >}}

各サービスをどのような比率で送信するかは、欧州各国政府が決める。

