---
title: "逆引き Azure CLI: BLOB ストレージにファイルをアップロードする (storage blob upload)"
url: "p/ptx36ue/"
date: "2020-03-17"
tags: ["Azure", "Azure CLI", "Azure Storage"]
weight: 201
---

Azure CLI を使ってストレージアカウント上に作成された、既存の BLOB コンテナにコンテンツをアップロードするには、**`az storage blob upload`** コマンドを使用します。
Azure ポータルのサイト上でポチポチやってアップロードすることもできますが、自動化のことを考えると、コマンドラインを使った方がよいでしょう。

{{< code title="書式" >}}
az storage blob upload --account-name <ストレージアカウント名> --account-key <キー> -c <コンテナ名> --file <ローカルファイル名> --name <アップロード後のファイル名>
{{< /code >}}

{{< code title="実行例" >}}
$ az storage blob upload --account-name yourstorage --account-key vFjRP7s+V+j4CcwDNHyTvxT7/UXlgKN4HvFoUuIhOuzH1YLaBWgVTWQadQB2Av2aoGP2mpyQcMm4C+R55o3N9g== -c $web --file index.html --name index.html
Finished[#############################################################]  100.0000%
{
  "etag": "\"0x8D7CA465578BC90\"",
  "lastModified": "2020-03-17T07:39:32+00:00"
}
{{< /code >}}

アップロード時にストレージアカウントのキーを指定するので、あらかじめ `az login` で Azure にログインしておく必要はありません。

{{% maku-common/reference %}}
- [逆引き Azure CLI: ストレージアカウントのキーを確認する (storage account keys list)](/p/3wk5vnw)
{{% /maku-common/reference %}}

