---
title: "Bot Framework: Web チャットの表示をカスタマイズする"
url: "p/oitn3a3/"
date: "2020-02-19"
tags: ["Azure", "Chatbot", "Bot Framework"]
---

Microsoft Bot Framework を使ってウェブサイト上にチャットボットを配置したときの表示のカスタマイズ方法です。
次のように、ボットやユーザーのアイコンを設定することができます。

{{< image border="true" src="img-001.png" >}}

このようなカスタマイズ表示を行うには、Azure portal 上の Web App Bot リソースの `Channels` タブから選択できる、**`Direct Line`** チャネルを使う必要があります。
`Web Chat` というチャネルを使うと、`iframe` タグで簡単にチャットウィンドウを埋め込むことができるのですが、そちらではあまりカスタマイズができないようです。

{{< image border="true" src="img-002.png" title="Direct Line チャネルのキーを確認" >}}

下記はチャットウィンドウをカスタマイズして表示するサンプルコードです。
Bot Framework が提供している `WebChat.renderWebChat()` 関数を呼び出すと、実際にチャットウィンドウが表示されるのですが、このときに **`styleOptions`** パラメータを指定することで表示方法をカスタマイズすることができます。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<html>
<head>
  <script src="https://cdn.botframework.com/botframework-webchat/latest/webchat.js"></script>
  <style>
    * {
      margin: 0;
    }
    #webchat {
      height: 100vh;
      width: 100vw;
      background: gray;
      border: solid 5px #f37;
      box-sizing: border-box;
    }
  </style>
</head>
<body>
  <div id="webchat" role="main"></div>
  <script>
    const TOKEN = 'HpJB2ofxzsA.h7...省略...';

    WebChat.renderWebChat({
      directLine: WebChat.createDirectLine({ token: TOKEN }),
      styleOptions: {
        // ボットのアイコン
        botAvatarImage: '/assets/img/bot-avatar.png',
        // ボットのイニシャル（アイコンが使えない場合に表示）
        //botAvatarInitials: 'Bot',
        // ユーザーのアイコン
        //userAvatarImage: '/assets/img/user-avator.png',
        // ユーザーのイニシャル（アイコンが使えない場合に表示）
        userAvatarInitials: 'You',
        // アイコンのサイズ
        avatarSize: 50,
        // Upload file のボタンを非表示
        hideUploadButton: true,
      }
    }, document.getElementById('webchat'));
  </script>
</body>
</html>
{{< /code >}}

{{% maku-common/reference %}}
- [GitHub: microsoft/BotFramework-WebChat](https://github.com/microsoft/BotFramework-WebChat/blob/master/README.md)
{{% /maku-common/reference %}}
