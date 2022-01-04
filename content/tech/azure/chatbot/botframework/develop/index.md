---
title: "チャットボット: Azure ポータルで生成されるボットのテンプレートコードを解読＆リファクタしてみる"
url: "/p/iob68qa"
date: "2019-03-26"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 231
description: "Bot Builder SDK を使って本格的にボットプログラムを作成する前に、ボットの初期化部分（各種設定の読み込み）と、メインの応答部分を切り分けて実装できるようにします。"
---

Azure が生成するテンプレートコードを理解する
----

下記の手順に従って Azure ポータル上で Web App Bot リソースを作成すると、ボットプログラムのテンプレートとして `index.js` や `bot.js` などのコードが自動生成されます。

- [Azure でボットをホストするための Web App Bot リソースを作成する](/p/tttou4o)

ボットサーバのエントリポイントとなる `index.js` には、設定情報の読み取りと Web サーバの立ち上げ処理が記述されており、`bot.js` の方にはボットの応答処理を記述するようになっています。

つまり、基本的にボットの作成者は `bot.js` の方にボットのコア部分を実装していけばよいのですが、LUIS や QnA Maker などのサービスと連携する場合は、それぞれの初期化処理が必要であり、結局のところ `index.js` 側の実装に関してもある程度理解しておく必要があります。

Azure ポータルで自動生成される `index.js` は決して理解しやすいものではないので（少なくとも記述時点では）、ここでは **`index.js` の内容を理解する目的と、わかりやすくリファクタする目的を兼ねて、ボットのベースとなるコードを作成**していきます。

最終的には Azure が生成するテンプレートコードと同様の振る舞いになることを想定しています（少なくとも、環境変数の名前などは合わせておいた方がよいです）。


全体の流れ
----

ボットプログラムのエントリポイントとなる `index.js` では、大まかに下記のような処理を行います。

1. `.env` ファイルを読み込み、環境変数の情報とマージする
2. 1 の情報を基に、`.bot` ファイルを読み込む
3. 2 の情報を基に、ボットサーバーを立ち上げる

Azure ポータルで生成されるテンプレートコードでは、上記の処理をすべて `index.js` の中で行っているのですが、ここでは設定の読み込み部分と、ボットサーバの立ち上げ部分を明確に分離してみます。

1. **`config.js`**: 環境変数や `.env` ファイルの情報を基に、`.bot` ファイルの設定を読み込む。
2. **`index.js`**: 上記の設定情報を基にボットサーバを立ち上げる。

という感じにします。


ステップ(1) 環境変数あるいは .env ファイルの読み込み (config.js)
----

ボットサーバーは、`.bot` ファイルに記述された設定情報に基づいて動作します。
この `.bot` ファイルを読み込むための情報（ファイルパスなど）は、環境変数や `.env` ファイルに記述されているため、まずはこれらを読み込まなければいけません。

環境変数や `.env` ファイルでは、下記のような情報が設定されています。
少なくとも、`botFilePath` と `botFileSecret` の設定は必須とされています。

| 変数名 | 意味 | 設定例 |
| ---- | ---- | ---- |
| `botFilePath` | `.bot` ファイルのパス | `./maku-bot.bot` |
| `botFileSecret` | `.bot` ファイルの復号化／暗号化キー | `9l/b88E/hiSpcIdxskqe4YSaE7sGSYXPaiuKZslE3XG=` |
| `NODE_ENV` | 実行環境（`.bot` のどのセクションを読むか） | `development`: ローカルテスト用 (DEFAULT)<br>`production`: 本番環境用 |

下記の `loadEnvFile` メソッドは、上記のような情報を読み込んでオブジェクトとして返します。

{{< code lang="js" title="config.js" >}}
// .env ファイルの設定を環境変数 (process.env) とマージし、
// .bot ファイルを読み込むための情報（パスなど）を返す。
function loadEnvFile() {
  const dotenv = require('dotenv');
  const path = require('path');
  const ROOT_DIR = __dirname;

  // Read .env file and merge info to process.env
  dotenv.config({ path: path.join(ROOT_DIR, '.env') });

  return {
    // Absolute path of .bot file
    botFilePath: path.join(ROOT_DIR, (process.env.botFilePath || '')),
    // Secret key of .bot file
    botFileSecret: process.env.botFileSecret,
    // Name of the endpoint configuration section from the .bot file
    botEnv: process.env.NODE_ENV || 'development',
  }
}
{{< /code >}}


ステップ(2) .bot ファイルの読み込み
----

`.bot` ファイルの読み込みには、Microsoft が提供している `botframework-config` モジュールの **`BotConfiguration`** クラスを使用します。
上記のステップで取得した、`.bot` ファイルのパス (`botFilePath`) と、復号化キー (`botFileSecret`) が必要になります。

下記の `loadBotFile` 関数は、ステップ (1) で取得した情報をパラメータで受け取り、`.bot` ファイルを読み込みます。

