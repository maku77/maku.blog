---
title: "Firestore のデータバンドルを作成してドキュメントの読み込み回数を削減する"
url: "p/dtdtbr8/"
date: "2022-06-05"
tags: ["Firebase/CloudFunctions", "Firebase/Firestore"]
---

何をするか？
----

Firestore はクライアントアプリから直接アクセスできることが利点ですが、多数のクライアントから複数のドキュメントを読み込んでいると、あっという間に無料枠を超えて高額な請求が発生してしまいます。

{{< image src="img-001.drawio.svg" title="同じデータなのに何度もドキュメントの Read が発生する" >}}

[Firebase 8.2.0](https://firebase.google.com/support/release-notes/js?hl=ja#version_820_-_december_11_2020) でリリースされた [Cloud Firestore Data Bundles](https://firebase.google.com/docs/firestore/bundles) という仕組みを使用すると、Firestore から取得したデータ（クエリ結果）をバンドルというデータにまとめておいて、それを使いまわすことができます。
データバンドルを CDN でキャッシュ、あるいはクライアントサイドでキャッシュすることにより、Firestore へのアクセスを発生させずに、あたかも Firestore からデータフェッチしたかのように動作させることが可能です。
ユーザー数の多いアプリに導入すれば、大きなコスト削減につながります。

データバンドルは Cloud Functions を使って作成してしまうのが簡単です。
下記のような構成にすれば、クライアントアプリは Firestore にアクセスする代わりにデータバンドルを取得して動作するようになります。

{{< image src="img-002.drawio.svg" title="Cloud Functions でバンドルを生成する" >}}

しかし、これだけでは複数のクライアントから Cloud Functions へのアクセスが発生してしまうので、結局はその都度 Firestore へのアクセスが発生してしまいます。
各クライアントアプリではキャッシュが有効ですが、そのキャッシュでさえ、`Ctrl(Cmd) + R` によるスーパーリロードで無視されてしまいます。
そこで、次のようにさらに CDN (Firebase Hosting) を挟んでデータバンドルをキャッシュすることで、各クライアントからのアクセスで Cloud Functions が起動されてしまうのを防ぎます。

{{< image src="img-003.drawio.svg" title="CDN でバンドルをキャッシュする" >}}

この構成になっていれば、クライアントがいくら強制リロードしようが、CDN (Firebase Hosting) にキャッシュされたデータバンドルのみが参照されるようになります。
Firestore へのアクセスが発生するのは、CDN 上のキャッシュが無効になったときのみです。
クライアント側のキャッシュ時間や、CDN のキャッシュ時間は、Cloud Functions の関数が返すレスポンスヘッダ (`Cache-Control`) で制御できます。


Cloud Functions でデータバンドルを作成する
----

下記のコードでは、Cloud Functions に登録する `createBundle` 関数を定義しています。
処理の流れは次のようになっています。

1. Firestore のコレクションからドキュメントを取得（ここでは最新の 50 件の books データ）
2. データバンドルを作成し、上記 books データを名前付きクエリ結果として格納
3. 関数のレスポンスとしてデータバンドルを返す

Firestore には既に `books` コレクションが登録されているものとします。

{{< code lang="ts" title="functions/index.ts" >}}
import * as admin from 'firebase-admin'
import * as functions from 'firebase-functions'

const COLLECTION_NAME = 'books' // Firestore のコレクション名
const QUERY_NAME = 'latest-books-query' // バンドルに作成する名前付きクエリの名前
const MAX_AGE = 300 // ブラウザにキャッシュさせる秒数
const S_MAXAGE = 600 // CDN にキャッシュさせる秒数

admin.initializeApp({ projectId: 'myapp-12345' })
const db = admin.firestore()

/** Cloud Functions に登録する関数 */
export const createBundle = functions
  .region('us-central1') // asia-northeast1 にしたいけど Firebase Hosting が対応してない
  .https.onRequest(async (req, res) => {
    // Firestore からデータを取得する（これは頻繁には呼ばれないようにしたい）
    console.log(`Fetch from Firestore [collection=${COLLECTION_NAME}]...`)
    const books = await db
      .collection(COLLECTION_NAME)
      .orderBy('date', 'desc') // 新しい順
      .limit(50)
      .get()

    // 取得したデータからデータバンドルと名前付きクエリを作成する
    console.log(`Create a named query [queryName=${QUERY_NAME}]`)
    const builder = db.bundle() // バンドル ID は省略可能
    const bundleBuffer = builder.add(QUERY_NAME, books).build()

    // 何度もこの関数が呼ばれないようにブラウザと CDN にキャッシュ時間を指示する
    res.set('Cache-Control', `public, max-age=${MAX_AGE}, s-maxage=${S_MAXAGE}`)

    // テストで Client JS から直接呼び出す場合は CORS 対応しておく
    res.set('Access-Control-Allow-Origin', '*')
    res.set('Access-Control-Allow-Methods', 'GET, HEAD, OPTIONS, POST')
    res.set('Access-Control-Allow-Headers', 'Content-Type, Authorization')

    // データバンドルを返す
    res.end(bundleBuffer)
  })
{{< /code >}}

ポイントは、CDN とクライアントアプリにデータバンドルをキャッシュさせるために __`Cache-Control`__ レスポンスヘッダを付加するところでしょうか（参考: [Manage cache behavior | Firebase](https://firebase.google.com/docs/hosting/manage-cache)）。

```ts
response.set('Cache-Control', 'public, max-age=300, s-maxage=600')
```

`Cache-Control` の各値は次のような意味を持っています。

- __`public`__ ... CDN (Firebase Hosting) でもデータバンドルをキャッシュさせます。これを指定しなかった場合のデフォルト値は `private` で、クライアントアプリ（ブラウザ）のみがデータバンドルをキャッシュ可能になります。
- __`max-age=300`__ ... キャッシュ有効時間（秒）を指示します。クライアントアプリ、および CDN がこの指示に従います。
- __`s-maxage=600`__ ... CDN のキャッシュの有効時間（秒）を別途指示します。これを省略した場合は、CDN のキャッシュ有効時間にも `max-age` が使用されます。

つまり、クライアントアプリでデータバンドルを 300 秒間キャッシュに保持、CDN で 600 秒間キャッシュに保持することになります。

{{% note title="なぜ us-central1 を使う？" %}}
[ホスティング動作を構成する | Firebase Documentation](https://firebase.google.com/docs/hosting/full-config?hl=ja) のドキュメントで、CDN (Firebase Hosting) をプロキシさせて Cloud Functions を呼び出す場合は、`us-central1` のみサポートされているとの記載があります（2022-06 現在）。

> 重要: Firebase Hosting は、us-central1 でのみ Cloud Functions をサポートします。

たしかに、`asia-northeast1` に Cloud Functions 関数をデプロイしてしまうと、CDN との連携がうまくいかない（問答無用で `us-central1` の Cloud Functions に転送されてしまう）ので、今はあきらめて `us-central1` にデプロイしておくしかなさそうです。
Google さん、対応してね！
{{% /note %}}


CDN (Firebase Hosting) でデータバンドルをキャッシュする
----

Firestore のデータバンドルをキャッシュするための CDN (Firebase Hosting) は簡単に作成することができます。
[firebase init functions コマンドなどで Firebase プロジェクトを作成](/p/fvevcs8/) している場合、プロジェクトのルートディレクトリに __`firebase.json`__ ファイルが生成されているはずです。
このファイルに次のように追記することで、Cloud Functions へのプロキシとなる CDN を配置できます。

{{< code lang="js" title="firebase.json" >}}
{
  "hosting": {
    "rewrites": [{ "source": "/createBundle", "function": "createBundle" }],
    "headers": [
      {
        "source": "*",
        "headers": [{ "key": "Access-Control-Allow-Origin", "value": "*" }]
      }
    ]
  },
  // ...
}
{{< /code >}}

設定内容はシンプルで、`/createBundle` という URL へのアクセスを、Cloud Functions の `createBundle` 関数の呼び出しに接続しています。
さらに、別ドメインに配置した Web アプリから CDN へのアクセスを許可するために、CORS 用のレスポンスヘッダ定義を追加しておく必要があります。

- 参考: [Configure hosting behavior | Firebase](https://firebase.google.com/docs/hosting/full-config)。

Cloud Functions 関数の実装と、CDN の設定が済んだら、プロジェクトルートで下記コマンドを実行してデプロイします。

```console
$ firebase deploy
```

デプロイに成功すると、Cloud Functions と CDN の次のようなエンドポイントが有効になります。

- Cloud Functions: `https://us-central1-myapp-12345.cloudfunctions.net/createBundle`
- CDN (Firestore Hosting): `https://myapp-12345.web.app/createBundle`

どちらも同じデータバンドルを返しますが、CDN の方はキャッシュ生成後は一瞬でデータを返してくれるはずです。


クライアントアプリからデータバンドルを取得する
----

最後に、CDN から Firestore データバンドルを取得するクライアントアプリ側の実装です。
Firebase SDK の基本的な扱い方は省略します（参考: [Next.js で Firebase: Cloud Firestore データベースを使う](/p/m3bjrz7/)）。

{{< code lang="ts" title="utils/firebase/firestore.ts（抜粋）" >}}
import { getDocsFromCache, getFirestore, loadBundle, namedQuery } from 'firebase/firestore'
import { Book } from '@/utils/types'

const QUERY_NAME = 'latest-books-query'

export async function getLatestBooksFromBundle(): Promise<Book[]> {
  console.log('Get latest books from Bundle...')
  const db = getFirestore()

  // CDN からデータバンドルを取得
  const bundle = await fetch('https://myapp-12345.web.app/createBundle')
  if (bundle.body == null) {
    console.warn('Bundle has no data')
    return []
  }

  // フェッチしたバンドルから SDK 内にクライアントキャッシュを生成
  await loadBundle(db, bundle.body)

  // クライアントキャッシュから名前付きクエリの Query オブジェクトを生成
  const query = await namedQuery(db, QUERY_NAME)
  if (query == null) {
    console.warn(`Bundle does not have namedQuery: ${QUERY_NAME}`)
    return []
  }

  // 名前付きクエリからデータを取り出す（必要に応じてコンバーターを適用するのはいつも通り）
  const booksQuery = query.withConverter(bookConverter)
  const snapshot = await getDocsFromCache(booksQuery)

  return snapshot.docs.map((doc) => doc.data())
}
{{< /code >}}

例えば、React のカスタムフックなどから上記の関数を呼び出すことで、データバンドルをもとに生成された `Book` 配列を取得できます。
CDN のキャッシュデータだけを参照するので、直接 Firestore にアクセスするも高速に動作し、かつ安価に運用できます。
もちろん、データ取得の柔軟性は減りますし、キャッシュ期間中は最新データを取得できないといった制約がありますが、多数のユーザーがアクセスするトップページの情報などは、この仕組みを導入する価値がありそうです。

