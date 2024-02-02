---
title: "MongoDB のインデックスについて"
url: "p/gii3wtb/"
date: "2024-02-02"
tags: ["MongoDB"]
---

MongoDB で大量のデータを扱う場合、高速な検索を実現するためにインデックスを作成しておく必要があります。
インデックスの詳細は [公式ドキュメント](https://www.mongodb.com/docs/manual/indexes/) にありますが、ちょっとややこしいのでポイントだけまとめておきます。


インデックスがないとシーケンシャルサーチになる
----

MongoDB のフィールドのうち、デフォルトでインデックスが作成されるのは `_id` フィールドだけです（削除もできません）。
インデックスが作成されていないフィールドを検索しようとすると、コレクション全体のシーケンシャルサーチ（線形探索）になるので、データ数が増えるごとに検索速度は遅くなっていきます。

特定のフィールドに対するインデックスを作成すると、内部的に [B-tree（B 木）](https://ja.wikipedia.org/wiki/B%E6%9C%A8) 構造の逆引きデータが作られるので、そのフィールドの値での検索が高速になります。

{{< code lang="js" title="score フィールド用のインデックスを作成する" >}}
coll.createIndex({ score : 1 })
{{< /code >}}

インデックス情報は昇順 (1: ascending) あるいは降順 (-1: descending) で並べられているため、値の完全一致だけではなく、大小比較による検索も高速に行えます。

{{< code lang="js" title="score フィールドが 70 以上のドキュメントを検索" >}}
coll.find({ score: { $gte: 70 } })
{{< /code >}}


複合インデックス (Compound Indexes)
----

複合インデックス (Compound Indexes) というのを作成しておくと、複数のフィールド値を指定した高速検索が可能になります。

{{< code lang="js" title="genre フィールドと price フィールドの複合インデックスを作成" >}}
coll.createIndex({ genre: 1, price: -1 })
{{< /code >}}

フィールド名の後ろの数値は、内部的にインデックスがどのような順序で保持されるかという sort order を示しており、__1__（昇順: ascending）あるいは __-1__（降順: descending）を指定します。
検索結果をソートしたい場合、この sort order 通りのソート順序であれば高速にソートされます。

{{< code lang="js" title="複合インデックスを利用したソート" >}}
coll.find({}).sort({ genre: 1, price: -1 })
coll.find({}).sort({ genre: -1, price: 1 })  // 完全に逆順でも高速になる
{{< /code >}}

複合インデックスによる検索は、すべてのフィールドを指定する必要はなく、途中までのフィールドの値を指定した場合も高速な検索が可能です。
このような仕様のため、複合インデックスにおけるフィールドの指定順序は重要です。

{{< code lang="js" title="複合インデックスを利用した検索" >}}
// 複合インデックスが使用される検索方法
coll.find({ genre: "RPG", price: { $gt: 100 } })
coll.find({ genre: "RPG" })

// 複合インデックスが使われない検索（フィールドの指定順序が間違っている）
coll.find({ price: { $gt: 100 }, genre: "RPG" })
coll.find({ price: { $gt: 100 })
{{< /code >}}


テキストインデックス (Text Indexes)
----

インデックスには大まかに下記の 2 種類があります。

- 通常のインデックス ... フィールド単位で完全一致、あるいは大小比較で高速検索
- テキストインデックス ... テキスト形式のフィールド内から部分文字列を高速検索

要するに、テキストの全文検索のようなことをやりたいときは、通常のインデックスではなく、テキストインデックスを作成する必要があります（参考: [Text Indexes — MongoDB Manual](https://www.mongodb.com/docs/manual/core/indexes/index-types/index-text/)）。

次の例では、2 つのフィールド用のテキストインデックスを作成しています。
通常のインデックスを作成するときは各フィールドに対してソート順序 (1 or -1) を指定しましたが、テキストインデックスを作成する場合は __`"text"`__ を指定します。

{{< code lang="js" title="title、comment フィールド用にテキストインデックスを作成" >}}
coll.createIndex({ title: "text", comment: "text" })
{{< /code >}}

あるいは、ワイルドカードを使って、すべてのテキストフィールド用にテキストインデックスを作成することもできます (Wildcard Text Index)。

{{< code lang="js" title="全フィールド用のテキストインデックスを作成" >}}
coll.createIndex({ "$**": "text" })
{{< /code >}}

テキストインデックスを使った検索を行うには、次のような構文を使用します。

{{< code lang="js" title="テキストインデックスで部分文字列を検索" >}}
coll.find({ $text: { $search: "hello" } })
{{< /code >}}

ここで、検索対象とするフィールド名が指定されていないところに注目です。
テキストインデックスは、コレクション内で 1 つしか生成できないという制約があり、フィールド名の指定は意味がないということですね。
公式サイトに、テキストインデックスの制約が次のように記述されています。

> [Create a Text Index — MongoDB Manual](https://www.mongodb.com/docs/manual/core/indexes/index-types/index-text/create-text-index/#about-this-task) より
>
> - A collection can have __at most one text index__.
> - Atlas Search (available in MongoDB Atlas) supports multiple full-text search indexes on a single collection. To learn more, see the Atlas Search documentation.
> - You can index multiple fields in a single text index. A text index can contain up to 32 fields. To see an example, see Create a Compound Text Index.

ようするに MongoDB 社の提供する MongoDB Atlas サービスを使ってね、ということですね。むふぅ。

