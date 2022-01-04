---
title: "逆引き Azure CLI: Azure にログインする (az login)"
url: "/p/ejar7k8"
date: "2020-03-17"
tags: ["Azure", "Azure CLI"]
weight: 101
---

**`az login`** コマンドを使って Azure にログインすると、Azure アカウントに紐づいた情報（サブスクリプション情報など）を取得できるようになります。

パラメータなしで実行してブラウザ上で認証することもできるし、コマンドラインからユーザ名とパスワードを指定することもできます。

{{< code title="ブラウザを起動して認証" >}}
$ az login
{{< /code >}}

{{< code title="ユーザー名をパラメータで指定" >}}
$ az login -u yourname@example.com
Password: ********
{{< /code >}}

{{< code title="ユーザー名とパスワードをパラメータで指定" >}}
$ az login -u yourname@example.com -p yourpass
{{< /code >}}

ログインが成功すると、**`az account show`** コマンドで、使用しているサブスクリプションの情報を確認できるようになります。

{{< code >}}
$ az account show
{
  "environmentName": "AzureCloud",
  "homeTenantId": "b431a0d2-3656-ed42-9497-c0dfd20ae040",
  "id": "343f3fd9-1f19-1d49-7b92-5f365bbc6fd6",
  "isDefault": true,
  "managedByTenants": [],
  "name": "従量課金",
  "state": "Enabled",
  "tenantId": "b431a0d2-3656-ed42-9497-c0dfd20ae040",
  "user": {
    "name": "yourname@example.com",
    "type": "user"
  }
}
{{< /code >}}

{{% private %}}
[Azure CLI を使用してサインインする](https://docs.microsoft.com/ja-jp/cli/azure/authenticate-azure-cli?view=azure-cli-latest)
{{% /private %}}

