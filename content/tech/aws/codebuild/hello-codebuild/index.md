---
title: "AWS CodeBuild で Hello World（GitHub との連携）"
url: "/p/xdnu3ah"
date: "2021-01-26"
tags: ["AWS/CodeBuild"]
---

何をやるか？
----

CodeBuild は AWS が提供するビルドサービスです。
例えば、GitHub に `buildspec.yml` というビルド方法を記述したファイルを置いておくと、GitHub へのコードプッシュ時に、その設定通りビルドを行ってくれます。
GitHub Actions や Azure Pipelines といったサービスと同様です。

ここでは、CodeBuild 導入の第一歩として、次のような構成でセットアップしてみます。

- {{< label ソースプロバイダ >}}: GitHub（の適当なテストリポジトリ）
- {{< label ビルド内容 >}}: "Hello World" と echo 表示する

つまり、テスト用の GitHub リポジトリに何らかのファイルをプッシュしたときに、CodeBuild によるビルドを開始するところまでです。

ソースプロバイダとしては、GitHub だけではなく、AWS CodeCommit、Bitbucket、GitHub Enterpise といった Git リポジトリを選択できますが、おそらく GitHub が一番よく使われているので、ここでは GitHub を使うことにします。

最初の一歩はできるだけ簡単に済ませることが肝心です！


アカウントとリポジトリを準備する
----

まず最低限の準備として、下記のアカウントは用意できているとします。

- AWS アカウント
- GitHub アカウント

次に、GitHub にテスト用の [リポジトリを作成](https://github.com/new) しておきます。
ここでは、リポジトリ名は `hello-codebuild` とでもしておきます。
`README.md` を自動生成しておくと、後のテストが楽です。


CodeBuild にビルドプロジェクトを作成する
----

次のようにして、CodeBuild に新規ビルドプロジェクトを作成します。

1. [AWS CodeBuild console](https://console.aws.amazon.com/codesuite/home) を開きます。
2. `ビルド (CodeBuild)` → `ビルドプロジェクト` → `ビルドプロジェクトを作成する` と選択します。
3. 以下のような内容を入力してビルドプロジェクトを作成します。
    - {{< label プロジェクト名 >}}: `build-hello`（名前は自由）
    - {{< label ソースプロバイダ >}}: `GitHub` → 作成しておいた `hello-codebuild` リポジトリを指定
    - {{< label ウェブフック >}}: `コードの変更がこのレポジトリにプッシュされるたびに再構築する` にチェックを入れる（これで GitHub へのプッシュ時にビルドトリガがかかります）
    - {{< label 環境イメージ >}}: `マネージド型イメージ` → `Amazon Linux 2` → ランタイムやイメージは適当に選択
    - {{< label サービスロール >}}: `新しいサービスロール` → `codebuild-build-hello-service-role`（デフォルト）
    - {{< label ビルド仕様 >}}: `buildspec ファイルを使用する`

以下のような感じでビルドプロジェクトが作成されれば成功です。

{{< image border="true" src="img-001.png" >}}


ビルド仕様ファイル (buildspec.yml) を作成する
----

ビルドプロジェクトの作成直後に、おもむろに `ビルドを開始` のボタンを押すと、ビルドは見事に失敗します。
`ビルド履歴` で対象のビルドを選択してビルドログを見ると、次のようなエラーが出力されていることが分かります。

{{< code >}}
[Container] 2021/01/26 15:32:21 Phase context status code: YAML_FILE_ERROR Message: YAML file does not exist
{{< /code >}}

エラーを見るときは、`フェーズ詳細` のタブの方がパッと見で分かりやすいかもしれません。
どのフェーズまでうまく進んだかが一目瞭然です。

{{< image border="true" src="img-002.png" >}}

このエラーは、リポジトリのルートにビルド仕様ファイル __`buildspec.yml`__ が存在しないというエラーです。
そこで、次のような内容の `buildspec.yml` ファイルを作成して GitHub へプッシュします。
あるいは、GitHub サイト上で直接ファイルを作っちゃっても OK です。

{{< code lang="yaml" title="buildspec.yml" >}}
version: 0.2

phases:
  install:
    commands:
      - echo 'Hello World (INSTALL phase)'
  pre_build:
    commands:
      - echo 'Hello World (PRE_BUILD phase)'
  build:
    commands:
      - echo 'Hello World (BUILD phase)'
  post_build:
    commands:
      - echo 'Hello World (POST_BUILD phase)'
{{< /code >}}

CodeBuild によるビルドは、`INSTALL` → `PRE_BUILD` → `BUILD` → `POST_BUILD` のようにフェーズが進んでいくのですが、ここではそれらの各フェーズで echo 出力をしています。
[buildspec の構文はこちらのドキュメント](https://docs.aws.amazon.com/ja_jp/codebuild/latest/userguide/build-spec-ref.html) で確認できます。

`buildspec.yml` ファイルを GitHub リポジトリのトップディレクトリに作成（あるいはプッシュ）すると、ウェブフックイベントにより CodeBuild のビルドが開始されます。
今度は buildspec ファイルが見つかるので、次のようにビルドが正常に実行されることが分かります（実際のログはもう少しいろいろ情報が表示されます）。

{{< code title="ビルドのログ（抜粋）" >}}
Hello World (INSTALL phase)
Hello World (PRE_BUILD phase)
Hello World (BUILD phase)
Hello World (POST_BUILD phase)
{{< /code >}}

これで AWS CodeBuild の Hello World は完了です！お疲れ様でした！

