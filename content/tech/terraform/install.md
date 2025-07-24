---
title: "Terraform の導入"
url: "p/tacgbjy/"
date: "2023-08-02"
tags: ["Terraform"]
---

Terraform とは
----

Terraform は、Azure、AWS、GCP など様々なクラウドプラットフォームに対応したインフラ構築ツールです。

__HCL (HashiCorp Configuration Language)__ という言語を使って、どのようにインフラを構築するかをコードで定義することができます __(IaC: Infrastructure as Code)__。
Azure や AWS などの個々のサービスと通信する機能は、__プロバイダー__ と呼ばれる Terraform のプラグインとして提供されており、何千ものプロバイダーが [Terraform Registry](https://registry.terraform.io/) で公開されています。
適切なプロバイダーが見つからなければ、Golang（Go 言語）で自作することも可能です。


インストール
----

Terraform の CLI ツール（__`terraform`__ コマンド）は、下記公式サイトの手順に従って簡単にインストールできます。

- [Install | Terraform | HashiCorp Developer](https://developer.hashicorp.com/terraform/downloads)

各 OS のパッケージマネージャー（macOS の `brew` など）でインストールするのが手っ取り早いですが、1 バイナリーで提供されているので、バイナリ―をダウンロードして適当なディレクトリに配置する方がむしろ簡単かもしれません。
Golang 製のツールは、1 バイナリ―で提供されていているのでよいですね。

次のように `terraform` コマンドを実行できるようになれば OK です。

```console
$ terraform -version
Terraform v1.5.4
on windows_amd64
```


Terraform コード (main.tf) の作成
----

ここでは、Terraform によるリソース作成の Hello World として、AWS の S3 バケットを作成してみます。
EC2 インスタンスを作成する例などがよくありますが、消し忘れが怖いので S3 バケットにしておきます。
前提条件として、AWS CLI（`aws` コマンド）の実行が可能になっているものとします（参考: [AWS の初期設定: AWS CLI と認証情報の設定](/p/j5iu7it/)。

Terraform のエントリーポイントとなるファイルは、__`main.tf`__ という名前で作成します（__Terraform コード__ と呼びます）。
`main.tf` では、使用するプロバイダー（Azure や AWS）の設定や、作成するリソースの定義を行います。
次の例では、S3 バケットを 1 つ定義しています。

{{< code lang="terraform" title="main.tf" >}}
# AWS プロバイダーの設定
provider "aws" {
  region = "ap-northeast-1"  # 使用する AWS リージョン
}

# 作成するリソースの設定
resource "aws_s3_bucket" "example" {
  bucket = "012345012345-bucket"  # バケット名（世界中で一意になるようにする）
  tags = {
    Terraform = "true"
    Name = "my first bucket"
    Environment = "dev"
  }
}
{{< /code >}}

`resource` のパラメーターには、作成するリソースのタイプ (`aws_s3_bucket`) と名前 (`example`) を指定しています。
このタイプと名前の組み合わせ (`aws_s3_bucket.example`) は、Terraform がリソースの更新や削除を行うときに、操作対象のリソースを特定するために使用します。
AWS 上に作成する S3 バケットの名前ではないことに注意してください。


terraform コマンドの実行
----

### terraform init

`main.tf` を作成したら、最初に __`terraform init`__ コマンドを実行して、必要なプラグイン（ここでは AWS）などをカレントディレクトリにダウンロードします。

{{< code lang="console" title="必要なプラグインをダウンロード" >}}
$ terraform init
{{< /code >}}

このとき、__`.terraform.lock.hcl`__ というロックファイルが生成されます。
ここには、ダウンロードしたプラグインの具体的なバージョン情報などが記載されているので、Git などのバージョン管理システムにコミットするようにします。
他のチームメンバーが `terraform init` コマンドを実行したときは、このロックファイルに従って各種プラグインがインストールされます。
ロックファイルの内容を更新したい場合（使用するプラグインを更新したい場合）は、__`-upgrade`__ オプションを付けて実行します。

{{< code lang="console" title="ロックファイル (.terraform.lock.hcl) の更新" >}}
$ terraform init -upgrade
{{< /code >}}

### terraform plan

次に、__`terraform plan`__ コマンドを実行して、どのようなリソースが生成されるのかを事前確認しておきます。

{{< code lang="console" title="作成されるリソースを確認" >}}
$ terraform plan
{{< /code >}}

### terraform apply

問題なさそうであれば、__`terraform apply`__ コマンドを実行して、実際にクラウド上にリソースを生成します。

{{< code lang="console" title="実際にリソースを生成" >}}
$ terraform apply
...

  Enter a value: yes （← 最終確認して yes と入力する）

aws_s3_bucket.example: Creating...
aws_s3_bucket.example: Creation complete after 1s [id=012345012345-bucket]

Apply complete! Resources: 1 added, 0 changed, 0 destroyed.
{{< /code >}}

[AWS コンソール](https://console.aws.amazon.com/) 上で S3 バケット (`012345012345-bucket`) が作成されているのを確認できれば成功です。

{{% note title="terraform.tfstate ファイル" %}}
`terraform apply` コマンドを実行すると、カレントディレクトリに __`terraform.tfstate`__ というステートファイルが生成されます。
このファイルは、Terraform が現在の実際のインフラ構成を追跡するために使用します。
デフォルトではローカルディレクトリに生成されますが、チーム内の複数メンバーが `terraform` コマンドを実行する場合は、このステートファイルを共有する必要があります。
ステートファイルを共有する方法としては、[S3 バケットを使う方法](https://developer.hashicorp.com/terraform/language/settings/backends/s3#data-source-configuration) や、[Terraform Cloud サービスを使う方法](https://cloud.hashicorp.com/products/terraform) があります。
{{% /note %}}

### terraform destroy

最後に、今回作成したリソースをまとめて削除するには __`terraform destory`__ コマンドを実行します。

{{< code lang="console" title="お片付け" >}}
$ terraform destroy
{{< /code >}}

これで、Terraform による Hello World は完了です。

٩(๑❛ᴗ❛๑)۶ わーぃ

