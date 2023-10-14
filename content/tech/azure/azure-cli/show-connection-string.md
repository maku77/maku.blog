---
title: "逆引き Azure CLI: ストレージアカウントの接続文字列を確認する (storage account show-connection-string)"
url: "p/hquhjki/"
date: "2020-03-17"
tags: ["Azure", "Azure CLI", "Azure Storage"]
weight: 200
---

Azure CLI で Azure ストレージアカウントの接続文字列（アカウント名とキーがペアになったもの）を取得するには、**`az storage account show-connection-string`** コマンドを使用します。

このコマンドを実行する前に、[az login で Azure にログイン](/p/ejar7k8) しておく必要があります。

{{< code title="ストレージアカウントの接続文字列を取得" >}}
$ az storage account show-connection-string --name yourstorage
{
  "connectionString": "DefaultEndpointsProtocol=https;EndpointSuffix=core.windows.net;AccountName=yourstorage;AccountKey=7s+V+j4CcwDNHyTvxTAv2aoGP2mpyQcMm4C+R7/UXlgKN4HvFoUuIhOuzH1YLaBWgVTWQadQB2vFjRP55o3N9g=="
}
{{< /code >}}

{{% maku-common/reference %}}
- [az storage account show-connection-string コマンド](https://docs.microsoft.com/en-us/cli/azure/storage/account?view=azure-cli-latest#az-storage-account-show-connection-string)
{{% /maku-common/reference %}}

