---
title: "NPM パッケージを作るときの package.json ファイルの書き方に関してのメモ"
url: "/p/ryq6it6"
date: "2022-03-16"
tags: ["Node.js"]
---

name, version フィールド
----

`name` と `version` フィールドは、パッケージを公開するつもりがないなら指定する必要はありません。


description フィールド
----

`description` プロパティは、ユーザーがこのパッケージを探しやすくするための説明文で、`npm search` を実行したときに表示されます。


パッケージ対象外にするファイル (.npmignore / .gitignore)
----

NPM パッケージを作るときに、`.npmignore` に書かれたファイルはパッケージングされなくなります。
`.npmignore` ファイルがない場合は、`.gitignore` ファイルが代わりに参照されます。
これらの設定にかかわらず、下記のファイルは必ずパッケージングされます。

- package.json
- README（大文字小文字と拡張子は問わない）
- CHANGES / CHANGELOG / HISTORY（大文字小文字と拡張子は問わない）

- LICENSE / LICENCE（大文字小文字と拡張子は問わない）

- NOTICE（大文字小文字と拡張子は問わない）

- `main` フィールドで指定されたファイル


bin フィールド
----

NPM パッケージで何らかの実行コマンドを提供したいときは、`bin` フィールドを使用します。
例えば、`mycommand` コマンドを提供するときは次のように記述します。

```js
{
  // ...
  "bin": { "mycommand": "./cli.js" }
}
```

単独のコマンドをインストールするための NPM パッケージを作る場合は、`bin` フィールドのコマンド名を省略して次のように記述できます。

```js
{
  "name": "mycommand",
  "version": "1.2.5",
  "bin": "./path/to/program.js"
}
```

上記の `bin` フィールドは次のように記述するのと同様に振舞います。

```js
{
  // ...
  "bin": { "mycommand": "./path/to/program.js" }
}
```

repository
----

`repository` フィールドでは、ソースコードの場所を示すことができます。
例えば、GitHub のリポジトリで管理している場合は次のような感じ。

```js
{
  // ...
  "repository": {
    "type": "git",
    "url": "https://github.com/maku77/myapp.git"
  }
}
```

`npm` 1.1.65 移行は、`url` 部分は省略して次のように書くことができます。

```js
{
  // ...
  "repository": {
    "type": "git",
    "url": "maku77/myapp"
  }
}
```

