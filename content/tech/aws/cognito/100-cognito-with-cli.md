---
title: "Amazon Cognito をコマンドライン (CLI) から操作する"
url: "/p/yzjj4c7"
date: "2022-04-06"
tags: ["AWS", "AWS/Cognito"]
weight: 100
---

{{% private %}}
- [cognito-idp — AWS CLI v2 Command Reference](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/cognito-idp/index.html)
- [cognito-identity — AWS CLI v2 Command Reference](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/cognito-identity/index.html)
{{% /private %}}

AWS CLI による Cognito 操作用のコマンドには、`cognito-idp` と `cognito-identity` があります。

- __`aws cognito-idp`__ ... Cognito User Pools の操作（ユーザープール／ID プロバイダー）
  - ユーザープール自体の作成や、ユーザーの追加・削除などを行えます。ユーザープールでの認証に成功すると、ユーザー情報を参照するためのトークンが発行されます。一般的にいうところの ID プロバイダー (IdP) です。
- __`aws cognito-identity`__ ... Cognito Federated Identities の操作（フェデレーテッドアイデンティティー／ID プール）
  - 何らかの ID プロバイダー (IdP) で認証されたユーザーに対して、AWS サービスにアクセスするための一時的なアクセストークンを発行します。ID プロバイダーは上記の Cognito ユーザープールでもよいし、Google や Facebook などのサードパーティ ID プロバイダーでもかまいません。


ユーザープールの操作 (cognito-idp)
----

`aws cognito-idp` のサブコマンドには、`admin-` プレフィックスが付くものと付かないバージョンがあったりしますが、プレフィックスがない方は、ユーザー認証後に取得できるトークンの指定 (`--access-token`) が必要です。

### ユーザープールを作成する (cognito-idp create-user-pool)

```console
$ aws cognito-idp create-user-pool --pool-name "Test user pool"
```

ユーザープールの作成に成功すると、初期設定の内容（パスワードのルールなど）が出力されます。

### ユーザープールを削除する (cognito-idp delete-user-pool)

```console
$ aws cognito-idp delete-user-pool --user-pool-id ap-northeast-1_5O6lfqrqN
```

`--user-pool-id` オプションには、ユーザープール名ではなく、ユーザープール ID を指定することに注意してください。
ユーザープール ID は、`cognito-idp list-user-pools` コマンドで確認できます。

### ユーザープールの一覧を取得する (cognito-idp list-user-pools)

```console
$ aws cognito-idp list-user-pools --max-results 10
```

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
UserPools:
- CreationDate: '2020-10-20T16:56:45.201000+09:00'
  Id: ap-northeast-1_vi4PeCz78
  LambdaConfig: {}
  LastModifiedDate: '2022-03-18T13:37:64.768000+09:00'
  Name: Test users 1
- CreationDate: '2021-05-17T19:41:04.870000+09:00'
  Id: ap-northeast-1_nFafZcqMh
  LambdaConfig: {}
  LastModifiedDate: '2021-10-27T18:31:07.327000+09:00'
  Name: Test users 2
{{< /code >}}
{{< /accordion >}}

### ユーザープールの情報を取得する (cognito-idp describe-user-pool)

```console
$ aws cognito-idp describe-user-pool --user-pool-id ap-northeast-1_5O6lfqrqN
```

{{< accordion title="実行例" >}}
{{< code lang="yaml" >}}
UserPool:
  AdminCreateUserConfig:
    AllowAdminCreateUserOnly: false
    UnusedAccountValidityDays: 7
  Arn: arn:aws:cognito-idp:ap-northeast-1:123456789012:userpool/ap-northeast-1_5O6lfqrqN
  CreationDate: '2022-04-07T01:56:38.960000+09:00'
  EmailConfiguration:
    EmailSendingAccount: COGNITO_DEFAULT
  EstimatedNumberOfUsers: 0
  Id: ap-northeast-1_5O6lfqrqN
  LambdaConfig: {}
  LastModifiedDate: '2022-04-07T01:56:38.960000+09:00'
  MfaConfiguration: OFF
  Name: Test user pool
  Policies:
    PasswordPolicy:
      MinimumLength: 8
      RequireLowercase: true
      RequireNumbers: true
      RequireSymbols: true
      RequireUppercase: true
      TemporaryPasswordValidityDays: 7
  SchemaAttributes:
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: false
    Name: sub
    Required: true
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '1'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: name
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: given_name
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: family_name
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: middle_name
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: nickname
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: preferred_username
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: profile
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: picture
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: website
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: email
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: Boolean
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: email_verified
    Required: false
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: gender
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: birthdate
    Required: false
    StringAttributeConstraints:
      MaxLength: '10'
      MinLength: '10'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: zoneinfo
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: locale
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: phone_number
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: Boolean
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: phone_number_verified
    Required: false
  - AttributeDataType: String
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: address
    Required: false
    StringAttributeConstraints:
      MaxLength: '2048'
      MinLength: '0'
  - AttributeDataType: Number
    DeveloperOnlyAttribute: false
    Mutable: true
    Name: updated_at
    NumberAttributeConstraints:
      MinValue: '0'
    Required: false
  UserPoolTags: {}
  VerificationMessageTemplate:
    DefaultEmailOption: CONFIRM_WITH_CODE
{{< /code >}}
{{< /accordion >}}

