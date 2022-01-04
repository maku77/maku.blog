---
title: "Next.js から AWS DynamoDB にアクセスする"
url: "/p/xp8o5k2"
date: "2021-11-02"
tags: ["Next.js", "AWS/DynamoDB"]
weight: 2
---

何をするか？
----

Next.js アプリの API routes (`pages/*.ts`) や、`getServerSideProps` などのサーバーサイドで実行される関数では、通常の Node.js モジュールを使うことができるため、AWS の DynamoDB からデータを取得する、といったことも自由に行えます。
ここでは、Next.js の API routes 機能を使って、DynamoDB から情報を取得する Web API を作ってみます。
具体的には次のようなことをします。

- テスト用の DynamoDB テーブル (`Books`) を作成する
- `Books` テーブルを参照するためのアクセスキー（IAM ユーザー）を作成する
- Next.js の API routes の実装 (`pages/api/books.ts`) を行う
    - AWS SDK を使って DynamoDB から情報を取得する
    - `/api/books/001` のような URL にアクセスすると JSON データを返す
    - アクセスキーは環境変数で設定する

{{% note title="アクセスキーを使った AWS リソースのアクセスについて" %}}
AWS のアクセスキーは、IAM ユーザーに設定されるものであり、このアクセスキーが漏洩すると、__そのユーザーの権限で何でもできる__ ということになります。
そのため、アクセスキーを使用するときは、IAM ユーザーの権限を適切に絞ることが大切で、そもそも本当に必要なケースでのみアクセスキーを使うようにすべきです。
アクセスキーが必要になるのは、AWS の外から AWS リソースに直接アクセスするケースです。
例えば、AWS CLI のコマンドで AWS の制御を行う場合や、今回の例のように AWS 外のサーバーから AWS へアクセスするようなケースです。

逆に、AWS 内の世界でのやりとりは、IAM ロールを使ってアクセス権を付けるのが常套手段です。
例えば、AWS App Runner 上で Next.js アプリを動かすのであれば、そこに IAM ロールを割り当てれれば、適切な AWS リソースに自由にアクセスできます（もちろん、サーバーサイドで実行される関数コードに限りますが）。
API Gateway (REST API) や AppSync (GraphQL API) に IAM ロールを割り当てれば、バックエンドの Lambda 関数に自由にアクセスできます。
{{% /note %}}

DynamoDB のテーブルを作成する
----

ここでは、テスト用の DynamoDB テーブルとして __`Books`__ テーブルを作成します。
プライマリキーとして `BookId` というパーティションキーだけを持つシンプルなテーブルです。
既存のテーブルがあればそちらを使っても構いません。

