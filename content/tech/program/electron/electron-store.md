---
title: "Electron で設定情報を扱う (electron-store)"
url: "/p/ev6env5"
date: "2020-08-03"
tags: ["Electron"]
---

[electron-store パッケージ](https://github.com/sindresorhus/electron-store) を使用すると、Electron アプリ用の設定を簡単にローカルファイルとして保存できます。


electron-store のインストール
----

{{< code >}}
$ npm install --save electron-store
{{< /code >}}

`electron-store` パッケージには TypeScript 用の型定義も含まれているため、別途型定義ファイルをインストールする必要はありません。


electron-store の使い方
----

### 簡単な使い方

`electron-store` モジュールは、キー＆バリューの形で設定値を保存する簡単な API (`get`/`set`) を提供しています。
下記の例では、`color` というキーで色情報を取得・保存しています。

{{< code lang="typescript" title="main.ts" >}}
import ElectronStore from 'electron-store';
const store = new ElectronStore();

// 設定情報を取得する（第2パラメーターはデフォルト値）
const color = store.get('color', 'unknown');
console.log(color);

// 設定情報の保存する
store.set('color', 'blue');
{{< /code >}}

1度目の実行では、設定情報が保存されていないので、`get` メソッドは第2引数で指定した `unknown` を返します。
2度目の実行では、前回保存した `blue` という値を取得できます。

### 設定値の型を定義する

TypeScript を使っている場合は、`get()` で取得した値を特定の型の変数で受け取ることになると思います。
`ElectronStore` はジェネリクスクラスとして定義されており、各設定キーの型をあらかじめ指定しておくことができます。
次の例では、`color` 設定を文字列型として宣言しています。

{{< code lang="typescript" >}}
import ElectronStore from 'electron-store';

type StoreType = {
  color: string;
}
const store = new ElectronStore<StoreType>();
const color: string = store.get('color', 'yellow');
{{< /code >}}

設定が保存されていないことを `undefined` で表したい場合は、次のように `undefined` 許容型で扱うことを明示しておく必要があります。

{{< code lang="typescript" >}}
type StoreType = {
  color: string | undefined;
}
const store = new ElectronStore<StoreType>();
const color: string | undefined = store.get('color');
if (!color) {
  console.log('設定値が見つかりません');
}
{{< /code >}}

### Settings モジュールを作成する

アプリの設定情報をモジュールの形で参照できるようにしておくと便利です。
下記の `settings` モジュールは、`name` 設定と `age` 設定を扱うための `Settings` オブジェクトを公開します。

{{< code lang="typescript" title="settings.ts" >}}
import ElectronStore from 'electron-store';

// 各設定値の型
type Type = {
  name: string | undefined;
  age: number | undefined;
};

// 設定値の取得と保存を行うためのクラス
class _Settings {
  private store = new ElectronStore<Type>();

  // name 設定の取得と保存
  get name(): Type['name'] { return this.store.get('name'); }
  set name(value: Type['name']) { this.store.set('name', value); }

  // age 設定の取得と保存
  get age(): Type['age'] { return this.store.get('age'); }
  set age(value: Type['age']) { this.store.set('age', value); }
}

export const Settings = new _Settings();
{{< /code >}}

設定値を扱いたいところで、次のようにインポートするだけで、設定値の参照・保存を簡単に行えます。

{{< code lang="typescript" >}}
import {Settings} from './settings';

console.log(Settings.name);  // 最初は undefined、次からは 'maku' になる
Settings.name = 'maku';  // 自動的にファイルに保存される
{{< /code >}}


{{< note title="Renderer プロセスでの設定値参照" >}}
Renderer プロセスからファイルアクセスするのはよくないとされているので、これに従うのであれば、上記の `Settings` モジュールも Renderer プロセスからは直接参照しない方がよさそうです。
Renderer プロセスは `ipcRenderer.invoke()` で Main プロセスへ設定値を要求し、Main プロセスは `ipcMain.handle()` で要求された設定値を返す、というやり方にすれば OK です。

参考: [How do I get store values in the renderer process, when my store was initialized in the main process?](https://github.com/sindresorhus/electron-store#how-do-i-get-store-values-in-the-renderer-process-when-my-store-was-initialized-in-the-main-process)
{{< /note >}}


設定ファイルの保存場所
----

`electron-store` が保存する設定ファイルは、`app.getPath('userData')` ディレクトリ以下に __`config.json`__ という名前で保存されます。
具体的なファイルパスは次のような感じになります。

{{< code >}}
/Users/maku/Library/Application Support/MyApp/config.json
{{< /code >}}

このファイルパスは、`ElectronStore` オブジェクトの __`path`__ プロパティで取得することができます。

{{< code lang="typescript" >}}
import ElectronStore from 'electron-store';
const store = new ElectronStore();
console.log(store.path);
{{< /code >}}


その他
----

- `store.openInEditor()` ... エディタで `config.json` ファイルを開く
- `store.clear()` ... 設定値をすべてクリアする

