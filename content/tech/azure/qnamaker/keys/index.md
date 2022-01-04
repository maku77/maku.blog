---
title: "QnA Maker (2) QnA Maker のサブスクリプションキー、エンドポイントキーとは"
linkTitle: "QnA Maker のサブスクリプションキー、エンドポイントキーとは"
url: "/p/t6n6jc8"
date: "2019-03-18"
tags: ["Azure", "Chatbot", "QnA Maker"]
description: "QnA Maker アプリで使用するキーには、サブスクリプションキーとエンドポイントキーの 2 種類があります。2 つのキーの違いと、管理方法について説明します。"
weight: 402
---

{{% private %}}
QnA Maker のキー管理に関する Microsoft ドキュメント

- [QnA Maker のリソースとキーの管理](https://docs.microsoft.com/ja-jp/azure/cognitive-services/qnamaker/how-to/key-management)
{{% /private %}}


サブスクリプションキー (Subscription Key)
----

**サブスクリプションキー**は、QnA Maker アプリ自体の作成や編集を行うためのキーです。
後述のエンドポイントキーよりも厳重に管理しなければならないキーです。

このキーは、[Azure ポータル](https://portal.azure.com/)上で QnA Maker のリソースを作成した際に生成されます。
QnA Maker アプリは、Azure 上にリソースを作ってからでないと作成できないため、QnA Maker アプリがすでに存在するのであれば、サブスクリプションキーも必ず存在することになります。

Azule の QnA Maker リソースに割り当てられたサブスクリプションキーは、Azure ポータルの <samp>QnA Maker</samp> リソースの <samp>Keys</samp> のページで確認することができます。

{{< image w="700" border="true" src="keys-azure.png" title="Azure ポータル上での QnA Maker のサブスクリプションキーの確認" >}}

LUIS の場合は、アプリ管理用のキーは**オーサリングキー**と呼んでいたりしますが、それの QnA Maker 版だと考えればよいです。
QnA Maker アプリの場合、このキーの管理は Azure ポータル上で管理されることになっており、Azure 上ではこのようなキーのことをサブスクリプションキーと呼んでいるんですね。
このあたりのチグハグ感に関しては [こちらを参照](/p/8myms6s)。


エンドポイントキー (Endpoint Key)
----

**エンドポイントキー**は、チャットアプリなどのユーザクライアント（Bot Framework では「チャンネル」と呼びます）が、QnA Maker に対してクエリを行うときに使用するキーです。
エンドポイントキーは、QnA Maker ポータルで対象となるナレッジベースを選択後、<samp>PUBLISH</samp> タブから Publish 処理を実行したとき、あるいは <samp>SETTING</samp> タブを選択することで確認することができます。

{{< image w="700" border="true" src="keys-endpoint.png" title="QnA Maker ポータル上での QnA Maker のエンドポイントキーの確認" >}}

チャットボットなどから QnA Maker を利用する場合、実運用時に設定する API アクセス用のキーは、こっちのエンドポイントキーです。


（コラム）QnA Maker アプリと Azure リソースの結び付け
----

LUIS アプリの場合は、LUIS ポータル上で LUIS アプリを作成した後に、Azure ポータルで作成したリソースに関連付けるという作業が必要でした。
一方、**QnA Maker アプリの場合は、この作業は必要ありません**。
なぜなら、QnA Maker ポータル上で QnA Maker アプリ（ナレッジベース）を作成するときに、最初に Azure 上で作成した QnA Maker リソースの結び付けが強制されるからです。

よって、QnA Maker の場合は、ナレッジベースを作成した時点で、その API を Azure リソース経由で呼び出す準備が既に整っていることになります。

