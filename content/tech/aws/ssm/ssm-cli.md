---
title: "AWS SSM をコマンドライン (CLI) で操作する"
url: "/p/aug76s5"
date: "2021-10-13"
tags: ["AWS", "AWS/SSM"]
---

{{% private %}}
--key-id パラメータを使用して類似したカスタマー管理の CMK を指定します
```
$ aws ssm put-parameter --name param1 --value "secret"
    --type SecureString --key-id 1234abcd-12ab-34cd-56ef-1234567890ab
```
{{% /private %}}


パラメーターストアにパラメーターを格納する (ssm put-parameter)
----

### String（文字列）

{{< code title="/myapp/param1 という名前のパラメーターとして value1 を格納" >}}
$ aws ssm put-parameter --name /myapp/param1 \
    --value "value1" --type String \
    --tags "Key=key1,Value=value1"
{{< /code >}}

既存のパラメーターの値を更新する場合は、__`--overwrite`__ オプションが必要です（Version 情報がインクリメントされます）。

{{< code title="/myapp/param1 の値を value2 に書き換え" >}}
$ aws ssm put-parameter --name /myapp/param1 \
    --value "value2" --type String --overwrite
Tier: Standard
Version: 2
{{< /code >}}

### SecureString（暗号化された文字列）

{{< code title="デフォルトの AWS マネージドキーを使って暗号化" >}}
$ aws ssm put-parameter --name /myapp/param1 \
    --value "secret_value" --type SecureString
{{< /code >}}

デフォルトのキーは、アカウントで共通のものになります。

{{< code title="作成した KMS キーを使って暗号化" >}}
$ aws ssm put-parameter --name /myapp/param1 \
    --value "secret_value" --type SecureString \
    --key-id "<KeyId>" \
{{< /code >}}

`KeyId` は次のいずれかの形式で指定できます。

- ARN (Amazon Resource Name)
    - `arn:aws:kms:us-east-2:123456789012:key/12345678-1234-1234-1234-123456789012`
- エイリアス ARN
    - `arn:aws:kms:us-east-2:123456789012:alias/MyAliasName`
- キー ID
    - `12345678-1234-1234-1234-123456789012`
- エイリアス
    - `alias/MyAliasName`

独自の KMS キーを使ってパラメーターを暗号化する場合は、KMS キーの `kms:Encrypt` 権限が必要です。


### StringList（文字列のリスト）

{{< code title="value1、value2、value3 という 3 つの値を格納" >}}
$ aws ssm put-parameter --name /myapp/param1 \
    --value "value1,value2,value3" --type StringList
{{< /code >}}


パラメーターストアからパラメーターを取得する (ssm get-parameter(s))
----

### パラメーターの値を取得する

{{< code >}}
$ aws ssm get-parameter --name /myapp/param1 --output yaml
{{< /code >}}

{{% accordion title="実行結果" %}}
{{< code >}}
Parameter:
  ARN: arn:aws:ssm:ap-northeast-1:123456789012:parameter/myapp/param1
  DataType: text
  LastModifiedDate: '2021-10-13T17:37:25.108000+09:00'
  Name: /myapp/param1
  Type: SecureString
  Value: AQICAHhLiblDAUFGpE...(省略)...1T9iD+1QpcEWcerOhvQ==
  Version: 1
{{< /code >}}
{{% /accordion %}}

### SecureString の値を復号化する

パラメーターの Type が `SecureString` の場合、デフォルトでは上記のように Value は暗号化された値で返されます。
Value の値を複合化するときは __`with-decryption`__ オプションを指定します。
パラメーターを複合化には、ユーザーに `kms:Decrypt` 権限が必要です。

{{< code title="パラメータ値を復号化する場合" >}}
$ aws ssm get-parameter --name /myapp/param1 --output yaml
{{< /code >}}

{{% accordion title="実行結果" %}}
{{< code >}}
Parameter:
  ARN: arn:aws:ssm:ap-northeast-1:123456789012:parameter/myapp/param1
  DataType: text
  LastModifiedDate: '2021-10-13T17:37:25.108000+09:00'
  Name: /myapp/param1
  Type: SecureString
  Value: "my secret value"
  Version: 1
{{< /code >}}
{{% /accordion %}}

### パラメーターの値だけを取得する

{{< code >}}
$ aws ssm get-parameter --name /myapp/param1 --query Parameter.Value
"Hello World"
{{< /code >}}

### 複数のパラメーターを同時に取得する

{{< code >}}
$ aws ssm get-parameters --names /myapp/param1 /myapp/param2
{{< /code >}}

コマンド名とオプション名に、複数形の `s` が付くことに注意してください。

{{< code title="値だけ取得したいとき" >}}
$ aws ssm get-parameters --names /myapp/param1 /myapp/param2 \
    --query "Parameters[*].Value"
{{< /code >}}


パラメーターストアからパラメーターを削除する (ssm delete-parameter)
----

{{< code title="パラメーター /myapp/param1 を削除する" >}}
$ aws ssm delete-parameter --name /myapp/param1
{{< /code >}}

- 参考: [ssm delete-parameter](https://docs.aws.amazon.com/cli/latest/reference/ssm/delete-parameter.html)


パラメーターストア内のパラメーターの変更履歴を取得する (ssm get-parameter-history)
----

`aws ssm put-parameter` でパラメーターの値を変更した場合に、その変更履歴を確認したいときは、`ssm get-parameter-history` コマンドを使用します。

{{< code >}}
$ aws ssm get-parameter-history --name /myapp/param1
{{< /code >}}


パラメーターを検索する (ssm describe-parameters)
----

{{< code title="パラメーター名が /myapp/dev/ で始まるものを検索する" >}}
$ aws ssm describe-parameters \
    --parameter-filters "Key=Name,Option=BeginsWith,Values=/myapp/dev/"
{{< /code >}}

`Option=BeginsWith,` の部分を省略すると、デフォルトの `Option=Equals` 扱いになり、完全一致での検索になります。

{{< code title="パラメーター名の一部に GitHub を含むものを検索する" >}}
$ aws ssm describe-parameters \
    --parameter-filters "Key=Name,Option=Contains,Values=GitHub"
{{< /code >}}

出力結果に、パラメータの「値」自体は含まれません。

- 参考: [describe-parameters — AWS CLI Command Reference](https://docs.aws.amazon.com/cli/latest/reference/ssm/describe-parameters.html)
- 参考: [Systems Manager のパラメータを検索する - AWS Systems Manager](https://docs.aws.amazon.com/ja_jp/systems-manager/latest/userguide/parameter-search.html)

