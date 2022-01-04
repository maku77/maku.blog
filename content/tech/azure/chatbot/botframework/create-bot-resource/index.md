---
title: "チャットボット (2-1) Azure でボットをホストするための Web App Bot リソースを作成する"
url: "/p/tttou4o"
linkTitle: "Azure でチャットボットをホストする"
date: "2019-03-19"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 221
description: "Bot Builder SDK などを使用して作成したチャットボットは、Azure 上のボットサービスを使用してホスティングすることができます。Azure 上でボットサービスを動作させることにより、様々なチャンネル（Skype や Teams、Slack など）からボットを利用できるようになります。もちろん、開発時にエミュレータから接続することも可能です。"
---

{{% private %}}
参考 Azure ドキュメント

- [Azure App Service プランの概要](https://docs.microsoft.com/ja-jp/azure/app-service/overview-hosting-plans)
- [App Service プラン（料金表）](https://azure.microsoft.com/ja-jp/pricing/details/app-service/plans/)
{{% /private %}}


Web App Bot リソースを作成する
----

作成したボットプログラムは、Azure 上の Web App Bot リソース上で動作させることができます。
このリソースのことを特にボットサービスと呼んだりします。

[Azure ポータル](https://portal.azure.com/) にログインし、下記のように辿ることで Web App Bot リソースの作成画面を表示できます。

1. <samp>＋リソースの作成</samp>
2. <samp>AI + Machine Learning</samp>
3. <samp>Web App Bot</samp>

{{< image w="700" border="true" src="create-bot-resource1.png" >}}

下記のような Web App Bot リソースの設定画面が表示されるので、1 つずつ入力していきます。

{{< image w="500" border="true" src="create-bot-resource2.png" >}}

ボット名
: 任意のボット名称。後から自由に変更することができるので、自分のわかりやすい名前を付けておけば OK です。例: `maku-bot`

サブスクリプション
: 月額の請求先となるサブスクリプションを選択します。最初の Azure トライアル期間であれば、`Free Trial` などを選択できるはずです。

リソースグループ
: この Web App Bot リソースを所属させるリソースグループを選択します。存在しない場合は <samp>新規作成</samp> のリンクをクリックして新しく作成します。

場所
: リソースグループの場所を選択。ここでは、地理的に近い `Japan East` を選択してます。

価格レベル
: チャンネルに応じたメッセージ制限解除のためのプラン設定です。
スタンダードチャンネル（Skype、Cortana、Teams、Facebook、Slack などの一般的なクライアント）とのやりとりは無制限なので、**通常は F0 の無料プランを選択しておけば OK** です。
一方で、プレミアムチャンネル（ユーザ独自のチャンネルや、Web ページ埋め込みチャットボットなど）と多くのメッセージをやりとりする予定がある場合は、有料の <samp>S1</samp> プランを選択する必要があります。
無料の <samp>F0</samp> だと 1 か月に 10,000 メッセージまでの制限があります。
（参考: [Standard channels と Premium channels について](https://azure.microsoft.com/en-us/pricing/details/bot-service/)）

アプリ名
: ボットサービスのエンドポイント URL となる `XXX.azurewebsites.net` の `XXX` の部分を指定します。
URL なので、**世界中で一意**である必要があります。
アプリ名は後から変更することはできません。

ボットテンプレート
: 自動生成されるボットのソースコードの種類を選択します。ベースとする SDK バージョンや、言語を指定します。
今回は、最もシンプルな構成になるように、<samp>SDK v4</samp>、<samp>Node.js</samp>、<samp>Echo Bot</samp> と選択しています。

App Service プラン
: **めっちゃ重要。**
App Service プランは、ボットを稼働させるために使用するコンピューティングリソース (VM) の課金プランです。
**サーバ使用料**と言えば分りやすいかもしれません。
上記で設定した「価格レベル」はあくまでチャットのメッセージ数制限を取り払うためのプラン設定であり、サーバの代金はこちらのプランによって変化します。
QnA Maker などで App Service プランを作成している場合は、それに乗っかる形で同じものを選択することができます。
App Service プランが存在しない場合はここから新規作成することができます。
App Service プラン名は、分かりやすいように `maku-bot-service-plan` のような名前を付けておくことをオススメします。
**デフォルトでは有料の価格レベルである `S1 Standard` で作成されます。**
お試しで使うのであれば、ここは無料の `F1` に変更しておいた方がよいのですが、この画面からは選択できないようなので、後から <samp>App Service プラン</samp>のリソースのページから価格レベルを変更しておく必要があります（後述）。これはワナかぁ～^^;

Azure Storage
: ボットプログラムがステータスを保持するために使用するストレージを選択します。最初は<samp>新規作成</samp>を選択しておけば OK です。

Application Insights
: アプリのトラフィックなどを分析するリソースを作成するかどうか。最初は必要ないと思ったら、余計なリソースを増やさないように<samp>オフ</samp>にしておけばよいです。

Microsoft アプリ ID とパスワード
: <samp>自動作成</samp>にしておけば OK。[Application Registration Portal](https://apps.dev.microsoft.com/) で別途作成したものを設定することも可能です。ちなみに、これまでに作成したアプリの ID のリストもそのサイトで確認できます。


App Service プランの価格レベルを変更する
----

上記で App Service サービスプランを新規作成した場合は、価格レベルが `S1 Standard` になっているので、**お試しで使うのであれば、忘れずにフリーの価格レベル `F1 Free` に変更しておきましょう**。

まず、Azure ポータルの<samp>すべてのリソース</samp>から、先ほど作成した <samp>App Service プラン</samp> のリソースのページを開きます（下記の例では `maku-bot-service-plan`）。

{{< image w="700" border="true" src="create-bot-resource3.png" >}}

App Service プランのリソースページが開いたら、

1. <samp>スケールアップ</samp>
2. <samp>開発／テスト</samp>
3. <samp>F1（無料）</samp>
4. <samp>適用</samp> ボタン

と順番にクリックしていけば価格レベルが `F1 Free` に変更されます。

{{< image w="700" border="true" src="create-bot-resource4.png" >}}

<samp>スケールアップ</samp>から入るところがなかなか分かりにくいですね。
でも、**これをやっておかないと、知らない間にガンガン課金されていた、ってことになりかねません**。


Web App Bot の動作確認
----

Web App Bot のリソースの作成が終わったら、チャットボットの動作確認をしておきましょう。
下記のようにすると、Azure ポータル上で Web チャット UI を表示してテストすることができます。

1. <samp>すべてのリソース</samp> を選択
2. 作成した <samp>Web App Bot</samp> のリソースを選択
3. <samp>Web チャットでテスト</samp> を選択

{{< image w="700" border="true" src="create-bot-resource5.png" >}}

今回は、Echo Bot のテンプレートを使用したので、「こんにちわ」と入力したときに、「You said "こんにちわ"」という応答が返ってきています。

次のステップはこちら：

- [Web App Bot で生成されたボットのコードを編集する](/p/bpqkm2o)
- [Azure にチャットボットをデプロイする](/p/gxm9shf)


