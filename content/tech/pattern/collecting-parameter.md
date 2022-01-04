---
title: "Kent Beck の CollectingParameter パターン"
url: "/p/8yhxet9"
date: "2011-04-10"
tags: ["デザインパターン"]
---

Collecting Parameter の特徴
----

巡回するオブジェクトの戻り値をベースに目的のデータを構築するのではなく、パラメータに渡したオブジェクトを使ってデータを構築する。

* 戻り値となるオブジェクトのインスタンス化を防ぐことができるので、コードの実行速度が上がる。
* 戻り値をベースにしたデータ構築よりも、より柔軟なタイミングでデータの構築が行える（例えば、メソッドの実行途中で結果を append できる）ようになり、コードを分割しやすくなる。

参考文献: {{< amazon-inline id="4822282384" title="パターン思考リファクタリング入門" >}} 第10章 累積処理


Collecting Parameter を使わない場合の実装例
----

{{< code lang="java" >}}
public String extractText() {
    StringBuilder results = new StringBuilder();
    Node node = null;
    while ((node = nodeList.next()) != null) {
        results.append(node.toString());  // 戻り値をベースにしたデータ構築
    }
    return results.toString();
}
{{< /code >}}


Collecting Parameter を使う場合の実装例
----

{{< code lang="java" >}}
public String extractText() {
    StringBuilder results = new StringBuilder();
    Node node = null;
    while ((node = nodeList.next()) != null) {
        node.getText(results);  // パラメータをベースにデータ構築
    }
    return results.toString();
}
{{< /code >}}

巡回される要素の方で、Collecting Parameter を使ってデータを構築します。

{{< code lang="java" >}}
public StringNode implements Node {
    ...
    public void getText(StringBuilder results) {
        results.append("<P>");
        results.append(text);
        results.append("</P>");
    }
}
{{< /code >}}

巡回される要素側にデータ構築のためのコードが含まれてしまうのを防ぎたい場合は、 __Visitor パターン__ を使用し、呼び出し側に処理を委譲するようにします。

