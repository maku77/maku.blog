---
title: "Azure Pipelines トラブル: npm タスクを実行できないとき"
url: "/p/7c2kdub"
date: "2020-07-22"
tags: ["Azure", "Azure Pipelines"]
---

Azure Pipelines の設定で、次のような感じで Npm タスクを追加したとき、

{{< code lang="yml" title="azure-pipelines.yml" >}}
steps:
- task: Npm@1
  inputs:
    command: 'ci'
  displayName: 'npm ci'

- task: Npm@1
  inputs:
    command: 'custom'
    customCommand: 'run lint'
  displayName: 'npm run lint'
{{< /code >}}

Node.js 系のコマンドが認識されていないと、エラーが出て Npm タスクを実行できません。

{{< code >}}
No agent found in pool Default which satisfies the specified demands: npm, Agent.Version -gtVersion 2.163.1
{{< /code >}}

そのような場合は、`steps` の先頭で、次のように __NodeTool__ (Node.js Tool Installer) タスクを実行しておくと、うまく動作するようになります 。

{{< code lang="yml" title="azure-pipelines.yml" >}}
steps:
# これで npm コマンドを認識するようになる
- task: NodeTool@0
  inputs:
    versionSpec: '10.x'
  displayName: 'Install Node.js'
{{< /code >}}

{{< reference >}}
- [Node.js Tool Installer task - Azure Pipelines | Microsoft Docs](https://docs.microsoft.com/ja-jp/azure/devops/pipelines/tasks/tool/node-js?view=azure-devops)
{{< /reference >}}

NodeTool タスクは実行のたびに Node.js をダウンロード＆インストールするわけではなく、VM 上に存在するキャッシュなどを使って Node.js の環境をセットアップするため、通常は数秒で実行が完了します。

`npm` コマンドにパスが通るので、次のようにスクリプトで直接 `npm` コマンドを実行することも可能です。

{{< code lang="yml" >}}
- script: |
    npm install
    npm run build
  displayName: 'npm install and build'
{{< /code >}}

