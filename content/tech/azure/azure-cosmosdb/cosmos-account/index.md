---
title: "Azure Cosmos DB にアカウントを作って MongoDB API でアクセスする"
url: "/p/cd9bg3x"
date: "2019-05-21"
tags: ["Azure", "Cosmos DB", "MongoDB"]
weight: 101
---

Azure Cosmos DB と MongoDB API
----

Azure Cosmos DB は、Microsoft の Azure 上に配置できるスケーラブルなデータベースで、**SQL や MongoDB API など様々なインタフェースでアクセスできる**ようになっています。

MongoDB を使った既存のアプリケーションがある場合、接続先を Azure Cosmos DB アカウントのアドレスに変更するだけで、簡単にクラウド上のデータを扱えるようになります。
ローカルの MongoDB サーバ (`mongod`) に接続する代わりに、Azure Cosmos DB に接続するということです。

ここでは、MongoDB API（MongoDB シェル）による Azure Cosmos DB へのアクセスを試してみます。
まずは、Azure 上に Cosmos DB のリソースを作成します。


Azure Cosmos DB アカウントを作成する
----

[Azure ポータル](https://portal.azure.com/)へログインし、<samp>Azure Cosmos DB</samp> のページを開き、<samp>Azure Cosmos DB アカウントの作成</samp> をクリックします。

{{< image w="700" border="true" src="cosmos-account-001.png" >}}

次の画面では、<samp>アカウント名</samp> や <samp>API</samp> の種類を設定します。

{{< image w="500" border="true" src="cosmos-account-002.png" >}}

<samp>アカウント名</samp> に入力した値は、下記のように接続 URI の一部として使われます。
よって、この**アカウント名は世界中で一意**である必要があります。

{{< code >}}
mongodb://＜アカウント名＞:＜キー＞@＜アカウント名＞.documents.azure.com:10255/?ssl=true&replicaSet=globaldb
{{< /code >}}

<samp>API</samp> の種類には、**MongoDB API** を指定してください。
各項目の入力が終わったら、<samp>確認と作成</samp> を押して数分待つと、Azure Cosmos DB アカウントの作成が完了します。


MongoDB シェル (mongo) を使って Cosmos DB へ接続する
----

MongoDB クライアントから Azure Cosmos DB へ接続するには、接続用の URL が必要です。
Azure ポータルから Azure Cosmos DB のリソースを開き、<samp>接続文字列</samp> のページを開くと接続文字列を確認することができます。

{{< image w="600" border="true" src="cosmos-account-003.png" >}}

この URL をコピーして、`mongo` コマンドのパラメータとして渡せば、Azure Cosmos DB に接続できます。

{{< code title="MongoDB シェルから Cosmos DB へ接続" >}}
$ mongo mongodb://my-cosmos-db:ABC(キーは長いので省略)==@my-cosmos-db.documents.azure.com:10255/?ssl=true

MongoDB shell version v4.0.9
connecting to: mongodb://my-cosmos-db.documents.azure.com:10255/?gssapiServiceName=mongodb&ssl=true
WARNING: No implicit session: Logical Sessions are only supported on server versions 3.6 and greater.
Implicit session: dummy session
MongoDB server version: 3.2.0
WARNING: shell and server versions do not match
globaldb:PRIMARY>
{{< /code >}}

あとは、普通に `mongo` シェルでデータベースの操作を行うだけです。

{{< code >}}
globaldb:PRIMARY> use mydb
switched to db mydb

globaldb:PRIMARY> exit
bye
{{< /code >}}

