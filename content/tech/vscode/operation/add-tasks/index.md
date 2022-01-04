---
title: "VS Code でビルドタスクやテストタスクを登録する (tasks.json)"
url: "/p/zn2er4g"
date: "2020-06-10"
tags: ["VS Code", "Node.js"]
weight: 300
---

タスク設定とは
----

VS Code のビルドタスク設定 (`tasks.json`) をしておくと、 __`Cmd + Shift + B`__ (`Ctrl + Shift + B`) というショートカットキーで、任意のビルドタスクを実行できるようになります。

「ビルド」タスクと言っていますが、実際には任意のコマンドを実行することができます。
例えば、Node.js アプリを起動するための `npm start` や、シェル上でのコマンドを素早く実行できるようになります。

ここでは、下記のようなコマンドを実行するタスクを VS Code に登録してみます。

- `npm start` コマンド（Node.js アプリの起動）
- `echo` コマンド（Hello World と表示するだけ）


サンプルアプリの準備
----

`npm start` で起動するサンプルアプリとして、簡単な Node.js アプリを作成しておきます。
プロジェクト用のディレクトリと `package.json` を作成し、VS Code で開きます。

{{< code >}}
$ mkdir myapp
$ cd myapp
$ npm init -y    # package.json の生成
$ code .         # VS Code で開く
{{< /code >}}

VS Code にプロジェクトとして認識させるには、ファイルではなくディレクトリを開く必要があることに注意してください。
VS Code が開いたら、`Cmd + N` で新しくエディタを開き、次のような内容の `main.js` を保存します。

{{< code lang="js" title="main.js" >}}
console.log('Hello World');
{{< /code >}}

