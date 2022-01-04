---
title: "AWS CloudFormation 入門 (Hello World)"
url: "/p/h7arpdj"
date: "2021-04-01"
tags: ["AWS", "AWS/CloudFormation"]
weight: 1
---

CloudFormation とは
----

AWS CloudFormation を使用すると、テンプレートファイル (YAML or JSON) で定義した AWS リソース群をまとめて生成（更新）することができます。
いわゆる IaC (Infrastructure as Code) 環境を提供するものであり、Ansible や Chef を使ったことがあれば、それの AWS リソース構築用だと考えると分かりやすいです。

テンプレートから生成されるインフラ（AWS リソース群）は __スタック__ と呼ばれ、テンプレートがあればスタックは何度でも生成することができます。
これはオブジェクト指向プログラミングにおける、クラスとインスタンスの関係に似ています。
必要のなくなったリソース群は、スタック単位でまとめて削除することができます。

CloudFormation の主な特徴は次の通りです。

- 何度でも同じ構成でリソースをセットアップできる（他のリージョンに複製したり、一時的に使うインフラを自動生成できる）
- テンプレートはテキストファイル (YAML/JSON) なので、GitHub などでバージョン管理することができ、PullRequest ベースのコードレビューを行える
- 冪等性が考慮されており、テンプレートには最終的な結果だけを定義しておけばよい（構成のアップデート時に差分を意識する必要はない）

例えば、次のようなテンプレートを使用すると、S3 サービスのバケットリソースを自動で作成することができます。

{{< code lang="yaml" title="template.yml" >}}
AWSTemplateFormatVersion: "2010-09-09"
Resources:
  HelloBucket:
    Type: AWS::S3::Bucket
{{< /code >}}

必須のルートプロパティは __`Resources`__ だけですが、テンプレートフォーマットのバージョンを示す __`AWSTemplateFormatVersion`__ は最低限指定しておくのがよいでしょう（2021年時点で、`2010-09-09` が最新バージョンです）。


CloudFormation コンソールからスタックを生成する
----

[CloudFormation マネージメントコンソール](https://console.aws.amazon.com/cloudformation/) を使うと、Web サイト上でスタックを生成することができます。

{{< image border="true" src="img-001.png" >}}

CloudFormation マネージメントコンソールを使ったスタックの生成手順は以下の通りです。

1. サイドバーから __`Stacks`__（スタック）を選択する
    - CloudFormation で管理されているスタックの一覧が表示されます。
1. （スタックの作成）ボタンを押す
2. テンプレートを指定する
    - S3 上に置いたファイルを指定することもできますが、ここではローカルに作成した YAML ファイルをアップロードします。そのためには、__`Template is ready`__（テンプレートの準備完了） → __`Upload a template file`__（テンプレートファイルのアップロード）を選択し、YAML ファイルを選択します。
3. 任意のスタック名（`mystack` など）を入力します
    - テンプレートファイル内にパラメータ (Parameters) を定義している場合は、ここで具体的な値を入力できます。
4. あとはそのまま進めていって、__`Create stack`__（スタックの作成）を押せば、スタックの生成処理が始まります。

スタックの一覧画面に戻ると、作成中のスタックのステータスが `CREATE_IN_PROGRESS` となって表示されます。
しばらくしてスタックの生成処理が完了すると、ステータスが `CREATE_COMPLETE` に変わります。
これで、テンプレートで定義した AWS リソース群が使用できる状態になります。

ちなみに、スタック内に作成される S3 バケットには、一意な物理 ID (Physical ID) が自動的に割り当てられるため、テンプレート内での論理 ID (Logical ID) はシンプルに保つことができます（この例では `HelloBucket`）。

- Logical ID: `HelloBucket`（テンプレートファイル内で付けた名前）
- Physical ID: `mystack-hellobucket-npd68k1m8ut8a`（CloudFormation が自動生成した ID）


AWS CLI を使ってスタックを生成する
----

[AWS CLI のセットアップ](/p/j5iu7it) が完了していれば、Web ブラウザを使わず、コマンドラインから CloudFormation のスタックを生成することができます。
例えば、カレントディレクトリにある `template.yml` を使ってスタック `mystack` を生成するには次のようにします。

{{< code >}}
$ aws cloudformation create-stack \
  --stack-name mystack --template-body file://template.yml
{{< /code >}}

作成されたスタックの情報は次のように確認できます。

{{< code >}}
$ aws cloudformation describe-stacks --stack-name mystack
{{< /code >}}

必要のないスタックは次のように削除します。

{{< code >}}
$ aws cloudformation delete-stack --stack-name mystack
{{< /code >}}

{{< reference >}}
- [AWS CloudFormation をコマンドライン (CLI) で操作する](/p/zkxamw9)
{{< /reference >}}

