---
title: "Azure DevOps で無料のプライベート Git リポジトリ (Repos) を使用する"
url: "/p/qt5qyzu"
date: "2019-09-30"
tags: ["Azure", "Git"]
weight: 1
---

Azure DevOps について
----

{{< image w="600" src="repos-001.png" border="true" >}}

GitHub は Microsoft によって買収されましたが、**Azure ブランドの DevOps サービスでも Git リポジトリを扱う Repos という機能が提供されています**。
GitHub は 2019 年から無制限に Private リポジトリを作成できるようになりましたが、Azure DevOps の方も無制限に Private リポジトリを作成することができ、5 ユーザーまでのコラボレーションが無料です（GitHub は 3 ユーザーまで）。
DevOps の Repos でも、GitHub のようにプルリクエストを使ったレビューを行えます。

さらに、**DevOps には Pipelines という、継続的インテグレーション/継続的デリバリー (CI/CD) の機能も含まれており**、こちらも毎月 1,800 分の実行まで無料で使用できます。
GitHub Actions なども同じような機能を提供する予定であり、これから Azure DevOps とどのような関係で進化していくのかわかりませんが、現状では Azure DevOps は魅力的な選択肢と言えそうです。


Azure DevOps の Git リポジトリ (Repos) を使用する
----

DevOps で Git リポジトリを作成する場合、まずは DevOps のプロジェクトを作成し、その中に Git リポジトリを作成していくという構成になります。
1 つの DevOps プロジェクトには、いくつでも Git リポジトリを作成できます。

### DevOps プロジェクトと Git リポジトリの作成

- [Azure Repos | Microsoft Azure](https://azure.microsoft.com/ja-jp/services/devops/repos/)

Azure Repos の <samp>無料で始める</samp> ボタンを押して、DevOps プロジェクトを作成します。

{{< image w="550" src="repos-002.png" border="true" >}}

Azure DevOps のプロジェクト作成画面に飛ぶので、プロジェクト名を入力して <samp>Create project</samp> を押せば完成です。
ここでは、`HelloWorld` という名前のプロジェクトを作成しています。

{{< image src="repos-003.png" border="true" >}}

作成したプロジェクトのダッシュボードが開いたら、左側のタブから **Repos** という項目を選択すると、プロジェクト内の Git リポジトリの一覧を確認することができます。
デフォルトでは、DevOps プロジェクトの名前と同じ名前の Git リポジトリ（ここでは `HelloWorld`）が 1 つだけ作成された状態になっています。
上部のヘッダーのプルダウンメニュー（リポジトリアイコンが付いた部分）を開くと、<samp>New repository</samp> という項目があり、ここからいくつでも Git リポジトリを追加できます。


### git clone できるようにする

ローカルの PC から、この Git リポジトリをクローンできるようにするには、アクセス用の Git credentials（要するにユーザー名とパスワード）を設定しておく必要があります。

左側のメニューから <samp>Repos / Files</samp> を選択し、<samp>Generate Git credentials</samp> ボタンを押すことでユーザー名とパスワードを登録するフォームが開きます。
アクセストークンを生成してそちらを使う方法もありますが、ここでは単純にユーザー名＋パスワードのペアでアクセスすることにします。

{{< image src="repos-004.png" border="true" >}}

<samp>Alias</samp> の欄にユーザー名、<samp>Password</samp> の欄にパスワードをそれぞれ入力し、<samp>Save Git Credentials</samp> ボタンを押せば設定完了です。

あとは、表示されている URL を使って、`git clone` コマンドを実行すれば、リポジトリをクローンすることができます。
この際、URL の `@` の前の部分だけ、設定したユーザー名 (Alias) に置き換えて実行してください。

{{< code >}}
$ git clone https://<ユーザー名>@dev.azure.com/makkuma/HelloWorld/_git/HelloWorld
Cloning into 'HelloWorld'...
Password for 'https://<ユーザー名>@dev.azure.com': ********
remote: Azure Repos
remote: Found 4 objects to send. (43 ms)
Unpacking objects: 100% (4/4), done.
Checking connectivity... done.
{{</ code >}}

これで、ローカル PC 上に `HelloWorld` ディレクトリが生成されます。

ちなみに、このユーザー名とパスワードは、DevOps プロジェクト内のすべての Git リポジトリに共通の設定となります（新しく Git リポジトリを作成するごとに登録する必要はありません）。

