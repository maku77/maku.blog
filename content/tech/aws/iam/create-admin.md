---
title: "AWS の初期設定: 管理者用の IAM ユーザーを作成する"
url: "/p/xbipv39"
date: "2021-02-07"
tags: ["AWS"]
weight: 101
---

{{< private >}}
- [最初の IAM 管理者のユーザーおよびグループの作成](https://docs.aws.amazon.com/ja_jp/IAM/latest/UserGuide/getting-started_create-admin-group.html)
{{< /private >}}

何をするか？
----

AWS のベストプラクティスでは、管理者であっても普段の作業では AWS アカウントのルートユーザーを直接使うべきではないとされています。

- 参考: [AWS アカウント、IAM ユーザー、グループ、ポリシーの違い](/p/4c7sdwg)

そこで、まずは管理者の普段の作業用に IAM ユーザーを作成します。
この作成作業は、AWS アカウントのルートユーザーで行う必要があります（初期状態ではユーザー作成の権限がルートユーザーにしかないため）。

ここでは、下記のような名前の IAM ユーザーとグループを作成することにします。

- IAM ユーザー: `admin`
- IAM グループ: `Admins` ... （`AdministratorAccess` ポリシーを割り当てます）


管理者用 IAM ユーザーの作成
----

[AWS IAM コンソール](https://console.aws.amazon.com/iam/) にサインインし、次のような感じで IAM ユーザーを作成します。

1. `アクセス管理` → `ユーザー` → __`ユーザーを追加`__ を選択し、次のように入力
    - ユーザー名: __`admin`__
    - アクセスの種類: CLI などで操作するなら __`プログラムによるアクセス`__ にチェック
2. `ユーザーをグループに追加` → __`グループの作成`__ を選択し、次のように入力
    - グループ名: __`Admins`__
    - ポリシー: __`AdministratorAccess`__ を選択

アクセスキーや、シークレットアクセスキーを作成したのであれば、このタイミングで表示されるので、なくさないよう厳重に保存します（後から確認することはできません）。

これで、管理者用の普段の作業も、ルートユーザーではなく、`admin` という IAM ユーザーで行えるようになります。


（応用）開発者用の IAM ユーザーの作成
----

上記では管理者用の IAM ユーザーを作成しましたが、それ以外の IAM ユーザー（開発者用など）も同様に作成することができます。
AWS は開発者用の IAM ポリシーをいくつか定義しています。
`AdministratorAccess` ポリシーとの大きな違いは、IAM 関連の操作（つまりユーザー管理）をするための権限を持っていないことです。

- [職務機能の AWS 管理ポリシー](https://docs.aws.amazon.com/ja_jp/IAM/latest/UserGuide/access_policies_job-functions.html)
    - {{< label データベース管理者 >}}: [DatabaseAdministrator](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/DatabaseAdministrator)
    - {{< label データサイエンティスト >}}: [DataScientist](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/DataScientist)
    - {{< label 開発者パワーユーザー >}}: [PowerUserAccess](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/PowerUserAccess)
    - {{< label ネットワーク管理者 >}}: [NetworkAdministrator](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/NetworkAdministrator)
    - {{< label セキュリティ監査人 >}}: [SecurityAudit](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/SecurityAudit)
    - {{< label サポートユーザー >}}: [SupportUser](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/SupportUser)
    - {{< label システム管理者 >}}: [SystemAdministrator](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/SystemAdministrator)
    - {{< label 閲覧専用ユーザー >}}: [ViewOnlyAccess](https://console.aws.amazon.com/iam/home#policies/arn:aws:iam::aws:policy/job-function/ViewOnlyAccess)

ここでは、AWS CLI を使って、コマンドラインからユーザーの作成と、`PowerUserAccess` ポリシーの割り当てをしてみます。
事前準備として、[AWS CLI の初期設定](/p/j5iu7it) は完了しており、`AdministratorAccess` ポリシーの付いた IAM ユーザーのアクセスキーでコマンド実行できるようになっているものとします。

{{< code title="IAM ユーザー (maku) の作成" >}}
$ aws iam create-user --user-name maku

User:
  Arn: arn:aws:iam::098342099475:user/maku
  CreateDate: '2021-02-07T12:20:38+00:00'
  Path: /
  UserId: ABS2Q6S3VUAIDAQXINMCQ
  UserName: maku
{{< /code >}}

{{< code title="IAM ユーザーのアクセスキーを発行" >}}
$ aws iam create-access-key --user-name=maku

AccessKey:
  AccessKeyId: AOXINMCQO7AASOQAKIAQ
  CreateDate: '2021-02-07T12:38:11+00:00'
  SecretAccessKey: MbX17I/DSInIN7dD1YOh5cE19EqG4gmblXSotFTj
  Status: Active
  UserName: maku
{{< /code >}}

ここで、忘れずにアクセスキー (`AccessKeyId`) とシークレットアクセスキー (`SecretAccessKey`) を保存しておきます。

{{< code title="IAM ユーザーに PowerUserAccess ポリシーを割り当てる" >}}
$ aws iam attach-user-policy --user-name=maku --policy-arn=arn:aws:iam::aws:policy/PowerUserAccess
{{< /code >}}

これで、開発用に使用する IAM ユーザーの作成は完了です。

