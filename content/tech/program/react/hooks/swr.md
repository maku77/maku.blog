---
title: "React フック: useSWR でデータフェッチ"
url: "/p/vm2ft83"
date: "2021-06-22"
tags: ["React"]
weight: 201
---

SWR とは
----

### useSWR フック

SWR (`useSWR`) は、Next.js を開発している人たち (Vercel) が開発したデータフェッチ用の React フックライブラリです。
React アプリでデータフェッチを真面目に実装しようとすると、大体最後にこのライブラリに行き着きますので、最初からこれを使いましょう（GraphQL の場合は Apollo Client がありますが）。

- [SWR - React Hooks library for data fetching](https://swr.vercel.app/)

React コンポーネント内から Web API などを呼び出してデータフェッチを行う場合、標準の仕組みだけで実現しようとすると、`useEffect` フックなどで `fetch` 関数を呼び出したりすることになります。

{{< code lang="tsx" title="useEffect による不完全なデータフェッチ実装" >}}
function Page () {
  const [user, setUser] = useState(null)

  useEffect(() => {
    fetch('/api/user')
      .then(res => res.json())
      .then(data => setUser(data))
  }, [])

  // ...
}
{{< /code >}}

もちろん、それで実現は可能なのですが、「データ取得前の表示」「エラー処理/リトライ処理」「非同期処理のキャンセル処理」などをちゃんとやろうとすると非常に煩雑なコードになってきます（上記コードはそれらが全く考慮できていません）。
`useSWR` フックを使うと、クライアントサイド JavaScript からのデータフェッチ処理をとても綺麗に記述することができます。

{{< code lang="tsx" title="useSWR の使用例" >}}
import { FC } from 'react'
import useSWR from 'swr'

const fetcher = (url: string) => fetch(url).then((r) => r.json())

const Profile: FC = () => {
  const { data, error } = useSWR('/api/user', fetcher)

  if (error) return <div>Failed to load</div>
  if (!data) return <div>Loading...</div>
  return <div>Hello {data.name}!</div>
}

export default FC
{{< /code >}}

### useSWR の特徴

`useSWR` は高度なキャッシュコントロール機能を備えています。

キー名によるキャッシュ共有
: 第1パラメータのキー名を共通にするだけで、ページ間でキャッシュを共有できます。
`useEffect` などでデータフェッチを行おうとすると、無駄なフェッチを防ぐために、トップレベルのコンポーネントで一度だけデータ取得して子コンポーネントにたらい回ししていく、という実装になりがちです。
`useSWR` であれば、各コンポーネントで個別にデータフェッチする感覚で呼び出せます。

自動リトライ、ポーリング
: エラー発生時のリトライ処理 ([exponential backoff アルゴリズム](https://en.wikipedia.org/wiki/Exponential_backoff))、ネットワーク復帰時の自動フェッチ (`{ revalidateOnReconnect: true }`)、ブラウザのタブ切り替え時の自動フェッチ (`{ revalidateOnFocus: true }`) などをデフォルトで行ってくれるため、ユーザーが最新データにアクセスしやすくなります。これらの設定は `useSWR` 呼び出し時のオプションで制御できます（参考: [useSWR のオプション](https://swr.vercel.app/ja/docs/options#%E3%82%AA%E3%83%97%E3%82%B7%E3%83%A7%E3%83%B3)）。

手動でのキャッシュ更新（ミューテート）
: `useSWR` フックは、任意のタイミングでキャッシュ更新をかけるためのミューテート関数 (`mutate`) を提供してくれます。キャッシュが更新されると、自動的にコンポーネントは再描画されます。また、グローバルな `mutate` API も提供されており、指定したキーのキャッシュをどこからでも更新できます。

フェッチ関数は自由
: `useSWR` フックは、デフォルトでフェッチ関数として `fetch` を使用（JSONデータを想定）しますが、`Promise` を返す任意のフェッチ関数を使用することができます。

TypeScript、Next.js 対応
: Vercel 製なので、もちろん Next.js に対応していて、プリレンダリング時にも呼び出せます。Web サイトビルド時にはその時点でのデータで静的なページを生成しておいて、実行時は最新データをフェッチ＆キャッシュするといったことが可能です。TypeScript の型情報も標準でサポートしています。


（コラム） SWR = stale-while-revalidate
----

SWR という名前の由来は、[RFC5861](https://datatracker.ietf.org/doc/html/rfc5861) で提唱されている HTTP レスポンスヘッダの `Cache-Control` 拡張のひとつである __stale-while-revalidate__ から来ています。

`Cache-Control` はデータフェッチ後のキャッシュ有効期間を示すために導入されたもので、例えば、`Cache-Control: max-age=600` が返された場合は、ブラウザは 600 秒間キャッシュを保持します（その間のデータフェッチ要求ではサーバーアクセスしない）。
そして、600 秒経過するとキャッシュは古い (stale) とみなされて、次のデータフェッチ時には実際にサーバーアクセスが発生します。
このとき、データフェッチ処理が同期的に行われるため、ユーザーはデータ表示まで少し待たされることになります。
この待ち時間を軽減しようというのが stale-while-revalidate (SWR) という拡張で、例えば、

{{< code >}}
Cache-Control: max-age=600, stale-while-revalidate=300
{{< /code >}}

というレスポンスが返されると、`max-age` の 600 秒経過後も、`stale-while-revalidate` で指定された 300 秒間は、古いキャッシュ (stale cache) がブラウザでの描画に使われます。
そして、その背後でサーバーに対してデータフェッチを行うことで、キャッシュの更新も同時に行います。
この仕組みにより、（多少古いかもしれない）キャッシュでとりあえず高速に初期表示しつつ、常に最新のデータをキャッシュしておく、ということが実現できます。

React ライブラリの `useSWR` フックもこの思想をもとに実装されており、ブラウザ側のキャッシュを使って高速な表示をしつつ、背後でキャッシュの更新を行って必要に応じて再描画を行う、といったことを可能にしています。
`useSWR` フックのすごいところは、この辺りの複雑なキャッシュコントロールを意識せずに、シンプルにデータフェッチ処理を記述できるところにあります。


SWR のインストール
----

SWR フックライブラリは `yarn` あるいは `npm` で簡単にインストールできます。

{{< code >}}
### yarn の場合
$ yarn add swr

### npm の場合
$ npm install swr
{{< /code >}}

これで、次のように `useSWR` 関数をインポートできるようになります。

{{< code lang="ts" >}}
import useSWR from 'swr'
{{< /code >}}


useSWR 関数の使い方
----

### 基本的な使い方

下記のコンポーネントは、GitHub のユーザー情報を REST API を使って取得＆表示します。

{{< code lang="tsx" title="src/components/GitHubUser.tsx" >}}
import { FC } from 'react'
import useSWR from 'swr'

const fetcher = (url: string) => fetch(url).then((r) => r.json())
type Props = { login: string }  // コンポーネントの props の型
type Data = { name: string }  // フェッチするデータの型

export const GitHubUser: FC<Props> = ({ login }: Props) => {
  const url = `https://api.github.com/users/${login}`
  const { data, error } = useSWR<Data, Error>(url, fetcher)

  if (error) return <div>Failed to load</div>
  if (!data) return <div>Loading...</div>
  return <div>Name: {data.name}</div>
}

// 使用例: <GitHubUser login="octocat" />
{{< /code >}}

`useSWR` 関数の第1パラメーターは、フェッチ関数に渡すパラメーターであり、キャッシュを識別するためのキーとしても使われます。
典型的には Web API の URL を渡しますが、配列の形で付加データを渡すことも可能です。

戻り値の `data` プロパティには、フェッチ関数によって取得したデータ（`Promise.resolve()` された値）が格納されますが、データ取得が完了するまでは `data` の値は `undefined` になります。
なので、ロード中であるかどうかを `if (!data)` で簡単に判別できます（`isLoading` とか存在しません）。
`error` プロパティも同様で、エラーが発生していなければ `undefined` になり、エラーが発生したらエラー内容（`Promise.reject()` された値）が格納されます。

### フェッチ関数を指定する

`useSWR` の第2パラメーターでは、データフェッチに使用するフェッチ関数を指定することができます。
下記は、JSON データを返す Web API を呼び出すときに使う典型的なフェッチ関数の使用例です。

{{< code lang="ts" hl_lines="1" >}}
const fetcher = (url: string) => fetch(url).then((r) => r.json())

export const Hello: FC = () => {
    // ...
    const { data, error } = useSWR<Data, Error>(url, fetcher)
    // ...
}
{{< /code >}}

フェッチ関数と言うと特殊なもののように聞こえますが、単なる getter 関数であり、`Promise<Xxx>` を返す非同期関数であれば、そのままフェッチ関数として使えると考えればよいです。
次の例では、`fetchBookData` 関数をフェッチ関数として使用しています。
この `fetchBookData` 関数はそれ単独でも使えるものですが、それをそのまま `useSWR` に渡すことができます。

{{< code lang="tsx" title="src/components/BookList.tsx" >}}
import useSWR from 'swr'

type BookData = {
  author: string
  titles: string[]
}

async function fetchBookData(author: string): Promise<BookData> {
  try {
    // ...本当はここで fetch 処理など
    const bookData = {
      author: author,
      titles: ['Title1', 'Title2', 'Title3'],
    }
    return bookData
  } catch (err) {
    // Error を再スローするだけなら、通常は try ... catch は必要ない
    throw err
  }
}

const BookList: React.FC = () => {
  const { data: bookData, error } = useSWR<BookData, Error>('Author1', fetchBookData)
  if (error) return <p>Error: {error.message}</p>
  if (!bookData) return <p>Loading...</p>
  return (
    <ul>
      {bookData.titles.map((title) => (
        <li key={title}>
          {bookData.author} - {title}
        </li>
      ))}
    </ul>
  )
}
{{< /code >}}

パラメーターが 2 つ以上の関数をフェッチ関数として使いたい場合は、`useSWR` の第1パラメータ―に引数を配列でまとめて渡します。
フェッチ関数側に配列が渡されるわけではないことに注意してください。

{{< code lang="ts" >}}
// パラメーターが 2 つ必要なフェッチ関数
async function fetchData(key1: string, key2: string): Promise<Data> {
  // ...
}

// useSWR 経由で上記関数を呼び出す
export const MyComponent: FC = () => {
  const { data, error } = useSWR<Data, Error>(['aaa', 'bbb'], fetchData)
  // ...
}
{{< /code >}}

フェッチ関数の仕組みは非常に柔軟で、時間がかかるデータフェッチをシミュレートしたり、エラーが発生したときにどう見えるかを簡単に確認できます。
例えば次のフェッチ関数は 1 秒後に現在時刻を返すか、エラーを発生させます。
通常、フェッチ関数のパラメーターとして、`useSWR` の第 1 引数で渡された値を受け取りますが、このフェッチ関数は特殊でパラメーターなしです。

{{< code lang="ts" >}}
const fetchCurrentTime = async () => {
  // 1秒待つ
  await new Promise((res) => setTimeout(res, 1000))

  // 現在時刻の文字列を返すか、エラーを発生させる
  if (Math.random() > 0.3) {
    return new Date().toLocaleString()
  } else {
    throw new Error('An error has occurred!')
  }
}

// 使用例
const { data, error } = useSWR<string, Error>('/fake', fetchCurrentTime)
{{< /code >}}

フェッチ関数の中でスローした Error オブジェクトは、`useSWR` 関数の戻り値オブジェクトの `error` プロパティにされます（エラーがしなかった場合は `undefined`）。

### フェッチ関数を省略する

[SWRConfig コンポーネント](https://swr.vercel.app/ja/docs/global-configuration) を使用すると、子コンポーネント内で `useSWR` を使用するときの共通設定（グローバル設定）を行うことができます。
具体的には、`useSWR` の第 3 引数に渡すオプションオブジェクトのデフォルト値として扱われます。
このオブジェクトには、次のようにのフェッチ関数 (`fetcher`) を指定しておくことができます。

{{< code lang="ts" title="pages/_app.tsx" >}}
import type { AppProps } from 'next/app'
import { SWRConfig } from 'swr'

/** Default fetcher for useSWR hook. */
const fetcher = (url: string) => fetch(url).then((r) => r.json())

export default function MyApp({ Component, pageProps }: AppProps): JSX.Element {
  return (
    <SWRConfig value={{ fetcher }}>
      <Component {...pageProps} />
    </SWRConfig>
  )
}
{{< /code >}}

これで、子コンポーネントから `useSWR` 関数を呼び出すときにフェッチ関数を省略できるようになります。

{{< code lang="ts" >}}
const { data: projects } = useSWR('/api/projects')
{{< /code >}}


useSWR はカスタムフックにラップして使う
----

`useSWR` 特有の話ではないですが、データフェッチ処理に関してはコンポーネントの実装から分離して、カスタムフックの形で定義しておくと保守性の高いコードになります。
次の `useIssueCount` カスタムフックは、指定した GitHub リポジトリ (`$org/$repo`) のイシュー数を取得します。

{{< code lang="ts" title="src/hooks/useIssueCount.ts" >}}
import useSWR from 'swr'

type UseIssueCountInput = {
  org: string
  repo: string
}

type UseIssueCountOutput = {
  count?: number
  error?: Error
  isLoading: boolean
}

const fetcher = (url: string) => fetch(url).then((r) => r.json())

export const useIssueCount = (
  input: UseIssueCountInput
): UseIssueCountOutput => {
  const { org, repo } = input
  const url = `https://api.github.com/repos/${org}/${repo}/issues`
  const { data, error } = useSWR<[], Error>(url, fetcher)

  return {
    count: data?.length,
    error,
    isLoading: !error && !data,
  }
}
{{< /code >}}

そして描画を担うコンポーネント側の実装では、データ取得処理はカスタムフックの呼び出しだけで済ませます。
カスタムフックを呼び出している時点で、データ取得処理が完全に分離できているとは言えませんが、こうすることで「見た目」の定義に集中できます。

{{< code lang="tsx" title="src/components/IssueCount.tsx（使用例）" >}}
import { FC } from 'react'
import { useIssueCount } from '../hooks/useIssueCount'

export const IssueCount: FC = () => {
  const { count, error, isLoading } = useIssueCount({
    org: 'vercel',
    repo: 'swr',
  })

  if (error) return <div>Error</div>
  if (isLoading) return <div>Loading...</div>
  return <div>Count: {count}</div>
}
{{< /code >}}

ちなみに、さらに「見た目」と「ロジック」を分離する手法に、Presentational component / Container component という方法があります。
これは、React コンポーネントを「見た目」用と「ロジック」用の 2 種類に分けて実装する考え方です。

- Presentational component ... 純粋な UI コンポーネント。渡された `props` をそのまま表示するだけ。
- Cotainer component ... ロジック（フックなどの呼び出し）を含むコンポーネント。内部でのデータ取得結果などを Presentational component に `props` で渡す。


応用
----

### フェッチ関数に追加のデータを渡す

API 呼び出し時に、フェッチ関数にトークン情報を渡したいことがあるかもしれません。
そのような場合は、`useSWR` 関数のキーパラメータを配列にすることで、トークン情報が変化した時に再フェッチさせることができます。

{{< code lang="ts" >}}
const { data: user } = useSWR(['/api/user', token], fetchWithToken)
{{< /code >}}

フェッチ関数 `fetchWithToken` は、2 つのパラメーター（上の例であれば、`/api/user` と `token`）を受け取り、`Promise<Xxx>` を返すように実装します。
パラメーターとして配列を受け取るわけではないことに注意してください（配列の要素が個別パラメーターとして渡されてきます）。

{{< code lang="ts" >}}
type Output = { user: string }

async function fetchWithToken(url: string, token: string): Promise<Output> {
  // ...
  return { user: 'Jojo' }
}
{{< /code >}}

### 依存関係のある useSWR の連続呼び出し

`useSWR` 関数の第1パラメーターが偽とみなされる値 (falsy) 場合、フェッチ処理はスキップされます。
次の例では、この性質を利用して、1番目の `useSWR` の結果を取得した段階で、2番目の `useSWR` によるデータフェッチを発火させています。
それまでは、1番目の `useSWR` の結果のみを使ってコンポーネントが描画されます。

{{< code lang="ts" >}}
const { data: user } = useSWR(['/api/user', token], fetchWithToken)
const { data: orders } = useSWR(user ? ['/api/orders', user] : null, fetchWithUser)
{{< /code >}}

ちなみに、`useSWR` は React フックなので、データフェッチが必要ないケースでも上記のように必ず呼び出す必要があります（条件分岐して呼び出し自体をスキップしてはいけません）。

### フェッチ関数から独自のエラーを投げる

`useSWR` 関数の第 2 引数で指定した独自のフェッチ関数からエラーオブジェクトを throw すると、`useSWR` 関数の戻り値オブジェクトの `error` プロパティとして受け取ることができます。
`useSWR` 関数を使用する箇所で、データフェッチに関する細かいエラー処理を行いたいのであれば、フェッチ関数の中で詳細情報を詰めたエラーオブジェクトを `throw` すれば OK です。

{{< code lang="ts" >}}
// 独自のエラークラスの定義例
export class GameError extends Error {
  errorId: string  // 独自のエラー ID
  statusCode: number  // HTTP ステータスコード

  constructor(errorId: string, statusCode: number, message?: string) {
    // Error オブジェクト用の設定
    super(message ?? `Error ${errorId} occurred.`)
    this.name = new.target.name

    // 独自のエラー情報
    this.errorId = errorId
    this.statusCode = statusCode
  }
}

// 独自のフェッチ関数
export async function fetchGame(url: string): Promise<Game> {
  const res = await fetch(url)
  if (!res.ok) {
    // ステータスコードが 2xx 以外の場合は、独自エラーを投げる
    throw new GameError('FETCH_GAME_ERROR', res.status)
  }
  return (await res.json()) as Game
}
{{< /code >}}

`useSWR` を呼び出すときは、第 2 型パラメーターでエラーオブジェクトの型を指定できます。

{{< code lang="ts" >}}
const { data, error } = useSWR<Game, GameError>(url, fetchGame)
if (error) {
  console.error(error.errorId)  // FETCH_GAME_ERROR
  console.error(error.statusCode)  // 404 など
  console.error(error.message)  // Error FETCH_GAME_ERROR occurred.
}
{{< /code >}}