### ユーザーを作成する (cognito-idp admin-create-user)

```console
$ aws cognito-idp admin-create-user \
    --user-pool-id ユーザープールID \
    --username ユーザー名 \
    --temporary-password 初期パスワード（省略可） \
    --message-action SUPPRESS（確認メールを送信しない場合）
```

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp admin-create-user --user-pool-id ap-northeast-1_sf1nSZ2Oj --username user1
User:
  Attributes:
  - Name: sub
    Value: c5d45c78-74d0-4c14-b089-3c01cfa2cea9
  Enabled: true
  UserCreateDate: '2022-04-06T16:04:22.543000+09:00'
  UserLastModifiedDate: '2022-04-06T16:04:22.543000+09:00'
  UserStatus: FORCE_CHANGE_PASSWORD
  Username: user1
{{< /code >}}
{{< /accordion >}}

### ユーザーを削除する (cognito-idp admin-delete-user)

```console
$ aws cognito-idp admin-delete-user \
    --user-pool-id ユーザープールID \
    --username ユーザー名
```

### ユーザーの一覧を取得する (cognito-idp list-users)

```console
$ aws cognito-idp list-users --user-pool-id ユーザープールID
```

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp list-users --user-pool-id ap-northeast-1_sf1nSZ2Oj
Users:
- Attributes:
  - Name: sub
    Value: 11f2d50e-5138-4aa8-967e-d0cc1267dc63
  Enabled: true
  UserCreateDate: '2022-04-06T16:09:19.621000+09:00'
  UserLastModifiedDate: '2022-04-06T16:09:19.621000+09:00'
  UserStatus: FORCE_CHANGE_PASSWORD
  Username: user1
{{< /code >}}
{{< /accordion >}}

### 指定したユーザーの情報を取得する (cognito-idp admin-get-user)

```console
$ aws cognito-idp admin-get-user \
    --user-pool-id ユーザープールID \
    --username ユーザー名
```

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp admin-get-user --user-pool-id ap-northeast-1_sf1nSZ2Oj --username user1
Enabled: true
UserAttributes:
- Name: sub
  Value: 11f2d50e-5138-4aa8-967e-d0cc1267dc63
UserCreateDate: '2022-04-06T16:09:19.621000+09:00'
UserLastModifiedDate: '2022-04-06T16:09:19.621000+09:00'
UserStatus: FORCE_CHANGE_PASSWORD
Username: user1
{{< /code >}}
{{< /accordion >}}

### ユーザープールにクライアントを追加する (create-user-pool-client)

```console
$ aws cognito-idp create-user-pool-client \
    --user-pool-id ユーザープールID
    --client-name クライアント名
```

クライアントアプリや AWS CLI で認証を行うには、ユーザープールに「クライアント」を追加しておく必要があります。

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp create-user-pool-client \
    --user-pool-id ap-northeast-1_sf1nSZ2Oj \
    --client-name client1
UserPoolClient:
  AllowedOAuthFlowsUserPoolClient: false
  ClientId: 5sf3ad654tid5qt4r1768stto
  ClientName: client1
  CreationDate: '2022-04-06T16:34:56.112000+09:00'
  EnableTokenRevocation: true
  LastModifiedDate: '2022-04-06T16:34:56.112000+09:00'
  RefreshTokenValidity: 30
  TokenValidityUnits: {}
  UserPoolId: ap-northeast-1_sf1nSZ2Oj
{{< /code >}}
{{< /accordion >}}

### ユーザープールのクライアントの一覧を取得する (list-user-pool-clients)

```console
$ aws cognito-idp list-user-pool-clients --user-pool-id ユーザープールID
```

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp list-user-pool-clients --user-pool-id ap-northeast-1_sf1nSZ2Oj
UserPoolClients:
- ClientId: 1me8va44u5ec9ttirfmo8abro1
  ClientName: myapp-browser
  UserPoolId: ap-northeast-1_sf1nSZ2Oj
- ClientId: 5sf3ad654tid5qt4r1768stto
  ClientName: client1
  UserPoolId: ap-northeast-1_sf1nSZ2Oj
{{< /code >}}
{{< /accordion >}}

### ユーザープールのクライアントの情報を取得する (describe-user-pool-client)

```console
$ aws cognito-idp describe-user-pool-client \
    --user-pool-id ユーザープールID \
    --client-id クライアントID
```

