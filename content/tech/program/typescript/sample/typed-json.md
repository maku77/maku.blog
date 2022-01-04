---
title: "TypeScript で JSON オブジェクトに型情報を付加する"
url: "/p/2hr3eqx"
date: "2021-06-26"
tags: ["JSON", "TypeScript"]
---

何が問題か？
----

JSON テキストファイルの内容を `JSON.parse` した結果や、`fetch` API のレスポンスを `json()` 関数にかけた結果は、型情報のない `any` 型のオブジェクトになります。
TypeScript コードから、このオブジェクトのプロパティを参照しようとすると、「定義されていないプロパティを参照している」という感じの ESLint エラーになってしまいます。
何より、型情報がないデータをそのまま扱おうとすると、VS Code などでプロパティ名の入力補完機能が働きません。

ここでは、サンプルデータとして次のような JSON 形式の文字列を使うことにします。

{{< code lang="ts" >}}
const jsonText = `{
  "games": [
    {
      "title": "Title1",
      "genres": ["ACT"]
    },
    {
      "title": "Title2",
      "genres": ["ACT", "RPG"]
    },
    {
      "title": "Title3",
      "genres": ["STG"],
      "note": "Fantastic shooting game"
    }
  ]
}
`
{{< /code >}}

この JSON 文字列を `JSON.parse` 関数でオブジェクトに変換して、そのプロパティを参照しようとすると、ESLint がエラーを出します。

{{< code lang="ts" >}}
// Unsafe assignment of an `any` value (@typescript-eslint/no-unsafe-assignment)
const jsonObj = JSON.parse(jsonText)

// Unsafe member access .games on an `any` value (@typescript-eslint/no-unsafe-member-access)
console.log(jsonObj.games[0].title)
{{< /code >}}


解決方法
----

このような `any` 型オブジェクトを TypeScript コードから扱うには、JSON データに一致する型情報（あるいはそのサブセット）を定義し、型アサーション (`as`) でその型を指定します。

下記のコードは、完全な型情報が付加された TypeScript コードです。

{{< code lang="ts" >}}
type Game = {
  title: string
  genres: string[]
}

type GameInfo = {
  games: Game[]
}

// JSON.parse() の結果を型アサーションして受け取る
const gameInfo = JSON.parse(jsonText) as GameInfo
gameInfo.games.forEach((game) => {
  console.log(game.title)
  console.log(game.genres)
})
{{< /code >}}

なお、JSON 側での記述をオプショナルにしているプロパティ（上記のサンプルデータでは `note` プロパティ）を扱いたい場合も、同じように型定義してしまえば OK です。

{{< code lang="ts" >}}
type Game = {
  title: string
  genres: string[]
  note?: string  // note フィールドも参照する場合
}
{{< /code >}}

型定義 (`type` や `interface`）は、あくまで TypeScript 側でどのようなプロパティ名での参照を許すかという付加情報であり、JSON テキストに含まれているデータは、メモリ上に全て保持されていることに注意してください。


fetch で Web API を呼び出す場合も同様
----

ここまでの例では `JSON.parse` 関数を使っていましたが、`fetch` 関数で Web API を叩いて JSON データを取得する場合も同様です。
次の例では、GitHub の REST API でユーザー情報を取得しています。
プログラム内で参照するフィールドは、`IGitHubUser` 型として定義しています。

{{< code lang="ts" >}}
type IGitHubUser = {
  name: string
  login: string
  location: string
}

async function showGitHubUser(name: string = 'octocat') {
  const url = `https://api.github.com/users/${name}`
  const res: Response = await fetch(url)
  if (res.ok) {
    const user = await res.json() as IGitHubUser
    console.log(user.name)
    console.log(user.login)
    console.log(user.location)
  } else {
    console.error('Could not obtain user info')
  }
}
{{< /code >}}

