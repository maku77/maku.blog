---
title: "Doxygen のコメントの書き方: 関数のコメント"
url: "/p/t3xmq3m"
date: "2009-06-09"
tags: ["Doxygen"]
---

関数のコメントの書き方
----

ここでは、Javadoc スタイルのコメント記法を使っています。

```
/**
 * @brief Brief description 1.
 *        Brief description 2.
 *
 * Detailed description 1.
 * Detailed description 2.
 * @param[in] param1  Param1's explanation.
 * @param[out] param2  Param2's explanation.
 * @param[in,out] param3  Param3's explanation.
 * @return The number of channels in the list.
 * @see RelatedFunction()
 */
int MyFunc(int param1, const Foo& param2, Bar& param3);
```

Brief description と Detailed description の間には空行を入れるようにします。
できれば Brief description は一行で書くようにしてください。

Detailed description を記述しない場合は、以下のように空行を省略しても大丈夫です（コマンドが切れ目になるので）。

```
/**
 * @brief Brief description.
 * @param[in] param1  Param1's explanation.
 * @param[in] param2  Param2's explanation.
 */
void MyFunc(int param1, int param2);
```

戻り値は、１つの `@return` で説明的に記述してもよいし、複数の `@retval` を記述してすべての戻り値を列挙してもよいです。

```
/**
 * ...
 * @retval true  Time is valid.
 * @retval false  Time is invalid. */
```

パラメーターなどの説明が必要なく、メソッドのコメントが Brief description 一行だけですむ場合は、C# スタイルのコメントを使うと便利です。

```
/// Brief description.
void MyFunc();
```

箇条書きを使用したいときは、インデントを揃えてハイフンを記述すれば OK です。
このあたりは Javadoc よりも便利ですね。

```
/**
 * @brief Brief description.
 *
 * Detailed description.
 *  - Item 1
 *  - Item 2
 *  - Item 3
 */
```


関数コメントのいろいろな書き方
----

メンバ関数のコメントで、いろんな書き方をした場合に、Brief description と Detailed description のどちらとみなされるのかをテストしてみます。

`JAVADOC_AUTOBRIEF` の設定が `YES`、`NO` の場合でルールが大きく変わるので、それぞれの設定でテストします。

### JAVADOC_AUTOBRIEF = NO の場合 (default)

デフォルト設定では、Brief description を入れるには、明示的に `@brief` タグを記述する必要があります。
空行が来るまでは Brief description とみなされます。

```
/**
 * @brief Brief description 1.
 *        Brief description 2.
 *
 * Detailed description 1.
 * Detailed description 2.
 */
void MyFunc();
```

C# スタイルのコメントを使って `@brief` の代わりにすることも可能です。

```
/// Brief description 1.
/**
 * Detailed description 1.
 * Detailed description 2.
 */
void MyFunc();
```

コメントとしてタグなしで普通に文章を記述すると、Detailed description になります。
ドキュメント化された時点で改行はなくなります。

```
/**
 * Detailed description 1.
 * Detailed description 2.
 * Detailed description 3.
 */
void MyFunc();
```

空行を入れると、パラグラフの区切りとみなされて改行が入ります。
ただし、すべて Detailed description であることに変わりはありません。

```
/**
 * Detailed description 1.
 *
 * Detailed description 2.
 * Detailed description 3.
 */
void MyFunc();
```

### JAVADOC_AUTOBRIEF = YES の場合

設定ファイル (`Doxyfile`) で `JAVADOC_AUTOBRIEF = YES` と設定しておくと、普通にコメントを書けば、最初のピリオド (`.`) までが自動的に Brief description とみなされます。

```
/**
 * Brief description 1.
 * Detailed description 1.
 * Detailed description 2.
 */
void MyFunc();
```

改行を含んでいても、ピリオドが出てくるまでは Brief description とみなされます。

```
/**
 * Brief description 1
 * Brief description 2
 * Brief description 3.
 * Detailed description 1.
 */
void MyFunc();
```

ピリオドがなくても、空行が現れた時点で、Brief description は終了します。
この場合、出力されるドキュメントには勝手にピリオドが付加されるっぽいです。

```
/**
 * Brief description 1
 * Brief description 2
 *
 * Detailed description 1.
 * Detailed description 2.
 */
void MyFunc();
```

ピリオドで強制的に区切られるので、基本的には 2 文以上の Brief description は記述できません。

```
/**
 * Brief description 1. Detailed description 1.
 * Detailed description 2.
 * Detailed description 3.
 */
void MyFunc();
```

Brief description にピリオドを含めたい場合は、ピリオドの後ろにバックスラッシュ (`\`) を入れる必要があります（気持ち悪っ^^;）。

```
/**
 * Brief description 1.\ Detailed description 1.
 * Detailed description 2.
 * Detailed description 3.
 */
void MyFunc();
```

### オススメの書き方（どのように記述するのがよいか？）

設定ファイルやコメントの記述方法によって、上記のように、どの部分が Brief description としてみなされるかが変わってくることが分かりました。
つまり、正しくドキュメントを出力するためには、プロジェクト内で共通の `Doxyfile` を使い、決められたフォーマットでドキュメンテーションコメントを記述するのが基本的なやり方になってくるでしょう。

ただし、少なくとも以下のようなルールでドキュメンテーションコメントを記述しておけば、`JAVADOC_AUTOBRIEF` の設定に左右されずに、想定通りのドキュメントが出力されるようになります。

* Brief description は `@brief` で始める。
* Brief description は1行で書く。
* Detailed description の前には空行を入れる。

