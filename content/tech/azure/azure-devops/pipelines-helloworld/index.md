---
title: "Azure Pipelines の使い方 (Hello World)"
url: "p/vxoctbs/"
date: "2020-03-25"
tags: ["Azure", "Azure Pipelines"]
weight: 101
description: "Azure Pipelines を使用すると、GitHub などのリポジトリへのコミットに連動させてビルド処理、デプロイ処理などを自動実行することができます。"
---

ここでは、Azure Pipelines の Hello World として、任意の GitHub リポジトリへ git push したときのビルド処理を自動化してみます。
ビルド処理は、Hello World らしく、`echo Hello, world!` を実行するだけにしておきます。


DevOps organization を作成する
----

Azure Pipelines は、Azure DevOps の中の 1 サービスであり、まず最初に Azure DevOps の organization（組織）を作成する必要があります。
Azure アカウント上でまだ DevOps organization を作成していない場合は、下記 URL にアクセスすることで作成することができます。

* [https://dev.azure.com/](https://dev.azure.com/)

DevOps organization の構成は次のようになっており、organization 以下に複数のプロジェクトを作成することができます。
各プロジェクトには、複数の Repos（Gitリポジトリ）や Pipelines (CI/CDの仕組み) を設定することができます。

```
+ Azure DevOps organization
    + Project
        - Repos (Gitリポジトリ）
        - Pipelines (CI/CDの仕組み)
        - Boards（かんばん、バックログ管理など）
        - ...
    + Project
        - Repos
        - Pipelines
        - Boards
        - ...
```

DevOps organization の作成が完了すると、その organization のポータルサイトには、次のような URL でアクセスできるようになります。

{{< code >}}
https://dev.azure.com/<organization名>/
{{< /code >}}


DevOps organization 内にプロジェクトを作成する
----

最初は DevOps organization 内に 1 つもプロジェクトが存在しないので、まずは [DevOps のサイト](https://devops.azure.com/) からプロジェクトを作成してください。
サイトにアクセスすると、自動的にプロジェクトの生成を促されるはずです。

{{< image w="700" border="true" src="img-001.png" >}}

プロジェクト名を入力して、`Create project` ボタンを押せばすぐに作成は完了します。
ここでは、`helloworld` というプロジェクト名を付けてみました。

プロジェクトを作成すると、そのプロジェクトのページには、次のような URL でアクセスできるようになります。

{{< code >}}
https://dev.azure.com/<organization名>/<project名>/
{{< /code >}}


プロジェクト内に Pipelines を作成する
----

### Git リポジトリの準備

Pipelines を設定するには、何らかの Git リポジトリが必要になるので、先に [GitHub](https://github.com/) などに適当なリポジトリを用意してください。
既存のリポジトリでも構いませんが、Azure Pipelines と連携させるため、自分が設定権限を持っているリポジトリでなければいけません。

あるいは、DevOps プロジェクト内にある Azure Repos に新しく Git リポジトリを作成することもできます。

{{% maku-common/reference %}}
- [Azure DevOps で無料のプライベート Git リポジトリ (Repos) を使用する](/p/qt5qyzu)
{{% /maku-common/reference %}}

### Pipelines の作成

DevOps プロジェクトのサイドバーには、DevOps で使用できるサービスのアイコンが並んでいます。
Azure Pipelines を示す **ロケット型のアイコン** を選択すると、新しく Pipelines を作成することができます。

{{< image border="true" src="img-002.png" >}}

`Create Pipelines` ボタンを押したら、ダイアログに従って、次のような項目を選択していけば Pipelines の作成が完了します。

1. 連携する Git リポジトリ（GitHub や Azure Repos 上の Git リポジトリを選ぶ）
2. 作成する Pipelines 設定ファイルのテンプレート (`azure-pipelines.yml`)

{{< note >}}
作成する Pipelines 設定ファイル (`azure-pipelines.yml`) は、複数のテンプレートの中から選択することができるのですが、Git リポジトリ内に既存プロジェクトのコードが存在する場合は、その内容に基づいてテンプレートの候補を提示してくれます。
{{< /note >}}

ここでは、ほぼ空っぽのテンプレートである `Starter pipeline` を選択しておきます。
最後に `Save and run` ボタンを押すと、次のような設定ファイルが Git リポジトリにコミットされ、最初のビルドが実行されます。

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
- master

pool:
  vmImage: 'ubuntu-latest'

steps:
- script: echo Hello, world!
  displayName: 'Run a one-line script'

- script: |
    echo Add other tasks to build, test, and deploy your project.
    echo See https://aka.ms/yaml
  displayName: 'Run a multi-line script'
{{< /code >}}

上記の記述から大体想像がつくと思いますが、次のような内容の Pipelines 設定となっています。

- 起動トリガは `master` ブランチの変更とする
- 実行環境として Ubuntu Linux を使用する
- ビルド処理としてシェルスクリプトで `echo` コマンドを実行する


Pipelines の実行結果を確認する
----

Azure Pipelines のページを開くと、実行された Pipelines の処理結果を確認することができます。

{{< image border="true" src="img-003.png" >}}

上の図では、`azure-pipelines.yml` の中で次のように設定されたスクリプトが実行されていることを確認できます。

{{< code lang="yaml" >}}
- script: echo Hello, world!
  displayName: 'Run a one-line script'
{{< /code >}}

これで、Azure Pipelines の Hello World は完成です。
あとは、`azure-pipelines.yml` の記述を更新することで、ビルド処理をカスタマイズしていきます。

