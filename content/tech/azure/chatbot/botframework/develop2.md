---
title: "チャットボット: LUIS や QnA Maker サービスへの接続情報を .bot ファイルから取得する"
url: "/p/o2bqajv"
date: "2019-04-15"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 233
description: "Bot Builder SDK v4 からは、LUIS や QnA Maker サービスなどの接続情報を .bot ファイルに記述しておくことが推奨されています。ここでは、.bot ファイルから接続情報を取得するためのクラスを作成します。"
---

- 参考: [MS Bot Framework の .bot ファイルで接続情報を管理する](/p/8choj4w)

ここで作るもの
----

[こちらの実装](/p/iob68qa) では、最初のステップとしてボットサーバ自体 (Azure Web Apps) のエンドポイント情報を `.bot` ファイルから取得する実装を行いました (`config.js`)。
ここでは、さらに、LUIS サービスや QnA Maker サービスを利用することを想定し、これらの情報も `.bot` ファイルから取得できるように拡張します。

使用イメージとしては、下記のようにしてそれぞれの接続情報を簡単に読み込めるようにします。

{{< code lang="js" >}}
const config = require('./config.js');

const LUIS_APP_NAME = 'maku-luis-sample';
const QNA_APP_NAME = 'maku-qna-sample';

const botEndpoint = config.loadBotEndpoint();  // ボット自体への接続情報
const luisEndpoint = config.loadLuisEndpoint(LUIS_APP_NAME);  // LUIS への接続情報
const qnaEndpoint = config.loadQnaEndpoint(QNA_APP_NAME);  // QnA Maker への接続情報
{{< /code >}}

単一のオブジェクトとしてまとめて取得するように実装することもできるのですが、分かりやすさのために、3 つの情報に分けて取得するようにしています。
LUIS や QnA Maker は、複数のアプリ（ナレッジベース）を同時に使用する可能性があるので、アプリ名を指定して接続情報を取得できるようにしています。

取得した情報は、次のように BotBuilder SDK が提供するクラスへの入力として使用することを想定しています。

- `botEndpoint` オブジェクトは、`botbuilder` パッケージの `BotFrameworkAdapter` クラスのコンストラクタに渡されます。
- `luisEndpoint` オブジェクトは、`botbuilder-ai` パッケージの `LuisRecognizer` クラスのコンストラクタに渡されます。
- `qnaEndpoint` オブジェクトは、`botbuilder-ai` パッケージの `QnAMaker` クラスのコンストラクタに渡されます。

下記は、実際に取得されるオブジェクトの内容の例です。
これらのオブジェクトは、上記のように SDK のクラスへの入力用に使用するので、各プロパティの値を直接参照することはないと思います。

{{< code title="上記のコードで得られる情報の例" lang="js" >}}
botEndpoint = {
  appId: '921c6a01-1948-0f4d-9a82-75daf6a6d43c',
  appPassword: '16lgoF+phvG:MPeg1eIDma*fcNU#!5jv',
  ...
}

luisEndpoint = {
  applicationId: 'c5514855-fa6b-8f4c-c695-9ac7d3519412',
  endpointKey: '9ccff1530f4214fd8e319434a1408fa2',
  endpoint: 'https://westus.api.cognitive.microsoft.com'
}

qnaEndpoint = {
  knowledgeBaseId: 'c78ec3e7-a58d-ca44-dbb6-e7805d8130c6',
  endpointKey: 'c9153f6c-980c-bd41-4ba6-370392cda0e8',
  host: 'https://maku-qna-sample.azurewebsites.net/qnamaker'
}
{{< /code >}}


botframework-config パッケージのインストール
----

`.bot` ファイルのロードには、Node の **`botframework-config`** パッケージが提供する `BotConfiguration` クラスを使用します。
必要があれば、下記のようにインストールして、`package.json` に依存関係を追記します。

{{< code >}}
$ npm install botframework-config --save
{{< /code >}}


実装
----

### config.js 全体のコード

{{< code lang="js" >}}
class Config {
  /**
   * コンストラクタ。
   * 環境変数や .env ファイルに書かれた botFilePath や botFileSecret の
   * 情報を基に、.bot ファイルを読み込みます。
   */
  constructor() {
    const env = this._loadEnvFile();
    this.botEnv = env.botEnv;
    this.botConfig = this._loadBotFile(env.botFilePath, env.botFileSecret);
  }

