---
title: "Amazon Cognito (2) サインイン後に AWS リソースへのアクセス権限を与える (Cognito Identity Pool)"
url: "/p/3mx8hr2"
date: "2021-05-31"
lastmod: "2021-06-22"
tags: ["AWS", "AWS/Cognito"]
weight: 2
---

何をやるか？
----

- 参考: [Amazon Cognito (1) サインイン可能な Web サイトを作る (with React)](/p/pufs8kx)

上記記事では、Cognito のユーザープール機能を使って、Web サイトにサインイン（サインアップ）機能を付けるところまでを説明しています。
ここでは、さらに Cognito の ID プール機能を使い、認証後のユーザーに AWS リソースへのアクセス権限を割り当てる方法を説明します。

Amazon Cognito サービスは、大きくわけて下記の 2 つの機能を提供しています。

- User Pool ... __認証 (Authentication)__
- Identity Pool ... 認証後のユーザーに対する __認可 (Authorization)__

「認証」の方は、Cognito の User Pool を使う方法以外にも、Amazon、Facebook、Google など、様々な認証サービス（これらを __認証プロバイダー (IdP)__ と呼びます）を使用することができますが、ここでは Cognito の User Pool でユーザー認証することを前提とします。

ここから先の説明は、「認可」に関しての説明になります。
Cognito Identity Pool は、何らかの認証プロバイダーによる認証済みユーザーに対して、IAM ロールを割り当てることによって、任意の AWS リソースへのアクセスを許可します。
また、認証していないユーザー（いわゆるゲストユーザー）に対して、リードオンリーな IAM ロールを割り当てたり、User Pool の方でユーザーをグループに登録しておいて、そのグループの IAM ロールをアクセス権限として割り当てるといったことも可能です。


ID プール（フェデレーティッドアイデンティティ）
----

Cognito の ID プールを使うと、各種認証プロバイダーサービス（Cognito の場合は「ユーザープール」）でサインインしたユーザーに対して、AWS リソース（API Gateway や DynamoDB など）へのアクセス権限を割り当てることができます。
ざっくり言うと、ID プールは、__ユーザーに対して IAM ロールを割り当てる仕組み__ です。

ID プールは次のように作成します。

1. [Cognito のマネージメントコンソール](https://console.aws.amazon.com/cognito/home)にアクセス
2. `ID プールの管理` → `新しいIDプールの作成` を選択します

{{< image border="true" w="500" src="img-001.png" >}}

3. ユーザー作成時の条件 (Attribute) などを設定して作成ボタンを押します

{{< image border="true" w="700" src="img-002.png" >}}

ID プール名は自由に入力してください。
`認証されていない ID` というところを有効にすると、認証していないユーザー（ゲストユーザー）に何らかの AWS リソースアクセス権限（IAM ロール）を与えることができます。
今回は、サインイン済みのユーザーにだけアクセス権限を与えたいので、__`認証プロバイダー`__ のところで、作成した Cognito __`ユーザープールの ID`__（例: `ap-northeast-1_w4Lb7OMrk`）と、__`アプリクライアント ID`__（例: `7egbq6mnk61udtpa0v7qr5u96o`）を設定して、__プールの作成__ ボタンを押して先へ進みます。

すると、具体的にどのような IAM ロールを与えるかを設定する画面が表示されるので、適切なポリシードキュメント（アクセス権限）を記述して __`許可`__ ボタンを押します（後から IAM のマネージメントコンソールから修正可能です）。

{{< image border="true" w="700" src="img-003.png" >}}

ちなみに、デフォルトで作成される IAM ロールのポリシー設定は次のような感じになっています。
S3 や DynamoDB などのリソースにアクセスできるようにするには、ここにポリシーステートメントを追加していく必要があります。

{{< accordion title="認証済みユーザーのポリシードキュメント" >}}
{{< code lang="json" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "mobileanalytics:PutEvents",
        "cognito-sync:*",
        "cognito-identity:*"
      ],
      "Resource": [
        "*"
      ]
    }
  ]
}
{{< /code >}}
{{< /accordion >}}

{{< accordion title="認証していないユーザーのポリシードキュメント" >}}
{{< code lang="json" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "mobileanalytics:PutEvents",
        "cognito-sync:*"
      ],
      "Resource": [
        "*"
      ]
    }
  ]
}
{{< /code >}}
{{< /accordion >}}

例えば、ある S3 バケットの内容を読み取れるようにしたい場合は、`Statement` プロパティの配列に、次のようなステートメントを追加します。

