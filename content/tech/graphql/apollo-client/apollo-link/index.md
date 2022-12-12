---
title: "Apollo Client の ApolloLink チェーンで HTTP リクエストをカスタマイズする"
url: "p/xa62yo4/"
date: "2022-10-26"
tags: ["Apollo", "GraphQL"]
---

ApolloLink とは？
----

{{< image w="600" src="img-001.drawio.svg" title="Apollo Client の Link チェ―ン" >}}

Apollo Client を使って GraphQL API を呼び出すとき、GraphQL サーバーに対して HTTP リクエストが送られるわけですが、Apollo Client は内部で Link チェーンというものを作って具体的な HTTP リクエストを構築します。
この構築処理を実装するものが Link オブジェクトであり、具体的には [ApolloLink](https://github.com/apollographql/apollo-client/blob/main/src/link/core/ApolloLink.ts) クラスのインスタンスです。
Link オブジェクトは図のように Link チェーンの形で連結して使用できるようになっており、__通信内容を加工する一連のミドルウェア__ として機能します。
Link オブジェクトの用途としては、次のようなものがあります。

- HTTP リクエストヘッダの付加
  - 認証情報の付加（`authorization` ヘッダー）
  - GraphQL サーバーで独自定義したヘッダー
- アクセス先 URL（GraphQL エンドポイント）の切り替え
- デバッグログの出力
  - 自動リトライ（参考: [RetryLink](https://www.apollographql.com/docs/react/api/link/apollo-link-retry)）
  - リクエスト回数をカウント
  - サーバー応答速度（ラウンドトリップ時間）の計測
- 複数のクエリのバッチ送信（参考: [BatchHttpLink](https://www.apollographql.com/docs/react/api/link/apollo-link-batch-http)）
- Persisted クエリ（参考: [PersistedQueryLink](https://www.apollographql.com/docs/react/api/link/persisted-queries)）

Link オブジェクトは任意の順序でチェーンできますが、末端の GraphQL サーバーに接続する部分は Teminating link として振る舞う必要があり、通常は [HttpLink](https://www.apollographql.com/docs/react/api/link/apollo-link-http/) クラスのインスタンスを設定します。
`HttpLink` クラスは `ApolloLink` を継承して実装されています。


ApolloClient に ApolloLink を設定する
----

`ApolloClient` コンストラクタに渡すオブジェクトの __`link`__ プロパティで、任意の `ApolloLink` インスタンスを設定できるようになっています。
下記は、GraphQL API のエンドポイントやリクエストヘッダーを設定する簡単な `HttpLink` を生成して設定する例です。

{{< code lang="ts" title="Link チェーンに HttpLink を設定" >}}
import { ApolloClient, HttpLink, InMemoryCache } from '@apollo/client'

// Creates a GraphQL client
const httpLink = new HttpLink({
  uri: '/graphql', // GraphQL エンドポイント (default: "/graphql")
  // headers: （任意の HTTP リクエストヘッダー）
  // fetch: （任意の fetch 関数）
})

const apolloClient = new ApolloClient({
  link: httpLink,
  cache: new InMemoryCache(),
})
{{< /code >}}

実は、GraphQL エンドポイントを表す `uri` プロパティや、ヘッダーを設定する `headers` プロパティは、`ApolloClient` のコンストラクタで直接指定できるようになっています。
単純な GraphQL リクエストであれば、`link` プロパティを設定せずに、これらのプロパティだけで済ませることができます。

```ts
const apolloClient = new ApolloClient({
  uri: 'http://localhost:8080/graphql',
  headers: {Authorization: 'Bearer abc123'},
  cache: new InMemoryCache(),
})
```

ただ、複数の処理を組み合わせた Link チェーンを構築したいときは、前述の `link` プロパティの設定が必要です。


Link チェーンを構築する
----

複数の `ApolloLink` インスタンスを連結することによって Link チェーンを作成することができます。
`ApolloLink` を連結して Link チェーンを構築するには、次のようなメソッド（および関数）を使用します。

- __`ApolloLink#concat(link)`__ ... 既存の `ApolloLink` の後ろに別の `ApolloLink` を連結した `ApolloLink` を作る。
- __`from([link1, link2, link3])`__ ... 複数の `ApolloLink` を連結した `ApolloLink` を作る。

連結後の Link チェーンも、`ApolloLink` インスタンスとして表現されるので、そのまま `ApolloClient` の `link` プロパティにセットできます。
次のコードでは、3 つの `ApolloLink` インスタンスを連結して作った Link チェーンを `ApolloClient` に設定しています。

```ts
import { ApolloClient, ApolloLink, from, HttpLink, InMemoryCache } from '@apollo/client'

// 1 つ目の Link オブジェクト（実装は空っぽ）
const link1 = new ApolloLink((operation, forward) => {
  return forward(operation)
})

// 2 つ目の Link オブジェクト（実装は空っぽ）
const link2 = new ApolloLink((operation, forward) => {
  return forward(operation)
})

// 3 つ目の Link オブジェクト（末端の teminating link）
const httpLink = new HttpLink({
  uri: 'http://localhost:4000/graphql',
})

// 上記 3 つの Link オブジェクトを連結して Link チェーンを作成
const linkChain = from([link1, link2, httpLink])
// 次のようにも記述できるけど、3 つ以上連結する場合は from を使った方が楽
// const linkChain = link1.concat(link2).concat(httpLink)

const apolloClient = new ApolloClient({
  link: linkChain,
  cache: new InMemoryCache(),
})
```

いずれの方法で `ApolloLink` を連結する場合も、末尾に terminating link となる `HttpLink` を配置しなければいけないことに注意してください。


独自の ApolloLink を実装する
----

Apollo Client ライブラリは、いくつかの `ApolloLink` 実装を提供しています (`HttpLink`、`RetryLink`、`PersistedQueryLink`）が、次のような方法で独自の `ApolloLink` インスタンスを作成できます。

- `new ApolloLink(request?: RequestHandler)` コンストラクタでハンドラー関数を渡して `ApolloLink` インスタンスを生成する。
- `ApolloLink` を継承した独自クラスで `request` メソッドを実装して、独自クラスのインスタンスを生成する。

前者のように、直接 `ApolloLink` インスタンスを生成する方法を使うとシンプルなコードになりますが、`ApolloLink` 実装をモジュール化しておきたいときは、後者の方法でサブクラス化しておくのがよさそうです（`new PuniPuniLink()` のように分かりやすい名前で使えるようになる）。

### ApolloLink コンストラクタを使う方法

`ApolloLink` コンストラクタで直接 `ApolloLink` インスタンスを生成するときは、引数としてハンドラー関数 (`RequestHander`) を渡します。
この中で任意の処理を行い、最後に `forward(operation)` を呼び出すことで、次の Link オブジェクトに処理を引き継ぎます。

```ts
const dumpOperationNameLink = new ApolloLink((operation, forward) => {
  // ここで任意の処理を行う
  console.log(`operationName = ${operation.operationName}`)

  // 次の Link オブジェクトへ処理をつなぐ
  return forward(operation)
})
```

ハンドラー関数のパラメーターとして渡されてくる [operation](https://www.apollographql.com/docs/react/api/link/introduction/#the-operation-object) オブジェクトには、その名の通り GraphQL 操作 (operation) に関する情報、例えば、操作名 (`operationName`) や、クエリ文字列 (`query`) が含まれています。
上記の例では、GraphQL クエリに付けた操作名をコンソールに出力しています。

### ApolloLink を継承したクラスを作成する方法

独自の Link クラスを作りたいときは、`ApolloLink` を継承して __`request`__ メソッドを実装します。
実装方法は、前述のハンドラー関数と同じです。

```ts
class DumpOperationNameLink extends ApolloLink {
  request(operation: Operation, forward: NextLink): Observable<FetchResult> {
    console.log(`operationName = ${operation.operationName}`)
    return forward(operation)
  }
}

// 使用例
const linkChain = new DumpOperationNameLink().concat(httpLink)
```


ApolloLink の実装サンプル
----

よくありそうな独自 `ApolloLink` の実装例を載せておきます。

### 例: GraphQL クエリの内容を出力する

GraphQL クエリを実行するたびに、クエリの内容をコンソールに出力します。
操作名や要求しているフィールド、変数などの情報を確認できます。

```ts
const dumpQueryLink = new ApolloLink((operation, forward) => {
  const jsonText = JSON.stringify(operation, undefined, 2)
  console.log(jsonText)
  return forward(operation)
})
```

ただ、こういった情報は、Web ブラウザーの開発コンソール (`F12`) で見たほうが早いですね (^^;

### 例: GraphQL クエリの呼び出し回数をカウントする

Link オブジェクトにインスタンス変数を持たせることで、複数の GraphQL クエリにまたがって状態を保持することができます。
このような Link オブジェクトを __stateful link__ と呼びます。
次の `OperationCounterLink` クラスは、GraphQL API の呼び出し回数を操作名ごとにカウントします。
GraphQL クエリで操作名が指定されていないときは、`ANONYMOUS_OPERATION` という操作名でカウントします。

```ts
/**
 * GraphQL の操作 (operation name) ごとに呼び出し回数を保持し、
 * コンソールに出力する ApolloLink 実装です。
 */
class OperationCounterLink extends ApolloLink {
  private counters: { [operationName: string]: number }

  constructor() {
    super()
    this.counters = {}
  }

  request(operation: Operation, forward: NextLink): Observable<FetchResult> {
    this.incCount(operation.operationName || 'ANONYMOUS_OPERATION')
    console.log(JSON.stringify(this.counters, undefined, 2))
    return forward(operation)
  }

  private incCount(operationName: string) {
    let prev = this.counters[operationName]
    if (prev == undefined) prev = 0
    this.counters[operationName] = prev + 1
  }
}
```

この Link オブジェクトを `ApolloClient` の Link チェーンに設定しておくと、GraphQL クエリの呼び出しごとに、ブラウザのコンソールに各 GraphQL 操作の呼び出し回数が出力されるようになります。
無駄なクエリが実行されていないか調査するときに便利です。

{{< code lang="json" title="出力例" >}}
{
  "QueryBook": 2,
  "QueryBooks": 17
}
{{< /code >}}

### 例: GraphQL サーバーの応答時間を計測する

ハンドラー関数に渡される `operation` オブジェクトの __`setContext`__ を使って、コンテキスト情報を設定することができます。
コンテキスト情報というのは、Link チェーンの中で共有されるグローバル変数のようなもので、設定されたコンテキスト情報は __`getContext`__ で参照することができます。
次の例では、リクエスト時に `start` という名前のプロパティで現在時刻を保存し、レスポンス時に現在時刻との差分を取ることで、GraphQL サーバーの応答時間（ラウンドトリップタイム）を計算しています。

```ts
const roundTripLink = new ApolloLink((operation, forward) => {
  // リクエスト時に現在時刻をコンテキストに保存
  operation.setContext({ start: new Date().getMilliseconds() })

  // レスポンス時にラウンドトリップタイムを出力
  return forward(operation).map((data) => {
    const time = new Date().getMilliseconds() - operation.getContext().start
    console.log(
      `Operation ${operation.operationName} took ${time} ms to complete`
    )
    return data
  })
})
```

Link オブジェクトでは、GraphQL API のリクエスト時の割り込み処理だけでなく、__レスポンス時の処理__ も記述することができます。
レスポンス時の処理を定義するには、ハンドラー関数の戻り値で `forward(opeartion)` をそのまま返すのではなく、上記のように __`map`__ 関数で処理内容を追加します。

{{< code title="出力例" >}}
Operation QueryBooks took 7 ms to complete
{{< /code >}}


### 例: 認証用のリクエストヘッダーを付加する

`operation` オブジェクトのコンテキスト情報の __`headers`__ プロパティには、HTTP リクエストとして送られる（予定の）ヘッダー情報が含まれています。
Link オブジェクトの実装でこのプロパティを書き換えることで、任意の HTTP リクエストヘッダーを付加できます。
通常のコンテキスト情報は Link チェーン内で使われるものなので、GraphQL サーバーに送られることはありませんが、この `headers` プロパティの値は `HttpLink` によって GraphQL サーバーに送られます。

次の Link オブジェクトでは、認証用の `authorization` ヘッダーを付加しています。
Link チェーン内で設定されたヘッダー情報が上書きされないように、ヘッダー情報をマージしていることに注意してください。

```ts
const authLink = new ApolloLink((operation, forward) => {
  // Get the access token from local storage if it exists
  const token = localStorage.getItem('ACCESS_TOKEN')

  // Return the headers to the context so httpLink can read them
  operation.setContext({
    headers: {
      ...operation.getContext().headers,
      authorization: token ? `Bearer ${token}` : '',
    },
  })

  return forward(operation)
})
```

実は、このように Link オブジェクト内でコンテキスト情報を付加するだけの場合は、簡単に実装するためのユーティリティ関数 ([setContext](https://www.apollographql.com/docs/react/api/link/apollo-link-context)) が用意されています。
上記と同等の `ApolloLink` インスタンスは、次のように生成することができます。

```ts
// import { setContext } from '@apollo/client/link/context'

const authLink = setContext((_, prevContext) => {
  const token = localStorage.getItem('ACCESS_TOKEN')
  const { headers } = prevContext

  return {
    headers: {
      ...headers,
      authorization: token ? `Bearer ${token}` : '',
    },
  }
})
```

非同期関数の結果を利用してコンテキスト情報をセットしたいときは、`setContext` に渡す関数で `Promise` オブジェクトを返すようにします。

```ts
// アクセストークンを非同期で取得する関数（1 秒待つフェイク実装）
async function getToken(): Promise<string> {
  return new Promise<string>((resolve) => {
    setTimeout(() => { resolve('FAKE_TOKEN') }, 1000)
  })
}

const authLink = setContext(async (_, prevContext) => {
  const token = await getToken()  // 非同期関数を呼ぶことが可能
  const { headers } = prevContext
  return {
    headers: { ...headers, authorization: `Bearer ${token}` },
  }
})
```

ただし、このような実装にすると、リクエスト毎に非同期関数が呼び出されることに注意してください。

### 例: 接続先の GraphQL API エンドポイントを動的に切り替える

`HttpLink` に設定する `HttpOptions` オブジェクト（あるいは `ApolloClient` に設定する `ApolloClientOptions` オブジェクト）の __`uri`__ プロパティには、GraphQL API エンドポイントの URL を指定するのですが、ここには単純な文字列 (`string`) の代わりに、クエリ実行時に呼び出される __`UrlFunction`__ を設定できるようになっています。

```js
// エンドポイントの URI を動的に構築するための関数（のインタフェース）
export interface UriFunction {
  (operation: Operation): string;
}

export interface Operation {
  query: DocumentNode;
  variables: Record<string, any>;
  operationName: string;
  extensions: Record<string, any>;
  setContext: (context: Record<string, any>) => Record<string, any>;
  getContext: () => Record<string, any>;
}
```

この仕組みを利用すると、例えば、GraphQL API エンドポイントをユーザー設定（Web ブラウザーの localStorage）の値によって切り替えたり、GraphQL クエリの内容（`Operation` パラメーター）によって切り替えたりすることができます。

```ts
// 動的に GraphQL API エンドポイントを決定する関数
const getEndpointUrl: UriFunction = () => {
  // 通常、エンドポイント情報は localStorage などで管理するが、ここではランダムに切り替え
  return Math.random() > 0.5 ? 'http://localhost:3000/graphql' : 'https://example.com/graphql'
}

// 上記の UriFunction 実装の参照を、ApolloClient か HttpLink の uri プロパティで指定すれば OK
const apolloClient = new ApolloClient({
  uri: getEndPointUri,  // 関数の参照を設定することに注意（関数を呼び出さないこと）
  cache: new InMemoryCache(),
})
```

GraphQL API で取得したデータは、Apollo Client がキャッシュしています（`InMemoryCache` など）。
これは、コンポーネント再描画時の余計な再取得を防ぐための仕組みですが、GraphQL API のエンドポイントを切り替えた場合は、過去に保存したキャッシュはゴミになっている可能性が高いです。
エンドポイントの切り替え後は、次のような感じで `ApolloClient` の参照を取得してキャッシュをクリアしてしまうのがよいでしょう。

```ts
import { useApolloClient } from '@apollo/client'

// コンポーネントの中で ApolloClient インスタンスを取得
const apolloClient = useApolloClient()

// エンドポイント切り替えのタイミングでキャッシュをクリア
apolloClient.cache.reset()
```

