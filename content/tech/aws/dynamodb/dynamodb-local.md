---
title: "DynamoDB Local で DynamoDB のローカルテスト環境を作る"
url: "/p/rdq4eq2"
date: "2021-02-23"
tags: ["AWS", "AWS/DynamoDB"]
---

{{% private %}}
- [DynamoDB ローカル](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/DynamoDBLocal.html)
{{% /private %}}

DynamoDB Local とは
----

DynamoDB Local を使うと、Amazon DynamoDB サービスを模倣するローカルサーバーを立ち上げることができます。
DynamoDB Local サーバーは、デフォルトでは `http://localhost:8000` で起動し、ここに対して AWS CLI や AWS SDK で接続して操作します。

DynamoDB ウェブサービスで複雑なデータ処理を行うときは、あらかじめ DynamoDB Local を使ってテストを行っておくと安心です。
DynamoDB の操作のために試行錯誤しても AWS の利用料金がかかることはありません。


DynamoDB Local のインストール
----

### 実行ファイルのダウンロード

DynamoDB Local には、Java の実行ファイル (JAR) や、それを含む Docker コンテナとして提供されています。
Java のインストールされた環境では、JAR ファイルをダウンロードして起動するのが手っ取り早いです。
下記から ZIP ファイルでダウンロードできます。

- [DynamoDB Local のダウンロード](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/DynamoDBLocal.DownloadingAndRunning.html)

ダウンロードした ZIP ファイルを展開すると、次のような構成のディレクトリが展開されます。

{{< code title="アーカイブの内容" >}}
dynamodb_local_latest/
  +-- DynamoDBLocal_lib/  ... 本体が使う依存ライブラリ
  +-- DynamoDBLocal.jar   ... 本体
  +-- その他のドキュメント
{{< /code >}}

起動に必要なのは、`DynamoDBLocal_lib` ディレクトリと `DynamoDBLocal.jar` だけなので、この 2 つを任意のディレクトリにコピーします。
ここでは、次のようなディレクトリにコピーすることにします（Windows であれば、`$HOME` は `%USERPROFILE%` に置き換えてください）。

{{< code title="適当な場所にコピー" >}}
$HOME/mybin/dynamodb-local/
  +-- DynamoDBLocal_lib/
  +-- DynamoDBLocal.jar
{{< /code >}}

### 起動用スクリプト（エイリアス）の作成

インストール自体はこれで終わりですが、DynamoDB Local サーバーを簡単に起動できるようにするために、__`dynamodb-local`__ というコマンドラインエイリアスを定義しておきます。

#### macOS / Linux の場合

{{< code lang="bash" title="~/.bash_profile" >}}
DYNAMODB=~/mybin/dynamodb-local
alias dynamodb-local='java -Djava.library.path=$DYNAMODB/DynamoDBLocal_lib -jar $DYNAMODB/DynamoDBLocal.jar -sharedDb -dbPath $DYNAMODB'
{{< /code >}}

#### Windows の場合

Windows の場合は、サーバー起動用のバッチファイルを作ってしまうのが楽かもしれません。
例えば、環境変数 `PATH` に `%USERPROFILE%\mybin` のパスを追加し、次のようなバッチファイルを作成します。
やっていることは、上記の macOS / Linux の場合と同様ですが、コマンドライン引数を受け取れるように末尾に `%*` を追加してます。

{{< code lang="bat" title="%USERPROFILE%\mybin\dynamodb-local.cmd" >}}
@echo off
setlocal
set DYNAMODB=%USERPROFILE%\mybin\dynamodb-local
java -Djava.library.path=%DYNAMODB%\DynamoDBLocal_lib -jar %DYNAMODB%\DynamoDBLocal.jar -sharedDb -dbPath %DYNAMODB% %*
{{< /code >}}

これで、どのディレクトリからでも `dynamodb-local` コマンドを実行するだけで、DyanmoDB Local サーバーを起動できるようになります。
上記の例では、`DynamoDBLocal.jar` に次のようなオプションを渡しています。

- __`-sharedDb`__ ... 使用するアクセスキーとリージョンごとにデータベースファイル (`.db`) を作らず、`shared-local-instance.db` という単一のファイルを作成する。
- __`-dbPath $DYNAMODB`__ ... 上記の `.db` ファイルをカレントディレクトリに生成せず、実行ファイルと同じ場所に生成する。


DynamoDB Local を使用する
----

### DynamoDB Local サーバーを起動する

ここまでの設定が完了していれば、任意のディレクトリから次のように DynamoDB Local サーバーを起動することができます（終了は `Ctrl + C` です）。

{{< code >}}
$ dynamodb-local
Initializing DynamoDB Local with the following configuration:
Port: 8000
InMemory: false
DbPath: null
SharedDb: true
shouldDelayTransientStatuses: false
CorsParams: *
{{< /code >}}

サーバー起動時には、上記のようにコンフィグ値が表示され、ポート番号が 8000 番であることや、CORS アクセスが全て許可されている (`*`)、といったことが分かります。
これらの設定は、`-port` オプションや `-cors` オプションで変更することができます。

{{< code >}}
$ dynamodb-local -port 8123
{{< /code >}}

コマンドラインオプションの詳細は、次のようにしてヘルプを表示するか、[こちらのサイト](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/DynamoDBLocal.UsageNotes.html) で確認できます。

