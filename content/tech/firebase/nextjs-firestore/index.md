---
title: "Next.js で Firebase: Cloud Firestore データベースを使う"
url: "/p/m3bjrz7"
date: "2021-12-30"
tags: ["Firebase/Firestore", "Next.js"]
---

何をするか
----

[Cloud Firestore](https://firebase.google.com/docs/firestore) は、Firebase サービス（あるいは GCP）が提供するサーバーレスな NoSQL データベースです。
Firestore データベースには様々な環境からアクセスできますが、ここでは Web サイトからアクセスすることを想定して、Firebase が提供する JavaScript SDK（クライアントサイド SDK）を使って Firestore データベースを操作してみます。

{{% note title="Firebase と GCP の使い分け" %}}
Firebase と GCP (Google Cloud Platform) の両方に Firestore の記載があるので混乱しますが、どちらのプロジェクトで作った Firestore データベースも内部的には共有されているようです。
Firebase プロジェクトも、GCP プロジェクトとして参照できるようになっています。
一方で、SDK は共通化されておらず、主に Firebase はクライアントサイド用の SDK を提供し、GCP はサーバーサイド用の SDK を提供しています。
{{% /note %}}

前提条件として、Firebase プロジェクトの作成は完了し、`FirebaseApp` インスタンスの初期化コードは準備できているものとします。

- 参考: [Next.js で Firebase: プロジェクトの作成と接続準備](/p/73eq2cm)


Firestore データベースの作成
----

クライアントアプリの実装を始める前に、Firebase プロジェクトに Firestore データベースを作成します。

1. [Firebase コンソール](https://console.firebase.google.com/) にサインインして、対象のプロジェクトを開く。
1. サイドバーから __`Firestore Database`__ を選択し、__`データベースの作成`__ をクリックする。
    - 保護ルールは `本番環境モード` を選んでおけば OK
    - ロケーションは `asia-northeast1`（東京）を選んでおけば OK

次のように空っぽのデータベースが作成されれば準備 OK です。

{{< image border="true" w="700" src="img-001.png" >}}


テスト用コレクション (books) の作成
----

Web アプリからのデータ取得を試したいので、まずは Firebase コンソール上で Firestore データベースのテスト用コレクション (books) を作成することにします。

1. Firestore Database のデータタブから __コレクションを開始__ を押して、`books` という名前でコレクションを追加します。
2. 最初のドキュメント（データ）の情報追加画面になるので、こんな感じでデータを追加します。
    - ドキュメントID: `id-1`
    - フィールド `title`: (string) `Title-1`
    - フィールド `author`: (string) `Author-1`
    - フィールド `price`: (number) `1000`

    {{< image border="true" src="img-002.png" >}}

{{< note title="コレクション作成と同時にドキュメント追加が必要なのはなぜ？" >}}
Firestore のコレクションはドキュメント（個々のデータ）の集まりです。
あるコレクション ID の下に 1 つ目のドキュメントを作成しようとすると、コレクションが自動的に作成されます。
逆に、コレクションから最後のドキュメントを削除すると、コレクションは自動的に削除されます。
つまり、ドキュメントなしではコレクションは存在することができないので、Firebase コンソール上で Firestore のコレクションを作成しようとすると、1 つ目のドキュメント入力が同時に求められます。
{{< /note >}}

無事に `books` コレクションを作成できたら、__ドキュメントを追加__ を押して、適当に 3 つくらいデータを登録しておきます。

{{< image border="true" w="500" src="img-003.png" >}}

例えば、こんな感じでドキュメントを追加しておきます。

| ドキュメントID | title | author | price |
| ---- | ---- | ---- | ---- |
| id-1 | Title-1 | Author-1 | 1000 |
| id-2 | Title-2 | Author-2 | 2000 |
| id-3 | Title-3 | Author-3 | 3000 |


セキュリティルールの設定
----

クライアントアプリ（Web アプリやモバイルアプリ）からの Firestore データベースへのアクセス制御は、[セキュリティルール](https://firebase.google.com/docs/rules) で設定します。
例えば、「サインイン済みのユーザーにのみコレクションの読み書き許可する」といった設定が可能です。

ここではとりあえず、`books` コレクションは誰でも読み取り可能で、書き込みは不可というルールに設定してみます。
Firestore Database の __ルール__ タブを選択すると、セキュリティルールを編集できます。
デフォルトでは、おそらく次のようになっています。

```
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /{document=**} {
      allow read, write: if false;
    }
  }
}
```

これは、「いかなるドキュメントに対しても読み書きを許可しない」という設定なので、次のように編集して保存します。

```
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /books/{document=**} {
      allow read: if true;
      allow write: if false;
    }
  }
}
```

上記のルール設定は、次のような意味を持っています。

- books コレクション以下の全てのドキュメントが対象 (`match /books/{document=**}`)
- 読み込みは許可 (`allow read: if true;`)
- 書き込みは不可 (`allow write: if false;`)

これで、Firestore 側の `books` コレクションの準備は完了です。


クライアントコードの作成
----

Firestore の準備ができたので、Web アプリ (Next.js/React) のコードからデータ取得してみます。
カスタムフック (`useBooks`) を作って、Firestore の `books` コレクション内のすべてのドキュメントを取得します。
firebase パッケージのインストールや、`FirebaseApp` インスタンスの初期化用コード `init.ts` の作成は終わっているものとします（参考: [FirebaseApp の初期化](/p/73eq2cm)）。

### Firestore アクセス部分

まずは、Firestore から `books` コレクション内のドキュメントを取得してくるコードを作成します。
`Book` データ型もここで定義しておきます。

{{< code lang="ts" title="utils/firebase/books.ts" >}}
import { collection, getDocs, getFirestore } from 'firebase/firestore'
// import './init' // Initialize FirebaseApp

export type Book = {
  id: string
  title: string
  author: string
  price: number
}

export async function getBooks(): Promise<Book[]> {
  const books = new Array<Book>()
  const db = getFirestore()
  const booksSnapshot = await getDocs(collection(db, '/books'))

  booksSnapshot.forEach((doc) => {
    const book = doc.data() as Book
    books.push({ ...book, id: doc.id })
  })

  return books
}
{{< /code >}}

### データ取得用のカスタムフック

React コンポーネントからデータ取得したいので、上記で作ったコードを `useBooks` カスタムフックの形にラップします。
ここでは、直接 `getBooks()` を呼び出していますが、最終的には [useSWR フックを使ってデータフェッチ](/p/vm2ft83) することをお勧めします。

{{< code lang="ts" title="hooks/useBooks.ts" >}}
import { useEffect, useState } from 'react'
import { Book, getBooks } from '../utils/firebase/books'

export type UseBooksOutput = {
  isLoading: boolean
  books: Book[]
}

const DEFAULT_OUTPUT: UseBooksOutput = {
  isLoading: true,
  books: [],
}

export function useBooks(): UseBooksOutput {
  const [output, setOutput] = useState(DEFAULT_OUTPUT)

  useEffect(() => {
    void (async () => {
      const books = await getBooks()
      setOutput({ isLoading: false, books })
    })()
  }, [])

  return output
}
{{< /code >}}

### React コンポーネント

あとは、適当なコンポーネントから、`useBooks` フックを使って取得した情報を表示すれば OK です。

{{< code lang="tsx" title="components/BookTable.tsx" >}}
import { FC } from 'react'
import { useBooks } from '../hooks/useBooks'

export const BookTable: FC = () => {
  const { isLoading, books } = useBooks()
  if (isLoading) return <p>Loading...</p>

  return (
    <ul>
      {books.map((book) => (
        <li key={book.id}>
          {book.title} / {book.author} / {book.price}
        </li>
      ))}
    </ul>
  )
}
{{< /code >}}

上記のコンポーネントをレンダリングして、次のように表示されれば成功です。

- Title-1 / Author-1 / 1000
- Title-2 / Author-2 / 2000
- Title-3 / Author-3 / 3000

### おまけ: Firestore にドキュメントを追加するコード

上記の例では、Firestore からのドキュメントの読み込みを試しましたが、ドキュメントの追加も似たようなコードで実現できます。
下記の `addBook` 関数は、渡された `Book` オブジェクトの情報を Firestore の `books` コレクションに追加します。

{{< code lang="ts" title="utils/firebase/books.ts" >}}
import { collection, doc, getDocs, getFirestore, setDoc } from 'firebase/firestore'

// ...
// export async function getBooks(): Promise<Book[]> { ... }
// ...

export async function addBook(book: Book): Promise<void> {
  const db = getFirestore()
  const docRef = doc(db, 'books', book.id)
  await setDoc(docRef,
    { title: book.title, author: book.author, price: book.price },
    { merge: true /* ドキュメントが存在する場合はフィールドを追記 */ }
  )
}
{{< /code >}}

実際にこのコードを呼び出すときは、Firestore のセキュリティルールで write 許可しておく必要があります。


次のステップ
----

- [Firebase Auth で admin ユーザーのみ Firestore に書き込みできるようにする](/p/dw9jt4e)
- [Firestore ドキュメントを TypeScript のユーザー定義型オブジェクトに変換する (withConverter)](/p/bw9kv6g)

