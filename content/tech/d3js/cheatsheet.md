---
title: "D3.js チートシート"
url: "p/456eght/"
date: "2023-09-08"
tags: ["D3.js"]
draft: true
---


D3 セレクションオブジェクトと DOM 要素
----

| コード | 説明 |
| ---- | ---- |
| `d3.select("#foo")` | 単一要素のセレクションオブジェクトを取得 |
| `d3.selectAll("rect")` | 複数要素のセレクションオブジェクトを取得 |
| `d3.selectAll("#foo rect)` | 入れ子構造でのクエリも可能 |

DOM 要素を D3.js のメソッドで操作したいときは、__`d3.select()`__ 関数などで DOM 要素に対応する __D3 セレクションオブジェクト__ を取得する必要があります。

```js
const svg = d3.select("#mysvg")  // D3 セレクションオブジェクトを取得（単一要素）
const svgs = d3.selectAll("svg")  // D3 セレクションオブジェクトを取得（複数要素）
```

逆に、D3 セレクションオブジェクトから DOM 要素を取り出すには、__`node()`__ メソッドや __`nodes()`__ メソッドを使用します。

```js
const svgElem = svg.node()
const svgElems = svgs.nodes()
```


CSS クラスの設定
----

```js
d3.select("#foo").attr("class", "aaa bbb ccc")  // class 属性の値を置き換え
d3.select("#foo").classed("aaa", true)   // aaa クラスを追加
d3.select("#foo").classed("aaa", false)  // aaa クラスを削除
```

- 属性値を設定するには __`attr()`__ メソッドを使います。
- スタイルを設定するには __`style()`__ メソッドを使います。
- アニメーションさせたいときは __`transition()`__ メソッドで属性値を指定します。
