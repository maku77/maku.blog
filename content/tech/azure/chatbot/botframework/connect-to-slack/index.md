---
title: "チャットボット: 作成したチャットボットを Slack に接続する"
url: "/p/rtxqceq"
linkTitle: "作成したチャットボットを Slack に接続する"
date: "2019-06-20"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Slack"]
description: "Bot Builder SDK を使って作成したチャットボットを Slack チャンネルに接続することで、Slack ワークスペース内のユーザーがチャットボットと会話できるようになります。"
weight: 235
---

Slack に Bot Framework で作成したボットを接続する
----

{{< image w="700" border="true" src="connect-to-slack-001.png" title="ボットとのダイレクトメッセージによる会話" >}}

Microsoft Azure 上に作成したボットアプリを Slack に接続するには、下記の Bot Service 公式ドキュメントで説明されている手順に従ってください。

- [（日本語）ボットを Slack に接続する - Bot Service ｜ Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/bot-service/bot-service-channel-connect-slack?view=azure-bot-service-4.0)
- [（英語）Connect a bot to Slack - Bot Service ｜ Microsoft Docs](https://docs.microsoft.com/en-us/azure/bot-service/bot-service-channel-connect-slack?view=azure-bot-service-4.0)

接続のおおまかな手順は下記のような感じです。

1. Slack アプリを作成する（何らかのワークスペースに所属させる形で作成する）
2. Slack アプリにボット用のユーザーを登録する
3. Azure ポータルから、Web アプリボットのチャンネルとして Slack を追加（Slack アプリ側の Client ID、Client Secret、Verification Token をコピペすれば OK）

この作業が終わると、ボット（アプリ）が Slack のワークスペースに参加している状態になります。
その時点ではどのチャンネルにも参加していませんが、ダイレクトメッセージを使って一対一でボットと会話することができます。

特定のチャンネルで会話している最中に `@ボット名` と話かけると、そのチャンネルにボットを招待することができます。
チャンネルにボットが参加すると、後はそのチャンネルに対してつぶやくだけでボットが反応するようになります。


わかりにくいところの補足
----

図入りで説明されているので、特に迷うことはないと思いますが、Microsoft の Bot チームのドキュメント通りにはうまくいかない部分があるので若干補足しておきます。

### ボットハンドル

「ボットのイベントをサブスクライブ」するという項目で、下記のような Request URL で[ボットハンドル (YourBotHandle)](https://docs.microsoft.com/ja-jp/azure/bot-service/bot-service-resources-identifiers-guide?view=azure-bot-service-4.0#bot-handle) を指定するところがあります。

{{< code >}}
https://slack.botframework.com/api/Events/{YourBotHandle}
{{< /code >}}

ドキュメント内では、ボットハンドルは [ボットポータル](https://dev.botframework.com/bots) 上でボットを選択すると確認できると記述されていますが、そもそもボットを [Azure ポータル](https://portal.azure.com/) から作成しているようなケースでは、ボットポータル上でボットを参照することができません。

{{< image border="true" src="connect-to-slack-002.png" >}}

ボットハンドルには、Azure ポータル上で見える Web アプリボットリソースの名前（上図）と同じものを指定すれば OK です。
Azure ポータル上でリソース名を確認しましょう。

