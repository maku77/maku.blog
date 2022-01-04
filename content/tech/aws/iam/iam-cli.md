---
title: "AWS IAM の設定をコマンドライン (CLI) で行う"
url: "/p/aov4bho"
date: "2021-02-07"
lastmod: "2021-10-01"
tags: ["AWS", "AWS/IAM"]
---

AWS CLI（`aws` コマンド）を使うと、様々な IAM 系の操作をコマンドラインから実行できるようになります。
ただし、IAM の制御を行えるのは、`AdministratorAccess` ポリシーなどが割り当てられ、IAM 操作の権限が付いているユーザーに限られます。

- [iam -- AWS CLI Command Reference](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/iam/index.html)


ユーザー関連 {#user}
----

### IAM ユーザーを作成する (iam create-user)

{{< code >}}
aws iam create-user --user-name <ユーザー名>
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
$ aws iam create-user --user-name user-1 --output yaml
User:
  Arn: arn:aws:iam::049957049344:user/user-1
  CreateDate: '2021-11-01T08:41:29+00:00'
  Path: /
  UserId: AIDAQXINMCQAPJRMFE22H
  UserName: user-1
{{< /code >}}
{{< /accordion >}}

[iam create-user](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/iam/create-user.html) コマンドを使うと、指定した名前の IAM ユーザーを作成できます。
作成したユーザーの情報は、[iam get-user](#get-user) や [iam list-users](#list-users) コマンドで取得できます。
作成したユーザー用の API アクセスキーを生成するには、ひきつづき [iam create-access-key](#create-access-key) コマンドを使用します。

### IAM ユーザーを削除する (iam delete-user)

{{< code >}}
aws iam delete-user --user-name <ユーザー名>
{{< /code >}}

指定したユーザーに何らかの設定情報（アクセスキーや SSH キー、MFA など）が設定されている場合は、先にそれらの情報を削除しておく必要があります。
例えば、アクセスキーが設定されている場合は、[delete-access-key](#delete-access-key) コマンドで削除してからユーザー自体を削除します。

### 指定したユーザーの情報を取得する (iam get-user) {#get-user}

{{< code >}}
aws iam get-user --user-name <ユーザー名>
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
$ aws iam get-user --user-name user-1 --output yaml
User:
  Arn: arn:aws:iam::049957049344:user/user-1
  CreateDate: '2021-11-01T08:33:41+00:00'
  Path: /
  UserId: AIDAQXINMCQANAORXL7JV
  UserName: user-1
{{< /code >}}
{{< /accordion >}}

### ユーザーの一覧を取得する (iam list-users) {#list-users}

{{< code title="ユーザーの一覧を取得" >}}
$ aws iam list-users
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
$ aws iam list-users --output yaml
Users:
- Arn: arn:aws:iam::123456789012:user/Admin
  CreateDate: '2014-10-16T16:03:09+00:00'
  PasswordLastUsed: '2016-06-03T18:37:29+00:00'
  Path: /
  UserId: AIDA1111111111EXAMPLE
  UserName: Admin
- Arn: arn:aws:iam::123456789012:user/backup/backup-user
  CreateDate: '2019-09-17T19:30:40+00:00'
  Path: /backup/
  UserId: AIDA2222222222EXAMPLE
  UserName: arq-45EFD6D1-CE56-459B-B39F-F9C1F78FBE19
- Arn: arn:aws:iam::123456789012:user/cli-user
  CreateDate: '2019-09-17T19:30:40+00:00'
  Path: /
  UserId: AIDA3333333333EXAMPLE
  UserName: cli-user
{{< /code >}}
{{< /accordion >}}

`--query` オプションで、取得結果をフィルタリングすることができます。

{{< code title="例: ユーザー名だけの一覧" >}}
$ aws iam list-users --output yaml \
    --query="Users[].UserName"
- UserName1
- UserName2
- UserName3
- ...
{{< /code >}}

{{< code title="例: ユーザー名と作成日、ログイン日の一覧" >}}
$ aws iam list-users --output text \
    --query "Users[].[UserName,CreateDate,PasswordLastUsed]"
UserName1  2018-08-07T00:22:35+00:00       2021-01-21T04:08:20+00:00
UserName2  2017-08-28T15:51:00+00:00       2020-05-14T02:25:36+00:00
{{< /code >}}


アクセスキー関連 {#access-key}
----

### アクセスキーの一覧を表示する (iam list-access-keys)

{{< code >}}
# CLI 使用中のユーザーのアクセスキーを表示
$ aws iam list-access-keys

# 指定したユーザーのアクセスキーを表示
$ aws iam list-access-keys --user-name=ユーザー名
{{< /code >}}

### アクセスキーを作成する (iam create-access-key) {#create-access-key}

{{< code >}}
# CLI 使用中のユーザーのアクセスキーを作成
aws iam create-access-key

# 指定したユーザーのアクセスキーを作成
aws iam create-access-key --user-name <ユーザー名>
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
$ aws iam create-access-key --user-name user1 --output yaml
AccessKey:
  AccessKeyId: AKIAQXINMCQALYKGR5JJ
  CreateDate: '2021-11-01T08:56:09+00:00'
  SecretAccessKey: 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
  Status: Active
  UserName: user1
{{< /code >}}
{{< /accordion >}}

アクセスキーは、1 つの IAM ユーザーに対して 2 つまで生成できるようになっています。
ただし、これはキーのローテーションを行うための措置であって、普段使いのためのものではないので注意しましょう。

### アクセスキーを削除する (iam delete-access-key) {#delete-access-key}

{{< code >}}
$ aws iam delete-access-key --access-key-id <アクセスキーID> \
    --user-name <ユーザー名>
{{< /code >}}

カレントユーザーのアクセスキーを削除する場合は、`--user-name` オプションは省略できます。

### アクセスキーが最後に使われた日を調べる (iam get-access-key-last-used)

{{< code >}}
$ aws iam get-access-key-last-used --access-key-id=アクセスキーID
{{< /code >}}


グループ関連 {#group}
----

### グループの一覧を表示する (list-groups)

{{< code >}}
# すべての情報
$ aws iam list-groups

# グループ名だけを一覧表示
$ aws iam list-groups --query='Groups[].GroupName'
{{< /code >}}

### 指定したグループの情報を取得する (get-group)

{{< code >}}
$ aws iam get-group --group-name <グループ名>
{{< /code >}}

指定したグループの情報と、所属するユーザーの一覧が表示されます。

### ユーザーをグループに登録する (add-user-to-group)

{{< code >}}
$ aws iam add-user-to-group --user-name <ユーザー名> --group-name <グループ名>
{{< /code >}}

### ユーザーが所属するグループを表示する (list-groups-for-user)

{{< code >}}
# すべての情報
$ aws iam list-groups-for-user --user-name <ユーザー名>

# ARN のみを一覧表示
$ aws iam list-groups-for-user --user-name <ユーザー名> --query 'Groups[].Arn'
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code >}}
$ aws iam list-groups-for-user --user-name susan --output text \
    --query "Groups[].GroupName"
HRDepartment    Developers      SpreadsheetUsers  LocalAdmins
{{< /code >}}

{{< code >}}
$ aws iam list-groups-for-user --user-name susan --output text \
    --query "Groups[].[GroupName]"
HRDepartment
Developers
SpreadsheetUsers
LocalAdmins
{{< /code >}}
{{< /accordion >}}


ポリシー関連 {#policy}
----

### マネージドポリシーの一覧を表示する (iam list-policies)

{{< code >}}
# すべてのマネージドポリシーを表示（大量）
$ aws iam list-policies

# ARN のみを一覧表示
$ aws iam list-policies --query='Policies[].Arn'

# 現在何らかのユーザー、グループ、ロールにアタッチされているマネージドポリシー
$ aws iam list-policies --only-attached
{{< /code >}}

### 指定したマネージドポリシーの情報を取得する (iam get-policy)

{{< code >}}
aws iam get-policy --policy-arn <ARN>
{{< /code >}}

{{< accordion title="実行例" >}}
{{< code >}}
$ aws iam get-policy --output yaml \
    --policy-arn arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole
Policy:
  Arn: arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole
  AttachmentCount: 0
  CreateDate: '2015-04-09T15:03:43+00:00'
  DefaultVersionId: v1
  Description: Provides write permissions to CloudWatch Logs.
  IsAttachable: true
  Path: /service-role/
  PermissionsBoundaryUsageCount: 0
  PolicyId: ANPAJNCQGXC42545SKXIK
  PolicyName: AWSLambdaBasicExecutionRole
  UpdateDate: '2015-04-09T15:03:43+00:00'
{{< /code >}}
{{< /accordion >}}

### ユーザーに割り当てられたマネージドポリシーの一覧を表示する (iam list-attached-user-policies)

{{< code >}}
$ aws iam list-attached-user-policies --user-name <ユーザー名>
{{< /code >}}

### グループに割り当てられたポリシーの一覧を表示する (iam list-attached-group-policies)

{{< code >}}
$ aws iam list-attached-group-policies --group-name <グループ名>
{{< /code >}}

### ユーザーにマネージドポリシーを割り当てる (iam attach-user-policy)

{{< code >}}
$ aws iam attach-user-policy --user-name <ユーザー名> --policy-arn <ARN>
{{< /code >}}

{{< code title="例: パスワードを変更できるようにする" >}}
$ aws iam attach-user-policy --user-name user1 \
    --policy-arn arn:aws:iam::aws:policy/IAMUserChangePassword
{{< /code >}}

### ユーザーにインラインポリシーを追加する (iam put-user-policy)

{{< code >}}
$ aws iam put-user-policy --user-name <ユーザー名> \
    --policy-name <インラインポリシー名> --policy-document <JSON文字列>
{{< /code >}}

ポリシーの指定はちょっと長くなるので、通常は外部の JSON ファイルなどで記述しておいて、次のように `file://ファイル名` で渡します。

{{< code >}}
$ aws iam put-user-policy --user-name <ユーザー名> \
    --policy-name <インラインポリシー名> --policy-document file://policy.json
{{< /code >}}
{{< code lang="json" title="policy.json" >}}

{{< /code >}}


### ユーザーのインラインポリシーの一覧を表示する (iam list-user-policies)

{{< code >}}
$ aws iam list-user-policies --user-name <ユーザー名>
{{< /code >}}

### ロールにポリシーを割り当てる (iam attach-role-policy)

{{< code >}}
$ aws iam attach-role-policy --role-name <ロール名> --policy-arn <ポリシーARN>
{{< /code >}}


ロール関連 {#role}
----

### IAM ロールの一覧を取得する (iam list-roles)

{{< code title="例: すべてのロールを表示" >}}
$ aws iam list-roles
{{< /code >}}

{{< code title="例: パスが /service-role/ で始まるロールを表示" >}}
$ aws iam list-roles --path /service-role/
{{< /code >}}

### IAM ロールを作成する (iam create-role)

{{< code title="例: Lambda 用の実行ロール lambda-ex を作成する" >}}
$ aws iam create-role --role-name lambda-ex \
      --assume-role-policy-document file://trust-policy.json
{{< /code >}}

{{% accordion title="trust-policy.json（入力ファイル）" %}}
STS: Security Token Service の権限を与えるためのポリシー設定です。

{{< code lang="json" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {
        "Service": "lambda.amazonaws.com"
      },
      "Action": "sts:AssumeRole"
    }
  ]
}
{{< /code >}}
{{% /accordion %}}

{{< accordion title="実行結果（YAML形式）" >}}
{{< code lang="yaml" >}}
Role:
  Arn: arn:aws:iam::123456789012:role/lambda-ex
  AssumeRolePolicyDocument:
    Statement:
    - Action: sts:AssumeRole
      Effect: Allow
      Principal:
        Service: lambda.amazonaws.com
    Version: '2012-10-17'
  CreateDate: '2021-03-12T07:20:28+00:00'
  Path: /
  RoleId: TA4JSU73BFIAROAVTXAP4
  RoleName: lambda-ex
{{< /code >}}
{{< /accordion >}}

ロールを作成するときに、__`--path`__ オプションでパスを設定しておくと、`iam list-roles` でロールを列挙するときに、パスのプレフィックスで検索できるようになります。

{{< code title="例: パス付きのロールを作成する" >}}
$ aws iam create-role --role-name lambda-ex \
      --path /division-abc/product-xyz/ \
      --assume-role-policy-document file://trust-policy.json
{{< /code >}}

### IAM ロールを削除する (iam delete-role)

{{< code title="例: lambda-ex という名前のロールを削除する" >}}
$ aws iam delete-role --role-name lambda-ex
{{< /code >}}

### IAM ロールにタグを設定する (iam tag-role)

{{< code title="例: ロールにタグ (AppName:HelloWorld) を設定する" >}}
$ aws iam tag-role --role-name my-role \
      --tags Key=AppName,Value=HelloWorld
{{< /code >}}

複数のタグをまとめて設定したいときは、`--tags` オプションで、キーと値のペアをスペースで区切って並べます。

{{< code >}}
--tags Key=key1,Value=value1 Key=key2,Value=value2
{{< /code >}}

### IAM ロールからタグを削除する (iam untag-role)

{{< code title="例: ロールからタグ Key1、Key2、Key3 を削除する" >}}
$ aws iam untag-role --role-name my-role \
      --tag-keys Key1 Key2 Key3
{{< /code >}}