  /**
   * 指定された Bot Endpoint (developement や production) への
   * 接続情報を取得します。
   */
  loadBotEndpoint() {
    // Get bot endpoint configuration by service name
    // Bot configuration as defined in .bot file
    const endpointConfig = this.botConfig.findServiceByNameOrId(this.botEnv);

    return {
      appId: endpointConfig.appId || process.env.microsoftAppID,
      appPassword: endpointConfig.appPassword || process.env.microsoftAppPassword,
      channelService: process.env.ChannelService,
      openIdMetadata: process.env.BotOpenIdMetadata,
    }
  }

  /**
   * 指定した LUIS アプリへの接続情報を取得します。
   * botbuilder-ai パッケージの LuisRecognizer クラスの
   * 初期化情報として使用することを想定しています。
   * 設定が見つからない場合は空のオブジェクトを返します。
   */
  loadLuisEndpoint(luisAppName) {
    const luisConfig = this.botConfig.findServiceByNameOrId(luisAppName);
    if (luisConfig == null) {
      return {};
    }

    // Map the contents to the required format for `LuisRecognizer`.
    return {
      applicationId: luisConfig.appId,
      endpointKey: luisConfig.subscriptionKey,
      endpoint: luisConfig.getEndpoint()
    };
  }

  /**
   * 指定した QnA Maker アプリへの接続情報を取得します。
   * botbuilder-ai パッケージの QnAMaker クラスの初期化情報として
   * 使用することを想定しています。
   * 設定が見つからない場合は空のオブジェクトを返します。
   */
  loadQnaEndpoint(qnaAppName) {
    const qnaConfig = this.botConfig.findServiceByNameOrId(qnaAppName);
    if (qnaConfig == null) {
      return {};
    }

    // Map the contents to the required format for `QnAMaker`.
    return {
      knowledgeBaseId: qnaConfig.kbId,
      endpointKey: qnaConfig.endpointKey,
      host: qnaConfig.hostname
    };
  }

