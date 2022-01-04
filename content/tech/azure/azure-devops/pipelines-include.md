---
title: "Azure Pipelinesメモ: azure-pipelines.yml から別の Yaml をインクルードする (template)"
linkTitle: "azure-pipelines.yml から別の Yaml をインクルードする (template)"
url: "/p/36h9xj9"
date: "2020-07-28"
tags: ["Azure", "Azure Pipelines"]
---

Template 機能
----

Azure Pipelines の Template 機能を使用すると、別の Yaml ファイルに記述したビルド設定をインクルードすることができます。

- [Templates - Azure Pipelines | Microsoft Docs](https://docs.microsoft.com/en-us/azure/devops/pipelines/process/templates?view=azure-devops)

変数を渡して、部分的に内容を置き換えることができるので、「インクルード」ではなく「テンプレート」と呼んでいるみたいです。

単純に共通の `steps` を読み込んで使うこともできるし、逆にテンプレートファイルに対してパラメータで `stepList` を渡すということもできます。


使用例（steps の共通化）
----

例えば、別の Yaml ファイル（テンプレート）に記述した `steps` 定義を、`azure-pipelines.yml` から読み込むとします。
テンプレートファイルには次のような感じで、ルートに `steps` 要素を記述します。

{{< code lang="yaml" title="common-steps.yml（テンプレート）" >}}
steps:
- task: NodeTool@0
  inputs:
    versionSpec: '10.x'
  displayName: 'Install Node.js'

- task: Npm@1
  inputs:
    command: 'ci'
  displayName: 'npm ci'

- task: Npm@1
  inputs:
    command: 'custom'
    customCommand: 'run lint'
  displayName: 'npm run lint'

# ...
{{< /code >}}

`azure-pipelines.yml` の `steps` 以下で、上記のテンプレートファイルを読み込むには、__`template`__ というキーワードを使用します。

{{< code lang="yaml" title="azure-pipelines.yml（抜粋）" >}}
pool:
  vmImage: 'ubuntu-latest'

steps:
- script: echo Hello
- template: common-steps.yml
- script: echo World
{{< /code >}}

`template` キーワードは任意の位置で使用できるので、上記のように前後に別の `step` を入れることが可能です。


使用例（jobs の共通化）
----

上記の例では `steps` のテンプレートを読み込む方法を示しましたが、`jobs` ごと読み込む場合も同様に記述できます。
`jobs` の下には、`steps` だけではなく、`variables` や `pool` などを記述できるため、`steps` 単位のテンプレートより使い勝手がよいかもしれません（参考: [jobs のスキーマ定義](https://docs.microsoft.com/en-us/azure/devops/pipelines/yaml-schema?view=azure-devops&tabs=schema%2Cparameter-schema#job)）。

{{< code lang="yaml" title="common-jobs.yml（テンプレート）" >}}
jobs:
- job: HugoBuild

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

{{< code lang="yaml" title="azure-pipelines.yml" >}}
jobs:
- template: azure-pipelines-common-jobs.yml
{{< /code >}}

