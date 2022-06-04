---
title: "Firestore ドキュメントを TypeScript のユーザー定義型オブジェクトに変換する (withConverter)"
url: "/p/bw9kv6g"
date: "2022-02-12"
tags: ["Firebase/Firestore"]
---

Firestore の JavaScript SDK でドキュメントを参照するときに、__`withConverter`__ 関数を組み合わせて使用すると、ドキュメントの読み書きを行うときに、TypeScript のユーザーデータ型との変換処理を自動的に呼び出すことができます。

例えば、次のような `Book` 型のデータを、Firestore の `books` コレクションに保存するとします。

```ts
export type Book = {
  id: string
  title: string
  price?: number
}
```

Firestore ドキュメントとユーザーデータ型の変換処理は、次のように [FirestoreDataConverter](https://firebase.google.com/docs/reference/js/firestore_.firestoredataconverter) インタフェースを実装する形で定義します。
今回は `Book` 型に `id` プロパティを含めましたが、Firestore はドキュメントの ID をパス情報として表現するので、そのあたりの変換処理に注意する必要があります。

```ts
// import {
//   DocumentData, FirestoreDataConverter, QueryDocumentSnapshot,
//   SnapshotOptions, serverTimestamp } from 'firebase/firestore'

/**
 * Firestore のドキュメントと Book オブジェクトの型変換を行います。
 */
const bookConverter: FirestoreDataConverter<Book> = {
  /**
   * Book オブジェクトを Firestore ドキュメントデータへ変換します。
   */
  toFirestore(book: Book): DocumentData {
    // id は Firestore のパスで表現されるのでドキュメントデータには含めない。
    // 下記の updatedAt のように、自動で更新時刻のフィールドを追加することも可能。
    return {
      title: book.title,
      price: book.price,
      updatedAt: serverTimestamp(),
    }
  },

  /**
   * Firestore ドキュメントデータを Book オブジェクトへ変換します。
   */
  fromFirestore(snapshot: QueryDocumentSnapshot, options: SnapshotOptions): Book {
    const data = snapshot.data(options)
    // Book オブジェクトの id プロパティには Firestore ドキュメントの id を入れる。
    return {
      id: snapshot.id,
      title: data.title,
      price: data.price,
    }
  },
}
```

あとは、次のように、`doc` や `collection` で Firestore のデータを参照するときに、`withConverter` で上記のコンバーターオブジェクトを渡してやります。

```ts
/** Firestore の books コレクションにドキュメントを追加する。 */
export async function addBook(book: Book) {
  const db = getFirestore()
  const docRef = doc(db, 'books', book.id).withConverter(bookConverter)
  await setDoc(docRef, book)
}

/** Firestore から books コレクションを読み込む。 */
export async function getBooks(): Promise<Book[]> {
  const db = getFirestore()
  const collRef = collection(db, '/books').withConverter(bookConverter)
  const snapshot = await getDocs(collRef)
  return snapshot.docs.map((doc) => doc.data())
}
```

これで、実際に `setDoc` や `getDocs` でデータアクセスするときに、自動的にデータ変換処理が行われるようになります。

