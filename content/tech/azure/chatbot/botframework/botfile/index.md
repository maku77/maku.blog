---
title: "チャットボット: MS Bot Framework の .bot ファイルで接続情報を管理する"
url: "/p/8choj4w"
date: "2019-04-03"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
description: "Microsoft の Bot Framework を使用してチャットボットを作成するときは、.bot ファイルという設定ファイルに各種サービスへの接続情報を記述します。"
weight: 232
---

エミュレータのための .bot ファイル
----

[Bot Framework Emulator](https://github.com/Microsoft/BotFramework-Emulator) は、自分自身（チャットクライアント）が接続するボットサーバのアドレスを `.bot` ファイルから取得します。

{{< image w="600" src="botfile1.png" >}}

`.bot` ファイルの中には、`"type": "endpoint"` というエンドポイント定義が 1 つ以上記述されており、ここには**ボットプログラムのアドレス**が記述されています。
エミュレータの設定で、どのエンドポイント設定を使用するかを切り替えることで、実際に接続するボットを使い分けることができます。
典型的には、開発時にローカルホスト上で動作させたボットサーバに接続するための **`development`** と、実稼働用に Azure 上で動作させたボットサーバに接続するための **`production`** というエンドポイントを定義します。

{{< image border="true" src="botfile2.png" title="エミュレータ上でのエンドポイント切り替え" >}}


ボットプログラムのための .bot ファイル
----

`.bot` ファイルは、ボットプログラムからも利用されます（こちらの方がメイン）。

{{< image w="700" src="botfile3.png" >}}

`.bot` ファイルには、**LUIS や QnA Maker のサービスを利用するためのエンドポイント情報**（アドレスやエンドポイントキー）が定義されており、ボットプログラムはこれらの情報を使って各サービスの API を利用します。
こららの情報は、ボット自体がローカルホスト上で動作していても、Azure 上で動作していても同様に利用されます。

また、ここでもボット自体の endpoint エントリが参照され、各種チャンネル（チャットクライアント）がボットにアクセスするときの認証処理のために使用されます。
このあたりの処理は、Bot Builder SDK を使ってボット実装を行っていれば、Adapter クラスとして抽象化されるため、特に意識せずに実装することができます。


.bot ファイルに LUIS や QnA Maker の接続設定を記述する
----

`.bot` ファイルは XML ファイルなので、フォーマットさえ理解すればテキストエディタなどで編集してしまうことはできますが、エンドポイントキー部分の復号化・暗号化が必要だったりして面倒です。
Bot Framework Emulator には、.bot ファイルの内容を GUI で編集する機能が付いているのでこの機能を使うのがよいでしょう。

### LUIS/QnA Maker サービスへの接続情報を設定する

`.bot` ファイルをダブルクリックしてエミュレータを起動したら、左上の <samp>SERVICES</samp> の <samp>＋</samp> ボタンを押すことで各サービスの接続設定を行うことができます。

{{< image src="botfile4.gif" border="true" title="LUIS の接続設定を .bot ファイルに追加する" >}}

Azure portal にログインしていない状態だと、ログインを求められるので、そのまま指示にしたがって Azure portal にログインします。
手順に従って LUIS/QnA Maker の接続情報を自動取得 or 手動入力します。
無事設定ができると、`.bot` ファイルの中に、次のような感じで LUIS/QnA Maker 接続のためのエンドポイント情報が保存されるはずです。

{{< code title="maku-bot.bot（一部抜粋）" lang="json" >}}
{
    "services": [
        {
            "type": "luis",
            "appId": "c5514855-fa6b-8f4c-c695-9ac7d3519412",
            "authoringKey": "Z5JHftI7h6CqW19iorgMeQ==!qmpIv7UyWR5KizeKQ/P5WotwnIw6LXYdhsSfRXbAjwMYoGTXVNd9uw/r8II5ph2B",
            "id": "c5514855-fa6b-8f4c-c695-9ac7d3519412",
            "name": "maku-luis-sample",
            "region": "westus",
            "subscriptionKey": "wGtV6dpEVwVRSn/1jiuqUw==!0KjfCqHek8hruxRyGdG9cMhYJqEpPp5hY/2or4vfoIRi2fvBNZavAvz6odDdG64Y",
            "version": "0.1"
        },
        {
            "type": "qna",
            "endpointKey": "etu4UQESnuvypEFZe+i+Qw==!ETEcbh/FkXtnQRjnHJlNXbYtD7Yha7WboqBX6skYAeXVh07qLO53UeqBPtohVZFQ",
            "hostname": "https://maku-qna-resource.azurewebsites.net",
            "kbId": "c78ec3e7-a58d-ca44-dbb6-e7805d8130c6",
            "name": "maku-qna-sample",
            "subscriptionKey": "LiFCYQxahtplAwEFQvLEWg==!QHxCEIby0NjXr4jsFbuDhGSzdvBslfWxDlfomiGiyHIa+T4keDRvTpxdtseddvmt",
            "id": "231"
        }
    ],
}
{{< /code >}}


### エンドポイントキーの暗号化について

LUIS の `authoringKey`/`subscriptionKey`、QnA Maker の `endpointKey`/`subscriptionKey` の部分は暗号化されていますが、この**暗号化処理はエミュレータが自動でやってくれます**。
`.bot` ファイルの内容を手動で書き換えた場合は、[botbuilder-tools に付属している msbot コマンド](https://github.com/Microsoft/botbuilder-tools/blob/master/packages/MSBot/docs/bot-file-encryption.md)を使って暗号化できます。

{{< code title="コマンドラインで .bot ファイルを暗号化する場合" >}}
$ msbot secret -b maku-bot.bot --secret XXXXXX
{{< /code >}}


ボットプログラムから .bot ファイルを読み込む
----

ボットプログラムから .bot ファイルを読み込んで、LUIS や QnA Maker への接続情報を取得することができます。
.bot ファイルの読み込み方に関しては下記の記事を参照してください。

- [LUIS や QnA Maker サービスへの接続情報を .bot ファイルから取得する](/p/o2bqajv)

