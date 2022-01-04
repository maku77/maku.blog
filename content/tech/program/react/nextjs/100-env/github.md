---
title: "Next.js アプリのソースコードを GitHub で管理する"
url: "/p/xjw8it5"
date: "2021-04-24"
tags: ["Next.js", "React"]
weight: 103
---

Next.js のプロジェクトは、`create-next-app` コマンドで作成済みであるとします。

- 参考: [Next.js で HelloWorld](/p/ubmu3bj)

`create-next-app` コマンドで Next.js アプリを新規作成すると、アプリのトップディレクトリに、Git リポジトリ (`.git/`) が自動的に生成されます。
`git log` コマンドを実行すると、初回のコミットがすでに生成されていることがわかります。

{{< code >}}
$ git log
commit 4607ecdd2f3f9f3d0d4c86442463c3b86856e88a (HEAD -> main)
Author: maku <maku@example.com>
Date:   Sun Apr 18 23:28:22 2021 +0900

    Initial commit from Create Next App
{{< /code >}}

この Next.js アプリを GitHub で管理したいときは、以下の手順で GitHub リポジトリと結びつけます。

1. GitHub 上に [新規リポジトリを追加](https://github.com/new) します
    - リポジトリ名は `create-next-app` で指定したアプリ名と合わせましょう。
    - `Initialize this repository with:` の項目では、`README.txt` の作成にはチェックを入れないでください。リポジトリファイル (`.git`) はすでに `create-next-app` によって生成されているので、GitHub 側のリポジトリは空の状態で作成する必要があります。
2. 既存の Next.js アプリを上記の GitHub リポジトリに結びつけます
    ```
    $ cd myapp
    $ git remote add origin https://github.com/<username>/myapp.git
    ```
3. 既存の Next.js アプリの内容（前述の初回コミット）をプッシュします
    ```
    $ git push -u origin main
    ```

あとは、コード修正しながらコミット＆プッシュを繰り返し、Next.js アプリを育てていけば OK です。

