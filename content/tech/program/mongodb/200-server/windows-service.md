---
title: "MongoDB サーバー (mongod) を Windows サービスとして登録する"
date: "2014-01-17"
lastmod: "2019-05-21"
url: "/p/tm4jzet"
tags: ["MongoDB"]
weight: 2
---

Windows サービスとして MongoDB サービスを登録する
----

MongoDB サーバー (`mongod`) を Windows サービスとして登録しておくと、バックグラウンドで MongoDB サーバーが動作するようになるため、いちいちコマンドラインで `mongod` を起動する必要がなくなります。

MongoDB サーバーを Windows サービスとして登録するには、**コマンドプロンプトを管理者権限で起動**し、次のように入力します。

{{< code title="Windows サービスとして MongoDB サービスを登録" >}}
C:\> mongod --install --dbpath <DBディレクトリ> --logpath <ログファイル名>
{{< /code >}}

コンソール上でログを確認することができなくなってしまうので、ログがファイルに保存されるように、ログファイル名を指定しておく必要があります。

下記は実行例です。
ここでは、データベースディレクトリとして `D:\mongo_data`、ログファイル名 `D:\mongo_log\log.txt` を指定しています。
ディレクトリはあらかじめ作成しておく必要があります。

{{< code >}}
C:\> mkdir D:\mongo_data
C:\> mkdir D:\mongo_log
C:\> mongod --install --dbpath D:\mongo_data --logpath D:\mongo_log\log.txt

Fri Jan 17 12:11:49.699 Trying to install Windows service 'MongoDB'
Fri Jan 17 12:11:49.711 Service 'MongoDB' (Mongo DB) installed with command line 'C:\app\mongodb\mongod.exe --dbpath D:\mongo_data --logpath D:\mongo_log\log.txt --service'
Fri Jan 17 12:11:49.712 Service can be started from the command line with 'net start MongoDB'
{{< /code >}}

これで、**`MongoDB`** という名前の Windows サービスとして認識されるようになります。
`sc` コマンドで、実際に登録されているか確認しておきましょう。

{{< code title="MongoDB サービスが登録されているか確認" >}}
C:\> sc query MongoDB

SERVICE_NAME: MongoDB
        TYPE               : 10  WIN32_OWN_PROCESS
        STATE              : 1  STOPPED
        WIN32_EXIT_CODE    : 1077  (0x435)
        SERVICE_EXIT_CODE  : 0  (0x0)
        CHECKPOINT         : 0x0
        WAIT_HINT          : 0x0
{{< /code >}}

サービス登録直後は、上記のように MongoDB サービスは停止状態 (STOPPED) になっています。


MongoDB サービスを起動する
----

Windows サービスとして登録された MongoDB サービスは下記のようにして起動します。

{{< code title="MongoDB サービスの開始" >}}
C:\> net start MongoDB
MongoDB サービスを開始します...
MongoDB サービスは正常に開始されました。
{{< /code >}}

次のようなエラーが発生する場合は、データベースファイルやログファイルを格納するためのディレクトリが存在しているかを確認してください。

{{< code >}}
C:\> net start MongoDB
The service is not responding to the control function.
More help is available by typing NET HELPMSG 2186.
{{< /code >}}

デフォルトポート (27017) で起動した MongoDB サービスには、下記のようにパラメータなしで MongoDB シェル (`mongo`) を実行すれば接続できます。

{{< code title="mongo シェルで接続" >}}
C:\> mongo
MongoDB shell version v4.0.9
...
{{< /code >}}


Windows サービスから MongoDB サービスを削除する
----

MongoDB サービスを Windows サービスから登録解除したいときは下記のように実行します。
この場合も、**コマンドプロンプトは管理者権限で実行**しなければいけないことに注意してください。

{{< code title="Windows サービスから MongoDB サービスを削除" >}}
C:\> mongod --remove
{{< /code >}}

{{< code title="実行例" >}}
C:\Windows\system32> mongod --remove
2019-05-21T23:47:37.500+0900 I CONTROL  [main] Automatically disabling TLS 1.0, to force-enable TLS 1.0 specify --sslDisabledProtocols 'none'
2019-05-21T23:47:37.502+0900 I CONTROL  [main] Trying to remove Windows service 'MongoDB'
2019-05-21T23:47:37.504+0900 I CONTROL  [main] Service 'MongoDB' removed
{{< /code >}}

もちろん、`sc` コマンドを使ってサービスを削除することもできます。

{{< code >}}
C:\> sc stop MongoDB
C:\> sc delete MongoDB
{{< /code >}}