{{< code >}}
$ dynamodb-local --help
{{< /code >}}
{{< accordion title="実行結果" >}}
{{< code >}}
usage: java -Djava.library.path=./DynamoDBLocal_lib -jar DynamoDBLocal.jar
            [-port <port-no.>] [-inMemory] [-delayTransientStatuses]
            [-dbPath <path>][-sharedDb] [-cors <allow-list>]
 -cors <arg>                Enable CORS support for javascript against a
                            specific allow-list list the domains separated
                            by , use '*' for public access (default is
                            '*')
 -dbPath <path>             Specify the location of your database file.
                            Default is the current directory.
 -delayTransientStatuses    When specified, DynamoDB Local will introduce
                            delays to hold various transient table and
                            index statuses so that it simulates actual
                            service more closely. Currently works only for
                            CREATING and DELETING online index statuses.
 -help                      Display DynamoDB Local usage and options.
 -inMemory                  When specified, DynamoDB Local will run in
                            memory.
 -optimizeDbBeforeStartup   Optimize the underlying backing store database
                            tables before starting up the server
 -port <port-no.>           Specify a port number. Default is 8000
 -sharedDb                  When specified, DynamoDB Local will use a
                            single database instead of separate databases
                            for each credential and region. As a result,
                            all clients will interact with the same set of
                            tables, regardless of their region and
                            credential configuration. (Useful for
                            interacting with Local through the JS Shell in
                            addition to other SDKs)
{{< /code >}}
{{< /accordion >}}

### DynamoDB Local に接続する

DynamoDB Local サーバーを起動したら、AWS CLI（`aws` コマンド）で接続してみます。
基本的には DynamoDB ウェブサービスを操作するときと同様ですが、__`--endpoint-url`__ オプションでサーバーのアドレスを指定する必要があります。

{{< code >}}
$ aws dynamodb list-tables --endpoint-url http://localhost:8000
TableNames:
- Games
- Musics
{{< /code >}}

{{% note title="--region local オプション" %}}
`--endpoint-url http://localhost:8000` の代わりに、__`--region local`__ でも DynamoDB Local に接続できるみたいです。
ただし、ポート番号を `8000` 以外に変えた場合は接続できないので、`--endpoint-url` の方を使っておいた方が安心かもしれません。
{{% /code %}}


（おまけ）間違ってウェブサービス側の DynamoDB を操作しないようにする
----

AWS CLI（`aws` コマンド）を使う時に、`--endpoint-url` オプションを指定し忘れると、ウェブサービス側の DynamoDB を操作することになります。
このような誤操作でウェブサービス側のデータを更新してしまうのが心配なときは、次のようにアクセスキーに偽物の情報（ここでは `fake`）をセットした状態で AWS CLI コマンドを実行するようにします。

{{< code lang="bash" >}}
$ export AWS_ACCESS_KEY_ID=fake
$ export AWS_SECRET_ACCESS_KEY=fake
$ aws dynamodb list-tables --endpoint-url http://localhost:8000
TableNames:
- Games
- Musics
{{< /code >}}

このように設定すると、DynamoDB Local サーバーへのアクセスは可能なままで、ウェブサービス側の DynamoDB へのアクセスは認証エラーで弾かれるようになります。

ちなみに、`aws dynamodb` コマンドの `--endpoint-url` オプションに関しては、現状は環境変数などでデフォルト値を設定しておくことはできないようです。
下記は、[DynamoDB ローカル使用に関する注意事項 - Amazon DynamoDB](https://docs.aws.amazon.com/ja_jp/amazondynamodb/latest/developerguide/DynamoDBLocal.UsageNotes.html) からの抜粋です。

> 注記：AWS CLI では、ダウンロード可能なバージョンの DynamoDB をデフォルトのエンドポイントとして使用することはできません。そのため、各 --endpoint-url コマンドで AWS CLI を指定する必要があります。

毎回 `--endpoint-url` を指定するのも面倒なので、何らかの自動化を行いたい場合は、素直に AWS SDK を使って Node.js や Python のスクリプトを作った方がよさそうです。
SDK が生成するアクセス用インスタンスには、エンドポイント URL を指定できるようになっています。


（おまけ）プロキシ環境下で HTTP 301 エラーになる場合
----

社内のプロキシ環境内から AWS CLI で DynamoDB Local に接続するとき、次のようなエラーが発生することがあります。

{{< code >}}
$ aws dynamodb list-tables --endpoint-url http://localhost:8000

An error occurred (301) when calling the ListTables operation: <HTML>
<HEAD><TITLE>Redirection</TITLE></HEAD>
<BODY><H1>Redirect</H1></BODY>
{{< /code >}}

これは、`http_proxy` 環境変数が設定されていることが直接の原因になっています。　
この環境変数を外すか、次のように `localhost` をプロキシ対象外に設定することで接続できるようになります。

{{< code >}}
### Linux の場合
$ export NO_PROXY=localhost

### Windows の場合
$ setx NO_PROXY localhost
{{< /code >}}

{{% maku-common/reference %}}
- [Amazon DynamoDB — AWS SDK for JavaScript](https://docs.aws.amazon.com/ja_jp/sdk-for-javascript/v3/developer-guide/dynamodb-examples.html)
- [Amazon DynamoDB — Boto3 Docs documentation](https://boto3.amazonaws.com/v1/documentation/api/latest/guide/dynamodb.html)
{{% /maku-common/reference %}}

