---
title: "MongoDB Atlas で不用意なコレクションの削除を防ぐ（読み込み専用ユーザー）"
url: "p/tzpxtnm/"
date: "2023-12-27"
tags: ["MongoDB"]
---

MongoDB の API は直感的でわかりやすいですが、開発中に手がすべって `db.drop()` や `collection.drop()` を実行してしまうと、データベースやコレクションが丸ごと削除されて悲しいことになります。
本番環境での運用時はもちろん、開発中もこのようなミスが起こらないように設定しておくと安心です。

例えば、MongoDB 本家のサービスである MongoDB Atlas では、Database ユーザーごとに Role を設定して権限を絞ることができます。

{{% note title="Database ユーザーとは" %}}
MongoDB Atlas の Database ユーザーとは、Node.js アプリなどからサーバーに接続するときに接続文字列 (SRV URI) 内で指定するユーザーのことを示しています。
Atlas サービス自体のアカウントとは別ものなので注意してください。
Database ユーザーは複数作成することができ、用途ごとに Role を割り当てることができます。
例えば、バックアップ用の `backup` ユーザーには読み込み権限だけを与えておいて、バックアップツールの実装では、このユーザーを接続 URI で指定するようにします。
{{% /note %}}

次の例では、ユーザー (`readuser`) に、__`Only read any database (readAnyDatabase)`__ という Role を設定して、データベースの読み込みだけを許可しています。

{{< image border="true" src="img-001.png" title="MongoDB Atlas のユーザー Role 設定" >}}

これで、Node.js アプリなどから次のような URI で MongoDB Atlas に接続した場合は、データベースの読み込みだけが可能になります。

```
mongodb+srv://readuser:<password>@<cluster>.<random>.mongodb.net:55001/
```

組み込みの Role としては以下が用意されていますが、一般的なアプリケーション用の Role であれば、Atlas admin (atlasAdmin) を選択する必要はありません。
コレクションへの書き込みが必要なバックエンド用であれば、__`Read and write to any database (readWriteAnyDatabase)`__ という Role を選択します。

- 参考: [Atlas Built-in Roles](https://www.mongodb.com/docs/atlas/security-add-mongodb-users/#built-in-roles)
  - Atlas admin (`atlasAdmin`) ... データベースの管理用（データベースの drop ができる）
  - Read and write to any database (`readWriteAnyDatabase`) ... コレクションの読み込み＆書き込み用（コレクションの drop はできるが、データベースの drop はできない）
  - Only read any database (`readAnyDatabase`) ... コレクションの読み込み用

他にも、カスタムロールというのを作成すれば、より細かいアクセス制御が可能ですが、とりあえず `readAnyDatabase` Role を選択しておけば、データが消える心配はなくなります。

٩(๑❛ᴗ❛๑)۶ 安心！

