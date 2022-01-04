---
title: "Azure: App Service の Node.js アプリのエントリポイントはどこで定義されているか？"
url: "/p/3u4hj7h"
date: "2019-10-08"
tags: ["Azure", "Azure App Service"]
---

Node.js アプリのエントリポイントの指定
----

Azure App Service で Node.js アプリをテンプレートから作成すると、うまいことルートに配置された `index.js` が起動してくれます。
この仕組みがブラックボックスな感じで気持ち悪いので調べてみたところ、この**エントリポイントの指定は `web.config` ファイルにある**ようです。

{{< code lang="xml" title="web.config（抜粋）" >}}
<add name="iisnode" path="index.js" verb="*" modules="iisnode"/>
{{< /code >}}

Windows ベースの App Service で Node.js アプリを動作させる場合、Windows の Web サーバーである IIS 上で動作する [iisnode](https://github.com/Azure/iisnode) という Node.js 実装が使用されます。
IIS が起動するときに設定ファイルである `web.config` が読み込まれ、上記の設定により `iisnode` がエントリポイント `index.js` を使って起動するという流れになります。
なので、作成している Node.js アプリのエントリポイント（メインの JS ファイル）が変わった場合は、この `web.config` ファイルを修正する必要があります。

あと、このような仕組みのため、`web.config` ファイルは必ずアプリを構成する JS ファイルと一緒にデプロイしないといけないということも分かります。
上記の設定のままであれば、デプロイ先のルートディレクトリに、少なくとも下記の 2 ファイルが配置されていなければいけません。

- web.config （IIS の設定ファイル）
- index.js （上記ファイルで指定されたエントリポイント）


なぜ紛らわしいのか？
----

Node.js アプリを開発するとき、通常は `package.json` の `start` スクリプトとしてエントリポイントを指定します。

{{< code lang="json" title="package.json（抜粋）" >}}
{
  "scripts": {
    "start": "node index.js"
  }
}
{{< /code >}}

こうすることで、`npm start` と実行するだけで、`index.js` をエントリポイントとして Node.js アプリが起動するようになっています。

[Azure App Service のドキュメント](https://docs.microsoft.com/ja-jp/azure/app-service/app-service-web-get-started-nodejs) にも、

> アプリケーションをローカルで実行すると、アプリケーションを Azure にデプロイするとどう表示されるかを把握できます。ターミナル ウィンドウを開き、`npm start` スクリプトを使用して、組み込みの Node.js HTTP サーバーを起動します。

と書かれています。

間違ってはいないのですが、こんな風に書かれたら、`package.json` の `scripts.start` の定義を変更すれば App Service 上の振る舞いも変わると思うじゃないですか。
ところがどっこい、`package.json` を修正しても、App Service 上での動作は何も変わりません。
`web.config` が真犯人だからです。

気をつけましょー。


コラム: PORT 環境変数
----

Azure App Service 上で Node.js アプリを動作させると、自動的に `PORT` 環境変数が定義されます。
なので、Node.js アプリ内で Web サーバーを起動させるときは、`process.env.PORT` に設定されたポート番号を使うようにします。

{{< code lang="js" >}}
const server = restify.createServer();
server.listen(process.env.port || process.env.PORT || 3978, () => {
  console.log(`Bot server listening on ${ server.port }`);
});
{{< /code >}}

App Service にはこういった約束事がいろいろありますね。

