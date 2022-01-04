---
title: "AWS CodeBuild をコマンドライン (CLI) から操作する"
url: "/p/bwamw8i"
date: "2021-02-22"
tags: ["AWS"]
working: true
---

CodeBuild プロジェクトの一覧を取得する (codebuild list-projects)
----

{{< code >}}
$ aws codebuild list-projects
{{< /code >}}

{{< code title="例: すべてのプロジェクト名を取得" >}}
$ aws codebuild list-projects
projects:
- app1-codebuild
- app2-codebuild
- app3-codebuild
{{< /code >}}

出力結果をソートするには、次のオプションが使用できます。

- __`sort-by`__ ... `NAME` / `CREATED_TIME` / `LAST_MODIFIED_TIME` のいずれかを指定
- __`sort-order`__ .. `ASCENDING` / `DESCENDING` のいずれかを指定

{{< code title="例: 最近作られたプロジェクトを 5 件表示" >}}
$ aws codebuild list-projects --sort-by CREATED_TIME --sort-order DESCENDING --max-items 5
{{< /code >}}

{{% accordion title="実行結果" %}}
{{< code >}}
NextToken: eyJuZXh0VG9rZW4iOiBudWxsLCAiYm90b190cnVuY2F0ZV9hbW91bnQiOiA1fQ==
projects:
- myapp25-codebuild
- myapp24-codebuild
- myapp23-codebuild
- myapp22-codebuild
- myapp21-codebuild
{{< /code >}}

`NextToken` は、続きのエントリを取得したいときに、`--starting-token` オプションで指定します。
{{% /accordion %}}

{{% note title="出力形式 (YAML or JSON) を変えたいとき" %}}
AWS CLI の出力形式は、デフォルトでは `~/.aws/config` ファイルの `output` プロパティで指定されている出力形式が使用されます。
明示的に出力形式を指定したい場合は、各コマンド実行時に `--output yaml` や `--output json` のようなオプション指定を行います。

- 参考: [AWS の初期設定: AWS CLI のセットアップ](/p/j5iu7it)
{{% /note %}}

{{% note title="存在するはずのプロジェクトが見つからない" %}}
作成したはずの CodeBuild プロジェクトがリストに出てこない場合は、CodeBuild プロジェクトの __リージョン__ と、AWS CLI が対象としているリージョンが異なっている可能性があります。
__`aws configure list`__ コマンドで AWS CLI が使用しているリージョンを確認し、想定と違うようでしたら、下記のようにリージョンを明示してコマンドを実行してみてください。

{{< code >}}
$ aws codebuild list-projects --region ap-northeast-1
{{< /code >}}
{{< /note >}}


