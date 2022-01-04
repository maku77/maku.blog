---
title: "Doxygen でどのコメントスタイルを使うべきかの考察"
url: "/p/qnnukuj"
date: "2006-08-08"
tags: ["Doxygen"]
description: "Doxygen のドキュメンテーションコメントを記述するときは、Javadoc スタイル、C# スタイル、Qt スタイルなど、いろいろなスタイルで記述することができます。ここでは、それぞれの記述スタイルの特徴をまとめておきます。"
---

方法(1) Javadoc スタイル
----

これが一番よく使われている記法だと思います。
特にこだわりがないのであれば、この記法を使っておくのが無難です。

```
/**
 * @brief Brief description.
 *
 * Detail description.
 * @param a  hogehoge
 * @param b  hogehoge
 */
void Hoge(int a, int b);
```

- 特徴
    - brief コマンドの後ろに空行が必要なため、行数が増えてしまうし、少しアンバランスな感じになる。
    - ひとつのコメントブロックとして判別しやすい。
    - 簡易説明は 2 行以上に渡って記述できる（その代わり、brief の下に空行が必要という制約がある）。
    - 簡易説明だけでよい場合にすっきり書くのが難しい（その場合は `///` で一行で済ませる手がある）。

一行の Brief description だけ記述したい場合でも、以下のように `@brief` が必要です。

```
/**
 * @brief Brief description.
 */
void Hoge();
```

あるいは、

```
/** @brief Brief description. */
void Hoge();
```

通常は Javadoc スタイルで書いておいて、一行の Brief description だけを記述したい場合だけ、C# スタイルのコメントを使うのがすっきりするかもしれません。

```
/// Brief description.
void Hoge();
```

Javadoc スタイルのコメントは、以下のような `file` スペシャルコマンドの書き方と統一しやすいのも利点です。

```
/**
 * @file $File$
 * @brief Brief description.
 *
 * Detail description.
 */
```


方法(2) C# スタイル
----

```
/// @brief Brief description.
///
/// Detail description.
/// @param a  hogehoge
/// @param b  hogehoge
void Hoge(int a, int b);
```

- 特徴:
    - 記号は 2 種類だけでよい。
    - brief コマンドの後ろに空行が必要なため、行数が増えてしまうし、少しアンバランスな感じになる。
    - 簡易説明は 2 行以上に渡って記述できる。


方法(3) Qt スタイル
----

一行要約に `//!` を使うことができます。

```
//! Brief description.
/*!
 * Detail description.
 * \param a  hogehoge
 * \param b  hogehoge
 */
void Hoge(int a, int b);
```

- 特徴
    - 見にくい！ 4種類の記号を使うので煩雑。
    - 簡易説明は一行しか書けない。


番外編(1) Qt スタイルと C# スタイルのハイブリッド
----

Brief description にだけ Qt 風の記法を使う方法です。

```
//! Brief description.
/// Detail description.
/// @param a  hogehoge
/// @param b  hogehoge
void Hoge(int a, int b);
```

- 特徴
    - brief コマンドのあとに、空行を入れなくてもよい。
    - 行数は最も少ないが、詰め込みすぎな感じもする。
    - C++ のコメントスタイルを使っているので、C 言語のコメントでまとめてコメントアウトできる（`/*` と `*/` で囲める）。

コンパクトに書けますが、ちょっとやりすぎかな。。パッと見、抵抗を感じます。


番外編(2) C# スタイルと Javadoc スタイルのハイブリッド
----

```
/// Brief description.
/**
 * Detail description.
 * @param a  hogehoge
 * @param b  hogehoge
 */
void Hoge(int a, int b);
```

- 特徴
    - brief コマンドのあとに、空行を入れなくてもよい。
    - 簡易説明は一行しか書けない。

簡易説明だけでよい場合に、以下のように C# スタイルで一行で記述するのであれば、このハイブリッド記法はまぁまぁイケてるかもしれません。

```
/// Brief description.
void Hoge();
```


まとめ
----

* Javadoc スタイルを使うのがオススメ
* メソッドのコメントが1行の要約だけでいい場合は、例外的に C# スタイルの `///` を使うとすっきりする。

