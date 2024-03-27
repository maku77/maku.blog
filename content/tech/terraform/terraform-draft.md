---
title: "DRAFT: Terraform メモ"
url: "p/67udc9g/"
date: "2024-03-19"
tags: ["Terraform"]
draft: true
---

変数
----

```terraform
variable "example_instance_type" {
  default = "t3.micro"
}
```

参照するときは `var.example_instance_type` のようにします。

コマンドライン引数で変数値を上書きすることもできます。

```console
$ terraform plan -var 'example_instance_type=t3.nano'
```

環境変数を __`TF_VAR_xxx`__ という名前で定義しておくと、terraform が自動的に変数として認識してくれます。

```console
$ TF_VAR_example_instance_type=t3.nano terraform plan
```


ローカル変数 (locals)
----

`variable` の代わりに __`locals`__ を使うと、コマンド実行時に変更できない変数を定義できます。

```terraform
locals {
  example_instance_type= "t3.micro"
}
```

参照するときは、`local.example_instance_type` のように記述します。
こちらは、`local` が単数形になるので注意してください。


出力値 (output)
----

`terraform apply` 時に作成されたリソースの情報（ID など）を出力したいときは、次のように `output` 定義します。

```terraform
resource "aws_instance" "example" {
  ...
}

output "example_instance_id" {
  value = aws_instance.example.id
}
```


プロバイダー (provider)
----

プロバイダー（Azure や AWS など）として何を使用するかは、terraform に自動判定させることも可能ですが、次のように明示的に定義しておくこともできます。

```terraform
provider "aws" {
  region = "ap-northeast-1"
}
```

これは AWS の東京リージョンを使用する場合の設定です。
プロバイダー関連のバイナリは `terraform init` コマンドを実行すると自動的にダウンロードされます。


組み込み変数 (file)
----

その場にそのファイルの内容を書いたかのように振る舞います。

```terraform
user_data = file("./user_data.sh")
```


AWS ポリシードキュメントの定義 (aws_iam_policy_document)
----

```terraform
data "aws_iam_policy_document" "allow_describe_regions" {
  statement {
    effect = "Allow"
    actions = ["ec2:DescribeRegions"]  # リージョン一覧の取得
    resources = ["*"]
  }
}
```

JSON で記述するのと比べて、コメントを記述できたり、変数参照などを行えるので便利です。


IAM ポリシー（リソース） (aws_iam_policy)
----

```terraform
resource "aws_iam_policy" "example" {
  name = "example"
  policy = data.aws_iam_policy_document.allow_describe_regions.json
}
```

ポリシードキュメントを保持する具体的な AWS リソースが IAM ポリシーです。


その他
----

- GitHub Actions で Pull Request 作成時に `terraform fmt -check` をかけるようにしておくとよさげ。
- `required_version` は __`~>`__ で指定しておくと不意のメジャーバージョンアップを防ぐことができる。
- __`variables.tf`__ ファイルで共通の変数を定義しておくことができる。

