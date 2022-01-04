---
title: "LUIS (1) LUIS とは？"
url: "/p/mkwnnsz"
date: "2019-03-13"
tags: ["チャットボット", "Azure", "LUIS"]
weight: 301
---

LUIS でできること
----

Microsoft が提供している LUIS (Language Understanding Intelligent Service) サービスを使用すると、**自然言語による文章（発話テキスト）を、コンピュータが理解しやすい形式に翻訳**することができます。
主にチャットボットのバックエンドとして使用されることが想定されているようです。

{{< image w="500" border="true" src="about.png" >}}

- [LUIS (Language Understanding) - Cognitive Services - Microsoft Azure](https://www.luis.ai/)
- [LUIS - Endpoint API](https://westus.dev.cognitive.microsoft.com/docs/services/5819c76f40a6350ce09de1ac/operations/5819c77140a63516d81aee78) メインのクエリ用 REST API
- [LUIS - Authoring API](https://westus.dev.cognitive.microsoft.com/docs/services/5890b47c39e2bb17b84a55ff/operations/5890b47c39e2bb052c5b9c2f) アプリ管理用の REST API

LUIS は、入力した発話テキストを**インテントとエンティティ**に分解します。

* <b>インテント</b> … 意図、目的。ユーザが何をしたいのかを表す。
* <b>エンティティ</b> … 文章の中のプレースホルダに当たる部分の値。求めているものや、条件などを示す部分。

例えば、下記のような入力テキストがあったとすると、

```
15時から4人で使える部屋はない？
```

LUIS は次のようなインテント＋エンティティの情報に翻訳します。

- <b>インテント</b>: SearchMeetingRoom
- <b>エンティティ</b>: Time=15時、People=4人

チャットボットのプログラムは、この翻訳された情報を見て、「ミーティングルームを探す」処理を、パラメータ「15時」、「4人」で行えばよいことになります。
LUIS は上記のような翻訳処理を行うためのサービスなので、その先のミーティングルームの検索処理などは独自に実装する必要があります。

LUIS の Web サイト上では、このような発話例 (Utterance) と、インテント、エンティティの情報を登録していくことで、モデルの学習を進めることができます。


LUIS のモデルを公開する
----

LUIS ポータル上で作成した自然言語解析のモデルを、Web API (REST API) の形で使用できるようにするには、下記のようなステップを踏みます（[QnA Maker](/p/2t6xrbm) のサービスと同様の手順です）。

1. [Azure ポータル](https://portal.azure.com/)の <samp>リソースの作成</samp> から Language Understanding (LUIS) のリソースを作成する
2. [LUIS ポータル](https://www.luis.ai/)で LUIS アプリ（モデル）を作成する（上記で作成した Azure の LUIS リソース経由でアクセスできるよう関連付ける）
3. LUIS ポータルでモデルを <samp>Publish</samp> して REST API として呼び出せるようにする

上記のようにしてナレッジベースを公開すれば、Node.js や C# など任意の言語から LUIS API を使用できるようになります。


その他 LUIS に関するメモ
----

* LUIS のドキュメントに出てくる「**app**」というのはいわゆる「**モデル**」のこと。
* インテントのリストには「**None**」というインテントが最初から存在しており、LUIS が発話テキストを理解できなかった場合のインテントとして使用される。None インテントにも、発話例としていくつかの文章を登録しておくのがよい。
* 各インテントには発話テキストの例 (**Utterance**) を登録していく。
    * 例: "評価が 70 以上のカベルネはありますか？"
    * 例: "シャンパンを評価 50 で検索してください"
    * 例: "評価が 80 以上の白ワインには何がありますか？"
* 2つ、3つの発話テキストを登録するだけでも、インテントに対応するコードを構築するのに十分役立つモデルを生成できる。
* エンティティには様々なエンティティタイプがあるが、通常は既定の **`simple`** を選択しておけば、チャットボットが操作できる値を抽出するに十分である（LUIS のエンティティタイプはまだ開発中の機能である）。
* **既製のエンティティ (prebuilt entities)** を使用すると、カスタムコードをあまり書かなくて済む。例えば、**`number`** というエンティティは、`70` や `seventy` などのワードを認識できるようになっている（現状、数字は半角で入力する必要がある）。他にも、`money`、`ordinal`、`percentage`、`keyPhrase`、`url`、`email`、`phoneNumber` などが用意されている。
* MS Bot Framework (SDK) において既製のエンティティ (prebuilt entities) を参照する場合、`builtin.` というプレフィックスが付く（例: `builtin.number`）。
* LUIS がユーザから受信した発話テキストの一覧は、<samp>Review endpoint utterance</samp> のタブを選択すると確認できる。
* LUIS モデルを使用するには、そのモデルを公開 (publish) する必要がある。

