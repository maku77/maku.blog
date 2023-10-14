---
title: "Next.js でサーバーサイドで JSON や YAML ファイルを読み込む (fs.readFileSync)"
linkTitle: "サーバーサイドで JSON や YAML ファイルを読み込む (fs.readFileSync)"
url: "/p/iz8fnu3"
date: "2021-05-09"
tags: ["Next.js"]
---

何をするか？
----

Next.js アプリのページコンポーネントの `getStaticProps` / `getServerSideProps` 関数や、API ルート (`pages/api/*.ts`) の `handler` 関数は、クライアントからのアクセス時やビルド時に、サーバーサイドで呼び出されます。
つまり、これらの関数の中では、Node.js の `fs` モジュールを使った（サーバー上の）ローカルファイルの読み込みが可能です。

ここでは、例として、

1. ページコンポーネントの `getStaticProps` 関数から JSON ファイルを読み込む方法
2. API ルートの `handler` 関数から YAML ファイルを読み込む方法

を紹介します。


JSON ファイルを読み込む（in getStaticProps 関数）
----

### 使用する JSON ファイル

サンプルデータとして次のような JSON ファイルをプロジェクト内に配置します。

{{< code lang="json" title="src/data/games.json" >}}
[
  { "id": "dq1", "title": "ドラゴンクエスト" },
  { "id": "dq2", "title": "ドラゴンクエスト2" },
  { "id": "dq3", "title": "ドラゴンクエスト3" }
]
{{< / code >}}

### JSON ファイルを読み込む

次のコードでは、`getStaticProps` 関数の中で `src/data/games.json` ファイルを読み込んで、ページコンポーネントに渡す `props` データを作成しています。

{{< code lang="tsx" title="src/pages/games.tsx" >}}
import * as fs from 'fs'
import * as path from 'path'
import { GetStaticProps, NextPage } from 'next'

type Game = { id: string, title: string }
type PageProps = { games: Game[] }

export const getStaticProps: GetStaticProps<PageProps> = async (context) => {
  // JSON ファイルを読み込む
  const jsonPath = path.join(process.cwd(), 'src', 'data', 'games.json')
  const jsonText = fs.readFileSync(jsonPath, 'utf-8')
  const games = JSON.parse(jsonText) as Game[]

  // ページコンポーネントに渡す props オブジェクトを設定する
  return { props: { games } }
}

const GamesPage: NextPage<PageProps> = ({ games }: PageProps) => {
  // ...
}

export default GamesPage
{{< /code >}}

JSON 形式のテキストは、JavaScript 標準の `JSON.parse` 関数でオブジェクトに変換することができるので、プログラム内での扱いはお手軽です。

ポイントは、JSON ファイルの絶対パスを構築している部分です。

{{< code lang="ts" >}}
const jsonPath = path.join(process.cwd(), 'src', 'data', 'games.json')
{{< /code >}}

一般的な Node.js プログラムで相対パスを扱うときは `__dirname` を使用することがあると思いますが、Next.js サーバー上で `__dirname` を参照すると、自動生成された `.next` ディレクトリ以下のパスが返されてしまってうまくいきまん。
代わりに、__`process.cwd()`__ でカレントディレクトリ（通常はプロジェクトのルートディレクトリ）のパスを取得して YAML ファイルのパスを構築するようにします

{{% maku-common/reference %}}
- [Reading files: Use process.cwd() - Next.js](https://nextjs.org/docs/basic-features/data-fetching#reading-files-use-processcwd)
{{% /maku-common/reference %}}


YAML ファイルを読み込む（in API ルート）
----

次に、YAML ファイルを API ルートの `handler` 関数から読み込んでみます。
基本的に JSON ファイルを扱うのと同様に、`fs.readFileSync` 関数を使うだけですが、Node.js は標準では YAML テキストをパースできないので、[js-yaml](https://github.com/nodeca/js-yaml) パッケージをインストールしておきます。

### js-yaml のインストール

`js-yaml` パッケージと、TypeScript 用の型情報 (`@types/js-yaml`) をインストールします。

{{< code >}}
$ npm install js-yaml
$ npm install @types/js-yaml --save-dev
{{< /code >}}

### 使用する YAML ファイル

サンプルデータとして次のような YAML ファイルをプロジェクト内に配置します。

{{< code lang="yaml" title="src/data/books.yaml" >}}
-
  title: Title 1
  author: Author 1
-
  title: Title 2
  author: Author 2
-
  title: Title 3
  author: Author 3
{{< /code >}}

### YAML ファイルを読み込む

この YAML ファイルを、API ルートの実装 (`pages/api/books.ts`) から読み込むには次のようにします。

{{< code lang="ts" title="src/pages/api/books.ts" >}}
import * as fs from 'fs'
import * as path from 'path'
import * as yaml from 'js-yaml'
import type { NextApiRequest, NextApiResponse } from 'next'

export default (req: NextApiRequest, res: NextApiResponse) => {
  // プロジェクトルートからの相対パスで YAML ファイルのパスを解決する
  const yamlPath = path.join(process.cwd(), 'src', 'data', 'books.yml')
  // YAML ファイルの内容を読み込んでそのまま JSON データとして返す
  const obj = yaml.load(fs.readFileSync(yamlPath, 'utf-8'))
  res.status(200).json(obj)
}
{{< /code >}}

### 動作確認

Next.js サーバーを起動 (`npm run dev`) して、`localhost:3000/api/books` にアクセスすると、次のような JSON テキストが得られるはずです。

{{< code lang="json" title="実際は一行" >}}
[
  {"title":"Title 1","author":"Author 1"},
  {"title":"Title 2","author":"Author 2"},
  {"title":"Title 3","author":"Author 3"}
]
{{< /code >}}

