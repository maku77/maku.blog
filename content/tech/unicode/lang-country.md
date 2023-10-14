---
title: "国コードや言語コードのまとめ (ISO 3166, ISO 639)"
url: "p/tfs5gr3/"
date: "2020-05-11"
tags: ["ISO", "Java"]
---

国コード
----

### ISO 3166-1 (country codes)

__ISO 3166-1__ は、国を示すコードとして、短いアルファベットや数値を割り当てています。
日本では __JIS X 0304__ として標準化されています。

- {{< label "ISO 3166-1 alpha-2" >}}: ラテン大文字 2 桁で国を表す（例: `JP`、`US`）
- {{< label "ISO 3166-1 alpha-3" >}}: ラテン大文字 3 桁で国を表す（例: `JPN`、`USA`）
- {{< label "ISO 3166-1 numeric" >}}: 数字 3 桁で国を表す（例: `392`、`840`）

下記は、これらの国コードの例です（[ISO - Country Codes Collection](https://www.iso.org/obp/ui) より）。

| Short name | Alpha-2 code | Alpha-3 code | Numeric code |
| ---- | :--: | :--: | :--: |
| [JAPAN](https://www.iso.org/obp/ui/#iso:code:3166:JP) | JP | JPN | 392 |
| [UNITED STATES OF AMERICA](https://www.iso.org/obp/ui/#iso:code:3166:US) | US | USA | 840 |
| [UNITED KINGDOM OF GREAT BRITAIN AND NORTHERN IRELAND](https://www.iso.org/obp/ui/#iso:code:3166:GB) | GB | GBR | 826 |
| [CHINA](https://www.iso.org/obp/ui/#iso:code:3166:CN) | CN | CHN | 156 |

### ISO 3166-2 (country subdivision code)

__ISO 3166-2__ では、国より細かい行政区画名（県や州）のコードを定義しています。

- 日本の北海道: `JP-01`
- アメリカ合衆国のニューヨーク州: `US-NY`
- イギリスのウェスト・バークシャー: `GB-WBK`
- 中華人民共和国の広東省: `CN-GD`

### プログラムから国コードを扱う

- [Java/Kotlin で ISO3166 国コードの一覧を取得する](/p/5weufam)


言語コード
----

### ISO 639-1 / 639-2 の概要

__ISO 639__ では短いアルファベットで言語識別子を定義しています。

- {{< label "ISO 639-1 alpha-2" >}}: ラテン小文字 2 桁で言語を表す（例: `ja`、`en`, `zh`）
- {{< label "ISO 639-2 alpha-3" >}}: ラテン小文字 3 桁で言語を表す（例: `jpn`、`eng`, `zho`）

ISO 639-1:2002 の 2 桁の言語識別子では 200 言語くらいが定義されていますが、ここに収まりきらない言語に対応するため、3 桁で構成される ISO 639-2:1998 が制定されました（550 言語くらい定義されています）。
現在ではさらに拡張され、ISO 639-1 から ISO 639-6 までの 6 部門が定義されています。

下記は ISO 639-1、ISO 639-2 の言語識別子の例です。

| Language name | ISO 639-1 | ISO 639-2 |
| ---- | :--: | :--: |
| Japanese | ja | jpn |
| English  | en | eng |
| Chinese  | zh | zho (T) / chi (B) |
| French   | fr | fra (T) / fre (B) |
| German   | de | deu (T) / ger (B) |

ISO 639-2 の 3 桁コードは、639-2/T:用語学用と、B:書誌用に分かれているものもあります。

### ISO 639-1 / 639-2 言語識別子の一覧

下記のサイトで、ISO 639-1、ISO 639-2(T/B) の一覧データ (CSV/JSON) をダウンロードすることができます。

- [ISO Language Codes (639-1 and 693-2) and IETF Language Types - Dataset - DataHub - Frictionless Data](https://datahub.io/core/language-codes#data)

例えば、CSV ファイルの内容は次のような感じになっています。
言語名は英語とフランス語のデータしか含まれていませんが、言語識別子の一覧が欲しい場合は便利です。
現状 488 行あるので、487 言語が登録されているようです（1 行目はヘッダー）。

{{< code title="language-codes-full_csv.csv" >}}
alpha3-b,alpha3-t,alpha2,English,French
aar,,aa,Afar,afar
abk,,ab,Abkhazian,abkhaze
ace,,,Achinese,aceh

...省略...

fon,,,Fon,fon
fre,fra,fr,French,français
frm,,,"French, Middle (ca.1400-1600)",français moyen (1400-1600)

...省略...
{{< /code >}}

下記のサイトでは、言語識別子や言語名から ISO 639 の定義を検索して確認することができます。

- [SIL - ISO 639 Code Tables](https://iso639-3.sil.org/code_tables/639/data)

{{% maku-common/reference %}}
- [Java/Kotlin で ISO 639 言語コードの一覧を取得する](/p/wjxanza/)
{{% /maku-common/reference %}}

