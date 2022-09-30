---
title: "GraphQL スキーマ仕様: ミューテーションを定義する（データ更新 API）"
url: "p/pk9c9qs/"
date: "2022-09-28"
tags: ["GraphQL"]
---

GraphQL API でサーバーサイドのデータを更新するには、ミューテーション操作 (__`mutation`__) を含むクエリを送信します。
このときどのようなミューテーションを行えるかは、GraphQL スキーマの中で、__`Mutation`__ 型のフィールドとして定義します。


ミューテーションのスキーマを定義する
----

次の例では、書籍データを新規作成するための `createBook` ミューテーションと、削除するための `deleteBook` ミューテーションを定義しています。
フィールド名は変更内容を表す動詞で始めるのが一般的です。

{{< code lang="graphql" title="GraphQL スキーマ定義" >}}
type Mutation {
  createBook(title: String!, author: String!): Book!
  deleteBook(id: ID!): Boolean!
}
{{< /code >}}

各フィールドにはパラメーター（上記の `title` など）を定義することができ、そのパラメーターの指定が必須である場合には、型名の末尾に `!` を付けます (`String!`)。

ミューテーションが呼び出されたときの実際の処理内容は、GraphQL サーバー側のミューテーション・リゾルバーで実装します。
リゾルバーの実装方法は、各言語の GraphQL サーバーライブラリによって異なります。
例えば、下記は Golang の gqlgen パッケージを使った場合のリゾルバー実装の例です。

{{< code lang="go" title="ミューテーション・リゾルバーの実装" >}}
// CreateBook is the resolver for the createBook field.
func (r *mutationResolver) CreateBook(
	ctx context.Context,
	title string,
	author *string,
) (*model.Book, error) {
	id := uuid.NewString()
	book := &model.Book{
		ID:     id,
		Title:  title,
		Author: author,
	}
	// 本当はこのあたりで DB などに book を追加
	return book, nil
}
{{< /code >}}


ミューテーションを実行する
----

GraphQL API で上記のミューテーションを呼び出すには、次のようなミューテーション操作 (__`mutation`__) を含むドキュメント（クエリ）を送信します。

{{< code lang="graphql" title="ミューテーションの実行" >}}
mutation CreateBook {
  createBook(title: "My book", author: "Maku") {
    id
    title
    author
  }
}
{{< /code >}}

クエリ操作 (`query`) と同様にミューテーション操作 (`mutation`) には戻り値があり、通常は次のように、更新後のオブジェクトを示す内容になっています。
この例では、`id` フィールドに GraphQL サーバー側で自動生成された UUID が格納されています。

{{< code lang="json" title="ミューテーションのレスポンス" >}}
{
  "data": {
    "createBook": {
      "id": "b468b5d5-4446-407b-b11d-eb7cefe438a0",
      "title": "My Book",
      "author": "Maku"
    }
  }
}
{{< /code >}}

`mutation` ドキュメントには、複数のフィールド（ミューテーション操作）を含むことができます。

{{< code lang="graphql" title="ミューテーションの実行" >}}
mutation CreateBook {
  createBook1: createBook(title: "Title1", author: "Author1") {
    id
    title
    author
  }
  createBook2: createBook(title: "Title2", author: "Author2") {
    id
    title
    author
  }
}
{{< /code >}}

複数の `query` 操作は並列に処理されますが、複数の `mutation` 操作に関しては上から順番に処理されていくことに注意してください。
いずれかの `mutation` 操作が失敗すると、その下の `mutation` 操作は実行されません（すでに実行された `mutation` 操作がロールバックされることもありません）。
RDB のトランザクション処理のようなことはできないので、1 つの `mutation` 処理ごとにデータの整合性を保てるよう実装されている必要があります。


（応用）ミューテーションに入力型の変数を使う
----

フィールドのパラメーターの数が増えてきたときは、入力型 (input type) を定義して、ひとつのオブジェクト型として渡すと分かりやすくなります。
入力型の詳細は下記を参照してください。

- 参考: [GraphQL スキーマ仕様: 入力型 (input type) を定義する](/p/nhhwqtu/)

