---
title: "逆引き Azure CLI: プロキシ環境下で Azure CLI (az) を使用する"
url: "/p/7g9seub"
date: "2020-03-17"
tags: ["Azure", "Azure CLI", "プロキシ設定"]
weight: 102
---

会社などのプロキシ環境下において `az` コマンドを実行する場合は、環境変数 `https_proxy` を設定しておきます。

{{< code title="Windows の場合" >}}
C:\> set https_proxy=http://proxy.example.com:8080
{{< /code >}}

{{< code title="Linux/Mac の場合" >}}
$ export https_proxy=http://proxy.example.com:8080
{{< /code >}}

