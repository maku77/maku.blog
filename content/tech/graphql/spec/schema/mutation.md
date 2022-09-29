---
title: "GraphQL スキーマ仕様: ミューテーションを定義する（データ更新 API）"
url: "p/pk9c9qs/"
date: "2022-09-28"
tags: ["GraphQL"]
---

GraphQL でサーバーサイドのデータを更新できるようにするためには、__ミューテーション (mutation)__ を定義します。
API のエントリポイントとなるミューテーションは、__`Mutation`__ 型のフィールドとして定義します。
フィールド名は変更内容を表す動詞にするのが一般的です。
次の例では、書籍データを新規作成するための `createBook` ミューテーションを定義しています。

{{< code lang="graphql" title="GraphQL スキーマ定義" >}}
type Mutation {
  createBook(title: String!, author: String!): Book!
}
{{< /code >}}

ミューテーションが呼び出されたときに、具体的のどのような処理を行うかは、GraphQL サーバー側のミューテーション・リゾルバー実装で定義します。
リゾルバーをどのように実装するかは、各言語の GraphQL サーバーライブラリによって異なります。
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

GraphQL API でこのミューテーションを呼び出すには、__`mutation`__ 操作を定義したドキュメントを送信します。
`query` と同様に、`mutation` の戻り値としてオブジェクトを受け取ることができます。
`mutation` の戻り値は、通常、更新されたオブジェクトの内容です。

{{< code lang="graphql" title="ミューテーションの実行" >}}
mutation CreateBook {
  createBook(title: "My book", author: "Maku") {
    id
    title
    author
  }
}
{{< /code >}}

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

