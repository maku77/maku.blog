---
title: "Go 言語と AWS SDK V2 で Amazon Cognito を操作する"
url: "/p/nej9wjb"
date: "2022-04-11"
tags: ["AWS", "AWS/Cognito"]
---

ここでは、AWS SDK for Go V2 を使って、Go 言語から Amazon Cognito を操作するサンプルコードを示します。
[Go 言語で AWS SDK を使うための開発環境](/p/xnogqgm) は構築済みとします。



指定したユーザープール内のユーザーリストを取得する (ListUsers)
----

{{< code lang="go" title="list_users.go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-sdk-go-v2/aws"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/cognitoidentityprovider"
)

var userPoolId = "ap-northeast-1_XXXXXXXXX" // ユーザープールの ID

func main() {
	listUsers()
}

func listUsers() {
	client := cognitoidentityprovider.NewFromConfig(loadAwsConfig())
	input := &cognitoidentityprovider.ListUsersInput {
		UserPoolId: &userPoolId,
	}
	output, err := client.ListUsers(context.TODO(), input)
	if err != nil {
		panic(err)
	}
	for _, user := range output.Users {
		fmt.Printf("%s, %s\n", user.UserCreateDate, *user.Username)
	}
	fmt.Printf("Found %d users\n", len(output.Users))
}

// 外部リソース（~/.aws/config など）からコンフィグ情報 (aws.Config) を生成します。
func loadAwsConfig() aws.Config {
	cfg, err := config.LoadDefaultConfig(context.TODO())
	if err != nil {
		panic(err)
	}
	return cfg
}
{{< /code >}}

{{< code lang="console" title="実行例" >}}
$ go run list_users.go
2021-11-25 06:37:28.751 +0000 UTC, user-name-1
2021-10-28 02:34:29.485 +0000 UTC, user-name-2
2021-11-30 13:49:28.566 +0000 UTC, user-name-3
Found 3 users
{{< /code >}}


ユーザーのパスワードを変更する (AdminSetUserPassword)
----

{{< code lang="go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-sdk-go-v2/aws"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/cognitoidentityprovider"
)

var userPoolId = "ap-northeast-1_XXXXXXXXX" // ユーザープールの ID

func main() {
	adminSetUserPassword()
}

func adminSetUserPassword() {
	client := cognitoidentityprovider.NewFromConfig(loadAwsConfig())
	user := "user1"
	pass := "Password#123"
	input := &cognitoidentityprovider.AdminSetUserPasswordInput{
		UserPoolId: &userPoolId,
		Username:   &user,
		Password:   &pass,
	}
	_, err := client.AdminSetUserPassword(context.TODO(), input)
	if err != nil {
		panic(err)
	}
	fmt.Println("Password changed")
}
{{< /code >}}


ユーザー名とパスワードで認証して ID トークンとアクセストークンを取得する (AdminInitiateAuth)
----

下記のように `AdminInitiateAuth` API でユーザー認証を行うには、Cognito ユーザープールの設定で対象のクライアントを選択し、__`ALLOW_ADMIN_USER_PASSWORD_AUTH`__ にチェックを入れておく必要があります。

{{< code lang="go" >}}
package main

import (
	"context"
	"fmt"

	"github.com/aws/aws-sdk-go-v2/aws"
	"github.com/aws/aws-sdk-go-v2/config"
	"github.com/aws/aws-sdk-go-v2/service/cognitoidentityprovider"
	"github.com/aws/aws-sdk-go-v2/service/cognitoidentityprovider/types"
)

var userPoolId = "ap-northeast-1_XXXXXXXXX" // ユーザープールの ID
var clientId = "XXXXXXXXXXXXXXXXXXXXXXXXX"

func main() {
	adminInitiateAuth()
}

func adminInitiateAuth() {
	client := cognitoidentityprovider.NewFromConfig(loadAwsConfig())
	input := &cognitoidentityprovider.AdminInitiateAuthInput{
		UserPoolId:     &userPoolId,
		ClientId:       &clientId,
		AuthFlow:       types.AuthFlowTypeAdminUserPasswordAuth,
		AuthParameters: map[string]string{"USERNAME": "user1", "PASSWORD": "Password#123"},
	}
	output, err := client.AdminInitiateAuth(context.TODO(), input)
	if err != nil {
		panic(err)
	}

	// レスポンスの出力
	fmt.Printf("ChallengeName = %s\n", output.ChallengeName)
	fmt.Printf("ChallengeParameters = %v\n", output.ChallengeParameters)
	if output.Session != nil {
		fmt.Printf("Session:\n%s\n", *output.Session)
	}
	if output.AuthenticationResult != nil {
		fmt.Println("AuthenticationResult:")
		fmt.Printf("    IdToken = %s\n", *output.AuthenticationResult.IdToken)
		fmt.Printf("    AccessToken = %s\n", *output.AuthenticationResult.AccessToken)
		fmt.Printf("    RefreshToken = %s\n", *output.AuthenticationResult.RefreshToken)
		fmt.Printf("    TokenType = %s\n", *output.AuthenticationResult.TokenType)
		fmt.Printf("    ExpiresIn = %d(sec)\n", output.AuthenticationResult.ExpiresIn)
	}
}
{{< /code >}}

認証に成功すると、API の戻り値の `AuthenticationResult` に各種トークン情報が格納されます。

```
ChallengeName = 
ChallengeParameters = map[]
AuthenticationResult:
    IdToken = eyJraWQiOiJvVU-UYPb...長いので省略...QlbXNjaDF4cHBRWWU
    AccessToken = eyJraWQiOiJ2a1W...長いので省略...MeaGq7Q-ecVo7UrQA
    RefreshToken = eyJjdHkiOiJKV1...長いので省略...FmbPUE4M-CFXNp9aA
    TokenType = Bearer
    ExpiresIn = 3600(sec)
```

場合によっては、次のように追加のチャレンジリクエストが返されることがあります。

```
ChallengeName = NEW_PASSWORD_REQUIRED
ChallengeParameters = map[USER_ID_FOR_SRP:user1 requiredAttributes:["userAttributes.email"] userAttributes:{"email":""}]
Session:
AYABeGUOL0khu9R69cL1oW5AlDMAHQABAAdTZXJ2aWNlABBDb2duaXRvVXN
lclBvVsigrdMZ2QLwOwDNyG485jP45wBAn19cJKXxHhAGETrvfQjHMSOIyz
                  ...長いので省略...
Uk9oosogIT_QEIXF5OrGGtWq7ELm56gXiW1hc06rp3cBYFbOlI5pjp36Jfd
SfWaFjq0yyWYVAwEA4NG0DgtW3xZjto4NpHVCIVdxBmmolAKDtbkvVsWNFQ
```

その場合は、続けて __`AdminRespondToAuthChallenge`__ API を呼び出し、`ChallengeName` で指定されたチャレンジ（上記の場合は `NEW_PASSWORD_REQURED`）に答える必要があります。
このとき、チャレンジ要求時に提示された Session 情報が必要になります。
下記は、`AdminRespondToAuthChallenge` の呼び出し方の例です（ここでは全部ハードコーディングしちゃってます）。

```go
func adminRespondToAuthChallenge() {
	client := cognitoidentityprovider.NewFromConfig(loadAwsConfig())
	session := "AYABeJDb9BhR5D...長いので省略...fZAPW2Ynnslwww"
	input := &cognitoidentityprovider.AdminRespondToAuthChallengeInput{
		UserPoolId:    &userPoolId,
		ClientId:      &clientId,
		ChallengeName: "NEW_PASSWORD_REQUIRED",
		ChallengeResponses: map[string]string{
			"USERNAME":             "user1",
			"NEW_PASSWORD":         "Password#123",
			"userAttributes.email": "user1@example.com",
		},
		Session: &session,
	}
	_, err := client.AdminRespondToAuthChallenge(context.TODO(), input)
	if err != nil {
		panic(err)
	}
	fmt.Println("Auth challenge succeeded")
}
```

チャレンジに正しく応答できると、`AdminRespondToAuthChannelgeInput` API の戻り値の `AuthenticationResult` フィールドに、ID トークンやアクセストークンが格納されます。
これは、`AdminInitiateAuth` API の戻り値と同様です。
チャレンジに応答した後に、再度 `AdminInitiateAuth` を呼び出して各トークンを取得することもできます。

