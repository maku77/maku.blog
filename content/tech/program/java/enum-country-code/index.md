---
title: "Java/Kotlin で ISO 3166 国コードの一覧を取得する"
url: "/p/5weufam"
date: "2020-05-11"
tags: ["Java", "Kotlin"]
---

Locale クラスで国コードのリストを取得する
----

[Locale クラスの getISOCountries() メソッド](https://docs.oracle.com/javase/10/docs/api/java/util/Locale.html#getISOCountries()) を使用すると、ISO 3166 で定義されている国コードの一覧を取得することができます。

- `String[] Locale.getISOCountries()`
    - ISO3166-1 alpha-2（2桁のアルファベット）
- `Set<String> Locale.getISOCountries(Locale.IsoCountryCode.PART1_ALPHA2)`
    - ISO3166-1 alpha-2（2桁のアルファベット）
- `Set<String> Locale.getISOCountries(Locale.IsoCountryCode.PART1_ALPHA3)`
    - ISO3166-1 alpha-3（3桁のアルファベット）
- `Set<String> Locale.getISOCountries(Locale.IsoCountryCode.PART3)`
    - ISO3166-3（4桁のアルファベット）


サンプルコード
----

次の Kotlin コードでは、`Locale.getISOCountries()` で ISO3166-1 alpha-2 の国コードをすべて取得し、それぞれについて、2桁の国コード、3桁の国コード、国名を表示しています。
[Try Kotlin](https://try.kotlinlang.org/) のサイトにコピペして実行できます。

{{< code lang="kotlin" >}}
import java.util.Locale

fun main() {
    for (alpha2 in Locale.getISOCountries()) {
        val loc = Locale("dummylang", alpha2)
        val alpha3 = loc.getISO3Country()
        val name = loc.getDisplayCountry()
        println("$alpha2  $alpha3  $name")
    }
}
{{< /code >}}

{{< code title="実行結果" >}}
AD  AND  Andorra
AE  ARE  United Arab Emirates
AF  AFG  Afghanistan
AG  ATG  Antigua and Barbuda
AI  AIA  Anguilla
AL  ALB  Albania

... 省略 ...

WS  WSM  Samoa
YE  YEM  Yemen
YT  MYT  Mayotte
ZA  ZAF  South Africa
ZM  ZMB  Zambia
ZW  ZWE  Zimbabwe
{{< /code >}}

- {{< file src="countries.txt" caption="実行結果 (countires.txt)" >}}

{{< reference >}}
- [国コードや言語コードのまとめ (ISO 3166, ISO 639)](/p/tfs5gr3)
- [Java/Kotlin で ISO 639 言語コードの一覧を取得する](/p/wjxanza)
{{< /reference >}}

