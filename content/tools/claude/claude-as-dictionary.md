---
title: "Claude Code を辞書にするスラッシュコマンドを定義する (/m-en, /m-ja)"
url: "p/52v9k2v/"
date: "2025-07-13"
lastmod: "2025-07-14"
tags: ["claude", "claude code"]
changes:
  - 2025-07-14: Windows (PowerShell) でのコマンドエイリアス設定方法を追記
---

Claude Code では、よく使うプロンプトを独自のスラッシュコマンドとして定義しておくことができます。
ここでは、**`/m-en`** と **`/m-ja`** というスラッシュコマンドを定義して、英語と日本語の翻訳機として使う方法を紹介します。

{{% note title="m- というプレフィックス" %}}
個人的なコマンドを定義するときは、いつも **`m-`** というプレフィックスを付けるようにしています。
こうすると、`m-` とタイプするだけで、自分で定義したコマンドを一覧表示できるので便利です。
みなさんは、このようなプレフィックスを付けずに、`/en` や `/ja` という名前でスラッシュコマンドを定義しても大丈夫です。
{{% /note %}}

スラッシュコマンドは、下記のようにホームディレクトリ以下に `.md` ファイルを作成することで定義できます。
この例では英語で記述していますが、日本語で記述しても大丈夫です。

{{< code lang="md" title="~/.claude/commands/m-en.md（英語への翻訳）" >}}
---
description: Translate text to English
---

Please translate the following text to English with natural, appropriate English expressions:

```
$ARGUMENTS
```

### Output Rules

- If the input argument is one or two words, please display the following:
  - Three English translation results (English word or phrase) with english pronunciation (e.g. hello /həˈloʊ/)
  - Three usage examples in the text in order of frequency, with Japanese translations
- If the input argument is a phrase or sentence, please display the following:
  - If the input text contains unnatural Japanese, provide a corrected version with natural Japanese expressions
  - Three English translation results (English sentences)
  - Three usage examples in the text in order of frequency, with Japanese translations

Please display responses simply without Markdown formatting.
Please don't display unnecessary messages at the beginning and end.
{{< /code >}}

{{< code lang="md" title="~/.claude/commands/m-ja.md（日本語への翻訳）" >}}
---
description: Translate text to Japanese
---

Please translate the following text to Japanese with natural, appropriate Japanese expressions:

```
$ARGUMENTS
```

### Output Rules

- If the input argument is one or two words, please display the following:
  - English pronunciation (e.g. hello /həˈloʊ/)
  - The word in Japanese
  - Three example sentences in order of frequency, with Japanese translations
- If the input argument is a phrase or sentence, please display the following:
  - If the input text contains unnatural English, provide a corrected version with natural English expressions
  - Japanese translation of the phrase or sentence

Please display responses simply without Markdown formatting.
Please don't display unnecessary messages at the beginning and end.
{{< /code >}}

それぞれ使用例などを表示するように指示しているため少々長くなっていますが、もっと簡単に「`$ARGUMENTS` を英語に翻訳してください」のような内容だけでも OK です。
自分の用途に会うように調整してください。

これらのスラッシュコマンドは、Claude Code の中から次のような感じで使用します。

```console
> /m-en 延期する

⏺ 延期する

  1. postpone /poʊstˈpoʊn/
  2. delay /dɪˈleɪ/
  3. defer /dɪˈfɜr/

  Usage Examples:

  1. We need to postpone the meeting until next week.
  来週まで会議を延期する必要があります。
  2. The flight was delayed due to bad weather.
  悪天候のため、フライトが延期されました。
  3. I'll have to defer my vacation plans.
  休暇の計画を延期しなければなりません。

> /m-ja withdraw

⏺ withdraw /wɪðˈdrɔ/

  撤退する、引き出す、取り下げる

  1. I need to withdraw money from the ATM.
  ATMからお金を引き出す必要があります。
  2. The company decided to withdraw from the market.
  その会社は市場から撤退することを決めました。
  3. He had to withdraw his application due to personal reasons.
  個人的な理由で彼は申請を取り下げなければなりませんでした。
```

いちいち Claude Code を起動してからスラッシュコマンドを実行するのが面倒な場合は、`claude` コマンドを実行するときに **`-p (--print)`** オプションで直接スラッシュコマンドを渡せば、単発のコマンドとして実行できます。

{{< code lang="console" title="辞書スラッシュコマンドの使用例" >}}
$ claude -p "/m-en 延期する"    # 英語への翻訳
$ claude -p "/m-ja withdrawk"   # 日本語への翻訳
{{< /code >}}

さらに、上記コマンドのエイリアスなどを定義しておけば、もっと簡単に起動できるようになります。
例えば、macOS や Linux で zsh を使っている場合は、次のように **`m-en`** 関数と **`m-ja`** 関数を定義できます。

{{< code lang="zsh" title="~/.zshrc" >}}
m-en() { claude -p "/m-en $*" }
m-ja() { claude -p "/m-ja $*" }
{{< /code >}}

Windows の PowerShell を使っている場合は、**`notepad $profile`** などで設定ファイルを開いて、下記のような感じで `Function` を定義します。
ちなみに、`$input |` で標準入力を扱えるようにし、`$($args)` で全ての引数をスペースで結合して渡すようにしています。

{{< code lang="powershell" title="PowerShell プロファイル" >}}
Function m-en { $input | claude -p "/m-en $($args)" }
Function m-ja { $input | claude -p "/m-ja $($args)" }
{{< /code >}}

- 参考: [PowerShell のプロファイルを作成して独自コマンドを定義する](/p/v7bkitw/)

これで、ターミナルから直接 **`m-en`** や **`m-ja`** と入力するだけで、翻訳できるようになります。

{{< code lang="console" title="使用例" >}}
$ m-en 延期する   # 英語への翻訳
$ m-ja withdraw   # 日本語への翻訳
{{< /code >}}

標準入力も扱えるので、下記のように他のコマンドの出力をパイプでつなげて翻訳することができます。

{{< code lang="console" title="標準入力を翻訳する" >}}
$ echo "To be or not to be, that is the question." | m-ja
**English:** To be or not to be, that is the question.
**Japanese:** 生きるべきか死ぬべきか、それが問題だ。

This is a famous quote from Shakespeare's Hamlet. The Japanese translation
captures the philosophical meaning of contemplating existence versus non-existence.
{{< /code >}}

便利っ ٩(๑❛ᴗ❛๑)۶

