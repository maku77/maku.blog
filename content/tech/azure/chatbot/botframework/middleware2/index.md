---
title: "チャットボット: 独自のミドルウェアを作成して禁止ワードを拒否するようにする"
url: "/p/gt9g2na"
date: "2019-07-23"
tags: ["Azure", "Bot Builder SDK", "Chatbot", "Node.js"]
weight: 272
---

[前回の記事](/p/fn3amda) では、チャットボットに独自のミドルウェアを追加して、ユーザー入力をログ出力できるようにしました。
今回は、ユーザーが NG ワードを入力したときに、警告を表示して処理を中断するようなミドルウェアを作成してみます。

その名も **`NgWordMiddleware`** です！

{{< code lang="js" title="middlewares/NgWordMiddleware.js" >}}
const NG_WORDS = /アホ|まぬけ|バカ/;

exports.NgWordMiddleware = class NgWordMiddleware {
  async onTurn(context, next) {
    if (context.activity.type === 'message') {
      const line = context.activity.text;
      if (NG_WORDS.test(line)) {
        await context.sendActivity('そんなこと言っちゃダメ');
        return;
      }
    }
    await next();  // Invoke a next middleware
  }
};
{{< /code >}}

上記の例では、`NG_WORDS` 定数に、禁止語句を正規表現の形で登録しています。
ユーザーが入力したテキストに、禁止語句が含まれていたら、「そんなこと言っちゃダメ」と返事して処理を進めないようにします（`next()` を呼び出さないことで後続の処理を打ち切る）。

このミドルウェアは、下記のようにアダプターに追加することで有効化できます。

{{< code lang="js" >}}
// const { NgWordMiddleware } = require('./middlewares/ngWordMiddleware.js');

const adapter = new BotFrameworkAdapter(botEndpoint);
adapter.use(new NgWordMiddleware());
{{< /code >}}

{{< image src="middleware2-001.png" >}}

この例では、単純に禁止語句が含まれているかだけをチェックしているので、「バカルディ」と入力した場合にも弾かれてしまいます。
このあたりは工夫して処理しなきゃですね。

