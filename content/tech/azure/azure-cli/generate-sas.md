---
title: "逆引き Azure CLI: Azure ストレージの SAS トークンを生成する (storage container generate-sas)"
url: "/p/n4yqdys"
date: "2020-03-17"
tags: ["Azure", "Azure CLI", "Azure Storage"]
weight: 201
---

Azure ストレージの操作を行うとき、有効期限付きのアクセストークンである **SAS トークン** が必要になることがあります。
SAS トークンは、**`az storage container generate-sas`** コマンドで生成することができます。

{{< code title="実際は 1 行" >}}
$ az storage container generate-sas
    --name <BLOBコンテナー名>
    --expiry "2020-07-07T00:00:00Z"
    --permission acdlrw
    --connection-string <ストレージアカウントの接続文字列>
{{< /code >}}

[ストレージアカウントの接続文字列](/p/dofzeua/)さえあれば、特に Azure ログインしたりせずに生成できます。
[接続文字列自体を Azure CLI で取得する](/p/hquhjki)こともできます。

SAS トークン生成時に指定するオプションの詳細は下記のコマンドリファレンスを参照してください。

{{< reference >}}
- [az storage container generate-sas コマンド](https://docs.microsoft.com/en-us/cli/azure/storage/container?view=azure-cli-latest#az-storage-container-generate-sas)
{{< /reference >}}

実行に成功すると、次のような文字列が標準出力に出力されます。
これが SAS トークンです。

{{< code >}}
"se=2020-07-07T00%3A00%3A00Z&sp=racwdl&sv=2018-11-09&sr=c&sig=c7bapOBvLkHVlebBIEQFQc2bGd%2BjmfScqKCbkLUzzoo%3D"
{{< /code >}}

この SAS トークンは、AzCopy (`azcopy`) ツールなどで、[BLOB ストレージにファイル転送](/p/gkardu9)したりするときに必要になります。

SAS トークンの生成には、有効期限を示す `--expiry` オプションの指定が必須になっています（なくてもトークンの生成には成功しますが、使用時に認証エラーになるようです。不親切）。
ここで指定する日時のフォーマットは、少しでも間違えると出力される SAS トークンの `se` パラメータが無効なものになってしまうので要注意です（こちらも生成時にはエラーになりません。不親切）。

生成される SAS トークンの最後には、このトークン自体の署名がついています。
これにより、SAS トークン内の有効期限 (`se`) などを部分的に改ざんしたりできないようになっています。

