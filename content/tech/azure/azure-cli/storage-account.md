---
title: "逆引き Azure CLI: ストレージアカウントのキーを確認する (storage account keys list)"
url: "/p/3wk5vnw"
date: "2020-03-17"
tags: ["Azure", "Azure CLI", "Azure Storage"]
weight: 200
---

Azure ストレージアカウントのキー（鍵）情報を取得するには、[Azure にログイン](/p/ejar7k8) した状態で、以下のように実行します。

{{< code >}}
$ az storage account keys list --account-name ストレージアカウント名
[
  {
    "keyName": "key1",
    "permissions": "Full",
    "value": "7s+V+j4CcwDNHyTvx7/UXlgKN4HvFoUuIhOuzH1YLaBWgVTWQadQB2vFjRPTAv2aoGP2mpyQcMm4C+R55o3N9g=="
  },
  {
    "keyName": "key2",
    "permissions": "Full",
    "value": "+uzKfRt3rCP7RkNBXG93lqEqD7MLvgFQmKxudWHjYbDMUeFH0VmdMhN8V/6ChCwVANi6jaDL4ZKopfwV5RjY9g=="
  }
]
{{< /code >}}

このストレージアカウントキーは、`az storage` コマンドを使って Azure ストレージ上のデータを操作するときに必要になります。

{{< reference >}}
- [BLOB ストレージにファイルをアップロードする (storage blob upload)](/p/ptx36ue)
{{< /reference >}}

