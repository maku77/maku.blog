---
title: "QnA Maker (4) Python から QnA Maker の API を利用する"
url: "/p/fwyi2fh"
linkTitle: "Python から QnA Maker の API を利用する"
date: "2019-06-04"
tags: ["Azure", "Chatbot", "Python", "QnA Maker"]
description: "QnA Maker の REST API を Python の urllib.request モジュールを使って実行してみます。"
weight: 404
---

QnA Maker は REST API を提供しているので、HTTP リクエストを発行できるプログラミング言語から簡単に制御することができます。
ここでは、Python に標準で付属している **`urllib.request`** モジュールを使って HTTP リクエストを発行し、QnA Maker にアクセスしてみます。


事前準備（アクセスキーの準備）
----

REST API を使用するには、HTTP リクエストのヘッダ情報としてアクセスキーを付加する必要があります。
[こちらの記事](/p/t6n6jc8) を参考に、下記のどちらかのアクセスキーを確認しておいてください。

- サブスクリプションキー (Subscription Key) ... 管理用
- エンドポイントキー (Endpoint Key) ... クエリ用

アクセスキーは `9d16b3e6345489ad4a57a0755eb4f96a` のような 16 進数文字列です。
QnA のクエリ実行だけであればエンドポイントキーの方を使えば大丈夫ですが、ナレッジベースの作成や更新などを行う場合は、サブスクリプションキーの方を使う必要があります。


QnA Maker の REST API を呼び出す Python ライブラリ
----

REST API は HTTP リクエストを送るだけで使用できるので、ここでは自力で QnA Maker の API を使用するライブラリを作ってみます。

下記の `QnaRequest` クラスは、ナレッジベースの一覧を取得する `getAllKnowledgeBases` メソッドと、指定したナレッジベースの情報を取得する `getKnowledgeBase` メソッドを提供しています。
戻り値は JSON 形式のテキストです。

{{< code lang="python" title="mylib/qna.py" >}}
import urllib.request

class QnaRequest:
    BASE_URL = 'https://westus.api.cognitive.microsoft.com/qnamaker/v4.0'

    def __init__(self, subscriptionKey):
        self._subscriptionKey = subscriptionKey
        self._setup_opener()

    def _setup_opener(self):
        self._opener = urllib.request.build_opener()
        self._opener.addheaders = [
            ('Ocp-Apim-Subscription-Key', self._subscriptionKey)
        ]

    def _get(self, path):
        with self._opener.open(QnaRequest.BASE_URL + path) as res:
            return res.read().decode('utf-8')

    def getAllKnowledgeBases(self):
        """すべてのナレッジベースの情報を取得します。"""
        return self._get('/knowledgebases/')

    def getKnowledgeBase(self, id):
        """指定したナレッジベースの情報を取得します。"""
        return self._get('/knowledgebases/{}'.format(id))
{{< /code >}}

HTTP リクエストを送るときには、`Ocp-Apim-Subscription-Key` ヘッダでアクセス用のキーを付加するようにしています。

QnA Maker には他にもたくさん REST API が用意されています。
下記のサイトを参考にすれば、同じように実装できると思います。

- [QnA Maker REST API Reference](https://docs.microsoft.com/en-us/rest/api/cognitiveservices/qnamaker/knowledgebase)
- [Node.js で QnA Maker REST API を使用する方法](https://docs.microsoft.com/ja-jp/azure/cognitive-services/qnamaker/quickstarts/nodejs)


テスト
----

`mylib/qna.py` ファイルに実装した `QnaRequest` クラスを使って REST API を呼び出してみます。
ここでは、ナレッジベースの一覧を取得しています。

{{< code lang="python" title="main.py" >}}
from mylib.qna import QnaRequest

# QnA のサブスクリプションキー（Azure ポータルで確認）
subscriptionKey = '9d16b3e6345489ad4a57a0755eb4f96a'

# QnA Maker の REST API を呼び出す
qna = QnaRequest(subscriptionKey)
json = qna.getAllKnowledgeBases()
print(json)
{{< /code >}}

ナレッジベースがひとつだけ登録されている状態で実行すると、下記のように結果が返ってきます。

{{< code lang="json" title="実行結果" >}}
{
  "knowledgebases": [
    {
      "id": "3700ed24-c581-c146-549b-4129f552e5e8",
      "hostName": "https://sample-qna.azurewebsites.net",
      "lastAccessedTimestamp": "2019-05-29T03:27:15Z",
      "lastChangedTimestamp": "2019-04-17T08:49:06Z",
      "lastPublishedTimestamp": "2019-04-17T06:33:42Z",
      "name": "My Sample Knowledge Base",
      "userId": "b931da0aee87aab17af1c3b7c0ec48ef",
      "urls": [
        "http://www.jma.go.jp/jma/kishou/know/faq/index.html"
      ],
      "sources": [],
      "language": "Japanese",
      "enableHierarchicalExtraction": false,
      "createdTimestamp": "2019-04-17T06:27:16Z"
    }
  ]
}
{{< /code >}}


