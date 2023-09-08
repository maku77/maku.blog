---
title: "D3.js チートシート"
url: "p/456eght/"
date: "2023-09-08"
tags: ["D3.js"]
draft: true
---

D3 セレクションオブジェクトと DOM 要素
----

DOM 要素を D3.js のメソッドで操作したいときは、__`d3.select()`__ 関数などで DOM 要素に対応する __D3 セレクションオブジェクト__ を取得する必要があります。

```js
d3.select()    // 例: const divElem = d3.select("div")
d3.selectAll()
```

逆に、D3 セレクションオブジェクトから DOM 要素を取り出すには、__`node()`__ メソッドや __`nodes()`__ メソッドを使用します。

```js
svg.node()    // 例: const node = div.node()
svgs.nodes()
```


CSS クラスの設定
----

```js
d3.select("div").attr("class", "hoge foo bar")  // class 属性の値を置き換え
d3.select("div").classed("hoge", true)   // hoge クラスを追加
d3.select("div").classed("hoge", false)  // hoge クラスを削除
```

- 属性値を設定するには __`attr()`__ メソッドを使います。
- スタイルを設定するには __`style()`__ メソッドを使います。
- アニメーションさせたいときは __`transition()`__ メソッドで属性値を指定します。
