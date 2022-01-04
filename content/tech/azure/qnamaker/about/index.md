---
title: "QnA Maker (1) QnA Maker とは？"
url: "/p/2t6xrbm"
linkTitle: "QnA Maker とは？"
date: "2019-02-28"
tags: ["Azure", "Chatbot", "QnA Maker"]
weight: 401
---

QnA Maker でできること
----

Microsoft が提供している QnA Maker というサービスを使用すると、「質問(Q)と回答(A)」のペアデータを登録するだけで、自然言語での FAQ 検索を行うための API を使用できるようになります。
Microsoft が提供している一連の Cognitive Service のひとつとして位置づけられていて、主に FAQ 系のチャットボット (Bot) を手軽に作成するために使用されています。

{{< image w="500" border="true" src="about.png" >}}

- [QnA Maker](https://www.qnamaker.ai)
- [QnA Maker Documentation](https://docs.microsoft.com/en-us/azure/cognitive-services/qnamaker/)
- [QnA Maker REST API V4.0](https://westus.dev.cognitive.microsoft.com/docs/services/5a93fcf85b4ccd136866eb37/operations/5ac266295b4ccd1554da75ff)
- [QnAMaker class | Microsoft Docs](https://docs.microsoft.com/en-us/javascript/api/botbuilder-ai/qnamaker?view=botbuilder-ts-latest)

例えば、下記のような Q&A のペアを登録していくだけで、機械学習によって回答のモデルが自動生成されます。
QnA Maker では、このモデルのことを**ナレッジベース (Knowledge base)** と呼んでいます。

```
Q. お店の営業時間を教えてください。
A. 営業時間は午前10時から午後6時までです。
```

実際にユーザが入力する質問文章は、登録した Q&A データの質問文と完全に一致する必要はありません。
QnA Maker がどの質問に近いかを判別して、対応する回答文（と一致度）を返してくれます。

```
ユーザ入力: 営業時間は？
QnA Makerの回答: 営業時間は午前10時から午後6時までです。
```

簡単に言ってしまえば、QnA Maker の API が提供する機能はこれだけです（データ管理用の API などもありますが）。
回答文を自動生成してくれるようなこともなく、返される文章は、Q&A データとして登録した回答文そのままです。
とはいえ、サクッと FAQ 系のサービスを作成するときには便利に使用できるサービスです。


QnA Maker のナレッジベースを公開する
----

QnA Maker ポータル上で作成したナレッジベースを、Web API (REST API) の形で使用できるようにするには、下記のようなステップを踏みます。

1. [Azure ポータル](https://portal.azure.com/)の <samp>リソースの作成</samp> から QnA Maker のリソースを作成する
2. [QnA Maker ポータル](https://www.qnamaker.ai/)で QnA Maker のナレッジベースを作成する（上記で作成した Azure の QnA Maker リソース経由でアクセスできるよう関連付ける）
3. QnA Maker ポータルでナレッジベースを <samp>Publish</samp> して REST API として呼び出せるようにする

上記のようにしてナレッジベースを公開すれば、Node.js や C# など任意の言語から QnA Maker API を使用できるようになります。

