---
title: "Azure: デプロイスロットでリリース時のダウンタイムをなくす（Blue-Green デプロイメント）"
url: "/p/bog7iq8"
date: "2020-02-12"
tags: ["Azure"]
---

なぜデプロイスロットが必要か？
----

App Service を作成すると、デフォルトでは運用スロット (production slot) がひとつだけ作成され、Azure Repos や GitHub からのアプリケーションのデプロイ先として使用されます。

アクセスの少ないアプリケーションであればこれでも問題ありませんが、デプロイ後にサーバー上でのビルド処理（ウォームアップ）が走るようなケースでは、少なからずダウンタイムが発生してしまいます。

このようなダウンタイムの発生を防ぐために、ステージング用のスロット (staging slot) を用意し、そこでのウォームアップが完了した後で運用スロット (production slot) と入れ替えるという方法を取ります。

{{< code >}}
GitHub or
AzureRepos  ─デプロイ→  staging スロット
                            ↑
                         (スワップ)
                            ↓
                        production スロット
{{< /code >}}

このようにサーバーインスタンス（ここではスロット）を 2 つ用意して、内容をスワップ（実際はアドレスをスワップ）することで運用環境を瞬間的に入れ替える手法を **Blue-Green デプロイメント** とか、A/B アップデートと呼びます。

Azure の App Service は、 **デプロイスロット (Deployment slots)** という機能名で、Blue-Green デプロイメントをサポートしています。

{{< image w="400" border="true" src="img-001.png" title="App Service の Deployments slots 設定" >}}

デプロイスロットを使ったスワップ運用には次のような利点があります。

1. ステージング環境 (staging slot) で事前に動作検証できる
2. 運用環境 (production slot) のダウンタイムをほぼゼロにできる
3. 運用環境 (production slot) で問題が発生したら、再度スワップして前のバージョンに戻すことができる


App Service にステージング環境用のデプロイスロットを追加する
----

### デプロイスロットの作成

まず、App Service に新しくステージング環境用のデプロイスロット (**Deployment slots**) を作成します。
対象となる App Service のリソースを選択し、次のように選択するとデプロイスロットの追加ダイアログが開きます。

1. `Deployment` カテゴリの `Deployment slots` を選択する。
2. `Add Slot` ボタンを押す。

追加ダイアログでは、スロットの名前を付けられるので、ここでは `staging` という名前を付けてみます。
また、既存の App Service の設定をコピーするよう指定します。

{{< image w="400" border="true" src="img-002.png" title="デプロイスロットの追加" >}}

次のように新しいスロットが追加さされれば成功です。

{{< image border="true" src="img-003.png" >}}

1 つ目に表示されているのはデフォルトで作成されている運用スロット (production slot) で、ユーザーが Web アプリにアクセスしたときにはこちらのスロットの内容が表示されます。
このフォームの `TRAFFIC` の割合を変更すれば、ユーザーのアクセス先を別のスロットに振り分けるともできます（ここではやりません）。

App Service に新しいデプロイスロットが作成されると、そこで動作する Web アプリには、`AppService名-スロット名.azurewebsites.net` というアドレスでアクセスできるようになります。
上記の例の場合は、Web アプリのルート URL は、

{{< code >}}
https://my-app-staging.azurewebsite.net/
{{< /code >}}

になります。

### デプロイスロットの削除

作成したデプロイスロットを削除したくなった場合は、 **削除対象のデプロイスロットのリソースの Overview** に表示される `Delete` ボタンを押します。
All resources で表示されるリソース一覧で、Type が `App Service (Slot)` となっているものが追加されたデプロイスロットのリソースです。
作成元の App Service の Deployment slots メニューからは削除できないみたいです。


ステージング環境用のスロットへのデプロイ設定 (Deployment Center)
----

Blue-Green デプロイメントにおけるリリースの流れは、作成したアプリケーションをステージング環境 (staging slot) にデプロイしてから、運用環境 (production slot) とスワップするという流れになります。

1. ステージング環境 (staging slot) へアプリをデプロイ
2. ステージング環境 (staging slot) でアプリを動作確認
3. 運用環境 (production slot) とスワップ

つまり、Web アプリの直接のデプロイ先は、運用環境 (production slot) ではなく、ステージング環境 (staging slot) です。
GitHub や Azure Repos などの Git リポジトリへソースコードをプッシュしたときに、自動的にビルドやデプロイを実行するには、 **ステージング環境 (staging slot) 側のデプロイセンター (Deployment Center)** を設定します。
運用環境 (production slot) へのリリースは、ステージング環境 (staging slot) とのスワップという形で実現されるため、運用環境 (production slot) 側のデプロイセンターに設定は必要ありません。

{{< image w="600" border="true" src="img-004.png" >}}

この設定では、ソースコードの取得先となる GitHub や Azure Repos のリポジトリを指定し、ビルドツールとして Kudu (App Service build service) と Azure Pipelines のどちらを使うかなどを指定します。
ビルド設定に関して説明すると長くなってしまうのでここでは省略します。

{{< reference >}}
- [Node.js アプリを App Service へデプロイする（Kudu ビルド編）](/p/wx3fvib/)
{{< /reference >}}

この設定が終わると、GitHub や Azure Repos にソースコードをコミット＆プッシュするだけで、自動的に Pipelines などでビルドが実行され、App Service のステージング環境 (staging slot) にデプロイされるようになります。


デプロイスロットのスワップ
----

ステージング環境 (staging slot) への Web アプリのデプロイが完了し、そこでの動作確認を終えたら、運用環境 (production slot) とのスワップを行います。
スロットのスワップは、Deployment slots の **Swap** ボタンから実行します。

{{< image w="600" border="true" src="img-005.png" >}}

Swap ボタンを押すとダイアログがポップアップし、具体的にどのスロットをスワップするかの設定と、アプリケーション設定（環境変数）が変化する場合はその差分が表示されます。

{{< image w="600" border="true" src="img-006.png" >}}

問題なければ、ダイアログ下部の Swap ボタンを押して実際のスワップ処理を開始します。
数分待てば、2 つのスロットの Web アプリの内容が入れ替わっているはずです。

