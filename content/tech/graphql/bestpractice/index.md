---
title: "GraphQL ベストプラクティス"
url: "p/4reqy9i/"
date: "2022-10-12"
lastmod: "2022-11-21"
tags: ["GraphQL"]
weight: 1
changes:
  - 2022-11-21: コンテキストや DataLoader について
  - 2022-10-24: static なクエリドキュメントについて
---

GraphQL のスキーマ定義やクエリドキュメントを記述するときのノウハウ集です（主にスキーマ設計に関するもの）。
主要なライブラリ (Apollo や gqlgen）の設計や、{{< amazon-inline title="GraphQL in Action" id="161729568X" >}}、『Production Ready GraphQL』などの書籍を参考にしています。

クライアントファースト、スキーマファースト
----

### クライアントファースト (Client First) で考える

何よりも、クライアントのユースケースを念頭において考えます。
これは、GraphQL を採用するそもそもの目的でもあります。
バックエンドで使うデータベースや言語などの、実装詳細に引っ張られた API 設計にならないように気を付けます。

- できるだけ早い段階でクライアントとのインテグレーションを始めるようにします。組織内での開発であれば、スキーマ定義が終わった段階で __すぐにモックサーバーを用意する__ ことで、クライアント開発チームが実装を開始できます。
- __クライアントのユースケース以上の API は作らない__ ようにします。[YAGNI (You Aren't Going to Need It)](https://ja.wikipedia.org/wiki/YAGNI) の原則に従いましょう。クライアント要求のない API を提供しようとすると、混沌とした設計になりがちで、パフォーマンスの悪化やセキュリティリスクを招きます。そして、deprecated な API の管理に苦しむことになります。
- 既存のデータベースから GraphQL API を生成するツールはいろいろありますが、クライアントファーストの考えからするとナンセンスであり、多くの場合は採用すべきではありません。クライアントのニーズを満たす API にはなりませんし、バックエンドの実装詳細に依存した API になってしまいます。必要のない情報まで公開してしまうことによるセキュリティリスクなど、総合的にデメリットが多いです。こういったツールは、本当にデータベースに依存した API がマッチするケースでのみ導入すべきです。REST API 定義から GraphQL スキーマを生成するツールもありますが、そもそも両者はデザインコンセプトから異なるものであり、それでよい API ができると考えてはいけません。

### スキーマファーストで実装を進める

多くのケースでは、スキーマ定義を先に考える __スキーマファースト__ で設計を進めると、サーバーとクライアントを並行して開発することができます。
どのような API を実装すればよいか、どのような情報を取得できるのかが、早い段階で全員に明らかになるからです。

{{< image w="430" src="img-001.drawio.svg" >}}

型やフィールドの定義には [description を含める](#description) ようにすることで、実装メンバーの認識を合わせます。
サーバー開発チームに、[GraphiQL](https://github.com/graphql/graphiql) などの内部サーバーを立ててもらえば、全員が常に最新のスキーマ定義（ドキュメント）を参照できます。


ドキュメンテーション
----

### スキーマの各要素に説明文 (description) を記載する {#description}

スキーマ定義ファイル (`.graphqls`) では、型やフィールドに description とよばれるドキュメントを記述することができます。
すべての要素に description を記述することで、実装者間で認識の祖語が出ないようにします。
フォーマットは Python のドキュメンテーションコメント同様で、説明文を 3 つのクォート (`"""`) で囲みます。

```graphql
"""
Images attached to the check run output displayed in the GitHub pull request UI.
"""
input CheckRunOutputImage {
  """
  The alternative text for the image.
  """
  alt: String!

  """
  A short image description.
  """
  caption: String

  # ...
}
```

descrition は、その型が __何を表現する__ ものなのか、その mutation が __何を行う__ ものなのかが伝わるよう意識して記述します。
コードレビューでも description が簡潔かつ明確に記述できているかしっかりチェックします。
開発を進めながら、description 記述は適宜更新していくのがよいです（仕様を変えてはいけませんが）。

- 参考: [GraphQL スキーマ仕様: 型やフィールドに説明文 (description) を記述する](/p/c5pbs6p/)

### スキーマとは独立したドキュメントを用意する

GraphQL スキーマの description 機能は、型やフィールドのドキュメンテーションには便利ですが、だからといって、他の API ドキュメントが必要ないというわけではありません。
例えば、API 全体の概要説明は最初にあった方がよいですし、エンドポイントや認証などのアクセス情報はスキーマでは表現しきれないでしょう。
Rate Limit など、API 呼び出しに関する注意事項も記述が必要かもしれません。
そういった説明は、独立したドキュメントサイトで参照できるようにしておくべきです。
下記は GitHub の GraphQL API ドキュメントサイトの例です。

- 参考: [GitHub GraphQL API - GitHub Docs](https://docs.github.com/ja/graphql)


命名 (Naming)
----

プログラムの関数名と同様、GraphQL API においても名前はとても重要なので、しっかり時間をかけて考えるようにします。
よい名前は正しい使用方法を促すものであり、ドキュメントを読むことに時間を取られずに済みます。
また、よい名前付けができると、API 自身の正しい設計につながります。

### 一貫性のあるネーミング

名前付けで重要なのは一貫性 (consistency) を持たせることです。
API 名に一貫性がないと、間違った使用の危険性があるだけでなく、クライアント実装者が目的の API を見つけることすらできなくなってしまいます。

{{< code lang="graphql" title="NG" >}}
# 名詞と動詞が混ざってしまっている
type Query {
  products(ids: [ID!]): [Product!]
  findPost(ids: [ID!]): [Post!]
}

# 使用する動詞に一貫性がない（add と create）
type Mutation {
  addProduct(input: AddProductInput!): AddProductPayload
  createPost(input: CreatePostInput!): CreatePostPayload
}
{{< /code >}}

動詞の対象性にも気を付けます。
例えば、`publishPost` の反対の操作を行う mutation を定義するなら `unpublishPost` にします。

ドメインオブジェクト（そのプロジェクトに特化した型）の名前の一貫性には特に注意します。
ある場所で `BlogPost` という単語を使っているのに、同じ意味で `Post` という単語を使わないようにします。


### 型名はある程度具体的に {#concrete-type}

スキーマが小さいうちは、シンプルな型名で十分かもしれませんが、将来的にスキーマを拡張していくことを想定しているのであれば、型名はある程度具体的に付けておくべきです。
例えば、`Lesson` よりも `CourseLesson` の方が望ましいかもしれません。
型に抽象的すぎる名前を付けると、スキーマが成長していく段階で、deprecated（非推奨）マークの付いた型であふれかえることになります。

型名に安易に `Event` や `User` という名前を使うと、より具体的な `Event` 型や `User` 型を使いたくなったときに困ります。
例えば、「カレントユーザー」と「チームメンバー」を同じ `User` 型で表現してしまうと、どちらか一方でしか使わないフィールドを追加したときに、API の破壊的な変更が必要になったりします。
クライアントアプリの実装も煩雑になってしまうでしょう。
「カレントユーザー」は `Viewer` 型、「チームメンバー」は `TeamMember` 型として定義しておけば、このような心配はなくなります。
その上で、`User` インタフェースをくくり出して共通化するのは問題ありません。


### フィールド名で型名を繰り返さない

[型にはある程度具体的な名前](#concrete-type) を付けるべきなのですが、フィールド名に関しては、必ずその上位の型（やフィールド名）で修飾される形になるので、冗長な名前を付けないようにします。
例えば、次の `Book` 型に含まれている `bookDetails` フィールドの `book` というプレフィックスは冗長です。

{{< code lang="graphql" title="NG" >}}
type Book {
  # ...
  bookDetails: BookDetails
}
{{< /code >}}

次のようなシンプルなフィールド名でも、書籍の詳細情報であることは明らかです。
もちろん、実際に返すデータの型は `BookDetails` のような具体的な名前で OK です。
フィールド名と型名を 1 対 1 で合わせる必要はないということです。

{{< code lang="graphql" title="Good" >}}
type Book {
  # ...
  details: BookDetails
}
{{< /code >}}

上記のようなスキーマになっていれば、クライアントのクエリもシンプルに記述できます。

```graphql
query QueryBooks {
  books {
    # ...
    details {
      # ...
    }
  }
}
```

### 入力型と出力型を分けて定義する

クエリあるいはミューテーション要求時のフィールド引数のための専用の型として、入力型 (input type) を定義することができます。

```graphql
input CreateBookInput {
  title: String!
  author: String
  year: Int
}
```

入力型の名前は、慣例的に上記のように __`〇〇Input`__ という名前が付けられます。
フィールド自体の型は、入力型の反対の意味で出力型 (output type) と呼ばれたりします。
入力型は、フィールド自体の型としては使えないようになっていますが、出力型（`type` で定義したもの）は、フィールド引数の型としても使えてしまいます。
データ生成系の API では、入力と出力のフィールドに共通部分が多くなるため、次のように入力と出力の型を共通化してしまいがちです。

{{< code lang="graphql" title="NG" >}}
type Mutation {
  createBook(input: Book!): Book
}
{{< /code >}}

上記のように入力型と出力型を共通化してしまうと、将来的に出力型にのみ `createdAt` フィールドを追加しようとしたときに困ります。
ミューテーション操作に必要な入力型が、現状は出力型でまかなえるとしても、最初から入力型は分けて定義しておくべきです。

{{< code lang="graphql" title="Good" >}}
type Mutation {
  createBook(input: CreateBookInput!): Book
}
{{< /code >}}


- 参考: [GraphQL スキーマ仕様: 入力型 (input type) を定義する](/p/nhhwqtu/)

### エンドポイント URL の末尾は /graphql

GraphQL サーバーを作成するとき、エンドポイントする URL に特に決まりはないのですが、[GraphQL over HTTP specification](https://graphql.github.io/graphql-over-http/) では、パスの最後の部分は __`/graphql`__ で終わることが推奨されています。
特に理由がなければ、このルールに従っておくと分かりやすいです。
URL を見るだけで、REST API ではないことが一目瞭然です。

{{< code title="推奨されているエンドポイント URL の例" >}}
http://example.com/graphql
http://product.example.com/graphql
http://example.com/product/graphql
{{< /code >}}

ちなみに、上記のような GraphQL over HTTP の仕様は、[GraphQL 本体の仕様](http://spec.graphql.org/) とは別で定義されています。
GraphQL はもともと HTTP 前提ではないのですが、実際には Web サーバーとして提供されることがほとんどなので、GraphQL over HTTP という仕様が後付けで定義されています。
そこでは、HTTP ヘッダーの扱い方や、HTTP POST メソッドでどのようにクエリドキュメントを送るかなどが定義されています。


フィールド (fields)
-----

### String 型よりも具体的な型を使えないか考慮する

`String` 型のフィールドは任意の文字列を格納できて便利ですが、指定できるフォーマットに制約があるのであれば、より具体的な型を割り当てられないかを考慮します。
例えば、使える文字列が固定で数が限られているなら、`String` ではなく、[enum 型を定義](/p/isotm77/) して使用します。

```graphql
enum SortDirection {
  UNSPECIFIED
  ASCENDING
  DESCENDING
}
```

キー＆バリューのリストが必要なら、JSON 形式の文字列などで表現するのではなく、キー＆バリューを保持する型を定義します。

{{< code lang="graphql" title="NG" >}}
type Book {
  # JSON string of book attributes
  attributes: String
}
{{< /code >}}

{{< code lang="graphql" title="Good" >}}
type Book {
  attributes: [BookAttribute!]!
}

type BookAttribute {
  key: String!
  value: String!
}
{{< /code >}}

それ以外のフォーマットが明確な文字列や数値は、カスタムスカラーを定義することで対応できます。

```graphql
scalar EmailAddress
scalar CreditCardNumber
```

- 参考: [Golang の gqlgen におけるカスタムスカラー](https://gqlgen.com/reference/scalars/)

### フィールドの役割を特化する

フィールドは多くのユースケースに対応できるように汎用化するよりも、あるユースケースに特化したものにした方が扱いが容易になります。
オプショナルなパラメーターを持つフィールド、つまり、汎用的なフィールドは、使い方が直感的でなく、キャッシュを導入するのも難しくなります。
汎用的なフィールドは、リゾルバー実装内での分岐が増えるため、パフォーマンスが悪くなります。
次のように、`Boolean` 型のオプショナルパラメーターが出てきたら不吉な兆候と捉えましょう。

{{< code lang="graphql" title="NG" >}}
type Query {
  posts(first: Int!, includeArchived: Boolean): [Post!]
}
{{< /code >}}

上記の `posts` フィールドは、アーカイブ済みポストも返せるように汎用化が進んでいます。
次のように、特化された `archivedPosts` フィールドを追加することで、同様のユースケースをサポートできます。

{{< code lang="graphql" title="Good" >}}
type Query {
  posts(first: Int!): [Post!]
  archivedPosts(first: Int!) [Post!]
}
{{< /code >}}

このように特化されたフィールドとして定義されていると、その API の使い方が明確になるため、クライアント実装の不具合が減ります。
オプショナルなパラメーターが複数になると、もっと顕著になります。
例えば、書籍データを ID かタイトルで検索するための次のような `book` フィールドがあったとします。

{{< code lang="graphql" title="NG" >}}
type Query {
  book(id: String, title: String): Book
}
{{< /code >}}

これは一見便利そうな API ですが、`id` と `title` の両方を指定した場合の振る舞いが明確ではありません。
次のようにフィールド自体が分かれていれば、スキーマ定義を見るだけで使い方がすぐに分かります。

{{< code lang="graphql" title="Good" >}}
type Query {
  bookById(id: String!): Book
  bookByTitle(title: String!): Book
}
{{< /code >}}

似たようなフィールドが増えるのは抵抗があるかもしれませんが、GraphQL においてそれがオーバーヘッドになることはありません。
むしろ、リゾルバー実装がシンプルになるため、パフォーマンスは向上します。
また、フィールドを参照しているクライアントを絞り込むことが容易になるので、フィールドの再設計をしやすくなるという利点もあります。
例外的に、SQL のような高度な検索機能を提供したい場合は、汎用的な検索クエリを扱うフィールドがあってよいかもしれません。
ただし、やはりユースケースごとに特化した方が通常はうまくいきます。

### 暗黙的なデフォルト値をリゾルバー内部で定義しない

フィールドパラメーターとして、ソート順序やフィルタ条件などのオプションを追加する場合、パラメーター省略時の振る舞いをスキーマから読み取れるようにすべきです。
次の例では、パラメーターのデフォルト値を設定することで、API の振る舞いを明確にしています。

```graphql
type Query {
  games(sort: SortOrder = DESC): [Game!]
}
```

- 参考: [GraphQL スキーマ仕様: 列挙型 (enum type) を定義する](/p/isotm77/)


Nullable と Non-null
----

### フィールドを Non-null にする場合は慎重に

あるフィールドを一度 Non-null 型（末尾に `!`）で定義してしまうと、__後から Nullable（Null 許容型）に戻すのは困難__ です。
サーバー側の一方的な都合で Nullable 型に変更すると、Non-null 前提で実装しているクライアントが動作しなくなってしまいます（破壊的変更）。
一般的なプログラミング言語内での関数実装では、Null 安全を考慮して、できるだけ Non-null 型を使用することが推奨されていたりしますが、GraphQL スキーマにおいてはこの考え方はあてはまらないので、Non-null 型を採用する場合は慎重になるべきです。
GraphQL において、`null` 値は次のような状態を表現するために使用されます。

- そのフィールドの値が `null` だった（データベースに格納されている値自体が `null` であるなど）
- そのフィールドのアクセス権限がなかった
- 指定された条件に一致するフィールドが見つからなかった
- その他、リゾルバー内部でエラーが発生した（外部要因、タイムアウトなど）

特に、最後のエラー発生の可能性は軽視しがちです。
リゾルバー内でデータベースにアクセスする場合や、ネットワーク経由でデータ取得する場合は、様々な原因によるエラーが発生します。
例えば、データベースサーバー側の不具合や、ネットワーク遅延によるタイムアウトなどです。
こういったデータ取得を伴うフィールドは、デフォルトの Nullable 型として定義しておくべきです。
ただし、リスト型の戻り値の場合、`null` の代わりに空リストを返すことができるので、Non-null 型で定義しても問題になることは少ないです。

{{% note title="Non-null フィールドで null を返すとどうなるか？" %}}
仮に、GraphQL サーバー側の実装で、Non-null 定義されているフィールドなのに強引に null を返そうとすると、親フィールドを遡っていき、最初に見つかった Nullable なフィールドが null になるという挙動をします。
これは、Non-null フィールドを null で返さないようにするためのフォールバック動作です。
こうなると、本来部分的に取得できていたはずのデータも取得できなくなり、クライアント側に効率の悪い実装を強いることになります。
{{% /note %}}

### 無意味な null フィールドの組み合わせが発生しないようにする

Nullable なフィールドは「データなし」を表現するのに便利ですが、複数の Nullable フィールドがあると、不正な組み合わせが発生しやすくなります。

{{< code lang="graphql" title="NG" >}}
type Payment {
  creditCardNumber: CreditCardNumber
  creditCardExpiration: CreditCardExpiration
  giftCode: String
}
{{< /code >}}

これは 3 つの Nullable なフィールドを持つ型の例ですが、最初の 2 つのフィールドは、どちらか一方だけ値がセットされても意味がありません。
このようなフィールドは、1 つの型にまとめて次のように定義すれば、どちらか一方だけが null になるケースを考慮しなくて済みます。

{{< code lang="graphql" title="Good" >}}
type Payment {
  creditCard: CreaditCard
  giftCode: String
}

type CreditCard {
  number: CreditCardNumber!
  expiration: CreditCardExpiration!
}
{{< /code >}}

### Nullable と Non-null のガイドライン

1. 何らかの原因で値を返せない可能性があるフィールドは、__Nullable__ にしておきます。
例えば、リゾルバーがデータソースとして、データベースやネットワークにアクセスする場合です。
2. フィールドパラメーターはできるだけ __Non-null__ にしておくと、その API の使い方が明確に伝わります（省略時の振る舞いを言及しなくてよい）。
ただし、パラメーターを後から追加する場合は、Nullable で（デフォルト値を付けて）追加することになります。
フィールドパラメーターとして、オプショナルなフラグを追加したくなったときは、より具体的な別のフィールドを定義できないか考えてみるのがよいです。
3. あるオブジェクト型のフィールドであり、そのオブジェクトが存在するときに必ず存在することが分かっているデータあれば、そのフィールドは __Non-null__ で定義することができます（そのフィールドを含むオブジェクト自身は Nullable になり得ます）。


ミューテーション (mutation)
----

### ミューテーション操作単位で一貫性を保つ

GraphQL クライアントがミューテーション要求を行う場合、`mutation` ドキュメントには複数のミューテーション操作を含めることができます。

{{< code lang="graphql" title="NG（かもしれない）" >}}
mutation ConfigServer {
  setAddress(address: "192.168.0.100") {
     ...serverConfig
  }
  setPort(port: 25052) {
     ...serverConfig
  }
}
{{< /code >}}

これらのミューテーション操作は上から順番に処理されていきますが、いずれかのミューテーション操作でエラーが発生すると、その後のミューテーション処理は実行されません。
GraphQL には、リレーショナルデータベース (RDB) のようなトランザクション処理の仕組みはないため、すでに実行されてしまったミューテーション操作をロールバックすることはできません。
上記の `setAddress` 操作と `setPort` 操作が必ずセットで実行しなければいけない処理であるなら、そもそもミューテーション操作の定義を 1 つにまとめるべきです。

{{< code lang="graphql" title="Good" >}}
mutation ConfigServer {
  changeServerConfig(address: "192.168.0.100", port: 25052) {
     ...serverConfig
  }
}
{{< /code >}}

このように、ミューテーション操作は整合性を保てる単位（RDB で言うところのトランザクション単位）で実行できるようにフィールド設計します。

- 参考: [GraphQL スキーマ仕様: ミューテーションを定義する（データ更新 API）](/p/pk9c9qs/)


有名な実装パターン
----

### サインイン済みユーザーを表す me フィールド

例えば、タスクの一覧を取得するための `taskList` ルートフィールドが次のように定義されているとします。

```graphql
type Query {
  tasklist: [Task!]!
}
```

ここには、全ユーザーのタスクが含まれていますが、カレントユーザーのタスクのみを取得したいときはどうすればよいでしょうか？
フィールドパラメーターとしてユーザー ID を指定できるようにするという方法もありますが、カレントユーザーの情報は頻繁に参照するため、専用の __`me`__ フィールドを用意するということがよく行われます。

```graphql
type Query {
  me: User
  # ...
}

type User {
  taskList: [Task!]!
  # ...
}
```

ユーザーがサインインしていない場合は、`me` フィールドの値は `null` になるので、サインイン済みかどうかの判断にも利用できます。

### ページネーション（Connection パターン）

クエリのレスポンスとして多数のデータを返す可能性がある場合は、ページネーションに対応したフィールドとして定義することで、データを部分的に取得できるようにします。
ページネーションの実装方法は自由ですが、Facebook の Relay ライブラリが提唱している __Connection パターン（カーソルベースのページネーション）__ を採用するのが一般的です。
Connection パターンでは、フィールドを定義するときに、値のリストの代わりに Connection 型のオブジェクトを返すようにし、統一された方法でページネーション処理を行えるようにします。

```graphql
type Query {
  games(
    after: String
    before: String
    first: Int
    last: Int
  ): GameConnection
}
```

- 参考: [GraphQL スキーマ仕様: ページネーション (pagination)](/p/vuo4px3/)
- 参考: [GraphQL Cursor Connections Specification - Relay](https://relay.dev/graphql/connections.htm)

Connection パターンに従ってスキーマ定義を行うことで、クライアントアプリの実装者は見慣れた方法でページネーション処理を実装できるようになります。
また、GraphQL ライブラリによっては、Connection パターンでのスキーマ定義を前提としていることがあります。

### Node インタフェースと Global Object Identification

GraphQL スキーマでは、`id: ID!` フィールドを持つ __`Node` インタフェース__ を定義し、すべてのオブジェクト型がこれを実装するようにします。
この `id` フィールドにはシステム全体でユニークな ID を格納し、すべてのオブジェクトを ID で特定できるようにします (__Global Object Identification__)。

```graphql
# An object with a Globally Unique ID
interface Node {
  # The ID of the object.
  id: ID!
}

# The query root of GitHub's GraphQL interface.
type Query {
  # Fetches an object given its ID.
  node(id: ID!): Node
}

type User implements Node {
  id: ID!
  login: String!
  name: String
  # ...
}
```

`Node` インタフェースを備えるすべてのオブジェクトは、トップレベルの __`node`__ クエリを使って参照できるようにします。
下記は、GitHub の GraphQL API を使ってユーザー情報 (`User`) を取得するクエリの例です。

{{< code lang="graphql" title="node クエリの例" >}}
query QueryUser {
  node(id: "MDQ6VXNlcjU1MTk1MDM=") {
    ... on User {
      id
      login
      name
    }
  }
}
{{< /code >}}

{{% note title="一意な ID の作り方" %}}
上記のクエリ例で指定している `MDQ6VXNlcjU1MTk1MDM=` という ID は、ランダムな文字列のように見えますが、実は `04:User5519503` という文字列を Base64 エンコードしたものです。
この文字列は __APIバージョン + オブジェクトタイプ + オブジェクトID__ という構成になっており、GitHub 内部の API 実装で利用されています。
このテクニックを使うと、GraphQL API のリゾルバーを実装する際に、ID のマッピングテーブルを作成する必要がなくなります。
{{% /note %}}

すべてのオブジェクト型に `Node` インタフェースを実装し、一意の ID を割り当てることには様々なメリットがあります。

- GraphQL ライブラリは、ID 情報をキーにしてオブジェクトをキャッシュすることができます（実際に Apollo Client ライブラリなどは、オブジェクト型が `id` フィールドを備えていることを前提とした作りになっています）
- クライアントアプリがオブジェクトの更新情報を取得するときに、統一された方法でフェッチできます
- GraphQL API サーバーで、あるオブジェクトの情報を構築するときに、複雑に階層化されたノードを処理する必要がなくなります（ワンステップでデータソースから情報を取り出せます）
- インタフェースが統一されることで、様々なツールとの連携が可能になります。例えば、ページネーション用のクエリを自動生成できます（ページネーション実装に使われる Connection パターンも `Node` インタフェースを利用しています）

`Node` インタフェースは、Facebook の Relay フレームワークの実装から生まれたコンセプトですが、[GraphQL 公式のベストプラクティス](https://graphql.org/learn/global-object-identification/) としても掲載されるようになりました。


リゾルバー実装（保守性、パフォーマンス、セキュリティ）
----

### ドメインロジックをリゾルバー関数から分離する

__リゾルバー関数の実装はできるだけ短く記述する__ ことがベストプラクティスとされています。

> By keeping resolvers thin as a best practice, you can safely refactor your backing logic while reducing the likelihood of breaking your API.
> --- Apollo GraphQL Tutorial

多くのケースでは、データの取得や加工処理といったドメインロジックを分離することで、リゾルバー関数の実装を 1 行～数行に抑えることができます。
ドメインロジックを分離することは、ユニットテストを記述できる設計にするということであり、安定した API をリリースし続けるためにも必要です。
次の例では、3 つのルートクエリに対するリゾルバーをすべて 1 行で実装しています。

{{< code lang="js" title="Apollo Server でのリゾルバー実装例" >}}
module.exports = {
  Query: {
    launches: (_, __, { dataSources }) => dataSources.launchAPI.getAllLaunches(),
    launch: (_, { id }, { dataSources }) => dataSources.launchAPI.getLaunchById({ launchId: id }),
    me: (_, __, { dataSources }) => dataSources.userAPI.findOrCreateUser(),
  },
}
{{< /code >}}

ドメインロジック部分は、GraphQL サーバーがなくても単独で動作するように実装すべきです。
リゾルバー関数は、ドメインロジックを呼び出すだけの橋渡しの役割になります。

### サブフィールド用のリゾルバー関数を実装して DataLoader で N+1 問題に対応する

入れ子構造になったオブジェクトを返す必要があるときは、サブフィールドの処理をそれ専用のリゾルバー関数に委譲することで、リゾルバーの実装をシンプルに保つことができます。
もちろん、すべてのオブジェクトフィールド用にリゾルバー関数を定義する必要はありません。

- 参考: [gqlgen で子フィールドの情報を返すリゾルバーを実装する](https://maku77.github.io/p/wdvdtap/)

サブフィールド用のリゾルバー関数を分けて定義すると、__N+1 問題__ が発生することがあります。
これは、サブフィールドの処理ごとにリゾルバー関数が呼び出され（N 回）、その中で毎回同じデータソースへのフェッチが実行されてしまうというパフォーマンス問題です。
これを解決する手法として、__DataLoader__ という実装パターン（ライブラリ）があります。
DataLoader は、連続したデータフェッチをバッチ処理化、キャッシュするためのユーティリティライブラリです。
[DataLoader ライブラリ](https://github.com/graphql/dataloader)は、もともと Facebook が作成したものですが、今では GraphQL コミュニティで管理されており、多数のプロジェクトが採用しています。

### データソースや認証情報をコンテキストで共有する

GraphQL API の __コンテキスト__ は、すべてのリゾルバーで共有されるオブジェクトです。
データベースのコネクションプールなどをコンテキストで共有すれば、複数のリゾルバー関数が呼び出されたときに、毎回データベースに接続し直すという無駄を省くことができます。
リゾルバー間で共有するといっても、コンテキストはリクエストごとに生成されることに注意してください（異なるユーザーからのリクエストでデータが共有されるとおかしなことになるので）。
あくまで、一つのリクエストで複数のリゾルバー関数が呼び出される場合に、そのリゾルバ間でコンテキストが共有されるということです。
コンテキストは、データソースの保持以外にも、認証情報（ユーザー情報やアクセス権限）の保持にも利用されます。

コンテキストを保持する方法は GraphQL のサーバーライブラリによって異なりますが、例えば、Apollo Server ライブラリであれば、`ApolloServer` インスタンスを生成するときに、`context` プロパティに任意のコンテキスト生成関数をセットすることができます。

{{< code lang="ts" title="コンテキストでデータベース接続を保持する例" >}}
context: async () => ({
  db: await client.connect(),  // 各リゾルバー関数内で context.db で参照できる
}),
{{< /code >}}

{{< code lang="ts" title="コンテキストでユーザー情報を保持する例" >}}
context: async ({ _, res }) => {
  const token = req.headers?.authorization || ''
  return {
    user: await getUser(token),  // 各リゾルバー関数内で context.user で参照できる
  }
},
{{< /code >}}

コンテキストオブジェクトをセットするのではなく、コンテキスト生成関数をセットしているのは、前述した通り、リクエスト時にコンテキストを生成する必要があるからです。

### エラーレスポンスに実装の詳細情報を含めない

GraphQL API の呼び出しでエラーが発生したときに、エラー情報としてサーバー側の実装詳細（スタックトレースなど）を返してしまうと、悪意のあるユーザーに不正アクセスのヒントを与えてしまいます。
とはいえ、API の実装中はサーバー側の詳細なエラー情報を返した方が都合がよかったりします。
そのような場合は、次のように、開発中（dev 環境）でのみエラー詳細を返すようにしておくと便利です。

```ts
return config.isDev ? errorReport : { message: 'Oops! Something went wrong.' }
```

### 公開 API には Rate Limit やクエリのネスト制限を設ける

GraphQL API を公開するときは、過剰なアクセスによりサーバーリソースを食いつぶされないように対策が必要です。
GraphQL はクエリドキュメントでフィールドをネストできるため、コストのかかるデータ取得処理を簡単に記述できてしまいます。
クライアント実装者に悪意がなくても、クエリの記述ミス（あるいは実装者の知識不足）により DOS 攻撃のようなリクエストが飛んでくる可能性があります。

```graphql
query QueryPosts {
  posts(count: 100) {
    related(count: 100) {
      related(count: 100) {
        related(count: 100) {
          title
        }
      }
    }
  }
}
```

コストのかかるリクエストを制限するためには、GraphQL サーバー側でクエリの __ネスト数に制限__ をかけたり、__Rate Limit__ を設けて実行可能なクエリ数を制限します。
GitHub の GraphQL API にも Rate Limit が採用されています。

- 参考: [Resource limitations - GitHub Docs](https://docs.github.com/en/graphql/overview/resource-limitations)
- 参考: [GitHub GraphQL API の呼び出し回数制限 (rate limit) の情報を取得する｜まくろぐ](/p/bqz7emt/)

通常、フィールドごとにデータ取得のコストは異なるため、要求されたフィールドに応じてそのコストを計算するという実装になります。
主要な GraphQL ライブラリには、クエリの複雑度（＝コスト）を計算するためのユーティリティが用意されています。
下記は、Golang の gqlgen ライブラリにおけるクエリコスト計算の例です。

- 参考: [Preventing overly complex queries — gqlgen](https://gqlgen.com/reference/complexity/)

{{% private %}}
- [Securing Your GraphQL API from Malicious Queries - Apollo GraphQL Blog](https://www.apollographql.com/blog/graphql/security/securing-your-graphql-api-from-malicious-queries/)
{{% /private %}}


クエリ（クライアント側）
----

GraphQL API を使用するクライアント側で指定するドキュメント（クエリ）に関してのベストプラクティスです。

### query や mutation には操作名を付ける

操作名 (operation name) はマルチ操作ドキュメント (multi-operation document) 以外ではオプショナルとされていますが、サーバーサイドでのロギングに役立つので、なるべく記述するようにすべきです。
REST API の場合は、操作ごとにエンドポイントが変わるので、どのような API 呼び出しが行われたかトラックしやすいのですが、GraphQL の場合は単一のエンドポイントになるので、分かりやすい操作名を付けることがロギングのポイントになります。
HTTP リクエストのペイロードに `operationName` が含まれるようになるので、Chrome のデベロッパーツールを使ったクライアントサイドでのデバッグにも役立ちます。

{{< code lang="graphql" title="NG" >}}
query {
  books { title author }
}
{{< /code >}}

{{< code lang="graphql" title="Good" >}}
query QueryBooks {
  books { title author }
}
{{< /code >}}

### static なクエリドキュメントを使用する

クエリドキュメント (`query {...}`) をプログラム実行時に動的に組み立てることは避け、静的な (static) クエリ文字列をそのまま扱うようにします（`StringBuilder` や `QueryBuilder` のようなものを使わない）。
通常は、クエリドキュメントを専用のファイル (`.graphql`) として作成するか、JavaScript のタグ付きテンプレート (`` gql`...` ``) などを使っていれば大丈夫です。
static なクエリドキュメントには、様々な利点があります。

クエリ編集時のサポート機能
: VS Code や IntelliJ といった IDE には、GraphQL クエリ言語を扱うプラグインが用意されており、ほぼ実装言語を意識せずに使用できます。スキーマ定義を入力データとして設定しておけば、エディタ上での補完や検証ができるようになっています。

各クエリ用の型ファイルの自動生成 (easier code generation)
: static なクエリドキュメント（とスキーマ定義）から、クエリレスポンスの型情報を自動生成することで、型安全かつ効率的なコーディングが可能になります。こういったツールとしては、[TypeScript の graphql-codegen](/p/n2k2hxd/) や、[Golang の gqlgen](https://maku77.github.io/p/v48adgi/) などがあります。

永続化クエリ (persisted queries)
: クライアントからの高コストなクエリを防ぐ方法として、永続化クエリという考え方があります。これは、アプリのリリース時に、必要なクエリに ID を割り当ててしまい、実際のクエリでは ID だけを送るという方法です。永続化クエリを取り入れるには、static なクエリ定義が必須になります。クエリ言語では変数を分離できるようになっているので、多少変化する部分があってもクエリの永続化が可能です。

外部ツールとの連携 (better tooling)
: コード内で static なクエリドキュメントを使う設計になっていれば、GraphiQL などの開発ツールで試行錯誤して完成させたクエリをそのままコードに取り込むことができます。逆に、コード内のクエリを GraphiQL でテストするのも容易です。

要するに、共通言語としてクエリ言語があるのだから、それを最大限に活用せよということです。

mutation 操作の場合は、可変なクエリになりやすいのではと思うかもしれません。
例えば、次のようなスケジュール追加の mutation が定義されているとします。

```graphql
type Mutation {
  addSchedule(input: AddScheduleInput!): AddSchedulePayload
}
```

クライアントが複数のスケジュールを同時に登録しようとした場合、複数の `addSchedule` 操作を呼び出すクエリを動的に生成することになってしまいます。
このようなユースケースが存在する場合は、入力変数のフィールドとして複数の値を渡せるような API を用意します。

```graphql
type Mutation {
  addSchedule(input: AddScheduleInput!): AddSchedulePayload
  addSchedules(input: AddSchedulesInput!): AddSchedulesPayload
}

input AddSchedulesInput {
  schedules: [ScheduleInput!]!
  # ...
}
```

こうすることで、可変部分は変数に切り出せるので、クエリ文字列を static に記述することができます。
