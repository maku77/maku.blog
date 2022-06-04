---
title: "Firebase Auth で admin ユーザーのみ Firestore に書き込みできるようにする"
url: "/p/dw9jt4e"
date: "2022-02-13"
tags: ["Firebase"]
---

何をするか？
----

ユーザーデータの管理に Firestore を使うと、Web アプリやモバイルアプリから手軽にデータを読み書きできるので非常に便利です。
一方で、データの書き込み (write) を誰にでもできるようにしていると、大切なデータを勝手に書き換えられてしまいます。

{{< code lang="js" title="Firestore の危険なセキュリティルールの例" >}}
rules_version = '2';

service cloud.firestore {
  match /databases/{db}/documents {
    match /{doc=**} {
      allow read, write: if true;  // 危険！
    }
  }
}
{{< /code >}}

ここでは、Firestore へのデータ書き込みを admin ユーザー（自分）だけに制限してみます。

なお、[FirebaseApp インスタンスの初期化 (initializeApp)](/p/73eq2cm) や、[Firebase Auth の導入](/p/8t6gq2b) 自体は完了しているものとします。
ユーザーごとにアクセス制御をしたいので、Firebase Auth による認証処理はほぼ必須になります。
とはいえ、Firebase アプリの認証まわりの実装はとても簡単で、最初に FirebaseApp インスタンスを初期化して、UI ライブラリ（`react-firebaseui` など）を使ってサインイン画面を出してユーザーにサインインさせるさせるだけです。
サインイン状態は Firebase が内部で管理してくれるので、Firestore API の呼び出し時には特に意識する必要はありません。
Firestore サービス側のセキュリティルールで、認証状態を意識した制限をかければよいだけです。


何をもって admin ユーザーとするか？
----

Firestore のデータ書き込み (write) を admin ユーザーだけに制限するといっても、そもそも何をもって admin ユーザーとみなすのかという問題があります。
このあたりは、ひとことで言うと「自由」です。
例えば、次のようなやり方が考えられます。

- 特定のユーザー ID (UID) を持つユーザーを admin とみなす
- Firestore の `users` コレクション以下にユーザー情報（ドキュメント）を格納しておいて、そのドキュメントに `admin: true` というデータが含まれていたら admin ユーザーとみなす

1 つ目の方法はちょっと横着なやり方ですが (^^；、とりあえず自分にだけ write 権限を与えておきたい、というときに手っ取り早いかもしれません。

よくサンプルコードなどで見かけるのは、後者のように各ユーザーの情報を保持する Firestore コレクションを用意する方法です。
Web サイトにサインイン機能を付けると、詳細なユーザー情報を Firestore で管理したくなります。
そのユーザー情報の中に、admin フラグを持たせるということですね。


admin ユーザーにのみ Firestore の write 権限を付ける
----

### 特定の UID をもつユーザーを admin とする方法

まず、Firebase Auth のページで、対象とするユーザーの UID を確認しておきます。

{{< image border="true" src="img-001.png" >}}

あとは、Firestore のセキュリティルールで、そのユーザーがアクセスしてきたときのみ、write アクセスを許可するように設定してやります。
下記の例では、アクセスしてきたユーザーが admin ユーザーかどうかを判断するための `isAdmin()` ユーザー関数を定義しています。
`isAdmin()` が true を返した場合に、`/books` コレクション内のドキュメントへの書き込み (write) を許可しています。

```js
rules_version = '2';

service cloud.firestore {
  function isAdmin() {
    return request.auth != null &&
      request.auth.uid == 'E9pvOlKxcHUd7RuekoGFiEj4XIW2'
  }

  match /databases/{db}/documents {
    match /books/{doc=**} {
      allow read;
      allow write: if isAdmin();
    }
  }
}
```

### Firestore の users コレクションで admin フラグを管理する方法

Firestore に `users` コレクションを作成して、次のような UID をドキュメント ID としたデータを入れておき、その中の admin フラグを使って admin ユーザーかどうかを管理する方法です。

{{< image border="true" src="img-002.png" >}}

セキュリティルールの中では、`isAdmin` 関数をちょっと書き換えて、`users` コレクションのデータを参照するようにします。
あと、データベース名の変数 (`db`) を参照するために、`isAdmin` 関数の階層をひとつ下に移動していることに注意してください。

```js
rules_version = '2';

service cloud.firestore {
  match /databases/{db}/documents {
    function isAdmin() {
      return request.auth != null &&
        exists(/databases/$(db)/documents/users/$(request.auth.uid)) &&
        get(/databases/$(db)/documents/users/$(request.auth.uid)).data.admin == true;
    }

    match /books/{doc=**} {
      allow read;
      allow write: if isAdmin();
    }
  }
}
```

もう少しシンプルな方法としては、`users` の代わりに `admins` コレクションを用意して、そこに対象ユーザーのドキュメントが存在するときは admin ユーザーとみなすというやり方も考えられます。

```js
rules_version = '2';

service cloud.firestore {
  match /databases/{db}/documents {
    function isAdmin() {
      return request.auth != null &&
        exists(/databases/$(db)/documents/admins/$(request.auth.uid))
    }

    match /books/{document=**} {
      allow read;
      allow write: if isAdmin();
    }
  }
}
```

この場合は、アクセスしてきたユーザーの UID が `E9pv...` だとしたら、Firestore に `/admins/E9pv...` というドキュメントが存在すれば admin ユーザーだとみなします。
ドキュメントの中身は空っぽで構いません。

以上、認証済みユーザーによるアクセス制御方法をいくつか見てきましたが、いずれの方法も簡単かつ柔軟に設定できることがわかると思います。
この手軽さに慣れてしまうと、なにかと複雑な AWS には近づきたくなくなっちゃいます(^^;

