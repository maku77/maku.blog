---
title: "AWS の初期設定: AWS CLI と認証情報の設定"
url: "/p/j5iu7it"
date: "2021-02-07"
tags: ["AWS", "AWS/CLI"]
weight: 1
---

AWS CLI とは？
----

AWS CLI (Command Line Interface) を使うと、AWS の各種サービスをコマンドラインから操作することができます。
例えば、次のように様々な処理を実行できます。

{{< code title="AWS CLI の使用例" >}}
### S3 バケットを作成する
$ aws s3 mb s3://バケット名

### S3 にファイルをコピーする
$ aws s3 cp ローカルファイル s3://バケット名/ディレクトリ名

### IAM ユーザーのアクセスキーを表示する
$ aws iam list-access-keys --user-name=ユーザー名

### DynamoDB にテーブルを作成する
$ aws dynamodb create-table --table-name テーブル名 ...
{{< /code >}}

AWS の各種サービスの設定は、それぞれのマネージメントコンソール（Web サイト）上で設定することができますが、AWS CLI を使うことで、Web ブラウザを開かずに設定を済ませられます。
頻繁に行う処理などは、シェルスクリプトなどにして自動化することができます。

AWS CLI を使用するには、下記のような IAM ユーザーのアクセスキー（及びシークレットアクセスキー）が必要です。
以降の説明では、これらの情報が取得できていることを前提とします。

- API 用アクセスキー（例）: `INMCAKIAQX77PNSXQAGP`
- API 用シークレットアクセスキー（例）: `2ufOA3Q2KAmpNTo2GHPDXFJK7Jiv9yfZGegIdbCW`

{{< reference >}}
- [IAM ユーザーのアクセスキーを作成するには - AWS CLI](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/cli-configure-quickstart.html#cli-configure-quickstart-creds)
{{< /reference >}}


AWS CLI のインストール
----

### インストール方法

- [macOS の場合](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-mac.html) ... [AWSCLIV2.pkg](https://awscli.amazonaws.com/AWSCLIV2.pkg) をダウンロードしてインストールできます。
- [Windows の場合](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-windows.html) ... [AWSCLIV2.msi](https://awscli.amazonaws.com/AWSCLIV2.msi) をダウンロードしてインストールできます。
- [Linux の場合](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html) ... `curl` コマンドでインストール用スクリプトをダウンロードしてインストールできます。

### インストール後の確認

何らかの `aws` コマンドが実行できるようになっていれば成功です。

{{< code lang="shell" >}}
$ aws --version
aws-cli/2.1.24 Python/3.7.4 Darwin/19.5.0 exe/x86_64 prompt/off
{{< /code >}}


AWS CLI の設定 (aws configure)
----

{{< private >}}
- [AWS CLI - 設定の基本](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/cli-configure-quickstart.html)
{{< /private >}}

### 設定ファイルを作成する

AWS CLI を使用して AWS の各種操作を行うには、credentials ファイルにアカウントキー情報を設定しておく必要があります。
__`aws congigure`__ コマンドで設定を行えます。

{{< code >}}
$ aws configure
AWS Access Key ID [None]: INMCAKIAQX77PNSXQAGP
AWS Secret Access Key [None]: 2ufOA3Q2KAmpNTo2GHPDXFJK7Jiv9yfZGegIdbCW
Default region name [None]: ap-northeast-1
Default output format [None]: yaml
{{< /code >}}

デフォルトリージョンは、AWS CLI でコマンド実行したときに、どのリージョンのサーバーにリクエストが送信されるかの設定です。
通常は、最寄りのリージョン（東京であれば `ap-northeast-1`）を指定しておけば大丈夫です（[リージョンの一覧はこちら](https://docs.aws.amazon.com/general/latest/gr/rande.html#regional-endpoints)）。
リージョンの指定は、`aws` コマンドを実行する際に `--region` オプションで指定することも可能です。
AWS CLI の出力フォーマットは、`yaml` や `json`、`table`、`text` などを指定できます。
人間にも機械にもやさしい `yaml` 形式がおすすめです（`yaml` 形式は CLI バージョン 2 から選択できます）。

設定が完了すると、`~/.aws` ディレクトリ以下に次のようなファイルとして保存されます。

{{< code lang="inf" title="~/.aws/config" >}}
[default]
region = ap-northeast-1
output = yaml
{{< /code >}}

{{< code lang="inf" title="~/.aws/credentials" >}}
[default]
aws_access_key_id = INMCAKIAQX77PNSXQAGP
aws_secret_access_key = 2ufOA3Q2KAmpNTo2GHPDXFJK7Jiv9yfZGegIdbCW
{{< /code >}}

設定を変更したいときは、上記のファイルを直接変更してもいいですし、もう一度 `aws configure` コマンドを実行しても OK です。

{{< code title="設定の一部を変更する" >}}
$ aws configure
AWS Access Key ID [****************QAGP]:
AWS Secret Access Key [****************dbCW]:
Default region name [ap-northeast-1]:
Default output format [yaml]: json
{{< /code >}}

### 現在の設定を確認する

`aws configure list` コマンドで AWS CLI の現在の設定を確認することができます。

{{< code >}}
$ aws configure list
      Name                    Value             Type    Location
      ----                    -----             ----    --------
   profile                <not set>             None    None
access_key     ****************QAGP shared-credentials-file
secret_key     ****************dbCW shared-credentials-file
    region           ap-northeast-1      config-file    ~/.aws/config
{{< /code >}}

個々の値を取得したいときは `aws configure get` コマンドを使用します。

{{< code >}}
$ aws configure get region
ap-northeast-1

$ aws configure get aws_access_key_id
AKIXXXXXXXXXXXXXQAGP

$ aws configure get aws_secret_access_key
8kR6XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXdbCW
{{< /code >}}

### （応用）別のプロファイルを作成する

別の IAM ユーザーのアクセスキーを使用したい場合などは、別のプロファイル設定を追加することで対応できます。
次の例では、`power` という名前のプロファイルを作成しています。

{{< code >}}
$ aws configure --profile=power
AWS Access Key ID [None]: XXXXXXXXXXXXXXXXXXXX
AWS Secret Access Key [None]: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Default region name [None]: ap-northeast-1
Default output format [None]: yaml
{{< /code >}}

これにより、設定ファイル内にそのプロファイル用の設定が追加されます。

{{< code lang="inf" title="~/.aws/config" >}}
[default]
region = ap-northeast-1
output = yaml

[profile power]
region = ap-northeast-1
output = yaml
{{< /code >}}

{{< code lang="inf" title="~/.aws/credentials" >}}
[default]
aws_access_key_id = INMCAKIAQX77PNSXQAGP
aws_secret_access_key = 2ufOA3Q2KAmpNTo2GHPDXFJK7Jiv9yfZGegIdbCW

[power]
aws_access_key_id = XXXXXXXXXXXXXXXXXXXX
aws_secret_access_key = XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{{< /code >}}

AWS CLI コマンドを使用するときにプロファイルを切り替えたいときは、`aws` コマンドの __`--profile`__ オプションか、__`AWS_DEFAULT_PROFILE`__ 環境変数でプロファイル名を指定します。

{{< code title="プロファイルを切り替えて AWS CLI を実行" >}}
（方法1）
$ aws --profile=power configure list

（方法2）
$ export AWS_DEFAULT_PROFILE=power
$ aws configure list
{{< /code >}}

