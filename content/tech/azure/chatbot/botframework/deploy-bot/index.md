---
title: "チャットボット (2-3) Azure の Web App Bot リソースにボットをデプロイする"
url: "/p/gxm9shf"
linkTitle: "Azure にチャットボットをデプロイする"
date: "2019-03-20"
tags: ["Azure", "Chatbot", "Bot Builder SDK"]
weight: 223
description: "ここでは、Azure 上に作成した Web App Bot リソース（ボットサービス）に、自分のボットプログラムをデプロイする方法を説明します。"
---

{{% private %}}
### Microsoft のドキュメント
- [Azure Bot Service - Deploy your bot](https://docs.microsoft.com/en-us/azure/bot-service/bot-builder-deploy-az-cli?view=azure-bot-service-4.0)
    - [（日本語版）ボットを Azure にデプロイする](https://docs.microsoft.com/ja-jp/azure/bot-service/bot-builder-deploy-az-cli?view=azure-bot-service-4.0)
- [プロジェクトの ZIP ファイルを作成する](https://docs.microsoft.com/ja-jp/azure/app-service/app-service-web-get-started-nodejs#create-a-project-zip-file)
{{% /private %}}

まず前提として、下記の手順により、Azure 上に Web App Bot リソースが作成済みであることとします。
ここに、ローカルで作成したボットをデプロイすることになります。

- [Azure でボットをホストするための Web App Bot リソースを作成する](/p/tttou4o)


Web ブラウザでデプロイする方法（KUDU の Zip Deploy UI）
----

プロジェクトのファイルを ZIP ファイルとしてアーカイブし、Zip Deploy UI という Web ページにドラッグ＆ドロップでデプロイする方法です。
この Web サイトには、下記のような URL でアクセスできます。
`<app_name>` の部分は、自分のボットアプリ名に置き換えてください。

{{< code >}}
https://<app_name>.scm.azurewebsites.net/ZipDeployUI
{{< /code >}}

{{< image w="700" border="true" src="deploy-bot-kudu.png" title="Web App Bot の Zip Deploy UI" >}}

左上に表示されるロゴからも分かるように、Azure の Web App サービスでは、[Kudu](https://github.com/projectkudu/kudu/wiki/) というデプロイエンジンが使われているようですね。

この `/wwwroot` ディレクトリの内容が表示されている画面で、エクスプローラ領域に ZIP ファイルをドラッグ＆ドロップすると、ZIP ファイル内のファイルがまとめて `/wwwroot` にアップロードされます。

{{< image w="700" border="true" src="deploy-bot-kudu2.png" title="ZIP ファイルのドラッグ＆ドロップでデプロイ" >}}

ボットプログラムのエントリポイントとなる `bot.js` ファイルを編集してから ZIP 化し、デプロイすることで、ボットの動作が変わることを確認できると思います。

### ZIP アーカイブ作成時の注意

ZIP ファイルの中身が展開された状態でデプロイされるので、ZIP の展開後に余計なディレクトリ階層ができないように ZIP アーカイブを作成してください。
具体的には、プロジェクトのディレクトリを ZIP 化するのではなく、プロジェクトのディレクトリ内のファイル群を ZIP 化します。
コマンドラインから ZIP アーカイブを作成するには、プロジェクトのルートディレクトリから下記のようにします。

{{< code title="Bash の場合" >}}
zip -r <file-name>.zip .
{{< /code >}}

{{< code title="PowerShell の場合" >}}
Compress-Archive -Path * -DestinationPath <file-name>.zip
{{< /code >}}

Windows であれば、プロジェクトのディレクトリで <kbd>Ctrl + A</kbd> でファイルを全選択し、右クリックから <samp>送る</samp> → <samp>圧縮（zip形式）フォルダー</samp> とするのが手っ取り早いかもしれません。

ちなみに、上記のサイトで、ファイル名の横にある編集アイコン（鉛筆マーク）をクリックすると、Web サイト上でソースコードを直接編集することができます。
ここでチョチョイとコードを変更すれば、直ちにボットの応答に反映されます。