ここでは、AWS CLI を使ってサクッと作ってしまいますが、本格的なプロダクト用に作成するときは、CDK や Terraform などを使ってリソースの生成を自動化しましょう。
手作業でやるなら CLI よりも DynamoDB の [マネージドコンソール](https://console.aws.amazon.com/dynamodbv2/) を使った方が楽かもしれません。

{{< code title="Books テーブルの作成" >}}
$ aws dynamodb create-table --table-name Books \
    --attribute-definitions AttributeName=BookId,AttributeType=S \
    --key-schema AttributeName=BookId,KeyType=HASH \
    --billing-mode PAY_PER_REQUEST
{{< /code >}}

テーブルができたら、適当にアイテムを追加しておきます。

{{< code title="Books テーブルにアイテムを追加" >}}
$ aws dynamodb put-item --table-name Books --item '{"BookId": {"S": "001"}, "Title": {"S": "Title-1"}}'
$ aws dynamodb put-item --table-name Books --item '{"BookId": {"S": "002"}, "Title": {"S": "Title-2"}}'
$ aws dynamodb put-item --table-name Books --item '{"BookId": {"S": "003"}, "Title": {"S": "Title-3"}}'
{{< /code >}}

最後に、アイテムが追加されているか確認しておきます。

{{< code >}}
$ aws dynamodb scan --table-name Books --output yaml
{{< /code >}}

{{< accordion title="実行結果" >}}
{{< code >}}
ConsumedCapacity: null
Count: 3
Items:
- BookId:
    S: '001'
  Title:
    S: Title-1
- BookId:
    S: '003'
  Title:
    S: Title-3
- BookId:
    S: '002'
  Title:
    S: Title-2
ScannedCount: 3
{{< /code >}}
{{< /accordion >}}

- 参考: [DynamoDB をコマンドライン (CLI) で操作する](/p/zkzamw8)


IAM ユーザーとアクセスキーを作成する
----

AWS の各種サービスにアクセスするためのアクセスキーが欲しいときは、まず IAM ユーザーを作成して、その IAM ユーザー用にアクセスキーを生成する、という手順を踏みます。
このあたりは、サービス側でアクセスキーを作ることのできる Azure とは違って、ややとっつきにくいかもしれません。
そして、そのアクセスキーの権限は、IAM ユーザーに設定された権限と等しいものになります。

ここでは、AWS CLI を使って、IAM ユーザーの作成、アクセスキーの作成、権限の設定を行います（もちろん AWS コンソールを使っても OK です）。
DynamoDB の `Books` テーブルにアクセスするための IAM ユーザーなので、ユーザー名は `user-for-books` としておきます。
このユーザーには、AWS コンソールにサインインするためのパスワードは設定せず、本当にアクセスキーを提供するためだけのユーザーという扱いにします。

{{< code title="IAM ユーザーとアクセスキーの作成" >}}
$ aws iam create-user --user-name user-for-books
$ aws iam create-access-key --user-name user-for-books
{{< /code >}}

このとき、次のようなアクセスキー ID とシークレットアクセスキーが表示されるのでメモしておきます。

{{< code >}}
AccessKeyId: AKIAQXINMCQAELHU2GXY
SecretAccessKey: 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
{{< /code >}}

DynamoDB の `Books` テーブルの読み取りアクセスを許可するためのインラインポリシーを IAM ユーザーに追加します。

{{< code title="IAM ユーザーにインラインポリシーを追加" >}}
$ aws iam put-user-policy --user-name user-for-books \
    --policy-name dynamo-books-read --policy-document file://policy.json
{{< /code >}}
{{< code lang="json" title="policy.json（入力ファイル）" hl_lines="15" >}}
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Sid": "ReadOnBooks",
      "Effect": "Allow",
      "Action": [
        "dynamodb:GetItem",
        "dynamodb:BatchGetItem",
        "dynamodb:DescribeTable",
        "dynamodb:Query",
        "dynamodb:Scan",
        "dynamodb:ConditionCheckItem"
      ],
      "Resource": "arn:aws:dynamodb:ap-northeast-1:123456789012:table/Books"
    }
  ]
}
{{< /code >}}

`Resource` のところで指定するテーブル ARN は、下記のコマンドで確認できます。

{{< code >}}
$ aws dynamodb describe-table --table-name Books --query Table.TableArn
{{< /code >}}

インラインポリシーがちゃんと設定されているかは、次のようなコマンドで確認できます。

{{< code >}}
$ aws iam list-user-policies --user-name user-for-books
$ aws iam get-user-policy --user-name user-for-books \
    --policy-name dynamo-books-read
{{< /code >}}

- 参考: [AWS IAM の設定をコマンドライン (CLI) で行う](/p/aov4bho)
- 参考: [DynamoDB 用のポリシー設定例](/p/gk6jx9k)


Next.js に Web API を実装する
----

まず、Web API の実装コードから AWS のアクセスキーを参照できるように、環境変数を設定しておきます。
ここでは、プロジェクトのルートに次のような `.env.local` ファイルを作成して環境変数を設定することにします。
このファイルは __Git にはコミットしないように注意__ してください。
Vercel などの環境にデプロイする場合は、Vercel サービス側の環境変数として設定してください。

{{< code title=".env.local" >}}
BOOKS_ACCESS_KEY_ID=AKIAQXINMCQAELHU2GXY
BOOKS_SECRET_ACCESS_KEY=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
{{< /code >}}

DynamoDB のアクセスのために、AWS SDK ver.3 の `DynamoDBClient` クラスを使うので、次のようにインストールしておきます。

{{< code >}}
# npm を使う場合
$ npm install @aws-sdk/client-dynamodb