  /**
   * .env ファイルの設定を環境変数 (process.env) とマージし、
   * .bot ファイルを読み込むための情報（パスなど）を返します。
   */
  _loadEnvFile() {
    const dotenv = require('dotenv');
    const path = require('path');
    const ROOT_DIR = __dirname;

    // Merge .env info to process.env
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

  /**
   * 暗号化された .bot ファイルを読み込みます。
   */
  _loadBotFile(botFilePath, botFileSecret) {
    const { BotConfiguration } = require('botframework-config');
    try {
      return BotConfiguration.loadSync(botFilePath, botFileSecret);
    } catch (err) {
      console.error('Error reading bot file.');
      console.error(err.message + '\n');
      console.error('Please ensure you have valid botFilePath' +
                    ' and botFileSecret set for your environment.');
      console.error('You can find the botFilePath and botFileSecret' +
                    ' in the Azure App Service application settings.');
      console.error('If you are running this bot locally, consider adding' +
                    ' a .env file with botFilePath and botFileSecret.');
      process.exit(1);
    }
  }
}

module.exports = new Config();
{{< /code >}}

### BotConfiguration.loadSync() のエラーについてのメモ

`BotConfiguration.loadSync()` でエラーが発生して `.bot` ファイルがうまく読み込めなかった場合、Microsoft 公式のサンプルコードでは「`botFilePath` と `botFileSecret` をちゃんと設定してください」といった感じのメッセージだけ表示して済ませています。
しかし、実際には `.bot` ファイルを読み込めても記述内容がおかしい場合にはエラーが発生します（URL が求められているプロパティなのに URL の形式になっていない場合など）。

このチグハグなエラーメッセージのせいで結構ハマりました。。。
`.bot` ファイルの読み込みがエラーになった場合に、その原因を明確にするためには、最低限 Error オブジェクトの内容（`err.message` など）を出力してあげた方がよいです。

### BotConfiguration#findServiceByNameOrId() で返される接続情報についてのメモ

`BotConfiguration#findServiceByNameOrId()` で返される各サービスの接続情報に何が含まれるかは、下記の TypeScript のクラスとして定義されています。

- [EndpointService class](https://docs.microsoft.com/en-us/javascript/api/botframework-config/endpointservice?view=botbuilder-ts-latest)
    - `appId`、`appPassword`、`endpoint` プロパティなどを持つ。
- [LuisService class](https://docs.microsoft.com/en-us/javascript/api/botframework-config/luisservice?view=botbuilder-ts-latest)
    - `appId`、`authoringKey`、`region`、`subscriptionKey`、`version` プロパティ、`getEndpoint()` メソッドなどを持つ。
    - `getEndpoint()` メソッドの戻り値を使用すれば、通常は `region` や `version` プロパティの値を個別に参照する必要はない。
    - `authoringKey` は管理用のキーなので、**実運用では `subscriptionKey` を LUIS のエンドポイントキーとして使用する**こと。
- [QnaMakerService class](https://docs.microsoft.com/en-us/javascript/api/botframework-config/qnamakerservice?view=botbuilder-ts-latest)
    - `endpointKey`、`hostname`、`kbId`、`subscriptionKey` プロパティなどを持つ。
    - `subscriptionKey` は管理用のキーなので、**実運用では `endpointKey` を QnA Maker のエンドポイントキーとして使用する**こと。

LUIS と QnAMaker のキーはそれぞれ 2 種類ずつあって、どちらを実際のエンドポイントキーとして使用するかは注意すべきポイントです。
キーの区別がよくわからなくなってきた場合は、こちらの記事「[LUIS と QnA Maker でキーの管理方法が異なるのはなぜか？](/p/8myms6s)」を参照してください。

`findServiceByNameOrId()` の返すオブジェクトには、各種サービスの API を利用するために必要十分な情報が含まれていますが、`.bot` ファイルに記述した内容をすべて取得できるというわけではないことに注意してください。

他にも、[DispatchService](https://docs.microsoft.com/en-us/javascript/api/botframework-config/dispatchservice?view=botbuilder-ts-latest)、[FileService](https://docs.microsoft.com/en-us/javascript/api/botframework-config/fileservice?view=botbuilder-ts-latest)、[CosmosDbService](https://docs.microsoft.com/en-us/javascript/api/botframework-config/cosmosdbservice?view=botbuilder-ts-latest)、[BlobStorageService](https://docs.microsoft.com/en-us/javascript/api/botframework-config/blobstorageservice?view=botbuilder-ts-latest) など、サービスタイプに応じたクラスが定義されています。
サービスタイプの一覧は、[ServiceType enum の定義](https://docs.microsoft.com/en-us/javascript/api/botframework-config/servicetypes?view=botbuilder-ts-latest)を見れば分かるでしょう。


使用例
----

上記の `config.js` で取得した接続情報の使用例です。
ボットコード全体を示すと長大になってしまうので、ここでは、**`BotFrameworkAdapter`**、**`LuisRecgonizer`**、**`QnAMaker`** クラスのインスタンスを作成する部分を抜粋して示します。

### BotFrameworkAdapter の初期化

{{< code lang="js" >}}
const { BotFrameworkAdapter } = require('botbuilder');
const config = require('./config.js');

const adapter = new BotFrameworkAdapter(config.loadBotEndpoint());
{{< /code >}}

### LuisRecognizer の初期化

{{< code lang="js" >}}
const { LuisRecognizer } = require('botbuilder-ai');
const config = require('./config.js');
const LUIS_APP_NAME = 'your-luis-app-name';

const luisEndpoint = config.loadLuisEndpoint(LUIS_APP_NAME);
const luisOptions = {
  includeAllIntents: true,
  log: true,
  staging: false
};
const luisRecognizer = new LuisRecognizer(luisEndpoint, luisOptions, true);
{{< /code >}}

### QnAMaker の初期化

{{< code lang="js" >}}
const { QnAMaker } = require('botbuilder-ai');
const config = require('./config.js');
const QNA_APP_NAME = 'your-qna-app-name';

const qnaEndpoint = config.loadQnaEndpoint(QNA_APP_NAME);
const qnaOptions = {};
const qnaMaker = new QnAMaker(qnaEndpoint, qnaOptions);
{{< /code >}}

