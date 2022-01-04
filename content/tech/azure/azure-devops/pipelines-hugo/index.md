---
title: "Azure Pipelines で Hugo サイトのビルド＆デプロイ"
url: "/p/kevcr7m"
date: "2020-03-30"
tags: ["Azure", "Azure Pipelines"]
weight: 102
description: "Git リポジトリ内の Hugo コンテンツが更新されたときに、自動的に hugo コマンドで Web サイトをビルドし、hugo deploy コマンドでデプロイするように設定してみます。"
image: "img-003.png"
---

前提知識
----

Azure Pipelines の基本的な使い方は下記ページを参考にしてください。

- [Azure Pipelines の使い方 (Hello World)](/p/vxoctbs)

hugo deploy コマンドによる Hugo サイトのデプロイについては下記ページを参考にしてください。

- [hugo deploy コマンドで Azure などのクラウドサービス上にデプロイする | まくまくHugo/Goノート](https://maku77.github.io/hugo/advanced/deploy.html)


事前準備
----

ここでは、Azure Pipelines の設定の説明をしますので、下記の作成・準備は終わっているものとします。

- Hugo コンテンツ用の Git リポジトリ
    - Azure Repos や GitHub の Git リポジトリに、Hugo サイトのコンテンツをコミットしてください。
- hugo deploy コマンドのための設定
    - Hugo の設定ファイル `config.toml` に、`deployment.targets` などの設定をしてください。
- Azure Pipelines の作成
    - Azure DevOps のプロジェクト内に、上記の Git リポジトリと連携する Pipelines を作成してください。


Hugo 自動ビルド＆デプロイのための Azure Pipelines 設定
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
- master

variables:
  hugo_version: '0.68.3'

pool:
  vmImage: 'ubuntu-latest'

steps:
- script: wget -O hugo.deb https://github.com/gohugoio/hugo/releases/download/v$(hugo_version)/hugo_extended_$(hugo_version)_Linux-64bit.deb
  displayName: 'Download Hugo $(hugo_version)'

- script: sudo dpkg -i hugo.deb
  displayName: 'Install Hugo $(hugo_version)'

- script: hugo
  displayName: 'Build Hugo site'

- script: hugo deploy
  displayName: 'Deploy Hugo site'
  env:
    AZURE_STORAGE_ACCOUNT: $(AZURE_STORAGE_ACCOUNT)
    AZURE_STORAGE_KEY: $(AZURE_STORAGE_KEY)
{{< /code >}}

この Pipelines 設定では、下記のことを順番にシェルスクリプトで実行しています。

1. Hugo パッケージのダウンロード（ここでは Linux の deb パッケージ）
2. Hugo パッケージのインストール
3. Web サイトのビルド (`hugo`)
4. Web サイトのデプロイ (`hugo deploy`)

順番に内容を見ていきます。

### Hugo パッケージのダウンロード

{{< code lang="yaml" >}}
- script: wget -O hugo.deb https://github.com/gohugoio/hugo/releases/download/v$(hugo_version)/hugo_extended_$(hugo_version)_Linux-64bit.deb
  displayName: 'Download Hugo $(hugo_version)'
{{< /code >}}

`wget` コマンドを使って、Hugo 本家のサイトから deb パッケージをダウンロードします。
ダウンロードする Hugo のバージョンは、YAML ファイルの先頭部分の変数で指定できるようにしています。

{{< code lang="yaml" >}}
variables:
  hugo_version: 0.68.3
{{< /code >}}

### Hugo パッケージのインストール

{{< code lang="yaml" >}}
- script: sudo dpkg -i hugo.deb
  displayName: 'Install Hugo $(hugo_version)'
{{< /code >}}

`dpkg` コマンドを使って、1 つ前のステップでダウンロードした deb パッケージをインストールします。

### Web サイトのビルド

{{< code lang="yaml" >}}
- script: hugo
  displayName: 'Build Hugo site'
{{< /code >}}

`hugo` コマンドを使って、Web サイトコンテンツをビルドします。
Pipelines 上のワーキングディレクトリに `public` ディレクトリが生成されます。

### Web サイトのデプロイ

{{< code lang="yaml" >}}
- script: hugo deploy
  displayName: 'Deploy Hugo site'
  env:
    AZURE_STORAGE_ACCOUNT: $(AZURE_STORAGE_ACCOUNT)
    AZURE_STORAGE_KEY: $(AZURE_STORAGE_KEY)
{{< /code >}}

`hugo deploy` コマンドを使って、`public` ディレクトリの内容を Azure などのクラウドストレージにデプロイします。
例えば、Hugo の設定ファイルで次のように設定されている場合は、Azure の BLOB ストレージへのデプロイが実行されます。

{{< code lang="toml" title="config.toml" >}}
[[deployment.targets]]
URL = "azblob://$web"
{{< /code >}}

このとき、Azure ストレージのアカウント名とキーを環境変数 (`AZURE_STORAGE_ACCOUNT` / `AZURE_STORAGE_KEY`) に設定しておく必要があるのですが、これらの秘密情報は、 **Azure Pipelines の変数として設定** して、その値を環境変数に伝搬させるようにしています。

Pipelines 上の変数は、Pipelines のエディット画面の右上の **`Variables`** ボタンから設定することができます。

{{< image w="700" src="img-001.png" >}}

変数名と値のペアを入力するダイアログが表示されるので、ここで必要な変数を設定します。
このとき、`Keep this value secret` にチェックを入れておくと、値が `***` のようにマスクされるようになります。

{{< image w="700" src="img-002.png" >}}

すべての変数の設定が終わったら、最後に **`Save`** ボタンを押すのを忘れないでください（個々の変数の `OK` ボタンだけでは保存されません）。

これで、Azure Pipelines による Hugo サイトのビルド＆デプロイが自動化されます。
GitHub などの Markdown コンテンツが更新されると、次のような Job が走り、自動的に Web サイトが更新されます。

{{< image w="700" src="img-003.png" >}}


（おまけ）コンテンツを格納するディレクトリを指定する
----

プロジェクトの構成によっては、Web サイトのコンテンツファイルが、Git リポジトリのルートディレクトリにないことがあると思います。
例えば、Hugo 関連のファイル群が、次のように `website` ディレクトリ以下にあるケースなどです。

```
YourRepository
    +-- azure-pipelines.yml
    +-- website/
        +-- architypes/
        +-- assets/
        +-- content/
        +-- layouts/
        +-- resources/
        +-- static/
        +-- config.toml
```

次の Pipelines 設定では、`root_dir` 変数を定義して、そこを作業ディレクトリとして `hugo` コマンドを実行するようにしています。

{{< code lang="yaml" title="azure-pipelines.yml（抜粋）" >}}
variables:
  hugo_version: '0.68.3'
  root_dir: 'website'

steps:

//...省略...

- script: hugo
  displayName: 'Build Hugo site'
  workingDirectory: $(root_dir)

- script: hugo deploy
  displayName: 'Deploy Hugo site'
  workingDirectory: $(root_dir)
  env:
    AZURE_STORAGE_ACCOUNT: $(AZURE_STORAGE_ACCOUNT)
    AZURE_STORAGE_KEY: $(AZURE_STORAGE_KEY)
{{< /code >}}

上記のように、Azure Pipelines 組み込みの [Command Line タスク](https://docs.microsoft.com/en-us/azure/devops/pipelines/tasks/utility/command-line?view=azure-devops&tabs=yaml) には、**`workingDirectory`** オプションを指定できるようになっています。
ここでは `root_dir` 変数で指定したディレクトリを作業ディレクトリとして使用するようにしています。

{{< note title="cd コマンドを使えばいいのでは？" >}}
スクリプトで `cd $(root_dir) && hugo` としてもほとんど同じですが、できるだけ OS 非依存の記述になるように、`workindDirectory` オプションを使った方がよいでしょう。
{{< /note >}}
