---
title: "AWS CLI: Windows で HOME 環境変数を設定したときに credentials が見つからなくなる場合の対処"
linkTitle: "Windows で HOME 環境変数を設定したときに credentials が見つからなくなる場合の対処"
url: "/p/2td9ouj"
date: "2021-03-29"
tags: ["AWS", "AWS/CLI"]
weight: 2
---

AWS CLI や AWS SDK が参照する共有認証ファイル（`credentials` および `config`）は、デフォルトでは下記のようなディレクトリになっています。

- Linux の場合: `~/.aws/credentials`
- Windows の場合: `~/%USERPROFILE%/.aws/credentials`

ところが、Windows を使用している場合に、環境変数 __`%HOME%`__ を設定していると、CLI や SDK が `%HOME%/.aws/credentials` を見に行ったりして、credentials が見つからない系のエラーが出ることがあります。
一貫して __`%USERPROFILE%`__ の方を見に行ってくれれればよいのですが、このあたりの振る舞いは結構ルーズなようです。

そこで、__Windows のジャンクション機能__ で、両方の `.aws` ディレクトリが同じディレクトリを指すようにしてみました。
ジャンクション生成のコマンドは、__`mklink /j <fromDir> <toDir>`__ です。

{{< code title="ジャンクションで 2 つの .aws を同一にする" >}}
C:\> mklink /j %HOME%\.aws %USERPROFILE%\.aws

C:\home\.aws <<===>> C:\Users\maku\.aws のジャンクションが作成されました
{{< /code >}}

これで、`%HOME%\.aws` ディレクトリを参照したときの実体として、`%USERPROFILE%\.aws` ディレクトリが使われるようになります。
今のところ快適に動作しています。

ちなみに、ジャンクションを削除したいときは、ジャンクション元の `%HOME%\.aws` ディレクトリを削除するだけです。
実体の方の `%USERPROFILET%\.aws` は削除されないので安全です。

