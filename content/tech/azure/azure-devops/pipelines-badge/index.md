---
title: "Azure Pipelines のビルド結果を GitHub にバッジ表示する"
url: "/p/teq2cmv"
date: "2020-04-10"
tags: ["Azure", "Azure Pipelines"]
description: "Azure Pipelines のステータスバッジを使用すると、GitHub の README ページなどにビルド結果を画像ファイルとして表示することができます。"
weight: 200
---

ステータスバッジとは
----

{{< image border="true" src="img-001.png" >}}

ステータスバッジというのは、GitHub プロジェクトのトップページ (`README.md`) でよく見かける上のようなアイコンのことです。
ステータスバッジを貼り付けておくことで、最新のソースコードが正しくビルドできていることを一目で確認することができますし、ちゃんと開発しているんだということをアピールすることにもなります。

Azure Pipelines のビルド結果を示すステータスバッジは、下記のように簡単に追加することができます。


Azure Pipelines のステータスバッジを表示する
----

ステータスバッジは、画像ファイルの URL の形で提供されているので、GitHub の `README.md` などにその URL を貼り付けるだけで OK です。

{{< image border="true" src="img-002.png" >}}

ステータスバッジの画像 URL を確認するには、Azure Pipelines のページを開き、右上のメニューアイコン → `Status badge` と選択します。

{{< image border="true" src="img-003.png" >}}

画像の URL と一緒に、Markdown ファイルに記述する場合のコード (Sample markdown) も表示してくれるので、GitHub のページに貼り付ける場合はそちらを使えばよいでしょう。
右側のコピーアイコンを押してクリップボードにコピーし、GitHub の `README.md` ファイルに次のような感じで貼り付ければ OK です。

{{< image border="true" src="img-004.png" >}}

これで、GitHub プロジェクトのトップページアクセスしたときに、次のようにステータスバッジが表示されるはずです。

{{< image border="true" src="img-001.png" >}}


ステータスバッジの画像が表示されないとき
----

Azure DevOps にサインインしていない状態で、ステータスバッジが表示されない場合は、Pipelines の設定を確認してみてください。

{{< image border="true" src="img-005.png" >}}

上記のように、__Disable anonymous access to badge__ という項目を __OFF__ にすれば表示されるようになると思います。

