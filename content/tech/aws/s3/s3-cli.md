---
title: "AWS S3 をコマンドライン (CLI) で操作する"
url: "/p/4ju5eow"
date: "2021-02-16"
lastmod: "2021-03-03"
tags: ["AWS"]
weight: 1
---

{{% private %}}
- [AWS CLI での Amazon S3 の使用](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/cli-services-s3.html)
{{% /private %}}

AWS CLI で S3 バケットの操作を行う場合、大きく分けて以下の 2 種類のコマンドがあります。

- __`aws s3`__ ... 高レベルコマンド
- __`aws s3api`__ ... API レベルコマンド

基本的には、`aws s3` コマンドでカバーできない操作が出てきたときに `aws s3api` コマンドの方を調べてみるというやり方でいいと思います。
`aws s3` コマンドの方は、OS のコマンドラインシェルのファイル操作コマンドのような体系になっています（`aws s3 ls` とか `aws s3 rm` とか）。

S3 バケットの操作を行うには、IAM ユーザーに適切な権限が割り当てられている必要がありますが、__`AmazonS3FulAccess`__ 管理ポリシーがあればほとんどの操作が可能です。


S3 バケットを作成する (s3 mb)
----

{{< code >}}
$ aws s3 mb s3://<バケット名>
{{< /code >}}

{{< code title="実行例" >}}
$ aws s3 mb s3://makutemp-123456789012-bucket-1
make_bucket: makutemp-123456789012-bucket-1
{{< /code >}}

{{% note title="同名バケットの再生成でエラー" %}}
バケットの削除後に、同名のバケットを再生成しようとすると、次のような __conflicting conditional operation__ のエラーになることがあります。
この場合は、バケットの削除処理が完了するまでしばらく待つ必要があります。

{{< code >}}
make_bucket failed: s3://makutemp-123456789012-bucket-1 An error occurred (OperationAborted) when calling the CreateBucket operation: A conflicting conditional operation is currently in progress against this resource. Please try again.
{{< /code >}}

このような待ち時間を防ぐためには、`s3 rb` でバケットを削除した後で再生成するのではなく、`s3 rm --recursive` でバケット内のオブジェクトだけをすべて削除します。
{{% /note %}}

S3 バケットの作成先リージョンを明示するには、__`--region`__ オプションを使用します。

{{< code title="例: 日本のリージョンを指定" >}}
$ aws s3 mb s3://makutemp-123456789012-bucket-1 --region ap-northeast-1
{{< /code >}}

