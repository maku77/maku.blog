---
title: "D3.js チートシート"
url: "p/456eght/"
date: "2023-09-08"
tags: ["D3.js", "cheatsheet"]
---


D3 セレクションオブジェクトの取得
----

| コード | 説明 |
| ---- | ---- |
| `d3.select("#foo")` | 先頭要素のセレクションオブジェクトを取得 |
| `d3.selectAll("rect")` | すべての要素のセレクションオブジェクトを取得 |
| `d3.selectAll("#foo rect)` | 入れ子構造でのクエリも可能 |
| `selection.select("#foo")` | セレクション以下から先頭要素のセレクションオブジェクトを取得 |
| `selection.select("rect")` | セレクション以下からすべての要素のセレクションオブジェクトを取得 |
| `selection.node()`/`nodes()` | セレクションから DOM 要素を取り出す |


属性・クラス・CSS スタイルの設定
----

| コード | 説明 |
| ---- | ---- |
| `selection.attr("cx")` | 属性値の取得 |
| `selection.attr("cx", 10)` | 属性値の設定 |
| `selection.classed("foo")` | CSS クラスが付加されているか (true/false) の取得 |
| `selection.classed("foo", true)` | CSS クラスを付加する |
| `selection.classed("foo bar", true)` | CSS クラスを付加する（複数） |
| `selection.classed("foo", false)` | CSS クラスを取り除く |
| `selection.style("color")` | CSS スタイルのプロパティ値を取得 |
| `selection.style("color", "red")` | CSS スタイルのプロパティ値を設定 |
| `selection.style({width:"100px", height:"50px"})` | オブジェクトでまとめて設定 |
| `selection.property("checked");` | チェックボックスの ON/OFF 状態を取得 (true/false) |
| `selection.property("checked", true);` | チェックボックスを ON にする |
| `selection.property("checked", false);` | チェックボックスを OFF にする |

複数要素のセレクション要素に対して属性やクラス、スタイル設定を行うときは、第 2 引数で値の代わりにコールバック関数を指定することで、各要素に紐づけられたデータ (`datum`) に基づいた値を設定できます。

```ts
// 人口を円の半径に反映
d3.selectAll(".city").attr("r", (d) => d.population);

// 警告数が多いセルに warn クラスを付加
d3.selectAll("table td").classed("warn", (d) => d.warnings > 10);
```


データ配列の割り当てと参照
----

| コード | 説明 |
| ---- | ---- |
| `selection.data()` | （通常は複数要素のセレクションに）割り当てられたデータ配列を参照する |
| `selection.data(values)` | （通常は複数要素のセレクションに）データ配列を割り当てる |
| `selection.data(values, (d) => d.id)` | データの割り当て先を記憶するためにキーを設定する |
| `selection.datum()` | （通常は単一要素のセレクションに）割り当てられたデータを参照する |
| `selection.datum(value)` | （通常は単一要素のセレクションに）データを割り当てる |


要素の追加・削除
----

| コード | 説明 |
| ---- | ---- |
| `selection.append(name)` | 末尾に要素を追加する |
| `selection.insert(name [, before])` | 指定した要素の前に要素を追加 |
| `selection.remove()` | この要素を取り除く |


テキスト、HTML の設定
----

| コード | 説明 |
| ---- | ---- |
| `selection.text()` | テキストを取得する |
| `selection.text("Hello")` | テキストを設定する |
| `selection.html()` | HTML を取得する |
| `selection.html("<b>Hello</b>")` | HTML を設定する |

