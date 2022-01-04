---
title: "Azure Functions に npm install で Node モジュールを追加する"
url: "/p/9t7hs4e"
date: "2020-08-17"
tags: ["Azure", "Azure Functions"]
---

Azure Functions への npm install
----

Azure Functions の関数を Node.js ランタイムで動かしている場合は、通常の Node.js アプリと同様に NPM パッケージを `npm` コマンドでインストールして使用することができます。
簡単なのは、[Azure ポータル](https://portal.azure.com/) の Function App リソースの __`コンソール`__ 画面から `npm install` を実行する方法です。

{{< image border="true" src="img-001.png" >}}

コンソールを開くと、Function App のルートディレクトリ（通常は `D:\home\site\wwwroot`）がカレントディレクトリになってプロンプトが表示されます。
ここから次のように `package.json` の作成と、Node モジュールのインストールを行うことができます。

{{< code >}}
D:\home\site\wwwroot> npm init -y
D:\home\site\wwwroot> npm install node-fetch --save
{{< /code >}}

これで、この Functions プロジェクト内のすべての関数から、インストールした Node モジュールを使用できるようになります。


npm install を実行するディレクトリ
----

Functions プロジェクトのディレクトリ階層は次のように、関数ごとにディレクトリが分かれています。

{{< code >}}
D:\home\site\wwwroot
   +-- host.json
   +-- MyFunc1
   |    +-- function.json
   |    +-- index.js
   +-- MyFunc2
        +-- function.json
        +-- index.js
{{< /code >}}

`npm init` および `npm install` を実行するディレクトリは、プロジェクトのルートディレクトリ、あるいは、関数ごとのディレクトリになります。

- {{< label "プロジェクトルートで実行" >}} ... すべての関数から Node モジュールを参照可能
- {{< label "関数ごとのルートで実行" >}} ... その関数からのみ Node モジュールを参照可能

特に理由がない場合は、ストレージ容量を節約するために、プロジェクトルートで `npm install` を実行することが推奨されています。
関数ごとに異なるバージョンの Node モジュールを使用したい場合は、関数のディレクトリ内で `npm install` します。

{{< private >}}
[Dependency Management](https://docs.microsoft.com/ja-jp/azure/azure-functions/functions-reference-node#dependency-management)
{{< /private >}}

