---
title: "チャットボット (2-2) Web App Bot で生成されたボットのコードを編集する"
url: "/p/bpqkm2o"
linkTitle: "チャットボットのコードを編集する"
date: "2019-03-20"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 222
description: "Azure 上に Web App Bot リソースを生成すると、ボットのテンプレートコードが生成されます。ここでは、そのコードを編集する方法を説明します。"
---

チャットボットのソースコードをダウンロードする
----

下記のページの手順に従い、[Azure ポータル](https://portal.azure.com/)上で Web App Bot のリソースを作成すると、自動的に Echo Bot のテンプレートコードが生成されているはずです（選択したテンプレートの種類によって変わりますが、ここでは Node.js 版の Echo Bot テンプレートを指定しているとします）。

- [Azure でボットをホストするための Web App Bot リソースを作成する](/p/tttou4o)

ボットプログラムを作成する場合は、基本的にはこのテンプレートコードをベースにして修正を行っていくのがよいでしょう。
自動生成されたコードは、Azure ポータルから下記のように辿ると ZIP アーカイブでダウロードすることができます。

1. <samp>すべてのリソース</samp> を選択
2. 対象の <samp>Web アプリボット</samp> リソースを選択（下記の例では `maku-bot`）
3. <samp>ボット管理</samp> の <samp>ビルド</samp> を選択
4. <samp>ボットのソースコードをダウンロードする</samp> のボタンをクリック

{{< image w="700" border="true" src="edit-bot-code1.png" title="ボットコードをダウンロード" >}}


bot ファイルの復号化・暗号化
----

### botFileSecret とは

ダウンロードした ZIP アーカイブの中には、ボットサーバの設定ファイルである `.bot` ファイルが含まれています。

このファイルは、ローカルでボットサーバを立ち上げたり、エミュレータからそのサーバに接続するときの設定ファイルとして使用するのですが、**自動生成された `.bot` ファイルは、接続情報などの値が暗号化されています**。
**`.bot` ファイルの復号化、および暗号化に使用されているキーのことを botFileSecret と呼びます**。

ボットサーバやエミュレータを正しく起動するためには、この botFileSecret を使って `.bot` ファイルをあらかじめ復号化しておくか、環境変数などでキーを設定しておく必要があります。

`.bot` ファイルを復号化した場合は、Azure 上の Web App Bot サービスにデプロイする前に、忘れずに暗号化しておく必要があります。


### botFileSecret の取得

初期の暗号化に使用されている `botFileSecret` は、Azure ポータルから下記のように辿ることで参照することができます。

1. <samp>すべてのリソース</samp> を選択
2. 対象の <samp>Web アプリボット</samp> リソースを選択（下記の例では `maku-bot`）
3. <samp>App Service 設定</samp> → <samp>アプリケーション設定</samp> を選択
4. <samp>アプリケーション設定</samp> のセクションの中に <samp>botFileSecret</samp> という項目が見つかります

{{< image w="700" border="true" src="edit-bot-code-secret.png" title="botFileSecret の確認" >}}

ちなみに、ここの変数は自由に変更できるようになっています。
`.bot` ファイルの暗号化に使うキーを変更したときは、ここの設定も忘れずに変更しておく必要があります。

### .bot ファイルの復号化・暗号化

`.bot` ファイルの暗号化や復号化を行うためのコマンドラインツールとして、**`msbot`** というツールが提供されています。
Node.js の `npm` コマンドを使用して以下のようにインストールすることができます。

{{< code title="msbot コマンドのインストール" >}}
npm install -g msbot
{{< /code >}}

`.bot` ファイルの復号化と暗号化は下記のように行います。

{{< code title=".bot ファイルの復号化" >}}
msbot secret --bot <bot-file> --secret "<bot-file-secret>" --clear
{{< /code >}}

{{< code title=".bot ファイルの暗号化" >}}
msbot secret --bot <bot-file> --secret "<bot-file-secret>" --new
{{< /code >}}



ローカル PC 上でボットサーバを起動する
----

ローカル環境でボットの開発を進めるには、ローカル PC 上でボットサーバを立ち上げて、エミュレータから接続してテストを行います（もちろん、エミュレータから Azure 上のボットサービスに接続することもできます）。

ダウンロードした ZIP アーカイブの中には、ボットサーバのエントリポイントとなる `index.js` や、パッケージ依存関係を記述した `package.json` が含まれています。
まず、`npm` コマンドを使って必要な Node パッケージ群をインストールします（参考: [まくまく Node.js ノート（npm コマンドの使い方など）](https://maku77.github.io/nodejs/)）。

{{< code title="ボットサーバに必要な Node パッケージをインストール" >}}
$ npm install
{{< /code >}}

次に、ボットサーバが使用する `.bot` ファイルのパスと、復号化のためのキーを、それぞれ **`botFilePath`** 環境変数と **`botFileSecret`** 環境変数に設定しておく必要があります。
あるいは、下記のようにプロジェクトのルートディレクトリに、**`.env`** ファイルを作成して、その中に環境変数を設定することもできます。

{{< code title=".env" >}}
botFilePath=./maku-bot.bot
botFileSecret=skaE77l/x98D/IdSYXPaiqe4YxsGhiSpXGSuKZsclE3=
{{< /code >}}

`.env` ファイルに botFileSecret の情報を記述する場合は、`.env` ファイルを間違えて Azure 上にデプロイしてしまったり、Git リポジトリにコミットしてしまわないように注意してください。
デフォルトで生成される `.gitignore` のエントリには、`.env` ファイルを Git のコミット対象にしないよう設定されています。

ここまでの設定が終わったら、**`npm start`** コマンドでボットサーバを起動することができます。

{{< code >}}
$ npm start

> echobot-with-counter@1.0.0 start /Users/maku/maku-bot
> node index.js

restify listening to http://[::]:3978

Get Bot Framework Emulator: https://aka.ms/botframework-emulator

To talk to your bot, open echoBot-with-counter.bot file in the Emulator
{{< /code >}}

次に、エミュレータからこのボットサーバに接続します。


Bot Framework Emulator からボットサーバに接続する
----

### ローカルのボットサーバへ接続する

チャットボットのテストは、[Bot Framework Emulator](https://github.com/Microsoft/BotFramework-Emulator) を使って行います。
このエミュレータがインストールされていれば、`.bot` ファイルをダブルクリックするだけで、ローカルに立ち上げたボットサーバ、あるいは Azure 上の Web App Bot サービスに接続することができます。

暗号化された `.bot` ファイルをダブルクリックしてエミュレータを立ち上げようとすると、下記のようなダイアログが表示されるので、ここに botFileSecret を入力してください。

{{< image w="400" border="true" src="edit-bot-code-emu-secret.png" title="エミュレータでの botFileSecret の入力" >}}

デフォルトでは、下記のように、ローカルで立ち上げたボットサーバに接続しに行くはずです。
左に表示されている、ENDPOINT の `development` が、ローカルへの接続設定を表しています。

{{< image w="700" src="edit-bot-code-emu-local.png" title="エミュレータからローカルのボットーサーバに接続" >}}

ここまでできるようになれば、チャットボットの開発を本格的に進められるようになっているはずです。

### Azure 上の Web App Bot サービスへ接続する

Azure で自動生成された `.bot` ファイルには、Azure 上の Web App Bot サービスに接続するための情報も記載されています（エンドポイント名 `production` として登録されています）。
接続のためのアプリ ID やキーも含まれているため、`.bot` ファイルがあれば接続設定はすべて揃うようになっています。

ひとつだけ、**エミュレータからリモートのサーバへ接続する場合は、ngrok というプロキシソフトをインストールしておく必要があります**。
エミュレータを立ち上げたときに、LOG の欄にも `ngrok` が設定されていないというメッセージが表示されています。

{{< image w="600" border="true" src="edit-bot-code-emu-ngrok.png" title="エミュレータの ngrok の設定を促すメッセージ" >}}

`ngrok` は下記からダウンロードしてインストールしてください。

- [ngrok](https://ngrok.com/)

`ngrok` のインストールが完了したら、エミュレータの設定アイコンをクリックし、`ngrok` の実行ファイルのパスを設定します。

{{< image w="600" border="true" src="edit-bot-code-ngrok-path.png" title="エミュレータに ngrok のパスを設定する" >}}

この設定が終われば、ENDPOINT の欄から **`production`** を選択することで、Azure 上に構築した Web App Bot サービスに接続できるようになります。


{{% private %}}
#### プロキシ環境の場合

`ngrok` をプロキシ環境から使用する場合は、さらに `ngrok` に関してプロキシ情報を設定しておく必要があります。
設定ファイルとして下記のようなファイルを作成し、

- `~/.ngrok2/ngrok.yml` (Linux, macos)
- `%HOMEPATH%\.ngrok2\ngrok.yml` (Windows系)

中に次のように記述しておけば OK です。

{{< code lang="yaml" title="ngrok.yml" >}}
http_proxy: "http://proxy.example.com:<port>"
http_proxy: "http://<user>:<password>@proxy.example.com:<port>"  #ユーザ/PWが必要な場合
{{< /code >}}
{{% /private %}}