CodeBuild プロジェクトの設定を取得する (codebuild batch-get-projects) {#batch-get-projects}
----

{{% private %}}
- [ビルドプロジェクトの詳細を表示する (AWS CLI)](https://docs.aws.amazon.com/ja_jp/codebuild/latest/userguide/view-project-details.html#view-project-details-cli)
{{% /private %}}

{{< code >}}
$ aws codebuild batch-get-projects --names <プロジェクト名1> <プロジェクト名2>...
{{< /code >}}

{{< code title="実行例" >}}
aws codebuild batch-get-projects --names app1-codebuild
{{< /code >}}
{{< accordion title="実行結果" >}}
{{< code >}}
projects:
- arn: arn:aws:codebuild:ap-northeast-1:123456789012:project/app1-codebuild
  artifacts:
    encryptionDisabled: false
    location: myapp-123456789012-output-bucket
    name: app1-codebuild
    namespaceType: NONE
    overrideArtifactName: false
    packaging: NONE
    path: ''
    type: S3
  badge:
    badgeEnabled: false
  cache:
    type: NO_CACHE
  created: '2021-03-03T22:12:32.120000+09:00'
  encryptionKey: arn:aws:kms:ap-northeast-1:123456789012:alias/aws/s3
  environment:
    computeType: BUILD_GENERAL1_SMALL
    environmentVariables: []
    image: aws/codebuild/amazonlinux2-x86_64-standard:3.0
    imagePullCredentialsType: CODEBUILD
    privilegedMode: false
    type: LINUX_CONTAINER
  lastModified: '2021-03-03T22:12:32.120000+09:00'
  logsConfig:
    cloudWatchLogs:
      status: ENABLED
    s3Logs:
      encryptionDisabled: false
      status: DISABLED
  name: app1-codebuild
  queuedTimeoutInMinutes: 480
  secondaryArtifacts: []
  secondarySourceVersions: []
  secondarySources: []
  serviceRole: arn:aws:iam::123456789012:role/service-role/app1-codebuild-service-role
  source:
    insecureSsl: false
    location: myapp-123456789012-input-bucket/app.zip
    type: S3
  tags:
  - key: project
    value: TempProject
  timeoutInMinutes: 60
projectsNotFound: []
{{< /code >}}
{{< /accordion >}}


CodeBuild プロジェクトの設定を変更する (codebuild update-project)
----

{{% private %}}
- [ビルドプロジェクトの設定の変更 (AWS CLI)](https://docs.aws.amazon.com/ja_jp/codebuild/latest/userguide/change-project-cli.html)
{{% /private %}}

[aws codebuild batch-get-projects](#batch-get-projects) コマンドで取得したプロジェクトの設定情報（YAML or JSON 形式）をもとに、設定の一部を変更することができます。
まずは、元の設定情報を YAML ファイルに保存しておきます。
出力形式を `--output yaml` オプションで明示しておいた方が安全かもしれません。

{{< code >}}
$ aws codebuild batch-get-projects --names app1-codebuild > projects.yml
{{< /code >}}

出力結果の YAML ファイルは次のような感じになっています。
`codebuild batch-get-projects` コマンドは複数のプロジェクトの情報を取得するコマンドなので、ルートプロパティとして `projects` があり、そこに配列形式で各プロジェクトの情報が含まれています。

{{< code lang="yaml" title="projects.yml" >}}
projects:
- arn: arn:aws:codebuild:ap-northeast-1:123456789012:project/app1-codebuild
  artifacts:
    encryptionDisabled: false
    location: app1-123456789012-output-bucket
    name: app1-codebuild
    namespaceType: NONE
    overrideArtifactName: false
    packaging: NONE
    path: ''
    type: S3
  badge:
    badgeEnabled: false
  ...
{{< /code >}}

設定の変更には、__`aws codebuild update-project`__ コマンドを使用するのですが、このコマンドは単一プロジェクトの設定更新を目的としているので、上記の YAML ファイルの構造を少し変更して、単一プロジェクトの情報だけを示す YAML ファイルに書き換えないといけません（といっても一階層減らすだけです）。
例えば、artifacts 設定（ここではバケット名）を変えるには、次のような YAML ファイルを用意します。

{{< code lang="yaml" title="change.yml" >}}
name: app1-codebuild
artifacts:
  encryptionDisabled: false
  location: app1-123456789012-output-bucket-CHANGED
  name: app1-codebuild
  namespaceType: NONE
  overrideArtifactName: false
  packaging: NONE
  path: ''
  type: S3
{{< /code >}}

少なくとも、対象の CodeBuild プロジェクト名を示す __`name`__ プロパティと、変更対象のプロパティ（とその兄弟プロパティ）が記載されている必要があるようです。
あとは、次のように、このファイルを使って CodeBuild プロジェクトの設定を更新することができます。

{{< code title="CodeBuild プロジェクトの設定を変更する" >}}
$ aws codebuild update-project --cli-input-yaml file://change.yml
{{< /code >}}

入力ファイルを JSON 形式で作成したいときは、`--cli-input-yaml` の部分を `--cli-input-json` に変更すれば読み込めます。


CodeBuild プロジェクトを削除する
----

{{< code >}}
aws codebuild delete-project --name <プロジェクト名>
{{< /code >}}

何も削除されなかった場合もエラー表示は出ないので、実行後は `aws codebuild list-projects` で消えているか確認してください。


ビルド履歴を表示する
----

{{< code >}}
$ aws codebuild list-builds
ids:
- build-hello:c8db24a0-9dc0-4cb3-9c47-2557502f0a59
- build-hello:de7dba14-e397-4fed-9e06-f7b98d25fb46
- build-hello:bf727395-ea1e-4266-a1d7-6217b909d729
- build-hello:67b5de96-4b4e-49ab-8eaa-c0781740597b
- build-hello:40bbb789-20b3-419b-9f41-8fd42e414a02
- build-hello:3901c823-a6a3-4d16-bff2-66b1f80a25e4
- build-hello:18a32311-de0e-4d08-b7e2-ba1ce207ff27
- build-hello:0afacf09-c930-476f-a948-f62dbc33c1d4
{{< /code >}}