# yarn を使う場合
$ yarn add @aws-sdk/client-dynamodb
{{< /code >}}

下記は、DynamoDB の `Books` テーブルからアイテムを 1 つ取得する `fetchBook` 関数の実装例です。
AWS のアクセスキーは環境変数から取得しています。

{{< code lang="ts" title="src/libs/booksUtil.ts" >}}
import { DynamoDBClient, GetItemCommand } from '@aws-sdk/client-dynamodb'

const dbClient = new DynamoDBClient({
  credentials: {
    accessKeyId: process.env.BOOKS_ACCESS_KEY_ID as string,
    secretAccessKey: process.env.BOOKS_SECRET_ACCESS_KEY as string,
  },
})

export type Book = {
  id: string
  title: string
}

// アイテムを取得する
export async function fetchBook(bookId: string): Promise<Book | undefined> {
  const command = new GetItemCommand({
    TableName: 'Books',
    Key: { BookId: { S: bookId } },
  })
  const output = await dbClient.send(command)
  const item = output.Item
  if (item == undefined) return undefined
  return {
    id: item['BookId']['S'],
    title: item['Title']['S'],
  } as Book
}
{{< /code >}}

あとは、Web API 用のコードを、上記の関数を使って実装します。

{{< code lang="tsx" title="src/pages/api/books/[bookId].ts" >}}
import type { NextApiRequest, NextApiResponse } from 'next'
import { Book, fetchBook } from '../../../libs/booksUtil'

// API のレスポンス型
export type BooksApiResponse = {
  book?: Book
  debugMessage?: string
}

export default async function booksApi(
  req: NextApiRequest,
  res: NextApiResponse<BooksApiResponse>
): Promise<void> {
  const id = req.query.bookId as string
  const book = await fetchBook(id)
  if (book) {
    res.status(200).json({ book })
  } else {
    res.status(400).json({ debugMessage: `Book [id=${id}] not found` })
  }
}
{{< /code >}}

これで、`http://localhost:3000/api/books/001` などにアクセスすると、次のような JSON データが返ってくるようになります。

{{< code lang="json" >}}
{"book":{"id":"001","title":"Title-1"}}
{{< /code >}}

React コンポーネントの中から、この Web API を呼び出す部分は省略しますが、`useSWR` フックを使うとお手軽に実装できます。

- 参考: [Next.js で環境変数を扱う (.env, NEXT_PUBLIC, NODE_ENV)](/p/gbpeyov)
- 参考: [Next.js の API Routes 機能で Web API を作成する](/p/qcp2coz)
- 参考: [DynamoDB を Node.js で操作する（SDK ver.3 の場合）](/p/5mv5dkt)
- 参考: [React フック: useSWR でデータフェッチ](/p/vm2ft83)


後始末
----

上記で使ったひととおりの AWS リソースを削除しておきます。

### IAM ユーザーの削除

アクセスキー用に作成した IAM ユーザー (`user-for-books`) を削除します。
IAM ユーザーを削除するときは、先にそれに付随する情報（インラインポリシーやアクセスキーなど）を削除する必要があります。

{{< code title="IAM ユーザーのインラインポリシーを削除" >}}
$ aws iam delete-user-policy --user-name user-for-books \
    --policy-name dynamo-books-read
{{< /code >}}

{{< code title="IAM ユーザーのアクセスキーを削除" >}}
# 下記でアクセスキー ID を確認して
$ aws iam list-access-keys --user-name user-for-books

# 次のように削除
$ aws iam delete-access-key --user-name user-for-books \
    --access-key-id AKIAQXINMCQAELHU2GXY
{{< /code >}}

{{< code title="IAM ユーザー自体を削除" >}}
$ aws iam delete-user --user-name user-for-books
{{< /code >}}

### DynamoDB テーブルの削除

DynamoDB の `Books` テーブルを削除します。
こちらは、テーブルにアイテムが格納されていても問答無用で削除されてしまうので、気をつけて実行しないといけません。

{{< code title="DynamoDB テーブル自体を削除" >}}
$ aws dynamodb delete-table --table-name Books
{{< /code >}}

これで、スッキリ、サッパリ！

