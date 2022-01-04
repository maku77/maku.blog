---
title: "ISO 639-2 (alpha-3) 言語コードから Java の Locale オブジェクトを生成する"
url: "/p/jbs8o3h"
date: "2020-05-13"
tags: ["Java", "Kotlin"]
---

Locale クラスは ISO 639-2（3桁）の言語コードを受け付けるのか？
----

ISO 639-2 (alpha-3) では、言語を 3 桁のアルファベットで識別できるよう定義しています（例: 日本語は `jpn`）。
この言語識別子を使って、Java の `Locale` オブジェクトを生成できるのでしょうか？

Java の [Locale クラス](https://docs.oracle.com/javase/jp/10/docs/api/java/util/Locale.html) の説明には、次のように記載されています。

> 言語に alpha-2 コードと alpha-3 コードの両方がある場合は、alpha-2 コードを使用する必要があります。

また、[Locale クラスのコンストラクタ](https://docs.oracle.com/javase/jp/10/docs/api/java/util/Locale.html#%3Cinit%3E(java.lang.String)) の第 1 引数の説明には、次のように書かれています。

> language - ISO 639 alpha-2 または alpha-3 言語コード、または最高 8 文字の言語のサブタグ。

実際に試してみると、alpha-2 コード（2桁）はほぼすべて対応しているのに対し、alpha-3 コード（3桁）は対応がいまいちのようです。

{{< code lang="kotlin" title="sample.kt" >}}
fun main() {
    // alpha-2 で指定
    println(Locale("ja").displayLanguage)  // 日本語 (OK)
    println(Locale("en").displayLanguage)  // 英語 (OK)
    println(Locale("fr").displayLanguage)  // フランス語 (OK)
    println(Locale("zh").displayLanguage)  // 中国語 (OK)

    // alpha-3 で指定
    println(Locale("jpn").displayLanguage)  // 日本語 (OK)
    println(Locale("eng").displayLanguage)  // 英語 (OK)
    println(Locale("fra").displayLanguage)  // fra (NG)
    println(Locale("zho").displayLanguage)  // zho (NG)
}
{{< /code >}}

`Locale` クラスが認識できる alpha-2 コードの一覧は、`Locale.getISOLanguages()` で取得できます。
一方、認識できる alpha-3 コードの一覧を取得するメソッドは存在しませんが、上記の結果をみる限り、フランス語を表す `fra` と中国語を表す `zho` という言語コードからは、`Locale` クラスは言語情報をうまく生成してくれません。
何とも使いにくいですね。

alpha-2 コード（2桁）で正しい言語情報を持つ `Locale` オブジェクトを生成できるのであれば、そのオブジェクトから `getISO3Language()` で alpha-3 コード（3桁）を取得することができます。
つまり、`fr` に対応するコードが `fra` であることは判別できます。
この情報を使って、うまく逆変換するテーブルを作れば、alpha-3 コードからももう少しロバストに `Locale` オブジェクトを生成できるようになりそうです。


ISO 639-2（3桁）の言語コードから Locale オブジェクトを生成するユーティリティ
----

下記の `LanguageLocales` クラスは、ISO 639-1（2桁）や ISO 639-2（3桁）の言語コードから `Locale` オブジェクトを取得するためのユーティリティクラスです。

{{< code lang="kotlin" title="LanguageLocales.kt" >}}
/**
 * Language utility for obtaining the Locale object corresponding
 * to the given ISO 639-1/2 language code such as "fra".
 *
 * ```
 * val locale = LanguageLocales["fra"]
 * ```
 */
class LanguageLocales {
    companion object {
        /** Map of ISO 639-1/2 code to Locale */
        private val localeMap = mutableMapOf<String, Locale>()

        init {
            // ISO 639 の言語識別子（2桁）リストを利用してマップを作成します
            val alpha2s: Array<String> = Locale.getISOLanguages();
            for (code in alpha2s) {
                val loc = Locale(code)
                localeMap[code] = loc  // from Alpha2
                localeMap[loc.getISO3Language()] = loc  // from Alpha3
            }
        }

        /** ISO 639 の言語コード（2桁/3桁）から Locale オブジェクトを取得します. */
        operator fun get(alpha2or3: String): Locale {
            var loc: Locale? = localeMap[alpha2or3]
            if (loc == null) {
                loc = Locale(alpha2or3)
                localeMap[alpha2or3] = loc
            }
            return loc
        }
    }
}
{{< /code >}}

`Locale` オブジェクトの第 1 引数に言語コードを渡してオブジェクト生成するより、`LanguageLocales` クラスを使って `Locale` オブジェクトを取得した方が、正しい言語情報を取得できる可能性が上がります。

{{< code lang="kotlin" title="main.kt" >}}
fun main() {
    // alpha-2 で指定
    println(LanguageLocales["ja"].displayLanguage)  // 日本語 (OK)
    println(LanguageLocales["en"].displayLanguage)  // 英語 (OK)
    println(LanguageLocales["fr"].displayLanguage)  // フランス語 (OK)
    println(LanguageLocales["zh"].displayLanguage)  // 中国語 (OK)

    // alpha-3 で指定
    println(LanguageLocales["jpn"].displayLanguage)  // 日本語 (OK)
    println(LanguageLocales["eng"].displayLanguage)  // 英語 (OK)
    println(LanguageLocales["fra"].displayLanguage)  // フランス語 (OK)
    println(LanguageLocales["zho"].displayLanguage)  // 中国語 (OK)
}
{{< /code >}}

もちろん、すべての ISO 639-2 (alpha-3) コードに対応しているわけではないので、その点は注意してください。