{{< code lang="json" >}}
{
  "Effect": "Allow",
  "Action": [
    "s3:GetObject",
    "s3:ListBucket"
  ],
  "Resource": [
    "arn:aws:s3:::temp-123456789012-bucket-1",
    "arn:aws:s3:::temp-123456789012-bucket-1/*"
  ]
}
{{< /code >}}

無事 ID プールの作成に成功すると、下記のように ID プールの ID を確認することができます。
この ID は、プログラム内で指定する必要があるので控えておきます。

{{< image border="true" w="700" src="img-004.png" >}}

{{% private %}}
- [Cognito ID に基づいた DynamoDB の項目レベルのアクセスを許可する](https://docs.aws.amazon.com/ja_jp/IAM/latest/UserGuide/reference_policies_examples_dynamodb_items.html)
{{% /private %}}


認可処理の実装
----

Cognito ID プールによる認可処理は、Amplify SDK (`aws-amplify`) を使って認証を済ませると自動的に行われます。
React アプリなどで認証処理を行う方法は、[こちらの記事](/p/pufs8kx) で説明していますが、ID プールによる認可処理まで自動実行するには、`Amplify.configure()` で渡すコンフィグ情報に、__`identityPoolId`__ を指定しておく必要があります。
ここに指定する値は、上記で作成した ID プールの ID（例: `ap-northeast-1:12e74c09-fe09-e845-f48e-929364272fb`）です。

{{< code lang="tsx" title="src/pages/_app.tsx（抜粋）" >}}
import { Amplify } from 'aws-amplify'

const amplifyConfig = {
  Auth: {
    region: 'ap-northeast-1',
    userPoolId: 'ap-northeast-1_w4Lb7OMrk',
    userPoolWebClientId: '7egbq6mnk61udtpa0v7qr5u96o',
    // AWS 認可処理にはこの ID プールの指定が必要↓
    identityPoolId: 'ap-northeast-1:12e74c09-fe09-e845-f48e-929364272fb',
  },
}

Amplify.configure(amplifyConfig)
{{< /code >}}

React 上での認証処理は、`@aws-amplify/ui-react` モジュールが提供する `AmplifyAuthenticator` コンポーネントなどで行うと簡単です。
認証後に表示するページで、次のようなコードを実行すると、アクセストークンの情報を取得できていることが分かります。

{{< code lang="tsx" title="src/pages/index.tsx" >}}
import React from 'react'
import { Auth } from 'aws-amplify'

const getAuth: () => void = async () => {
  try {
    const cred = await Auth.currentCredentials();
    console.log(cred.authenticated)    // true
    console.log(cred.accessKeyId)      // P4TAUTOAEOY5ASIAVTXA
    console.log(cred.secretAccessKey)  // b3jXGfXXfI7toiiH6WAVXqRyeFdfnvFC58Lo+6iD
    console.log(cred.sessionToken)     // ...長大...
  } catch (e) {
    console.error(e)
  }
}

const IndexPage: React.FC = () => {
  getAuth()
  return <div>Hello</div>
}
{{< /code >}}

あとは任意の AWS SDK（`@aws-sdk/client-s3` など）を使って AWS リソースへアクセスすることができます。
例えば、次のサンプルコードでは、S3 バケット内のオブジェクト名（キー）の一覧を取得しています。
ちなみに、React (Next.js) アプリからデータフェッチを行うときは、この例のように [useSWR フックを使う方法](/p/vm2ft83) が常套手段です。

{{< code lang="tsx" >}}
import useSWR from 'swr'
import { Auth } from 'aws-amplify'
import { S3Client, ListObjectsV2Command } from '@aws-sdk/client-s3'

type BucketData = {
  keys: string[]
}

// S3 バケット内のオブジェクトの一覧を取得する
async function fetchS3Objects(bucket: string): Promise<BucketData> {
  try {
    const s3 = new S3Client({
      region: 'ap-northeast-1',
      credentials: await Auth.currentCredentials(),
    })
    const output = await s3.send(
      new ListObjectsV2Command({
        Bucket: bucket,
        MaxKeys: 10, // 最大10件まで取得
      })
    )
    if (!output.Contents) return []
    return {
      keys: output.Contents.map((c) => c.Key!)
    }
  } catch (err) {
    throw err
  }
}

export const ObjectList: React.FC = () => {
  const { data, error } = useSWR<BucketData, Error>(
    'temp-123456789012-bucket-1',
    fetchS3Objects,
  )
  if (error) return <div>Error: {error.message}</div>
  if (!data) return <div>Loading...</div>
  return (
    <ul>
      {data.keys.map((k) => <li key={k}>{k}</li>)}
    </ul>
  )
}
{{< /code >}}

Web ブラウザ上の JavaScript で S3 バケットにアクセスしようとすると、デフォルトではクロスドメインアクセスのため CORS policy エラーになります。
S3 バケットの CORS 設定をしておく必要があります。

{{< code title="S3 アクセス時の CORS policy エラー" >}}
Access to fetch at 'https://temp-123456789012-bucket-1.s3.ap-northeast-1.amazonaws.com/?list-type=2&max-keys=10' from origin 'http://localhost:3000' has been blocked by CORS policy: ...
{{< /code >}}

[Amazon S3 のマネージメントコンソール](https://s3.console.aws.amazon.com/) で対象のバケットを選択し、__アクセス許可__ のタブの下の方に、__Cross-Origin Resource Sharing (CORS)__ という項目があるので、そこに次のような感じで入力すれば、クロスドメインアクセスができるようになります。

{{< code lang="json" title="S3 バケットの CORS 設定" >}}
[
    {
        "AllowedHeaders": [
            "*"
        ],
        "AllowedMethods": [
            "HEAD",
            "GET",
            "PUT",
            "POST",
            "DELETE"
        ],
        "AllowedOrigins": [
            "*"
        ],
        "ExposeHeaders": [
            "ETag"
        ]
    }
]
{{< /code >}}

ここでは、まずはアクセスできることを確認するために、すべてのドメインからの CORS アクセス許可をしていますが、実際には `AllowdOrigins` のところでドメイン名を絞り込んでください。
ドメイン名の指定では、一箇所でワイルドカード (`*`) を使えますが、`http` や `https` などのスキーム指定は必須です（ポート番号 80 以外の場合はポート番号も必須です）。

{{< code lang="js" >}}
{
    // ...
    "AllowedOrigins": [
        "http://localhost:*",
        "https://example.com",
        "https://*.example.com",
    ],
    // ...
}
{{< /code >}}

これでアクセスして 403 Forbidden (Access Denied) エラーが出るようでしたら、おそらく IAM Role のポリシーで S3 アクセスが許可できていません。
Role のポリシー設定を確認してください。


ユーザーが所属するグループに応じてアクセス権限を割り当てる
----

ユーザーの種類によって AWS リソースへのアクセス権限を細かく制御したいということはよくあります。
例えば、管理者ユーザーと一般ユーザーを分けたり、チームごとに参照可能なリソースを制御したいという場合です。
これを実現するには、Cognito ユーザープールの __グループ__ 機能を使用します。

グループは、[Cognito ユーザープールのコンソール](https://console.aws.amazon.com/cognito/) から簡単に作成できます。

{{< image w="500" border="true" src="img-group-001.png" >}}

{{< image w="500" border="true" src="img-group-002.png" >}}

ユーザーは複数のグループに所属することができるため、その場合にどの IAM Role を優先的に適用するかを、__優先順位__ の欄で設定しておくことができます（0 が最優先）。
グループを作成したら、任意のユーザーをグループに追加してください。

次に、[Cognito ID プール（フェデレーテッドアイデンティティ）のコンソール](https://console.aws.amazon.com/cognito/) から、認可に使用する ID プールを選択し、認証後に上記グループの IAM Role を適用するように設定します。
ID プールの編集画面を開き、`認証プロバイダ` の `認証されたロールの選択` の項目で、__`トークンからロールを選択する`__ を選択して、`変更の保存` ボタンを押します。

{{< image w="500" border="true" src="img-group-003.png" >}}

これで、Cognito ユーザープールによって認証されたユーザーに対して、グループに設定した IAM Role が割り当てられるようになります。
`トークンからロールを選択する` とか、分かりにくい表現ですが、Cognito では内部的に JSON Web Token を使った処理が行われていて、その過程でグループ側の IAM Role を割り当てる、といった感じのことを示しているようです。
なお、IAM Role の優先順位が解決できない場合や、どのグループにも属していないユーザーに対しては、これまで通り、「認証されたロール」で設定した IAM Role が割り当てられます。


参考
----

- [Amazon Cognito (1) サインイン可能な Web サイトを作る (Cognito User Pool)](/p/pufs8kx)

