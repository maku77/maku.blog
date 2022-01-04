---
title: "Android開発: ShapeDrawable で基本図形を描く"
linkTitle: "ShapeDrawable で基本図形を描く"
url: "/p/qj8p3eq"
date: "2014-08-03"
tags: ["Android"]
---

矩形の描画
----

{{< code lang="java" >}}
private ShapeDrawable mDrawable = new ShapeDrawable();

private void draw(Canvas canvas) {
    mDrawable.setBounds(100, 100, 200, 200);
    mDrawable.getPaint().setColor(Color.YELLOW);
    mDrawable.draw(canvas);
}
{{< /code >}}


円、楕円の描画
----

{{< code lang="java" >}}
private ShapeDrawable mDrawable = new ShapeDrawable(new OvalShape());

private void draw(Canvas canvas) {
    mDrawable.setBounds(100, 100, 200, 200);
    mDrawable.getPaint().setColor(Color.YELLOW);
    mDrawable.draw(canvas);
}
{{< /code >}}

