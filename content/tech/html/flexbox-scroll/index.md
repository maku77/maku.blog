---
title: "フレックスボックス (Flexbox) レイアウトでサイドバーと本文を別々にスクロールできるようにする"
url: "/p/zgqz8gp"
date: "2020-07-19"
tags: ["HTML/CSS"]
---

{{< image w="450" border="true" src="img-001.png" >}}

CSS のフレックスボックスの登場により、サイドバーなどの2段組レイアウトを簡単に実現できるようになりました。
アプリによっては、サイドバーと本文部分を別々にスクロールできるようになっていると便利です。
次の例では、フレックスボックスのアイテムとして配置した 2 つの `div` 要素 (left と right) を、別々に縦スクロールできるようにしています。

{{< code lang="html" title="HTML" >}}
<div class="container">
  <div class="left">
    Left<br>Left<br>Left<br>Left<br>Left<br>Left<br>
    Left<br>Left<br>Left<br>Left<br>Left<br>Left<br>
    Left<br>Left<br>Left<br>Left<br>Left<br>Left<br>
  </div>
  <div class="right">
    Right<br>Right<br>Right<br>Right<br>Right<br>
    Right<br>Right<br>Right<br>Right<br>Right<br>
    Right<br>Right<br>Right<br>Right<br>Right<br>
  </div>
</div>
{{< /code >}}

{{< code lang="css" title="CSS" >}}
body {
  margin: 0;
  height: 100vh;
}

.container {
  display: flex;
  height: 100%;
}

.left {
  width: 100px;
  background: #f9c;
  overflow-y: auto;
}

.right {
  flex: 1;
  background: #9cf;
  overflow-y: auto;
}
{{< /code >}}

ポイントは次の通りです。

* 親要素 (container) の __`height`__ で高さを指定する
* 子要素 (left/right) の __`overflow-y`__ に __`auto`__ を指定する

これらの設定により、子要素の内容が表示領域 (`height`) に収まらないときに、自動的にスクロールバーが表示されるようになります。

{{< note title="height: 100% 指定するときの注意" >}}
`height` プロパティを `100%` のようにパーセンテージ指定する場合は、親要素の高さが明確になっている必要があります。
この例では `body` 要素の高さを画面いっぱい (`100vh`) に設定し、その子要素 (`.container`) の高さをその 100% になるように設定しています。
{{< /note >}}

