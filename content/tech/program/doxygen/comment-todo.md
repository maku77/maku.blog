---
title: "Doxygen のコメントの書き方: TODO コメント"
url: "/p/8nnbyho"
date: "2009-06-09"
tags: ["Doxygen"]
---

関数のコメントとして、以下のように **`@todo`** タグを入れておくと、
ドキュメント内の Member Function Documentation の節で、TODO リストとして表示されるようになります。

ヘッダファイルではなく、CPP ファイル内にインラインコメントとして `@todo` を含めることもできます。
この場合は、ヘッダファイル内の関数コメントとして有効な Doxygen コメントを記述しておく必要があります。

{{< code lang="cpp" >}}
/**
 * @brief Brief description.
 * @todo Modify macros.
 */
void Hoge();

void MyClass::Hoge() {
     /// @todo Modify macro name.
     TEMPORARY_MACRO();
}
{{< /code >}}

