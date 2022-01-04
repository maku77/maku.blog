---
title: "MongoDB for VS Code で Azure Cosmos DB を操作する"
url: "/p/dt3ahpw"
date: "2021-06-12"
tags: ["Azure", "Cosmos DB", "MongoDB", "VS Code"]
---

Cosmos DB インスタンスを [MongoDB API アクセス用に作成](/p/cd9bg3x) しておくと、さまざまな MongoDB 用のツールでデータベースにアクセスできるようになります。
ここでは、VS Code 用の拡張「__MongoDB for VS Code__」を使って、Cosmos DB を操作できるようにしてみます。
TypeScript を使って Web アプリを作成しているときは、エディタとして VS Code を使っていることが多いでしょうから、同じ環境上で Cosmos DB を操作できると開発が捗ります。


MongoDB for VS Code のインストール
----

{{< image border="true" src="img-001.png" title="MongoDB for VS Code のインストール" >}}

MongoDB for VS Code は、VS Code の Extesions バー (`Cmd/Ctrl + Shift + X`) で __`MongoDB`__ で検索すれば簡単にインストールできます。


VS Code から Cosmos DB (MongoDB) に接続する
----

### 接続文字列で簡単接続

MongoDB for VS Code をインストールすると、サイドバーに __葉っぱのアイコン__ が出てくるので、ここから MongoDB サーバーに接続することができます。
ちなみに、MongoDB が葉っぱアイコンを使うのは、それを使うことが「シンプルで自然であるから」らしいです（じゃあ水でもいいじゃん、とは言いますまい）。

{{< image border="true" src="img-002.png" title="MongoDB サーバーへ接続する" >}}

`Connext` ボタンを押して、`mongodb://` で始まる接続文字列を指定するだけで簡単に MongoDB サーバーへ接続できます（接続文字列は [Azure Portal](https://portal.azure.com/) の Cosmos DB リソースのページで確認してください）。

### Playgrounds 機能でスクリプト実行

接続に完了すると、__Playgrounds__ 機能でデータベースを操作できるようになります。
これは次のような `.mongodb` 拡張子を持つファイルのエディタ機能で、右上の実行ボタンを押すと、接続先の MongoDB データベース（今回の例では Cosmos DB データベース）を操作できます。

{{< image border="true" src="img-003.png" title="Playgrounds 機能" >}}

MongoDB for VS Code がインストール済みの環境であれば、以降はこの `.mongodb` ファイルを開くだけで Playgrounds 機能が起動します。
このファイルをプロジェクトのリポジトリにコミットしておけば、データベース操作のサンプルコードとして使用できます。

