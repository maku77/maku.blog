---
title: "Android開発: レイアウトの種類まとめ"
linkTitle: "レイアウトの種類まとめ"
url: "/p/64zojt6"
date: "2011-01-10"
tags: ["Android"]
working: true
---

- FrameLayout
    - ウィジェットを左上に１つだけ配置する。
    - FrameLayout の中で複数の View を配置した場合は、最後に表示したものが上に重ねて表示される。
- LinearLayout
    - 水平か、垂直に一直線にウィジェットを並べる。
    - 水平: `android:orientation="horizontal"`
    - 垂直: `android:orientation="vertical"`
- [ConstraintLayout](https://developer.android.com/training/constraint-layout?hl=ja)
    - フラットなビュー階層を持つ大きくて複雑なレイアウトを作成する。
    - RelativeLayout より柔軟。
    - LinearLayout を組み合わせてレイアウトを入れ子構造にするとパフォーマンスが悪くなるが、ConstraintLayout ひとつでフラットにビューを配置すると効率的な描画が可能。
- TableLayout
    - HTML の TABLE 要素のようにウィジェットを並べる。
- RelativeLayout
    - 他のウィジェットとの相対的な位置関係で配置する。
    - 参照されるウィジェットは、参照するウィジェットよりも先に定義されていなければならない。
- AbsoluteLayout（非推奨）
    - 絶対座標でウィジェットを配置する。

それぞれの Layout を入れ子にして、その中にウィジェットを配置していくことで複雑なレイアウトを作成することができます。

{{< code lang="xml" >}}
<LinearLayout ...>
    <LinearLayout ...>
        ...
    </LinearLayout>
    <LinearLayout ...>
        ...
    </LinearLayout>
</LinearLayout>
{{< /code >}}

