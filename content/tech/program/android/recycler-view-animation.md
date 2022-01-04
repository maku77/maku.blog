---
title: "Androidメモ: RecyclerView にアニメーションを追加する方法"
linkTitle: "RecyclerView にアニメーションを追加する方法"
url: "/p/8avcr5h"
date: "2020-01-21"
tags: ["Android"]
---

Android の RecyclerView にリスト表示する各要素を、左や右からスライドインさせながら表示する方法を説明します。


RecyclerView へのアニメーション追加方法
----

RecyclerView の各要素をアニメーションさせるには、下記のような方法があります。

1. カスタム ItemAnimator を作成してセットする方法
2. LayoutAnimation をセットする方法（RecyclerView だけでなく、すべての ViewGroup に適用できます）

1 の方法を使うと、アイテムの追加・削除に応じたアニメ―ションを定義できるので、柔軟なカスタマイズが可能になりますが、2 の方法であれば、アニメーションの定義だけを行えばよいので簡単です。
ここでは、シンプルな 2 の方法を説明します。

アニメーションの定義は、XML 形式のリソースファイルで行います。
ディレクトリ名やファイル名は任意ですが、通常は `res/anim/xxx.xml` のような名前にし、コードの中から `R.anim.xxx` と参照できるようにします。


要素を左からスライドインさせる
----

下記の `res/anim/slide_from_left.xml` リソースでは、画面内の要素をどのように表示するかを定義しています。

{{< code lang="xml" title="res/anim/slide_from_left.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<layoutAnimation xmlns:android="http://schemas.android.com/apk/res/android"
    android:animation="@android:anim/slide_in_left"
    android:delay="10%"
    android:animationOrder="normal"
    />
{{< /code >}}

各属性の説明です。

- `android:animation="@android:anim/slide_in_left"`
    - 個々の要素がどのようにアニメーションするかを指定します。ここでは、Android にあらかじめ定義されている `slide_in_left` を指定しています。
- `android:delay="10%"`
    - 1 つ前の要素のアニメーションが何％まで完了してから、次の要素のアニメーションを開始するかを指定します。`100%` と指定すると、各要素のアニメーションが完全に終了してから次の要素のアニメーションが開始されるため、すべての要素が表示されるまでに非常に時間がかかります。
- `android:animationOrder="normal"`
    - 要素がどのような順番で表示されていくかを指定します。`normal`、`reverse`、`random` などを指定できます。

ここでは、個々の要素のアニメーション方法として、Android 付属の `@android:anim/slide_in_left`  を使っていますが、これは次のように定義されています。

{{< code lang="xml" title="@android/anim/slide_in_left（抜粋）" >}}
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android">
    <translate android:fromXDelta="-50%p" android:toXDelta="0"
        android:duration="@android:integer/config_mediumAnimTime"/>
    <alpha android:fromAlpha="0.0" android:toAlpha="1.0"
        android:duration="@android:integer/config_mediumAnimTime" />
</set>
{{< /code >}}

アニメーションの開始位置、終了位置、透過度の変化などを定義しています。

RecyclerView に LayoutAnimation を適用するには、レイアウトファイルの中の `layoutAnimation` 属性を使用します。

{{< code lang="xml" >}}
<androidx.recyclerview.widget.RecyclerView
    android:id="@+id/addressList"
    android:layoutAnimation="@anim/slide_from_left"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    />
{{< /code >}}

あるいは、Kotlin (Java) コードの中で、RecyclerView インスタンスの `layoutAnimation` プロパティを設定する方法もあります。

{{< code lang="kotlin" title="MyFragment.kt（抜粋）" >}}
recyclerView.layoutAnimation = AnimationUtils.loadLayoutAnimation(
    context, R.anim.slide_from_left)
{{< /code >}}


要素を右からスライドインさせる
----

各要素を「左」からではなく、「右」からスライドインさせる場合も、ほぼ同様に定義できます。

まずは、全体の要素のアニメーション方法を定義します。

{{< code lang="xml" title="res/anim/slide_from_right.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<layoutAnimation xmlns:android="http://schemas.android.com/apk/res/android"
    android:animation="@anim/item_slide_from_right"
    android:delay="10%"
    android:animationOrder="normal"
    />
{{< /code >}}

「右」からスライドインさせる場合の各要素のアニメーションは、Android 標準では用意されていないようなので、`@android/anim/slide_in_left` のコードを参考に、次のように自力で定義します（開始位置と終了位置をいじっただけ）。

{{< code lang="xml" title="res/anim/item_slide_from_right.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<set xmlns:android="http://schemas.android.com/apk/res/android">
    <translate
        android:fromXDelta="50%p"
        android:toXDelta="0"
        android:duration="@android:integer/config_mediumAnimTime"
        />
    <alpha
        android:fromAlpha="0.0"
        android:toAlpha="1.0"
        android:duration="@android:integer/config_mediumAnimTime"
        />
</set>
{{< /code >}}

あとは、RecyclerView インスタンスの `layoutAnimation` プロパティを設定してやるだけです。

{{< code lang="xml" >}}
<androidx.recyclerview.widget.RecyclerView
    android:id="@+id/addressList"
    android:layoutAnimation="@anim/slide_from_right"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    />
{{< /code >}}

