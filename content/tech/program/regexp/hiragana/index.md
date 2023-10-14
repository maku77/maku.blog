---
title: "正規表現: ひらがなとカタカナを正規表現で表す"
linkTitle: "ひらがなとカタカナを正規表現で表す"
url: "p/kkaq7q5/"
date: "2020-02-17"
tags: ["正規表現"]
---

ひらがな・カタカナの正規表現
----

ひらがなの 1 文字、カタカナの 1 文字を正規表現で表すと次のようになります。

- ひらがな
    - `[\u3041-\u3096]`
    - `[\x{3041}-\x{3096}]`
    - `\p{Hiragana}` （Unicode 文字プロパティが使える場合）
- カタカナ
    - `[\u30A1-\u30FA]`
    - `[\x{30A1}-\x{30FA}]`
    - `\p{Katakana}` （Unicode 文字プロパティが使える場合）

参考
----

[Wikipedia: Hiragana (Unicode block)](https://en.wikipedia.org/wiki/Hiragana_%28Unicode_block%29) より

{{< image src="img-001.png" >}}

[Wikipedia: Katakana (Unicode block)](https://en.wikipedia.org/wiki/Katakana_%28Unicode_block%29) より

{{< image src="img-002.png" >}}

{{% maku-common/reference %}}
- [TypeScriptサンプル: 文字列内のひらがなとカタカナを変換する](/p/tzjvcad)
{{% /maku-common/reference %}}

