---
title: "プログラムでレインボーカラー（虹色）のグラデーションを作成するには"
url: "/p/4cbbh6o"
date: "2019-12-09"
tags: ["プログラミング"]
---

とあるコーディングにおいて、色を滑らかに変化させる必要があったのでメモメモ。


RGB ではなく HSV で考える
----

何らかのプログラミング言語から虹色のグラデーションを作る必要がある場合、RGB の色空間で色調整を行うのは大変ですが、HSV の色空間で考えると簡単に表現することができます。

{{< image src="hsv.png" title="HSV 色空間" >}}

- 参考: [HSV 色空間 - Wikipedia](https://ja.wikipedia.org/wiki/HSV%E8%89%B2%E7%A9%BA%E9%96%93)

HSV はそれぞれ下記のような情報を表しており、

- 色相 (Hue): 0～360
- 彩度 (Saturation・Chroma): 0～1
- 明度 (Value・Brightness): 0～1

このうち、色相 (Hue) の値を 0～360 の間で変化させてやることでレインボーカラーを表現することができます。


プログラムのサンプル
----

{{< image src="rainbow-bar.png" title="虹色グラデーションの描画" >}}

例えば、Android では、`android.graphics.Color.HSVToColor()` という関数を使用すると、HSV 色空間における値を、描画に使用するカラーデータに変換することができます。

下記の `ColorGenerator` クラスの `nextColor()` メソッドを連続して呼び出すと、徐々に変わっていく色をひとつずつ取り出すことができます。
やっていることは、メソッドの呼び出しごとに、色相 (Hue) の値を少しずつ変化させているだけです。
色相 (Hue) が、何度の `nextColor()` 呼び出しで一周するかは、コンストラクタの `steps` パラメータで指定できるようにしています。

{{< code lang="kotlin" title="ColorGenerator.kt" >}}
class ColorGenerator(val steps: Int, initialHue: Float = 0.0F) {
    private val hueStep: Float = 360F / steps
    private var currentHue = initialHue

    fun nextColor(): Int {
        val hsv = floatArrayOf(currentHue, 1.0F, 1.0F)
        val color: Int = Color.HSVToColor(255, hsv)
        currentHue = (currentHue + hueStep) % 360F
        return color
    }
}
{{< /code >}}

次のコードは、このクラスを使って、虹色のグラデーション（細い矩形の連続）を描画するコードの抜粋です。
ここでは、色を 30 段階に分けて描画しています。

{{< code lang="kotlin" title="虹色の矩形を描画する（抜粋）" >}}
val colorGen = ColorGenerator(30)

override fun onDraw(canvas: Canvas){
    var left = 0F
    val paint = Paint()
    for (i in 0 until colorGen.steps) {
        paint.color = colorGen.nextColor()
        c.drawRect(left, 0F, left + 10, 100F, paint)
        left += 10
    }
}
{{< /code >}}