{{< code lang="js" title="config.js（続き）" >}}
// 暗号化された .bot ファイルを読み込み、
// 指定されたエンドポイント (developement や production) の設定情報を返す。
function loadBotFile(env) {
  const { BotConfiguration } = require('botframework-config');

  let botConfig;
  try {
      // Read bot configuration from .bot file.
      botConfig = BotConfiguration.loadSync(env.botFilePath, env.botFileSecret);
  } catch (err) {
      console.error(`\nError reading bot file. Please ensure you have valid botFilePath and botFileSecret set for your environment.`);
      console.error(`\n - You can find the botFilePath and botFileSecret in the Azure App Service application settings.`);
      console.error(`\n - If you are running this bot locally, consider adding a .env file with botFilePath and botFileSecret.`);
      console.error(`\n - See https://aka.ms/about-bot-file to learn more about .bot file its use and bot configuration.\n\n`);
      process.exit();
  }

  // Get bot endpoint configuration by service name (such as "development" and "production")
  const endpointConfig = botConfig.findServiceByNameOrId(env.botEnv);

  return {
    appId: endpointConfig.appId || process.env.microsoftAppID,
    appPassword: endpointConfig.appPassword || process.env.microsoftAppPassword,
    channelService: process.env.ChannelService,
    openIdMetadata: process.env.BotOpenIdMetadata,
  }
}
{{< /code >}}

これで、`config.js` が担うべき設定情報の読み込み処理は完成です。
コードの末尾部分では、実際にこれらの関数を呼び出し、モジュールのプロパティとして参照できるように公開しておきます。

{{< code lang="js" title="config.js（続き）" >}}
const env = loadEnvFile();
module.exports = loadBotFile(env);
{{< /code >}}

ボットのエントリポイントとなる `index.js` から、このファイルを `require('./config.js')` で読み込んで使用します。


ステップ(3) ボットサーバーを立ち上げる
----

ここまでのステップで、ボットサーバーを立ち上げるための設定情報を読み込めるようになりました。
次に、実際にその情報を使ってボットを設定し、起動します。

ボットは、**`restify`** モジュールを使って起動した Web サーバーの上で動作します。
Bot Builder SDK として公開されている **`BotFrameworkAdapter`** クラスは、チャンネル（チャットクライアント）と、ボットの本体プログラムを繋ぐアダプタとして動作します。
接続における認証処理などもこのクラスが担ってくれるので、とても重要なクラスです。

下記の `index.js` は、ボットのエントリポイントとなるファイルで、

1. 設定情報の読み込み (上記で作成した `config.js` を使用）
2. その設定を使ってアダプタを初期化
3. ボットの本体となる Bot オブジェクトを生成（後述の `bot.js` を使用）
4. Web サーバを立ち上げ、アダプタでチャンネルとボット本体を接続する

という処理を行います。

{{< code lang="js" title="index.js" >}}
// ボットアダプターを生成する。
// Connector と Bot プログラムの橋渡し、認証処理などを行う。
function createBotAdapter(config) {
  // See https://aka.ms/about-bot-adapter to learn more about bot adapter.
  const { BotFrameworkAdapter } = require('botbuilder');
  const adapter = new BotFrameworkAdapter(config);

  // Catch-all for any unhandled errors in your bot.
  adapter.onTurnError = async (context, error) => {
      // This check writes out errors to console log .vs. app insights.
      console.error(`\n [onTurnError]: ${ error }`);
      // Send a message to the user
      await context.sendActivity(`Oops. Something went wrong!`);
  };

  return adapter;
}

// ボットサーバーを起動する
function startServer(adapter, bot) {
  const restify = require('restify');

  let server = restify.createServer();
  server.listen(process.env.port || process.env.PORT || 3978, function() {
    console.log(`\nBot server listening to ${ server.url }`);
  });

  // Listen for incoming activities and route them to your bot main dialog.
  server.post('/api/messages', (req, res) => {
    adapter.processActivity(req, res, async (context) => {
      await bot.onTurn(context);
    });
  });
}

// エントリポイント
(function main() {
  const config = require('./config.js');
  const adapter = createBotAdapter(config);
  const { Bot } = require('./bot.js');
  const bot = new Bot();
  startServer(adapter, bot);
})();
{{< /code >}}


ステップ(4) ボット本体 (bot.js)
----

最後にボット本体となる `Bot` クラスの実装です（すでに上記の `index.js` から参照していますが）。
ここでは、チャットクライアントから受け取ったメッセージをそのままオウム返ししています。

{{< code lang="js" title="bot.js" >}}
const { ActivityTypes } = require('botbuilder');

class Bot {
  async onTurn(turnContext) {
    const type = turnContext.activity.type;
    if (type === ActivityTypes.Message) {
      const text = turnContext.activity.text;
      await turnContext.sendActivity(`You said "${text}"`);
    } else {
      await turnContext.sendActivity(`[${type} event detected]`);
    }
  }
}

exports.Bot = Bot;
{{< /code >}}


テスト実行
-----

これで、チャットボットのひと通りのベース実装は完成です。
下記のように実行すれば、チャットサーバーが起動して、Bot Framework Emulator などから接続できるようになります。

```
$ node index.js

Bot server listening to http://[::]:3978
```

Azure ポータルで自動生成された `package.json` ファイルがあるはずなので、下記のようにしても起動できると思います。

```
$ npm start
```

起動できない場合は、`.env` ファイルに `.bot` ファイルの情報が正しく設定されているか確認してみてください。

{{< code title=".env" >}}
botFilePath=./maku-bot.bot
botFileSecret=9l/b88E/hiSpcIdxskqe4YSaE7sGSYXPaiuKZslE3XG=
{{< /code >}}


ソースコード
----

ここで作成したソースコードを置いておきます。

- {{% file src="develop-files/config.js.txt" caption="config.js （設定の読み込み部分）" %}}
- {{% file src="develop-files/index.js.txt" caption="index.js （ボットサーバーの立ち上げ部分）" %}}
- {{% file src="develop-files/bot.js.txt" caption="bot.js （ボット本体）" %}}

