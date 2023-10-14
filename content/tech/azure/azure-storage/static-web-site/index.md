---
title: "Azure Storage で静的 Web サイトをホスティングする"
url: "p/gkardu9/"
date: "2020-03-17"
tags: ["Azure", "Azure Storage"]
description: "HTML や CSS、JavaScript といった静的なファイルのみを扱う Web サイトであれば、Azure Storage（BLOB ストレージ）を使って簡単にホスティングすることができます。"
weight: 200
---

（Azure Storage による静的な Web サイトのホスティング機能は [2018 年末にリリース](https://azure.microsoft.com/ja-jp/blog/static-websites-on-azure-storage-now-generally-available/) されました）

{{% private %}}
- [Azure Storage での静的 Web サイトホスティング](https://docs.microsoft.com/ja-jp/azure/storage/blobs/storage-blob-static-website)
- [チュートリアル: Blob Storage で静的な Web サイトをホストする](https://docs.microsoft.com/ja-jp/azure/storage/blobs/storage-blob-static-website-host)
{{% /private %}}

{{< image border="true" src="img-003.png" >}}

ストレージアカウントを作成する（まだ作成していない場合）
----

Azure 上に静的な Web サイトをホスティングするためのストレージを作成するには、ストレージアカウントが必要です。
まだ作成していない場合は、下記の手順に従ってストレージアカウントを作成してください。
静的な Web サイトをホスティングする場合は、`アカウントの種類 (Account kind)` の項目で **`StorageV2（汎用v2）`** というのを選んで作成しておく必要があります。

→ [Azure のストレージアカウントを作成する](/p/7axgzfu/)


静的な Web サイトを有効にする
----

ストレージアカウントを作成したら、コンテンツのアップロード先である Azure ストレージコンテナーと、Web サイトの URL を生成します。
といっても、ストレージアカウントがあれば、[Azure ポータル](https://portal.azure.com/) から数秒で自動作成できます。

{{< image border="true" src="img-001.png" >}}

1. ストレージアカウントのページを開き、`設定` → `静的な Web サイト` を選択します。
2. `静的な Web サイト` のスイッチを **`有効`** に切り替えて `保存` ボタンを押します。

これで、Web サイトをホスティングするための Azure ストレージコンテナー（BLOB を入れるコンテナー）が作成されます。
コンテナー名は自動的に **`$web`** になるようです。

同時に、Web サイトの URL も自動的に生成されます。

{{< image border="true" src="img-002.png" >}}

これが Web ブラウザからサイトにアクセスするときのアドレスになります。
あとは、コンテナーに HTML ファイルをアップロードするだけです。


Web サイトのコンテンツをアップロードする
----

作成されたコンテナー (`$web`) にコンテンツをアップロードするには、Azure CLI などのコマンドラインツールを使用します。
Azure ポータルのサイト上で BLOB リソースの項目からポチポチやってアップロードすることもできますが、自動化のことを考えると、コマンドラインツールを使った方がよいでしょう。

### Azure CLI で単一ファイルをアップロードする

手始めに、Azure CLI の `az` コマンドを使って、`index.html` をアップロードしてみます。
Azure CLI をインストールしていない場合は、先に[インストール](/p/dwsog4p)してください。

{{< code title="実際は 1 行で実行" >}}
$ az storage blob upload --account-name <ストレージアカウント名>
                         --account-key <ストレージアカウントのキー>
                         -c $web
                         --file index.html
                         --name index.html

Finished[########################################]  100.0000%
{
  "etag": "\"0x8D7CA465578BC90\"",
  "lastModified": "2020-03-17T07:39:32+00:00"
}
{{< /code >}}

Web ブラウザから、プライマリエンドポイント（`https://xxx.yyy.web.core.windows.net/`) にアクセスして、`index.html` の内容が表示されれば成功です。

{{< image border="true" src="img-003.png" >}}

{{% maku-common/reference %}}
- [逆引き Azure CLI: BLOB ストレージにファイルをアップロードする (storage blob upload)](/p/ptx36ue/)
{{% /maku-common/reference %}}


### AzCopy で複数のファイルをまとめてアップロードする

Azure CLI (`az`) では、1 つのファイル (BLOB) ずつしか転送できません。
複数のファイルをまとめて転送するには、**`azcopy`** コマンドを使用します。
各プラットフォーム用の `azcopy` コマンドは以下からダウンロードできます。

- [AzCopy v10 を使用して Azure Storage にデータをコピーまたは移動する](https://docs.microsoft.com/ja-jp/azure/storage/common/storage-use-azcopy-v10)

Windows であれば、ダウンロードした `azcopy.exe` を `C:\mybin` ディレクトリなどにコピーして PATH を通しておけばよいでしょう。

`azcopy` コマンドを使うには **SAS トークン** が必要で、これは Azure CLI（`az` コマンド）などで生成することができます。
例えば、次のようにして `$web` コンテナーにアクセスするための SAS トークンを生成します。

{{< code title="SAS トークンの生成（実際は1行）" >}}
$ az storage container generate-sas
    --name "$web"
    --expiry "2020-07-07T00:00:00Z"
    --permission racwdl
    --connection-string <ストレージアカウントの接続文字列>

"se=2020-07-07T00%3A00%3A00Z&sp=racwdl&sv=2018-11-09&sr=c&sig=c7bapOBvLkHVlebBIEQFQc2bGd%2BjmfScqKCbkLUzzoo%3D"
{{< /code >}}

{{% maku-common/reference %}}
- [逆引き Azure CLI: Azure ストレージの SAS トークンを生成する (storage container generate-sas)](/p/n4yqdys)
{{% /maku-common/reference %}}

標準出力に表示された `"se=..."` という文字列が SAS トークンです。

SAS トークンが取得できたら、それを使って [azcopy sync コマンド](https://docs.microsoft.com/ja-jp/azure/storage/common/storage-ref-azcopy-sync) で BLOB へのファイルアップロードを行うことができます（似たようなものに `azcopy copy` コマンドもありますが、`azcopy sync` の方を使うと、タイムスタンプを比較して更新されているもののみをアップロードしてくれます）。
例えば、`src` ディレクトリ内のファイルをすべてアップロードするには次のようにします。

{{< code title="ディレクトリ内のファイルをアップロード（実際は1行）" >}}
$ azcopy sync --delete-destination true src
    https://<ストレージアカウント名>.blob.core.windows.net/<コンテナー名>?<SASトークン>
{{< /code >}}

- オプションの意味:
    - `--delete-destination true`: コピー元にないファイルはコピー先から削除する

ストレージアカウント名と、SAS トークンは実際に使用するものに置き換えてください。
コンテナー名は `$web` で共通ですね。

{{< code title="実行例" >}}
$ azcopy sync --delete-destination true src https://yourstorage.blob.core.windows.net/$web?"se=..."
{{< /code >}}

認証に失敗して 40X 系のエラーが出る場合は、末尾の `?` 以降に指定する SAS トークンが間違っていることが多いです。
SAS トークンはダブルクォーテーション (`"`) で囲まれたものをそのまま指定することに注意してください。


静的な Web サイトのコスト
----

Azure Storage による静的な Web サイトをホスティングしたときの利用料金は、

* ストレージコスト（コンテンツの容量に応じた維持コスト） → [Azure Storage Overview pricing](https://azure.microsoft.com/en-us/pricing/details/storage/)
* データ送信料（Web サイトの表示時の転送量に応じたコスト） → [Bandwidth Pricing Details](https://azure.microsoft.com/en-us/pricing/details/bandwidth/)

によって決まります。
いずれにしても、小規模なサイトであればめちゃくちゃ安くすむと思います。
1GB のストレージコストは 10 円/月以下、転送サイズは 5GB までは無料で、それを超えた場合でも 10 円/GB 程度です。
1 ヵ月当たりの転送量が 100 GB を超えたりする場合は、毎月 1000 円以上取られることになるので、ちょっと考えた方がよいですね。


（おまけ）PowerShell スクリプトでアップロードを自動化する
----

下記のスクリプトを実行すると、`src` ディレクトリ内に格納されたファイル群 (HTMLファイルなど）を、`$STORAGE_ACCOUNT` で指定したストレージコンテナーに転送（同期）します。
`src` ディレクトリ内に存在しないファイルは、Azure 側からも削除されるので注意してください。

{{< code lang="powershell" title="upload.ps1" >}}
$STORAGE_ACCOUNT = 'yourstorage'  # ストレージアカウント名
$SRC_DIR = 'src'     # ローカルファイルが格納されたディレクトリ
$CONTAINER = '$web'  # 転送先の Storage コンテナ名
$CONN_STR = $env:AZURE_STORAGE_CONNECTION_STRING  # 接続文字列

# 環境変数にストレージアカウントの接続文字列がセットされているか確認
if ([string]::IsNullOrEmpty($CONN_STR)) {
    Write-Host 'AZURE_STORAGE_CONNECTION_STRING is not set'
    exit
}

# 有効期限の日時文字列を生成（現在から10分後）
$expiry = (Get-Date).AddMinutes(10).ToUniversalTime().ToString('yyyy-MM-ddTHH:mm:ssZ')

# SASトークンを生成
Write-Host 'Generate SAS Token...'
$sas = az storage container generate-sas `
    --name $CONTAINER `
    --expiry $expiry `
    --permission racwdl `
    --connection-string $CONN_STR

# src ディレクトリ内のファイルをすべてアップロード
Write-Host 'Upload Files...'
azcopy sync --delete-destination true $SRC_DIR `
    "https://$STORAGE_ACCOUNT.blob.core.windows.net/${CONTAINER}?$sas"
{{< /code >}}

事前準備としては下記が必要です。

- スクリプトの先頭にある **`$STORAGE_ACCOUNT`** 変数の値を、アップロード先のストレージアカウント名に変更する
- 環境変数 **`STORAGE_ACCOUNT_CONNECTION_STRING`** に、ストレージアカウントの「接続文字列」を指定しておく
- Azure CLI (`az`) と AzCopy (`azcopy`) を使用できるようにしておく（パスを通しておく）

あと、当然ですが、この `upload.ps1` スクリプトと同じディレクトリ内に、`src` ディレクトリを作成し、そこに `index.html` などのコンテンツファイルを格納してください。
コマンドプロンプトから実行するには下記のようにします。

{{< code >}}
C:\> powershell ./upload.ps1
Generate SAS Token...
Upload Files...
{{< /code >}}