クライアント名ではなく、クライアント ID を指定する必要があることに注意してください。
クライアント ID は、`list-user-pool-clients` コマンドで確認できます。

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ aws cognito-idp describe-user-pool-client --user-pool-id ap-northeast-1_sf1nSZ2Oj --client-id 5sf3ad654tid5qt4r1768stto
UserPoolClient:
  AllowedOAuthFlowsUserPoolClient: false
  ClientId: 5sf3ad654tid5qt4r1768stto
  ClientName: client1
  CreationDate: '2022-04-06T16:34:56.112000+09:00'
  EnableTokenRevocation: true
  LastModifiedDate: '2022-04-06T16:34:56.112000+09:00'
  RefreshTokenValidity: 30
  TokenValidityUnits: {}
  UserPoolId: ap-northeast-1_sf1nSZ2Oj
{{< /code >}}
{{< /accordion >}}

### ユーザープールのクライアントの設定を変更する (update-user-pool-client)

```console
$ aws cognito-idp update-user-pool-client \
    --user-pool-id ユーザープールID \
    --client-id クライアントID \
    ...(設定変更のオプション)...
```

{{< accordion title="実行例（Auth flow の設定）" >}}
{{< code lang="console" >}}
$ aws cognito-idp update-user-pool-client \
    --user-pool-id ap-northeast-1_m43jEsA8r \
    --client-id 2dkct9bm65e9oj3q3bsg40u5g5 \
    --explicit-auth-flows ALLOW_REFRESH_TOKEN_AUTH \
                          ALLOW_ADMIN_USER_PASSWORD_AUTH

UserPoolClient:
  AllowedOAuthFlowsUserPoolClient: false
  ClientId: 2dkct9bm65e9oj3q3bsg40u5g5
  ClientName: client1
  CreationDate: '2022-04-07T02:29:09.818000+09:00'
  EnableTokenRevocation: true
  ExplicitAuthFlows:
  - ALLOW_ADMIN_USER_PASSWORD_AUTH
  - ALLOW_REFRESH_TOKEN_AUTH
  LastModifiedDate: '2022-04-07T03:21:50.683000+09:00'
  RefreshTokenValidity: 30
  TokenValidityUnits: {}
  UserPoolId: ap-northeast-1_m43jEsA8r
{{< /code >}}
{{< /accordion >}}

### ユーザーのパスワードを設定する (admin-set-user-password)

```console
$ aws cognito-idp admin-set-user-password \
    --user-pool-id ユーザープールID \
    --username 既存のユーザー名 \
    --password 新しいパスワード
```

{{< code lang="console" title="実行例" >}}
$ aws cognito-idp admin-set-user-password \
    --user-pool-id ap-northeast-1_sf1nSZ2Oj \
    --username user1 --password Password!123
{{< /code >}}

### ユーザー認証する (admin-initiate-auth)

```console
$ aws cognito-idp admin-initiate-auth \
    --user-pool-id ユーザープールID \
    --client-id クライアントID \
    --auth-flow ADMIN_USER_PASSWORD_AUTH
    --auth-parameters USERNAME=ユーザー名,PASSWORD=パスワード
```

ユーザープールに登録されたユーザーの名前とパスワードで認証し、トークンを取得します。
この管理 API でユーザー認証を行うには、Cognito ユーザープールの設定で対象のクライアントを選択し、__`ALLOW_ADMIN_USER_PASSWORD_AUTH`__ にチェックを入れておく必要があります。
これを有効にしておかないと、管理者用の Cognito API（`admin-initiate-auth` コマンドなど）で認証できません。

{{< accordion title="実行例（パスワード変更が必要な場合）" >}}
{{< code lang="console" >}}
$ aws cognito-idp admin-initiate-auth --user-pool-id ap-northeast-1_sf1nSZ2Oj --client-id 5sf3ad654tid5qt4r1768stto --auth-flow ADMIN_USER_PASSWORD_AUTH --auth-parameters USERNAME=user1,PASSWORD=Password!123
ChallengeName: NEW_PASSWORD_REQUIRED
ChallengeParameters:
  USER_ID_FOR_SRP: user1
  requiredAttributes: '["userAttributes.email"]'
  userAttributes: '{"email":""}'
Session: AYABeBBlOn...（長いので省略）...Q_FXfCoWCsI-w
{{< /code >}}
{{< /accordion >}}

レスポンスとして、`ChallengeName: NEW_PASSWORD_REQUIRED` が返ってきた場合は、新しいパスワード設定する必要があります。
新しいパスワードの設定も CLI から行えます（`admin-initiate-auth` のレスポンスに含まれているセッショントークン `Session` の値が必要です）。

```console
$ aws cognito-idp admin-respond-to-auth-challenge \
    --user-pool-id ap-northeast-1_sf1nSZ2Oj \
    --client-id 5sf3ad654tid5qt4r1768stto \
    --challenge-name NEW_PASSWORD_REQUIRED \
    --challenge-responses USERNAME=user1,NEW_PASSWORD="passWORD#123",userAttributes.email="user1@example.com" \
    --session "AYABeBBlOnhrpnx82...（省略）...L7uMc6Q_FXfCoWCsI-w"
```

