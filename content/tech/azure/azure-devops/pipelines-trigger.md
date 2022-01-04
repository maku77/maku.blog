---
title: "Azure Pipelines の Pull Request 時の起動トリガ設定"
url: "/p/i6549xd"
date: "2020-05-13"
tags: ["Azure", "Azure Pipelines"]
description: "Azure Pipelines のビルドトリガの設定が意外と混乱するので、ユースケースごとにまとめました。特に Pull Request 時にどう扱うかなど。"
weight: 200
---

{{< private >}}
- [Triggers in Azure Pipelines - Azure Pipelines | Microsoft Docs](https://docs.microsoft.com/en-us/azure/devops/pipelines/build/triggers?view=azure-devops)
{{< /private >}}


ビルドしない
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger: none  # will disable CI builds entirely
{{< /code >}}

Azure Pipelines によるビルドを起動しないようにします。

`trigger` プロパティを省略すると、デフォルトですべてのブランチへのプッシュ時にビルドが走るので、ビルドしないようにするには、上記のように明示的に __`none`__ 指定が必要です。
この設定を行うと、`pr` プロパティの設定（後述）も無効になります（Pull Request 時もビルドは実行されない）。


master ブランチへのプッシュ or マージでビルド？
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
- master
{{< /code >}}

次のように複数のブランチをビルド対象とすることもできます。

{{< code lang="yaml" >}}
trigger:
- master
- release
{{< /code >}}

このように記述すると、指定したブランチへのプッシュ or マージ時にのみ Pipelines が起動しそうに見えますが、この指定だけだと、 __あらゆる Pull Request の作成時に Pipelines が起動します__。
`master` ブランチや `release` ブランチをターゲットとしない Pull Request でもビルドが走ります。
なぜなら、Pull Request トリガの設定がデフォルトで次のようになっているからです。

{{< code lang="yaml" >}}
pr:
  branches:
    include:
    - '*'
{{< /code >}}

このため、一連のステップにデプロイ処理まで組み込んでいると、レビュー用に Pull Request を上げただけでデプロイまで実行されてしまうという振る舞いになります。


Pull Request 時にビルドしない
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
- master

pr: none  # no PR triggers
{{< /code >}}

上記のように設定しておくと、Pull Request を作成したときには Pipelines は起動しなくなります。
つまり、`master` ブランチへのプッシュ or マージ時のみビルドが実行されます。


特定のブランチをターゲットとした Pull Request 時にビルドする
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
pr:
- master
{{< /code >}}

`master` ブランチをターゲットとする Pull Request を作成したとき（および追加プッシュ）があった場合にのみ Pipelines が起動します。
これも `trigger` の指定と同様に、複数のターゲットブランチを指定できます。

{{< code lang="yaml" >}}
pr:
- master
- releases/*
{{< /code >}}


Pull Request 時のみビルドする
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
- DUMMY_BRANCH_TO_BUILD_ONLY_ON_PR
{{< /code >}}

これ、正式なやり方が分かりませんでした。
`pr` プロパティはデフォルトで全 Pull Request に対してビルドを実行する設定になっているのですが、`master` ブランチへのプッシュ時のビルドを停止するために `trigger: none` を指定してしまうと、Pull Request 時のビルドも無効になってしまうし。。。

しょうがないので、絶対に使わないであろうブランチ名を `trigger` に指定しています。
結果として、`master` ブランチなどへのプッシュ時にはビルドは実行されず、任意の Pull Request を作成したときのみにビルドが実行されるようになります。


特定のファイルがコミットされた場合はビルドしない
----

{{< code lang="yaml" title="azure-pipelines.yml" >}}
trigger:
  branches:
    include:
    - master
  paths:
    include:
    - '*' # same as '/' for the repository root
    exclude:
    - '/azure-pipelines-for-pr.yml'
    - '/docs/*' # same as '/docs/'
{{< /code >}}

`trigger` 設定は上記のように階層化して記述することで、さらに細かい設定を行うことができます。
例えば、

- ドキュメントの更新時にはビルドは必要ない (`/docs/*`）
- 別の Pipelines 設定ファイルの更新は無視したい (`/azure-pipelines-for-pr.yml`)

といった場合は、上記のように `trigger.paths.exclude` プロパティでファイルのパスを指定することで除外できます。