参考: [S3 バケットのリージョンを確認する](#get-bucket-location)


S3 バケットの一覧、オブジェクトの一覧を表示する (s3 ls)
----

### S3 バケットの一覧を取得

{{< code >}}
$ aws s3 ls
{{< /code >}}

{{< code title="実行例" >}}
$ aws s3 ls
2021-02-18 04:43:42 makutemp-123456789012-bucket-1
2021-02-19 05:14:48 makutemp-123456789012-bucket-2
2021-02-20 19:32:05 makutemp-123456789012-bucket-3
{{< /code >}}


### バケットに含まれるオブジェクトの一覧を取得

{{< code >}}
$ aws s3 ls s3://<バケット名>
{{< /code >}}

{{< code title="例: バケット内のファイルを取得" >}}
$ aws s3 ls s3://makutemp-123456789012-bucket-1
2021-03-03 21:13:55        213 sample1.txt
2021-03-03 21:14:30        593 sample2.txt
2021-03-03 21:15:13        477 sample3.txt
{{< /code >}}

{{< code title="例: 指定したディレクトリ内のファイルを取得" >}}
$ aws s3 ls s3://makutemp-123456789012-bucket-1/dir/
2021-03-03 19:13:55         53 hoge1.txt
2021-03-03 19:14:30         37 hoge2.txt
{{< /code >}}

ディレクトリ名まで指定する場合は、`dir/` のように最後のスラッシュが必要です。
これを付けないと、`dir` というファイルを探してしまいます。

{{< code title="例: 深いディレクトリのファイルまですべて取得" >}}
$ aws s3 ls s3://makutemp-123456789012-bucket-1 --recursive
2021-03-03 19:13:55         53 dir/hoge1.txt
2021-03-03 19:14:30         37 dir/hoge2.txt
2021-03-03 21:13:55        213 sample1.txt
2021-03-03 21:14:30        593 sample2.txt
2021-03-03 21:15:13        477 sample3.txt
{{< /code >}}


オブジェクトをコピーする (s3 copy)
----

{{< code >}}
$ aws s3 cp <source> <target> [--options]
{{< /code >}}

__`s3 copy (cp)`__ コマンドでは、ローカルファイルのバケットへの転送、バケットからのダウンロード、バケット内でのコピーなどを行うことができます。

### ローカル → S3 バケット

{{< code title="例: ローカルの sample.txt を my-bucket バケットにアップロード" >}}
$ aws s3 cp sample.txt s3://my-bucket
{{< /code >}}

{{< code title="例: ローカルの data ディレクトリ内のファイルを my-bucket バケットにアップロード" >}}
$ aws s3 cp data s3://my-bucket --recursive
{{< /code >}}

S3 バケットの方にも data ディレクトリを作りたければ、ターゲットを明示的に `s3://my-bucket/data` とする必要があります。
更新されたファイルだけを効率的にアップロードしたいときは、[s3 sync](#s3-sync) コマンドを使ってください。

### S3 バケット → ローカル

{{< code title="例: my-bucket バケット内のファイルをローカルの out ディレクトリにダウンロード" >}}
$ aws s3 cp s3://my-bucket/sample.txt out
{{< /code >}}

{{< code title="例: my-bucket バケット内の全てのファイルを再帰的にローカルの out ディレクトリにダウンロード" >}}
$ aws s3 cp s3://my-bucket out --recursive
{{< /code >}}

### S3 バケット → S3 バケット

{{< code title="例: bucket-1 バケット内のファイルを bucket-2 バケットにコピー" >}}
$ aws s3 cp s3://bucket-1/sample.txt s3://bucket-2
{{< /code >}}

{{< code title="例: bucket-1 バケット内のファイルを bucket-2 バケットの dir/ 以下にコピー" >}}
$ aws s3 cp s3://bucket-1/sample.txt s3://bucket-2/dir/
{{< /code >}}

{{< code title="例: bucket-1 バケット内のすべてのファイルを bucket-2 バケットにコピー" >}}
$ aws s3 cp s3://bucket-1 s3://bucket-2 --recursive
{{< /code >}}


ディレクトリの内容を同期する (s3 sync) {#s3-sync}
----

{{< code >}}
$ aws s3 sync <LocalDir> <S3Uri>
$ aws s3 sync <S3Uri> <LocalDir>
$ aws s3 sync <S3Uri> <S3Uri>
{{< /code >}}

__`s3 sync`__ コマンドは、2 回目以降の実行では更新されたファイルのみを転送します。
似たようなコマンドに `s3 cp --recursive` がありますが、すでに存在するファイルもすべて転送してしまうので効率が悪いです。

{{< code title="例: ローカルディレクトリ → S3 バケット" >}}
$ aws s3 sync photos s3://mybucket/photos
$ aws s3 sync photos s3://mybucket/photos --delete
{{< /code >}}

`--delete` オプションを付けると、同期元に存在しないファイルを同期先から削除します。

{{< code title="例: S3 バケット → ローカルディレクトリ" >}}
$ aws s3 sync s3://mybucket/photos photos
$ aws s3 sync s3://mybucket/photos photos --delete
{{< /code >}}

{{< code title="例: 同期対象外にするファイルのパターンを指定" >}}
$ aws s3 sync . s3://mybucket/myapp --exclude "*.zip"
$ aws s3 sync . s3://mybucket/myapp --exclude "*node_modules/*" --exclude "*test/*"
{{< /code >}}

注意点としては、深い階層にあるファイルにマッチさせるためには、`*hello.txt` のように先頭に `*` を付ける必要があるというところです。
つまり、ほとんどのケースでは `*` プレフィックスを付ける必要があります。
あと、`*/hello.txt` のようなスラッシュ付きのプレフィックスを付けてしまうと、最上位にあるファイルにマッチしなくなってしまうようです。
ちょっと使いにくいコマンド仕様ですね。。。


オブジェクトを削除する (s3 rm)
----

{{< code >}}
$ aws s3 rm s3://<バケット名>/<オブジェクト名>
{{< /code >}}

{{< code title="例: バケット内のファイルを 1 つ削除" >}}
$ aws s3 rm s3://my-bucket/sample1.txt
{{< /code >}}

{{< code title="例: ディレクトリ以下をすべて削除" >}}
$ aws s3 rm s3://my-bucket/dir/ --recursive
{{< /code >}}

{{< code title="例: バケット内のファイルをすべて削除（バケットを空にする）" >}}
$ aws s3 rm s3://my-bucket --recursive
{{< /code >}}

バケットのバージョニングが有効になっている場合は、バケットの中身を空にすることはできません。


バケットを削除する
----

バージョニング設定が無効であれば、下記のコマンドでバケットを削除することができます。

{{< code >}}
$ aws s3 rb s3://<バケット名>
{{< /code >}}

ただし、空ではないバケット（オブジェクトを含むバケット）を削除しようとすると、`BucketNotEmpty` というエラーが発生します。

{{< code title="例: バケット削除エラー" >}}
$ aws s3 rb s3://my-bucket

remove_bucket failed: s3://my-bucket An error occurred (BucketNotEmpty)
when calling the DeleteBucket operation: The bucket you tried to delete
is not empty
{{< /code >}}

含まれているオブジェクトもすべて削除してしまってよい場合は、バケット削除時に __`--force`__ オプションを使用します。
S3 Glacier ストレージクラスに移行済みのオブジェクトも含めて、オブジェクトがすべて削除されます。

{{< code title="例: 中のオブジェクトを含めてバケットを削除" >}}
$ aws s3 rb s3://my-bucket --force
{{< /code >}}


バケットのタグ関連の操作
----

### バケットのタグを取得する (s3api get-bucket-tagging)

{{< code >}}
$ aws s3api get-bucket-tagging --bucket <バケット名>
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code >}}
$ aws s3api get-bucket-tagging --bucket bucket-123456789012-hello
TagSet:
- Key: DeploySlot
  Value: prod
- Key: Department
  Value: Marketing
- Key: CostCenter
  Value: 1234ABCD
{{< /code >}}
{{< /accordion >}}

### バケットにタグを設定する (s3api put-bucket-tagging)

{{< code >}}
$ aws s3api put-bucket-tagging --bucket <バケット名> --tagging=<タグセット>
{{< /code >}}

次の例では、2 つのタグを S3 バケットにセットしています。
既存のタグは上書きされてしまうことに注意してください。

{{< code title="実行例" >}}
$ aws s3api put-bucket-tagging --bucket bucket-123456789012-hello
    --tagging "TagSet=[{Key=key1,Value=val1},{Key=key2,Value=val2}]"
{{< /code >}}

JSON ファイルでタグの内容を記述しておいて、それを `--tagging` オプションで渡すこともできます。

{{< code title="実行例" >}}
$ aws s3api put-bucket-tagging --bucket bucket-123456789012-hello \
    --tagging file://tagging.json
{{< /code >}}

{{< accordion title="tagging.json" >}}
{{< code lang="json" >}}
{
  "TagSet": [
    {"Key": "key1", "Value": "val1"},
    {"Key": "key2", "Value": "val2"},
    {"Key": "key3", "Value": "val3"}
  ]
}
{{< /code >}}
{{< /accordion >}}


バケットのバージョニング (s3api put-bucket-versioning)
----

バケットのバージョニングを有効にすると、S3 バケットに同名のキーでファイルをアップロードしたときに、過去のバージョンが残るようになります。

具体的には、バージョニング有効時にファイルをアップロードすると、そのオブジェクトに一意の __バージョン ID__ が割り当てられて、過去のファイルと区別できるようになります。
バージョニング無効時にアップロードされたファイルのバージョン ID は __null__ になります。

| キー | バージョン ID | 説明 |
| ---- | ---- | ---- |
| `hello.txt` | `8PqtjVIxmjtn_aJ3aVxAJHVxOWNdWlXk` | 3 回目のアップロード（バージョニング有効時） |
| `hello.txt` | `2vE4HOEHRPPd5hSqAazAHQczmQnmmlQ7` | 2 回目のアップロード（バージョニング有効時） |
| `hello.txt` | null | 1 回目のアップロード（バージョニング無効時） |

{{< code title="例: バージョニングを有効にする" >}}
$ aws s3api put-bucket-versioning --bucket mybucket \
    --versioning-configuration Status=Enabled
{{< /code >}}

{{< code title="例: バージョニングを無効にする" >}}
$ aws s3api put-bucket-versioning --bucket mybucket \
    --versioning-configuration Status=Suspended
{{< /code >}}

バージョニングを無効に切り替えても、バージョニング有効時にアップロードしたファイル群が消えることはありません。
新しくアップロードされるファイルのバージョン ID が null になるだけです。
このとき、バージョン ID が null のオブジェクトが既に存在する場合は上書きされます（過去にバージョニング無効時にアップロードしたものがある場合）。

各バージョンのファイルは、S3 のマネージメントコンソールから個別に削除することができます。


バケットのリージョンを確認する (s3api get-bucket-location) {#get-bucket-location}
----

S3 バケットがどのリージョンに作成されているかを調べるには以下のようにします。

{{< code >}}
$ aws s3api get-bucket-location --bucket <バケット名>
{{< /code >}}

{{< code title="実行例" >}}
$ aws s3api get-bucket-location --bucket makutemp-123456789012-bucket-1
LocationConstraint: ap-northeast-1
{{< /code >}}

このコマンドでは、バケット名に `s3://` プレフィックスは付けてはいけないことに注意してください。
バケット名やバケットの ARN を指定します。

出力のフォーマットは、設定によって JSON 形式だったり YAML 形式だったりします。
上記の例は、YAML 形式です。

