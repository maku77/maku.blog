---
title: "Deno のタスクランナーの使い方 (deno task)"
url: "p/ho4gs5h/"
lastmod: "2023-01-14"
tags: ["Deno"]
---

Deno のタスクランナーとは？ npm スクリプトとの違いは？
----

Deno は標準で [deno task コマンド](https://deno.land/manual/tools/task_runner) を提供しており、__`deno.json`__（あるいは __`deno.jsonc`__）の __`tasks`__ プロパティで定義したタスクを実行できるようになっています。
これは、Node.js で言うところの npm スクリプト (`npm run`) に相当するものですが、`deno task` の場合は、__クロスプラットフォームなタスクを定義できる__ ところが大きな特徴になっています。

{{< code lang="json" title="deno.json でのタスク定義" >}}
{
  "tasks": {
    "hello": "echo Hello, Deno Task Runner!"
  }
}
{{< /code >}}

{{< code lang="console" title="タスクの実行方法" >}}
$ deno task hello
{{< /code >}}

npm スクリプトの場合は、定義したタスクが OS のシェル上で実行されることが前提だったので、シェルの機能をうかつに使ってしまうと、Linux では動くけど Windows では動かないというタスクなってしまいがちでした。
例えば、ファイルをまとめて削除するタスクを作りたい場合、Linux に依存した `rm -rf` コマンドは使えず、いちいち `rimraf` モジュールをインストールするといったことが必要でした。
一方、Deno 場合は、クロスプラットフォームで動作する __組み込みコマンドとして `rm` コマンドを備えています__。
他にも、`cp`、`mv`、`mkdir`、`echo`、`cat` といったよく使うコマンドが、Linux や Windows、macOS などで共通で使えるようになっています。

また、Linux と Windows では、パイプやリダイレクトの記述方法も微妙に異なっていますが、Deno のタスクランナーでは、__組み込みでパイプやリダイレクトの構文を処理する__ ようになっており、連続したコマンド実行も OS のシェルに依存せずに定義できます。


jsonc フォーマットがおすすめ (JSON with Comments)
----

`deno.json` の代わりに、__`deno.jsonc`__ という名前で Deno の設定ファイルを作成すると、ファイル内にコメントを記述することができます。
JSONC は JSON with Comments の略です。

{{< code lang="js" title="deno.jsonc" >}}
{
  "tasks": {
    // データ収集と分析を実行します
    "data": "deno task collect && deno task analyze",
    // データ収集を実行します
    "collect": "deno run --allow-read=. --allow-write=. scripts/collect.js",
    // データ分析を実行します
    "analyze": "deno run --allow-read=. scripts/analyze.js"
  }
}
{{< /code >}}


タスクの一覧を表示する
----

`deno task` コマンドを __引数なしで実行__ すると、定義されているタスクの一覧を表示できます。

```console
$ deno task
Available tasks:
- analyze
    deno run --allow-read=. scripts/analyze.js
- collect
    deno run --allow-read=. --allow-write=. scripts/collect.js
- data
    deno task collect && deno task analyze
```

Deno プロジェクトを `git clone` してきたときに、`deno task` でできることがざっと分かるようになっていると親切ですね。


タスク実行時にコマンドライン引数を渡す
----

`deno task <タスク名> <引数...>` のように渡した引数は、タスクで定義されているコマンドに渡されます。

{{< code lang="json" title="deno.json" >}}
{
  "tasks": {
    "my-echo": "echo"
  }
}
{{< /code >}}

例えば、上記のように定義した `my-echo` タスクを、次のように引数 `A B C` 付きで実行すると、それらは `echo` に対して渡されます。

```console
$ deno task my-echo A B C
Task my-echo echo "A" "B" "C"
A B C
```

コマンドライン引数がどのようにタスクに渡されたかは、出力の 1 行目を見るとわかります。
この 1 行目の出力を抑制するには、`deno task` コマンドに __`-q (--quiet)`__ オプションを指定します。

```console
$ deno task -q my-echo A B C
A B C
```

