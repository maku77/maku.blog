---
title: "rclone コマンドで Cloudflare R2 とファイルを同期する"
url: "p/p6o6m3i/"
date: "2023-05-04"
tags: ["Cloudflare", "rclone"]
---

rclone とは？
----

[rclone](https://rclone.org/) は、Cloudflare R2 や AWS S3、Azure Blob Storage などのクラウドストレージにファイルを同期アップロードするためのコマンドラインツールです。
似たようなコマンドラインツールとしては `rsync` がありますが、`rsync` が SSH での通信をベースとしているのに対し、`rclone` は様々なクラウドサービスへの接続をターゲットにしています。

ここでは、同期先のストレージサービスとして [Cloudflare R2](https://www.cloudflare.com/ja-jp/products/r2/) を使うことを想定し、ローカル PC 上のファイル（ディレクトリ）を同期してみます。

{{% note title="R2 vs S3" %}}
R2 は AWS S3 や Azure Blob Storage と比べて、エグレス料金が無料（下り転送が無料）という太っ腹な料金体系になっており、今後の展開が楽しみなサービスです。
ちなみに、R2 という名前は、S3 より前という意味が込められているとのこと。
{{% /note %}}


rclone のインストール
----

`rclone` コマンドは Golang で実装されているため、単一の実行バイナリとして提供されています。
下記のページから各 OS 用の実行ファイルをダウンロードして、パスの通ったディレクトリに置くだけでインストールは完了です。

- [Rclone downloads](https://rclone.org/downloads/)

macOS や Windows であれば、パッケージマネージャでインストールするのが簡単です。

{{< code lang="console" title="macOS の Homebrew でインストール" >}}
$ brew install rclone
{{< /code >}}

{{< code lang="console" title="Windows の Chocolatey でインストール" >}}
$ choco search rclone
$ choco install rclone
{{< /code >}}

{{< code lang="console" title="Windows の Scoop でインストール" >}}
$ scoop install rclone
{{< /code >}}

次のように `rclone` コマンドを実行できるようになれば OK です。

```console
$ rclone version
rclone v1.62.2
...
```


Cloudflare R2 用の API トークンの作成
----

### R2 アクセス用のエンドポイント URL を確認する

`rclone` による同期先として、Cloudflare R2 を使用する場合、設定ファイルでエンドポイント URL の指定が必要になります。
エンドポイント URL は、Cloudflare のアカウント ID を組み合わせたものになるので、次のようにアカウント ID を確認しておきます。

1. [Cloudflare ダッシュボード](https://dash.cloudflare.com/) を開きます（Cloudflare のアカウントがなければ作成してください）。
2. サイドバーから __R2__ を選択し、__Account ID__ （32 文字の文字列）を確認します（R2 ではなく Workers のページなどでも確認できます）。

R2 のエンドポイント URL は、このアカウント ID を組み合わせた次のような URL です。

{{< code title="R2 エンドポイント URL" >}}
https://<アカウントID>.r2.cloudflarestorage.com
{{< /code >}}

### R2 用の API トークンを発行する

R2 バケットを `rclone` コマンドで操作するには、AWS 互換の API トークン（__Access Key ID__ と __Secret Access Key__）を使用します。
下記のように辿ると、R2 用の API トークンを発行できます。

1. [Cloudflare ダッシュボード](https://dash.cloudflare.com/) を開きます
2. サイドバーから __`R2`__ を選択し、__`Manage R2 API Tokens`__ をクリックします。
3. __`Create API token`__ ボタンを押して、新規トークンの設定画面を開きます。
4. R2 バケットへの書き込みを許可するため、Permissions で __`Edit`__ を選択してトークンを発行します。
   - Edit: Allow edit access of all objects and List, Write, and Delete operations of all buckets

次のような感じの API トークンが発行されれば成功です。

- Access Key ID: `04db978c136...省略...ae05e2ec7e`
- Secret Access Key: `fef28946ebfa44e34b2e...省略...ffd9f76069eed6a039b`


rclone でファイル同期
----

### 接続先を登録する

`rclone` の接続先の情報は、下記のような設定ファイル (__`rclone.conf`__) に記述しておく必要があります。
ここでは、__`r2`__ という名前の接続先を登録しています。
`rclone` コマンドでは、R2 は S3 互換サービスの一種という扱いなので、`type` には `s3` を指定することに注意してください。

{{< code lang="ini" title="~/.config/rclone/rclone.conf" >}}
[r2]
type = s3
provider = Cloudflare
access_key_id = 04db978c136...省略...ae05e2ec7e
secret_access_key = fef28946ebfa44e34b2e...省略...ffd9f76069eed6a039b
endpoint = https://abcde12345abcde12345abcde12.r2.cloudflarestorage.com
acl = private
{{< /code >}}

`rclone config` コマンドを使って、質問形式で設定ファイルを作成・修正することもできます。

### ディレクトリの内容を同期してみる

あとは、__`rclone sync`__ コマンドなどを使って、ローカルとリモートのファイルを同期させることができます。
次の例では、ローカルの `dir` ディレクトリ内のすべてのファイルを R2 側の `my-bucket` バケットに同期させています（バケットがない場合は自動的に生成されます）。
`sync` という名前のサブコマンドですが、ディレクトリの内容が変化するのは、転送先のバケットのみです。

{{< code lang="console" title="R2 バケットに dir ディレクトリ内のファイルをアップロード" >}}
$ rclone sync ./dir r2:my-bucket      # R2 側には dir ディレクトリを作成しない
$ rclone sync ./dir r2:my-bucket/dir  # R2 側にも dir ディレクトリを作成する
{{< /code >}}

逆に、リモートのファイルをダウンロードしたいときは、引数の順番を入れ替えます。

{{< code lang="console" title="R2 バケットの内容を local_dir に取得" >}}
$ rclone sync r2:my-bucket ./local_dir
{{< /code >}}


（応用）rclone のオプションとサブコマンド
----

### 同期の内容を詳しく表示する (-v, --progress)

`rclone sync` コマンドを __`-v`__ オプションを付けて実行すると、各ファイルがどのように処理されたかをデバッグメッセージの形で出力してくれます（__`-vv`__ とするとさらに詳しく表示します）。
`rclone sync` コマンドは、オプションなしで実行すると、簡単な統計情報しか表示してくれないので、`-v` オプションを指定して実行すると分かりやすいです。

```console
$ rclone sync -v ./dir r2:my-bucket
2023/05/05 22:25:09 INFO  : old/a.txt: Deleted
2023/05/05 22:25:09 INFO  : old/b.txt: Deleted
2023/05/05 22:25:09 INFO  : There was nothing to transfer
2023/05/05 22:25:09 INFO  :
Transferred:      0 B / 0 B, -, 0 B/s, ETA -
Checks:           5 / 5, 100%
Deleted:          2 (files), 0 (dirs)
Elapsed time:     0.7s
```

各ファイルがどう処理されたかは、__`-P/--progress`__ オプションでも確認することができます。
こちらは、`-v` オプションとは違い、現在処理中の内容がリアルタイムに切り替わりながら表示されます（最終的には統計情報のみが出力された状態になります）。

```console
$ rclone sync --progress ./dir r2:my-bucket
Transferred:      67 B / 67 B, 100%, 22 B/s, ETA 0s
Checks:           102 / 209, 49%
Transferred:      1 / 1, 100%
Elapsed time:     4.3s
Checking:
 *        mcd-1992-tenbu-mega-cd-special/box.png: checking
 *       mcd-1992-tenbu-mega-cd-special/snap.png: checking
 *      mcd-1992-tenbu-mega-cd-special/title.png: checking
 *             mcd-1992-thunder-storm-fx/box.png: checking
 *            mcd-1992-thunder-storm-fx/snap.png: checking
 *           mcd-1992-thunder-storm-fx/title.png: checking
 *                     mcd-1992-time-gal/box.png: checking
 *                    mcd-1992-time-gal/snap.png: checking
```

### Dry run 実行 (--dry-run)

実際には同期を行わず、どのように処理されるかをシミュレートしたいときは、__`--dry-run`__ オプションを付けて実行します。
同時に __`-vv`__ オプションを指定することで、各ファイルがどう処理されるのかを事前に確認できます。

```console
$ rclone sync -vv --dry-run ./dir r2:my-bucket
```

### 同期するファイルをパターンで指定する

__`--include`__ オプションや __`--exclude`__ オプションを指定することで、ファイル名のパターンをもとに同期するファイルをフィルタリングすることができます。
次の例では、画像ファイルのみを同期しています。

```console
$ rclone sync --include "*.{jpg,png,webp}" --delete-excluded ./dir r2:my-bucket
```

パターンに一致しなかったファイルを同期先から削除するには、__`delete-excluded`__ オプションを一緒に指定する必要があることに注意してください。
`rclone` のフィルタリング機能は奥が深いので、詳しくは公式サイトの [Rclone Filtering](https://rclone.org/filtering/) のページを参照してください。

### 登録されているリモートの一覧を表示する (listremote)

設定ファイル (`~/.config/rclone/rclone.conf`) で登録されているリモートの一覧を表示するには、__`listremotes`__ サブコマンドを使用します。

```console
$ rclone listremotes
r2:
```

### バケットとファイルの一覧を表示する (ls)

特定のリモートのバケットとファイル（オブジェクト）の一覧を確認するには、__`ls`__ サブコマンドを使用します。

{{< code lang="console" title="リモート (r2) のオブジェクトの一覧を表示" >}}
$ rclone ls r2:
     2323 my-bucket/a.txt
      200 my-bucket/b.txt
     1309 my-bucket/c.txt
   763183 my-bucket2/test/nanachi.gif
{{< /code >}}

