---
title: "TypeScriptの型: 既存の型をちょっと変えた型を作る（ユーティリティ型）"
linkTitle: "既存の型をちょっと変えた型を作る（ユーティリティ型）"
url: "/p/9t6gr3c"
date: "2022-02-08"
tags: ["TypeScript"]
---

ユーリティティ型とは？
----

TypeScript には、既存の型を加工して新しい型を生み出す [ユーティリティ型 (Utility Types)](https://www.typescriptlang.org/docs/handbook/utility-types.html) というものが用意されています。
例えば、`Partial` を使用すると、ある型のプロパティをすべてオプショナルにした型を簡単に作ことができます。
以下、使いやすそうなユーティリティ型を紹介しておきます。

| ユーティリティ型 | 概要 |
| ---- | ---- |
| `Omit` | 指定したプロパティを取り除く |
| `Pick` | 指定したプロパティだけ抽出する |
| `Partial` | 全プロパティをオプショナルにする |
| `Required` | 全プロパティを必須にする |
| `Readonly` | 全プロパティを readonly にする |


ユーティリティ関数の使用例
----

### Omit / Pick ... プロパティを削除・抽出する

__`Omit`__ を使用すると、既存の型から指定したプロパティを取り除いた型を作成できます。
次の例では、`Book` 型から `id` プロパティを取り除いて、新しい `NewBook` 型を定義しています。

```ts
type Book = {
  id: string
  title: string
  price: number
}

type NewBook = Omit<Book, 'id'>

// 上記は以下と同様
// type NewBook = {
//   title: string
//   price: number
// }

// 使用例: 新しい書籍を登録する関数（id は自動生成する想定）
function addBook(book: NewBook): Book {
  // ...
}
```

__`Pick`__ は逆に、指定したプロパティのみを持つ型を作りたいときに使用します。
次の例では、`title` と `price` プロパティのみを持つ新しい `NewBook` 型を定義しています。

```ts
type NewBook = Pick<Book, 'title' | 'price'>
```

### Partial / Required ... プロパティをオプショナル・必須にする

__`Partial`__ を使うと、既存の型のすべてのプロパティをオプショナルにした新しい型を生成できます。

```ts
type Game = {
  title: string
  genre: string
  price?: number
}

type GamePartial = Partial<Game>

// type GamePartial = {
//   title?: string
//   genre?: string
//   price?: number
// }

// 使用例: 指定したフィールドで検索する関数
async function findGames(condition: Partial<Game>): Promise<Game[]> {
  // ...
}
```

__`Required`__ は逆に、すべてのプロパティを必須にした型を生成します。

```ts
type GameRequired = Required<Game>

// type GameRequired = {
//   title: string
//   genre: string
//   price: number
// }
```

### Readonly ... プロパティを readonly にする

__`Readonly`__ を使うと、既存の型のすべてのプロパティを読み取り専用にした新しい型を生成できます。

```ts
type Game = {
  title: string
  genre: string
  price?: number
}

type ReadonlyGame = Readonly<Game>

// type Game = {
//   readonly title: string
//   readonly genre: string
//   readonly price?: number
// }
```

