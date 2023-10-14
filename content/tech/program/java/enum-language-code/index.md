---
title: "Java/Kotlin で ISO 639 言語コードの一覧を取得する"
url: "p/wjxanza/"
date: "2020-05-11"
tags: ["Java", "Kotlin"]
---

Locale クラスで言語識別子のリストを取得する
----

[Locale クラスの getISOLanguages() メソッド](https://docs.oracle.com/javase/10/docs/api/java/util/Locale.html#getISOLanguages()) を使用すると、ISO 639-1 alpha2 で定義されている 2 桁の言語識別子の一覧を取得することができます。

- `String[] Locale.getISOLanguages()`


サンプルコード
----

次の Kotlin コードでは、`Locale.getISOLanguages()` で ISO 639-1 alpha-2 の言語識別子をすべて取得し、それぞれについて、2桁の言語識別子、3桁の言語識別子 (ISO 639-2/T)、言語名を表示しています。
[Try Kotlin](https://try.kotlinlang.org/) のサイトにコピペして実行できます。

{{< code lang="kotlin" title="langcodes.kt" >}}
import java.util.Locale

fun main() {
    for (alpha2 in Locale.getISOLanguages()) {
        val loc = Locale(alpha2)
        val alpha3 = loc.getISO3Language()
        val name = loc.getDisplayLanguage()
        println("$alpha2  $alpha3  $name")
    }
}
{{< /code >}}

{{< code title="実行結果" >}}
aa  aar  Afar
ab  abk  Abkhazian
ae  ave  Avestan
af  afr  Afrikaans
ak  aka  Akan
am  amh  Amharic

... 省略 ...

xh  xho  Xhosa
yi  yid  Yiddish
yo  yor  Yoruba
za  zha  Zhuang
zh  zho  Chinese
zu  zul  Zulu
{{< /code >}}

- {{< file src="langcodes.txt" caption="実行結果 (langcodes.txt)" >}}


おまけ
----

いろんな言語名をいろんな Locale 設定で出力してみるサンプルコードです。
HTML の表として出力しています。

{{< code lang="kotlin" title="languages.kt" >}}
import java.util.Locale

// 表示する言語名のリスト（戻り値は便宜上 Locale 型）
fun getLanguageLocales(): List<Locale> {
    // ここでは ISO 639 の 言語識別子（2桁）のリストを使うことにする
    val languages: Array<String> = Locale.getISOLanguages();
    val locales = mutableListOf<Locale>()
    languages.forEach { locales.add(Locale(it)) }
    return locales
}

// 表示用のロケールリスト
fun getDisplayLocales(): List<Locale> {
    return mutableListOf<Locale>().apply {
        add(Locale("en", "US"))
        add(Locale("ja", "JP"))
        add(Locale("es", "ES"))
        add(Locale("zh", "CN"))
    }
    // システムがサポートする全ロケールを返す場合
    // return Locale.getAvailableLocales()
}

fun main() {
    val dispLocales: List<Locale> = getDisplayLocales()
    val langLocales: List<Locale> = getLanguageLocales()

    println("""
        <!DOCTYPE html>
        <html lang="ja">
        <head>
            <meta charset="UTF-8">
            <style>
                table { border-collapse: collapse; }
                th { color: #555; background: #ccc; white-space: nowrap; }
                th, td { padding: 0.3em 0.6em; border: solid 1px #999; }
                tr:nth-child(odd) { background-color: #fcfcfc; }
                tr:nth-child(even) { background-color: #eeeeee; }
            </style>
        </head>
        <body>
        <table style="border: solid 1px;">
        <tr>
            <th>Locale</th>
            <th>ISO 639-1</th>
            <th>ISO 639-2/T</th>
            <th>Display Language Name</th>
        </tr>
    """.trimIndent())
    for (disp: Locale in dispLocales) {
        for (lang: Locale in langLocales) {
            println("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>".format(
                disp.toString(),
                lang.getLanguage(),
                lang.getISO3Language(),
                lang.getDisplayLanguage(disp)))
        }
    }
    println("</table></body></html>")
}
{{< /code >}}

実行結果は長いので HTML ファイルとして添付しておきます。

- {{< file src="languages.html" caption="実行結果 (languages.html)" >}}


{{% maku-common/reference %}}
- [国コードや言語コードのまとめ (ISO 3166, ISO 639)](/p/tfs5gr3/)
- [Java/Kotlin で ISO3166 国コードの一覧を取得する](/p/5weufam/)
{{% /maku-common/reference %}}
