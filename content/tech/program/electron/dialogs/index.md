---
title: "Electron で各種ダイアログを表示する (dialog)"
linkTitle: "各種ダイアログを表示する (dialog)"
url: "/p/ihwanyc"
date: "2020-06-29"
tags: ["Electron"]
---

`electron` モジュールが提供する __`dialog`__ を使用すると、いろいろなダイアログを表示することができます。
ダイアログの表示は、メインプロセスからしか行えません。

- 参考: [Electron Documentation - dialog](https://www.electronjs.org/docs/api/dialog)


エラーボックス (showErrorBox)
----

一番簡単に使えそうなエラーボックスから。
ダイアログのタイトルと、メッセージの内容を指定するだけで表示できます。

- `dialog.showErrorBox(title, content)`

{{< image w="400" src="img-error-box.png" >}}

{{< code lang="typescript" >}}
import { dialog } from 'electron';

dialog.showErrorBox('File Not Found','指定されたファイルが見つかりませんでした。');
{{< /code >}}

このエラーボックスが表示されている間は、他の全てのウィンドウが操作できなくなるようです（つまり、モーダルダイアログとして振る舞います）。

ドキュメントによると、この `dialog.showErrorBox` は、「`app` モジュールが `ready` イベントを emit する前に呼び出すことができる」とされています。
つまり、アプリケーションの起動時に行うエラーチェックなどで使用できるということです（メインウィンドウ表示前に設定ファイルの内容が正しいか調べたり）。

レンダラープロセス側では `alert()` を使っちゃうことが多いかもしれません。


メッセージボックス (showMessageBox, showMessageBoxSync)
----

メッセージボックスを表示するには、次のような関数を使用します。

- `dialog.showMessageBox(options)`
- `dialog.showMessageBox(browserWindow, options)`
- `dialog.showMessageBoxSync(options)`
- `dialog.showMessageBoxSync(browserWindow, options)`

表示内容を `options` パラメータでいろいろ制御できます（下記に例を示します）。

メッセージボックスが表示されている間は、基本的には他のウィンドウを操作できなくなります。
`browserWindow` パラメーターで、親ウィンドウとなる `BrowserWindow` インスタンスを指定すると、その親ウィンドウのモーダレスダイアログとなり、その親ウィンドウだけ操作できなくなります。

{{< note >}}
`showMessageBoxSync()` にも親ウィンドウを指定するバージョンがありますが、この関数を呼び出すと結局他のウィンドウも操作できなくなります。
いまいち使いどころが分かりませんが、親ウィンドウを指定することで、親ウィンドウの表示位置に近いところにダイアログが表示されるという効果は得られます。
{{< /note >}}

### 例: OK ボタンだけあるダイアログ

{{< image w="400" src="img-message-box1.png" >}}

{{< code lang="typescript" >}}
const options: Electron.MessageBoxOptions = {
  type: 'info',  // none/info/error/quetion/warning
  title: 'タイトル',
  message: 'メッセージ',
  detail: '説明文'
};
dialog.showMessageBox(options);
{{< /code >}}

オプションで `buttons` の指定を省略すると、デフォルトで OK ボタンがひとつだけあるダイアログが表示されます。
`type` オプションの指定により表示されるアイコンが変化したりします。

### 例: 複数のボタンがあるダイアログ

{{< image w="500" src="img-message-box2.png" >}}

{{< code lang="typescript" >}}
const options: Electron.MessageBoxSyncOptions = {
  type: 'question',  // none/info/error/question/warning
  title: 'タイトル',
  message: 'メッセージ',
  detail: '説明文',
  buttons: ['OK', 'Cancel', 'ボタン1', 'ボタン2'],
  cancelId: -1  // Esc で閉じられたときの戻り値
};

const selected: number = dialog.showMessageBoxSync(options);
console.log(`${selected} 番目が選択されました`);
{{< /code >}}

__`buttons`__ オプションで、表示するボタンを指定することができます。
OS によって右から並べられたりするようなので（macOS では上の図のように右からになった）、ボタン名に連番などは振らない方がよいでしょう。

ユーザーが選択したボタンのインデックス（0始まり）は、戻り値で取得することができます。
上記の例の場合は、各ボタンを押したときに次のような値が返されます。

- `OK` ... 0
- `Cancel` ... 1
- `ボタン1` ... 2
- `ボタン2` ... 3

`Esc` キーなどでキャンセルした場合は、デフォルトでは `Cancel` や `No` といったテキストを含むボタンのインデックスが返されるようになっていますが、__`cancelId`__ オプションで任意の値に変更することができます。
ここでは同期版の `showMessageBoxSync()` を使用していますが、代わりに `showMessageBox` を使用すると、戻り値の型が `Promise` 型に変わるので注意してください（使用例は下で説明します）。

### 例: 非同期に複数のダイアログを表示する

{{< image w="600" src="img-message-box3.png" >}}

`showMessageBox()` でダイアログを表示するときに親ウィンドウを指定すると、その他のウィンドウは操作可能な状態でキープされます。
次の例では、2 つのウィンドウ（`win1`、`win2`）が表示されている状態で、それぞれのウィンドウを親とするモーダルダイアログを同時に表示しています。

2 つのダイアログは、どちらも操作可能な状態になっており、どちらのダイアログを先に操作するかはユーザーが自由に選択できます。
ユーザーが選択したボタンのインデックスは、`showMessageBox()` の戻り値である `Promise<Electron.MessageBoxReturnValue>` オブジェクトから非同期に取り出すことができます。

{{< code lang="typescript" >}}
// 2つのウィンドウを表示
const win1 = new BrowserWindow({ title: 'win1', width: 500, height: 200, })
const win2 = new BrowserWindow({ title: 'win2', width: 500, height: 200, })

// ダイアログの設定
const options: Electron.MessageBoxOptions = {
  type: 'question',
  message: '確認',
  detail: '本当に削除しちゃっていいですか？',
  buttons: ['削除', 'キャンセル'],
};

// 2つのダイアログを同時に表示（それぞれ別の親ウィンドウを持つ）
const result1: Promise<Electron.MessageBoxReturnValue> =
  dialog.showMessageBox(win1, options);
const result2: Promise<Electron.MessageBoxReturnValue> =
  dialog.showMessageBox(win2, options);

// 結果はそれぞれ非同期に取得できる
result1.then(val => console.log(val.response));
result2.then(val => console.log(val.response));
{{< /code >}}
