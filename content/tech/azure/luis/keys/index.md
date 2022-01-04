---
title: "LUIS (2) LUIS のオーサリングキー、エンドポイントキーとは"
url: "/p/mdyedwq"
date: "2019-03-14"
tags: ["Azure", "Chatbot", "LUIS"]
weight: 303
---

{{% private %}}
LUIS のキー管理に関する Microsoft ドキュメント

- [LUIS のアカウントとオーサリングキーを管理する](https://docs.microsoft.com/ja-jp/azure/cognitive-services/luis/luis-how-to-account-settings)
- [LUIS のオーサリングキーとクエリ予測エンドポイントキー](https://docs.microsoft.com/ja-jp/azure/cognitive-services/luis/luis-concept-keys)
- [LUIS アプリでのサブスクリプションキーの使用](https://docs.microsoft.com/ja-jp/azure/cognitive-services/luis/luis-how-to-azure-subscription)
{{%/ private %}}

LUIS アプリで使用するキーの種類には、**オーサリングキー (Authoring Key)** と**エンドポイントキー (Endpoint Key)** の 2 種類があります。


オーサリングキー (Authoring Key)
----

オーサリングキーは LUIS アカウントの作成時に自動的に作成される、無料のキーです。
オーサリングキーはリージョン内で共通であり、1 つだけ作成されます。
最初に作成されるので、**スターターキー (Starter Key)**、**作成者キー**とも呼ばれます。

オーサリングキーは、LUIS アカウントに結び付けられているので、LUIS 右上のアカウント名をクリックし、<samp>Settings</samp> を選択することで確認できるようになっています。

{{< image w="500" border="true" src="keys-authoring.png" >}}

オーサリングキーは、**LUIS アプリ自体の作成や、公開、コラボレーターの管理、バージョン管理など**を行うときに必要になります。
つまり、あなたが作成する LUIS アプリの管理用のキーであり、LUIS の Web サイトで行えることほぼすべてをこのキーを使ったオーサリング API 経由で行えます。

LUIS ポータル上にログインして作業している間は、オーサリングキーの存在を意識しなくても LUIS アプリの管理を行うことができますが、**Web API を使って LUIS アプリの管理作業を自動化したいときなどにオーサリングキーが必要**になります。

作成した個々の LUIS アプリに対するクエリ実行のためにもオーサリングキーを使用できますが、これは実装中のテスト用途に限られます（権限の強いオーサリングキーを、公開アプリからのクエリ用途に使うのは避けるべきです）。
実運用でのクエリ実行は、後述のエンドポイントキーを使用します。

ちなみに、オーサリングキーは Microsoft のユーザアカウントごとに割り当てられるものです。
LUIS アプリのコラボレータとして登録されたユーザは、自分のオーサリングキーを使って LUIS アプリの管理を行います。


エンドポイントキー (Endpoint Key)
----

運用環境で LUIS アプリに対するクエリ実行を行うには、Azure の LUIS リソースとして作成されたエンドポイント URL とエンドポイントキーを使用します。
[Azure ポータル](https://portal.azure.com/) にログインして、LUIS リソースを作成することでエンドポイントキーを取得できます。

Azure 上の LUIS リソースは、1 つのリソースグループに所属し、そのリソースグループは 1 つのサブスクリプションに所属します。
毎月の課金はサブスクリプションに対して行われます。
階層的には下記のような感じで LUIS リソースが配置されているイメージです。

```
Azureアカウント
　└ サブスクリプション
　　　 └ リソースグループ
　　　　　 └ LUISリソース ← LUISポータルで作成したアプリをこのリソースで動かす
```

Azule ポータルで作成した LUIS リソースに割り当てられたエンドポイントキーは、Azure ポータルの <samp>LUIS (Language Understanding)</samp> リソースの <samp>Keys</samp> のページで確認することができます。

{{< image w="600" border="true" src="keys-endpoint.png" >}}

Azure のリソースにアクセスするためのキーなので、**リソースキー**と呼ばれることもあります。

それにしても、キーの呼び名が統一されてなくてわかりにくいですね。。。


LUIS ポータルで LUIS アプリを Azure リソースに結びつける
----

実運用で LUIS アプリへのクエリをかける場合、Azure ポータルで作成した LUIS リソース上で LUIS アプリを稼働させる必要があります（正確には LUIS リソースを作成した時点で稼働しているので、Azure の LUIS リソースのエンドポイントを、LUIS アプリと関連付けるというのが正しいです）。

{{% shout "ユーザ → Azure の LUIS リソース ⇔ LUIS アプリ" %}}

LUIS ポータルで <samp>MANAGE</samp> タブ → <samp>Keys and Endpoints</samp> と辿ると、デフォルトでは 1 つのエンドポイント (URL) がリソース名 **`Starter_Key`** というリソースで動作するように登録されています。
これは、LUIS アプリ管理用のエンドポイントであり、オーサリングキー（スターターキー）を使用して API 呼び出しを行えるようになっています（アプリ自体の管理や、クエリのテスト実行に使用する）。

{{< image w="600" border="true" src="keys-assign1a.png" >}}

この、`Starter_Key` というリソースは LUIS ポータル上で LUIS アプリを作成すると自動的に作成されるため、Azure ポータル上で LUIS リソースを割り当てなくても最初から API 呼び出しができるようになっています。

ただし、実運用で LUIS アプリを使用するときは、Azure 上で作成した LUIS リソースを、この LUIS アプリに結び付けなければいけません。
<samp>＋Assign resources</samp> ボタンを押すと、Azure 上の LUIS リソースを関連付けるためのダイアログが表示されます。

{{< image w="600" border="true" src="keys-assign1b.png" >}}

<samp>LUIS resource name</samp> というところで、結び付けたい Azure の LUIS リソース名を選択します。

{{< image w="500" border="true" src="keys-assign2.png" >}}

すると、Endpoint Keys の一番下のところに、Azure の LUIS リソースを使うためのエンドポイント URL と、エンドポイントキーのペアが登録されます。

{{< image w="600" border="true" src="keys-assign3.png" >}}

ここに表示されるエンドポイントキーは、Azure ポータル上の LUIS リソースに設定されているエンドポイントキーと同じ値になっているはずです。
これで、Azure の LUIS リソースのエンドポイントへのアクセスが、この LUIS アプリに関連付けられて動作するようになります。

全体的には下記のようなイメージでつながっていると考えるとわかりやすいです。

{{< image w="550" src="keys-overview.png" >}}

まず、LUIS ポータルは LUIS アプリを作成することを一番の目的としていますが、同時に、そのアプリ自体を管理するためのエンドポイントとして `Starter_Key` というリソースが自動的に生成されます。
管理者はこのリソースに対してオーサリングキーでアクセスして LUIS アプリの管理を行えます。
一方で、一般クライアント（Skype チャネルや Slack チャネルなど）からは、Azure ポータルに作成した LUIS リソース経由で LUIS アプリを使います。
このとき使用するキーは、Azure ポータルの LUIS リソースに対して発行されたエンドポイントキーで、クエリ用途に特化されています。

ちなみに、LUIS アプリに他の開発者をコラボレータとして登録した場合は、コラボレータから見ると、エンドポイントのリストのところに、<samp>This app has 1 collaborator key(s) you do not have permission to view.</samp> のように表示されるようです。

{{< image w="600" border="true" src="keys-collaborator.png" >}}

このあたり複数人コラボレータがいても 1 collaborator と表示されたり、若干不安定なところがありますが、少なくともコラボレータからは、LUIS アプリのオーナー（作成者）が管理しているエンドポイントの情報は見えないようになっているようです。

