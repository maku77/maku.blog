---
title: "チャットボット: 作成したチャットボットを LINE に接続する"
url: "/p/asuzg7k"
linkTitle: "作成したチャットボットを LINE に接続する"
date: "2019-04-19"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
description: "作成したチャットボットをチャンネル（LINE、Slack、Skype などのチャットクライアント）に接続することで、世界中のユーザーがチャットボットにアクセスできるようになります。ここでは、Microsoft Azure 上に作成したボットアプリを LINE に接続する方法を説明します。"
weight: 234
---

LINE に Messaging API チャネルを作成する
----

自作したチャットボットアプリを LINE から「友だち」として見えるようにするには、まず LINE 側に「プロバイダー」を作成し、そこに「**Messaging API チャネル**」を追加する必要があります。
この Messaging API チャネルは、LINE アプリから「友だち」として見える単位だと考えればよいでしょう。

### LINE に開発者として登録する

LINE のプロバイダー登録作業などは、下記の **LINE Developer Console** から行うことができます。
初めてアクセスする場合は、開発者としての登録を求められるので、LINE アカウントでログインして開発者情報を入力してください。

- [LINE Developer Console](https://developers.line.biz/console/register/messaging-api/provider/)

### プロバイダーを新規作成する

LINE に開発者登録できたら、<samp>新規プロバイダー作成</samp> のボタンを押して、プロバイダーを作成します。

{{< image w="700" border="true" src="connect-to-line-001.png" >}}

### Messaging API チャネルを追加する

プロバイダーの作成が終わったら、そこに **Messaging API チャネル** を追加します。

{{< image w="700" border="true" src="connect-to-line-002.png" >}}

チャネルの作成時には、アプリアイコンやアプリ名を自由に登録することができます。
アプリ名は一度設定すると 7 日間は変更できないようなので慎重に決めましょう（アイコンは 1 時間経てば変更できます）。

{{< image w="500" border="true" src="connect-to-line-003.png" >}}

下のように、プロバイダー上に Messaging API チャネルが追加されていれば OK です。

{{< image w="500" border="true" src="connect-to-line-004.png" >}}

### スマホの LINE に友達登録する

Messaging API のチャネルを選択し、<samp>チャネル基本設定</samp> タブを見ると、LINE アプリ用の QR コードが見つかります。

{{< image w="500" border="true" src="connect-to-line-005.png" >}}

この QR コードを LINE アプリのカメラで読み取ることで、チャットボットを友達追加することができます。

{{< image w="200" border="true" src="connect-to-line-006.png" >}}

ただ、まだボットアプリ本体（自分で作成したボット実装）には接続していないので、話しかけても何も返答してくれません。
下記のようなテンプレートメッセージだけが表示されます。

> メッセージありがとうございます。
> 申し訳ございませんが、このアカウントから個別に返信することはできません。
> 次回の配信をお楽しみに。

{{< image w="200" border="true" src="connect-to-line-007.png" >}}


LINE のチャネルを Azure 上の自作ボットに接続する
----

LINE の Developer Console から Messaging API チャネルを作成すると、LINE アプリからチャットボットが見えるようになります。
ただし、この時点では何も頭脳を持たない空っぽのボットです。

この Messaging API チャネルを、自作したボットアプリ（Azure 上にした配置したプログラムなど）に接続することで、LINE アプリと自作したボットとの間で会話できるようになります。

### LINE チャネルから接続用の情報を取得する

Azure の Web アプリボットのチャンネルとして LINE を登録するには、LINE 側で生成された **チャンネルシークレット** と、**チャンネルアクセストークン** が必要です。
先ほど作成した LINE の Messaging API チャネルのページを開き、<samp>チャンネル基本設定</samp> のタブを選択するとこれらの値を確認できます。

{{< image border="true" w="500" src="connect-to-line-008.png" title="チャンネルシークレットの確認" >}}
{{< image border="true" w="500" src="connect-to-line-009.png" title="チャンネルアクセストークンの確認" >}}

アクセストークンが表示されていない場合は、<samp>再発行</samp> ボタンを押すと表示されます。

### Azure 上の Web アプリボットに LINE を接続する

LINE 側のチャンネルシークレットと、アクセストークンを確認したら、Azure 上のチャットボットアプリを開き、<samp>チャンネル</samp> → <samp>LINE</samp> を選択します。

{{< image w="600" border="true" src="connect-to-line-010.png" title="Azure 上の Web アプリボットへの LINE チャンネルの追加" >}}

<samp>LINE の資格情報を入力</samp> というセクションに、LINE 側で発行された <samp>チャンネルシークレット</samp> と <samp>アクセストークン</samp> をコピペしてください。

{{< image w="600" border="true" src="connect-to-line-011.png" title="Azure 上の Web アプリボットへの LINE チャンネルの追加" >}}

最後に、LINE との双方向通信を行えるようにするために、上の図の緑枠で表示された **Webhook URL を LINE 側に登録する**必要があります。
URL をコピーして、下記のように LINE 側のチャンネル基本設定のページで、<samp>Webhook URL</samp> を設定し、<samp>Webhook 送信</samp> を「利用する」に変更します。

{{< image w="600" border="true" src="connect-to-line-012.png" title="LINE チャンネルに webhook URL を登録" >}}

これで、LINE アプリと、自作したボットが繋がります！

{{< image w="200" border="true" src="connect-to-line-013.png" >}}

あれ？「返信することはできません」の自動応答メッセージがまだ出たままですね。
この自動応答メッセージは、LINE の <samp>チャネル基本設定</samp> の <samp>自動応答設定</samp> からオフにすることができます。

