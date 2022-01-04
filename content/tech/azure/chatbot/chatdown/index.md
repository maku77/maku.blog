---
title: "チャットボット: Chatdown（.chat ファイル）を使ってボットの会話をデザインする"
url: "/p/a6yzskr"
linkTitle: "Chatdown（.chat ファイル）を使ってボットの会話をデザインする"
date: "2019-03-27"
tags: ["チャットボット", "Azure", "Bot Builder SDK"]
description: "Chatdown というフォーマットで会話テキストを作成しておくと、チャットクライアント上でそのやりとりを再現することができます。プロト段階で表示イメージを確認しておきたいときなどに便利です。"
weight: 239
---

{{% private %}}
参考

- [CLI ツールを使用したボットの管理 - Bot Service | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/bot-service/bot-builder-tools?view=azure-bot-service-4.0)
- https://codepunk.io/chatdown-for-the-bot-framework-introduction-and-processing-multiple-files/
- [How to Write Chatdown Markup - ChatBot Pack](https://www.chatbotpack.com/chatdown)
{{% /private %}}

Chatdown フォーマットとは
----

[Chatdown フォーマット](https://github.com/Microsoft/botbuilder-tools/tree/master/packages/Chatdown)は、会話の設計をテキストベースで行うことを意図したフォーマットです。
拡張子は **`.chat`** で、下記のような感じで会話例を記述していきます。

{{< code title="sample.chat" >}}
user=Joe
bot=LulaBot

bot: Hi!
user: yo!
bot: [Typing][Delay=3000]
Greetings!
What would you like to do?
* update - You can update your account
* List - You can list your data
* help - you can get help

user: I need the bot framework logo.
{{< /code >}}

上記のように、チャットボットがタイプ中であることや、応答までのディレイなどもデザインすることができます。
Markdown 形式で書式設定できるようになっているのが Chatdown という名前の由来ですね。


Bot Framework Emulator で会話を再現する
----

[Bot Framework Emulator](https://github.com/Microsoft/BotFramework-Emulator) で会話ファイルを読み込むと、チャット UI 上で会話を再現することができます。
ただし、Emulator が読み込むことのできるファイルは `.transcript` 形式のファイルなので、あらかじめ `.chat` ファイルを変換して `.transcript` ファイルを生成しておく必要があります。

### chatdown ツールによる transcript ファイルの生成

**`chatdown`** というコマンドラインツールを使用して、`.chat` ファイルから `.transcript` ファイルを生成することができます。

- [botbuilder-tools/packages/Chatdown at master · Microsoft/botbuilder-tools](https://github.com/Microsoft/botbuilder-tools/tree/master/packages/Chatdown)

`chatdown` コマンドは、Node パッケージとして公開されているので `npm` コマンドで簡単にインストールすることができます。

{{< code title="chatdown コマンドのインストール" >}}
$ npm install -g chatdown
{{< /code >}}

下記のように実行すると、`.chat` ファイルから `.transcript` ファイルを生成できます。

{{< code title=".transcript ファイルを生成" >}}
$ chatdown sample.chat > sample.transcript
{{< /code >}}

### Bot Framework Emulator で会話を再現する

`.transcript` ファイルが用意できたら、[Bot Framework Emulator](https://github.com/Microsoft/BotFramework-Emulator) で読み込むことで会話を再現することができます。

1. Bot Framework Emulator を起動する
2. メニューから <samp>File</samp> → <samp>Open transcript...</samp> と辿り、`.transcript` ファイルを選択する

すると、下記のようにチャットクライアント上であたかも実際の会話があったかのように表示されます。

{{< image w="600" border="true" src="chatdown.png" title="Bot Framework Emulator による会話の再現" >}}


トラブルシューティング: chatdown コマンドがフリーズする場合
----

プロキシ環境下などでは、`chatdown` コマンドを実行したときに処理が進まなくなることがあるようです（Version 2.0.0 で発生することを確認）。

コードを調べたところ、`latest-version` モジュールの `latestVersion` 関数を同期呼び出ししており、そこでフリーズしてしまっているようです。

このような場合は、スクリプト本体である `chatdown.js` ファイルを、下記のようにコメントアウトすれば動かせるようになります。

{{< code lang="js" title="%APPDATA%\npm\node_modules\chatdown\bin\chatdown.js" >}}
async function runProgram() {
    const args = minimist(process.argv.slice(2));

    if (args.prefix) {
        intercept(function(txt) {
            return `[${pkg.name}]\n${txt}`;
        });
    }

//    let latest = await latestVersion(pkg.name, { version: `>${pkg.version}` })
//                        .catch(error => pkg.version);
//    if (semver.gt(latest, pkg.version)) {
//        process.stderr.write(chalk.default.white(`\n     Update available `));
//        process.stderr.write(chalk.default.grey(`${pkg.version}`));
//        process.stderr.write(chalk.default.white(` -> `));
//        process.stderr.write(chalk.default.greenBright(`${latest}\n`));
//        process.stderr.write(chalk.default.white(`     Run `));
//        process.stderr.write(chalk.default.blueBright(`npm i -g ${pkg.name} `));
//        process.stderr.write(chalk.default.white(`to update.\n`));
//    }
//    ...
{{< /code >}}