これで、サンプルアプリは完成です。
``Ctrl + ` `` でターミナルパネルを開いて、次のように実行できるかテストしましょう。

{{< code >}}
$ node main
{{< /code >}}

最後に、`npm start` でアプリ起動できるように、`package.json` に `start` スクリプトを登録しておきます。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "scripts": {
    "start": "node main"  // start スクリプト
  }
}
{{< /code >}}

これで、`node main` とする代わりに、`npm start` でアプリを起動できるようになります。


ビルドタスクの設定
----

VS Code で作成したコードを実行するときに、毎回ターミナルから `node main` とか `npm start` とか入力するのは面倒です。
VS Code のビルドタスクを設定すると、こういったコマンドを、__`Cmd + Shift + B`__ というショートカット一発で起動できるようになります（Windows の場合は `Ctrl + Shift + B`）。

### npm start をタスクとして登録する

まずは、`npm start` コマンドを VS Code のビルドタスクとして実行できるようにしてみましょう。
VS Code 上でビルドを実行するために `Cmd + Shift + B` と入力すると、最初は次のように `No build task to run found. Configure Build Task...` といったメッセージが表示されるはずです。

{{< image src="img-001.png" >}}

これは、「ビルドタスクが何も登録されていない」というメッセージなので、そのまま `Enter` キーを押して、ビルドタスクの登録に進みます。

{{< note >}}
`Configure Build Task` の項目が表示されない場合は、__`Cmd + Shift + P`__ でコマンドパレットを開き、__`Tasks: Configure Default Build Task`__ を選択します。
{{< /note >}}

{{< image src="img-002.png" >}}

すると、デフォルトのビルドタスクとして何を登録するかを選択するメニューが表示されるので、`npm start` を選択します。
VS Code は、`package.json` のスクリプト設定を認識し、タスクとして登録するものの候補を表示してくれます。

これで、次のようなタスク設定ファイル (`tasks.json`) が自動生成されます（コメントを追加しています）。

{{< code lang="js" title=".vscode/tasks.json" >}}
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "npm: start",  // コマンドパレットに表示される名前
      "detail": "node main",  // その下に表示される説明文
      "type": "npm",          // npm によるタスク実行
      "script": "start",      // 実行する npm スクリプト名
      "group": {
        "kind": "build",      // ビルドタスクとして認識させる
        "isDefault": true     // Cmd + Shift + B で即実行
      },
      "problemMatcher": []
    }
  ]
}
{{< /code >}}

このタスクは、デフォルトのビルドタスクとして登録されている (`"isDefault": true`) ので、今後は `Cmd + Shift + B` と入力するだけで、`npm start` が実行されるようになります。
ターミナルパネルに `main.js` の実行結果が表示されるか確認しましょう。

### 任意のコマンド (echo) をタスクとして登録する

次に、別のビルドタスクとして、`echo` コマンドを実行するタスクを登録してみます。
`tasks.json` の `tasks` プロパティに、次のように `Hello World` という名前のタスクを追加します。

{{< code lang="js" title=".vscode/tasks.json" >}}
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "npm: start",
      "detail": "node main",
      "type": "npm",
      "script": "start",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": []
    },
    {
      "label": "Hello World",
      "detail": "Hello World と出力するだけです。",
      "type": "shell",       // シェルコマンドを実行する
      "command": "echo",     // 実行するコマンド
      "args": [              // コマンドへ渡す引数
        "Hello World"
      ],
      "group": "build",      // ビルドタスクとして認識させる
      "presentation": {
        "reveal": "always",  // Terminal パネルを必ず開く
        "clear": true,       // 実行前に Terminal をクリア
        "echo": false        // タスク名をエコー表示しない
      },
      "problemMatcher": []
    }
  ]
}
{{< /code >}}

`type` で `shell` と指定するところと、`command` で実行したいコマンド（ここでは `echo`）を指定するところがポイントですね。

上記の例では、さらに __`presentation.clear`__ を __`true`__ に設定することで、タスク実行のたびにターミナルをクリアするようにしています。
前回の実行結果を参照する必要がない場合は、この設定をしておくと出力がスッキリするのでおすすめです。

このビルドタスク (`Hello World`) は、デフォルトタスクとしては登録していないので、`Cmd + Shift + B` コマンドで起動することはできません。
__`Cmd + Shift + P`__ でコマンドパレットを開き、__`Tasks: Run Build Task`__ を選択すると、タスク一覧が表示されるので、その中から `Hello World` タスクを選ぶことで実行できます。

{{< image src="img-003.png" >}}

タスク名は定義した順に表示されるのではなく、最近実行したものが上に表示されるみたいですね。


テストタスクの設定
----

上記ではビルドタスク (`kind: build`) を登録しましたが、テストタスク (`kind: test`) としてタスクを登録することもできます。
タスクを種類ごとにグルーピングして管理できるということです。

### サンプルのテストコマンドを用意する

ここでは、`package.json` にテスト用スクリプト (`test`) を追加し、それを VS Code のテストタスクとして実行できるようにしてみます。
テストコマンドは、次のように単純に `echo Test` するだけです。

{{< code lang="js" title="package.json" >}}
{
  "name": "myapp",
  "version": "0.0.1",
  "scripts": {
    "start": "node main",
    "test": "echo Test"  // テストを実行しているつもり
  }
}
{{< /code >}}

`package.json` に登録されているスクリプトは、VS Code がうまいこと認識してくれるので、実はこの時点でタスクとして実行できるようになっています。

1. __`Cmd + Shift + P`__ でコマンドパレットを開き、__`Tasks: Run Test Task`__ を選択
2. テストタスクの一覧が表示されるので、`npm: test` を選択

`package.json` でのスクリプト名が `test` になっているだけで、VS Code はテストタスクとして認識してくれるんですね。

### デフォルトのテストタスクとして登録する

このままでも十分使えますが、ここでは VS Code にデフォルトのテストタスクとしてちゃんと登録してみます。
`tasks.json` にテストタスクを追加するには、次のようにします。

1. __`Cmd + Shift + P`__ でコマンドパレットを開き、__`Tasks: Configure Default Test Task`__ を選択
2. 候補の中から `npm: test` を選択

これで、`tasks.json` にテストタスクが追加されます。

{{< code lang="js" title=".vscode/tasks.json" >}}
{
  "version": "2.0.0",
  "tasks": [
    // 省略
    {
      "label": "npm: test",
      "detail": "echo Test",
      "type": "npm",
      "script": "test",
      "group": {
        "kind": "test",
        "isDefault": true
      },
      "problemMatcher": []
    }
  ]
}
{{< /code >}}

ビルドタスクの場合は `group.kind` が `build` になっていましたが、テストタスクの場合は `test` になっています。
`npm test` がデフォルトのテストタスクとして登録されたので、`Cmd + Shift + P` のコマンドパレットから __`Tasks: Run Test Task`__ を選択するだけで `npm test` が実行されるようになります。

### テストタスク実行用のショートカットキーを登録する

もっと簡単にテストタスクを起動したいのであれば、テストタスクを起動するためのショートカットキーを登録しておくとよいでしょう（ビルドタスクには `Cmd + Shift + B` というショートカットキーがデフォルトで登録されていますが、テストタスクには登録されていません）。

ショートカットキーの設定画面は、 __`Cmd + K`__ → __`Cmd + S`__ で開くことができます。
ここでは、__`Tasks: Run Test Task`__ のショートカットキーを `Cmd + Shift + T` に設定します。

{{< image src="img-004.png" >}}

{{< note >}}
`Cmd + Shift + T` は、デフォルトでは `View: Reopen Closed Editor` に割り当てられていますが、テストタスクの方が頻繁に実行されるという想定で、ここでは `Tasks: Run Test Task` に割り当てちゃいました。
{{< /note >}}

