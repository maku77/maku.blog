---
title: "Azure Static Web Apps で静的ウェブサイトを作成する"
url: "/p/vgt5fqy"
date: "2020-08-15"
tags: ["Azure"]
---

Azure Static Web Apps とは
----

2020年5月に Azure Static Web Apps のプレビュー版が公開されました。
これまでは、Azure 上で静的なウェブサイトを作成する場合は、BLOB ストレージを使って HTML/JS ファイルなどをホストする方法がとられていましたが、今後は Static Web Apps のサービスが主流になりそうです。

- 参考: [Azure Storage で静的 Web サイトをホスティングする](/p/gkardu9)

Azure Static Web Apps は、サイトのビルドやデプロイに GitHub Actions を使うことを前提としているため、Web サイトのコンテンツを GitHub 上で管理することが強制されます。

GitHub Pages でも静的な Web サイトを作成できますが、Azure Static Web Apps を使うと Azure Functions などの API サービスと統合することができます。
静的 Web サイトといいつつも、より高度な Web アプリを作成することができそうです。

Azure Static Web Apps は、ベータ版のうちは無料で使えるようです。
そのうち従量課金に切り替わると思いますが、BLOB ストレージを使った場合の利用料金はめちゃ安だったので、こちらも安価な料金が設定されることを期待しています。


GitHub リポジトリの準備
----

下記のような簡単な HTML ファイルを作成して、[GitHub リポジトリを新規作成](https://github.com/new) してコミットしておきます。

{{< code lang="html" title="index.html" >}}
<!DOCTYPE html>
<meta charset="UTF-8">
<title>My website</title>
<p>Hello World</p>
{{< /code >}}


Static Web Apps リソースの作成
----

{{< image w="600" border="true" src="img-001.png" >}}

[Azure Portal のリソースの作成](https://portal.azure.com/#create/hub) のページから、__Static Web App__ を選択してリソースを新規作成します。

{{< image w="600" border="true" src="img-002.png" >}}

作成画面の「基本」タブでは、Web サイトの __`名前`__ や、使用する GitHub リポジトリなどを指定します。
ここでは、簡単に `mysite` としました。
`地域` は、住んでいる場所の近くを選んでおけば OK です。

{{< image w="600" border="true" src="img-003.png" >}}

「ビルド」タブでは、各種ディレクトリのパスなどを設定します。

- {{< label "アプリの場所" >}}: プロジェクトのルートディレクトリを指定します。
通常は `/` のままで大丈夫ですが、何らかの理由でリポジトリのルートディレクトリを使えない場合は、サブディレクトリ名を指定しておくことができます。
- {{< label "API の場所" >}}: Azure Functions の実装ファイルが格納されたディレクトリを設定します。
デフォルトでは `api` ディレクトリが指定されていますが、Azure Functions を使わないのであれば、この項目は削除しておきます。
- {{< label "アプリの成果物の場所" >}}: デプロイすべきファイル群が格納されるディレクトリを設定します。
TypeScript や webpack などで HTML/JS ファイルをビルドする場合は、`dist` や `build` といったディレクトリにビルド結果のファイル群を格納するはずなので、そのディレクトリ名を指定しておきます。
リポジトリのルートディレクトリに `index.html` ファイルなどを格納している場合は、この項目は空のままで構いません。

入力が完了したら、__`作成`__ ボタンを押して数分待てば、リソースの作成が完了します。

{{< image w="600" border="true" src="img-004.png" >}}

リソースのページにアクセスすると、Web サイトの URL を確認することができます。
URL には、自動的に `<ランダムID>.azurestaticapps.net` というアドレスが割り当てられるようです。
上記ページの __`参照`__ ボタンを押すと、Web ページを開くことができます。

{{< image w="600" border="true" src="img-005.png" >}}

このように、最初に作成した `index.html` の内容が表示されれば成功です。
GitHub アクションの設定も自動的に完了しているため、GitHub 上の HTML ファイルが更新されると、自動的に Web サイトに反映されます。

めっちゃ楽ですね！


デプロイ前にビルドが必要な場合
----

Azure Static Web Apps にデプロイするファイル群を生成するために、webpack や Parcel などで Web サイトコンテンツをビルドする必要があるかもしれません。

実は、GitHub Actions が実行されると、デフォルトで __`npm run build`__ によるビルドが実行されるようになっています。
なので、`package.json` の中で、`build` スクリプトを定義しておくだけで、Web サイトコンテンツのビルドは自動的に実行されることになります。

{{< code lang="js" title="package.json（抜粋）" >}}
"scripts": {
  "start": "parcel src/index.html --open",
  "build": "rm -r build && parcel build src/index.html -d build"
},
{{< /code >}}

`npm run build` 以外の別のビルドコマンドを実行したいときは、`.github/workflows` ディレクトリ以下に生成された Yaml ファイルの中で設定できます。

- 参考: [Azure Static Web Apps の GitHub Actions ワークフロー | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/static-web-apps/github-actions-workflow#custom-build-commands)

具体的には、`Build And Deploy` ステップの `with` セクションで、__`app_build_command`__ プロパティを追加することで設定できるようです。
とはいえ、通常は `npm run build` でビルドできるようにしておくのが分かりやすいと思います。

ビルド結果の出力ディレクトリは、同じく `with` セクションの __`app_artifact_location`__ プロパティで指定しておきます。
Azure Statie Web Apps には、ここで指定したディレクトリ内のファイルがデプロイされます。
下記の例では、`build` ディレクトリを指定しています。

{{< code lang="yaml" title=".github/workflows/xxxxxxxxxx.yml（抜粋）" >}}
# Repository/Build Configurations
# These values can be configured to match you app requirements.
# For more information regarding Static Web App workflow configurations,
# please visit: https://aka.ms/swaworkflowconfig
app_location: "/" # App source code path
api_location: "" # Api source code path - optional
app_artifact_location: "build" # Built app content directory - optional
# End of Repository/Build Configurations
{{< /code >}}

コメントなどを読めば気付くと思いますが、上記のディレクトリ設定は、Static Web Apps リソースを作成するときに指定した下記の項目に対応しています。

- {{< label "app_location" >}}: アプリの場所
- {{< label "api_location" >}}: API の場所
- {{< label "app_artifact_location" >}}: アプリの成果物の場所

一般的な JavaScript フレームワークでどのような設定をすべきかは、下記のページにまとめられています。

- 参考: [Azure Static Web Apps Preview を使用してフロントエンドフレームワークを構成する | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/static-web-apps/front-end-frameworks)

