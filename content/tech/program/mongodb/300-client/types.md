---
title: "mongo シェルで各フィールドの型を調べる (typeof, instanceof)"
url: "p/yejmu4v/"
date: "2013-10-22"
tags: ["MongoDB"]
---

`mongo` シェルで、各フィールドの型（タイプ）を調べたいときは、__`typeof`__ や __`instanceof`__ を使用します。

{{< code title="フィールドの型を調べる" >}}
typeof mydoc._id
{{< /code >}}

{{< code title="指定した型のフィールドかどうか調べる" >}}
mydoc._id instanceof ObjectId
{{< /code >}}

{{% maku-common/reference %}}
- [Data Types in the mongo Shell — MongoDB Manual](http://docs.mongodb.org/manual/core/shell-types/#check-types-in-shell)
{{% /maku-common/reference %}}

