---
title: "React の useMemo と useCallback で描画を最適化する"
url: "/p/6nw7fow"
date: "2020-08-30"
tags: ["React", "TypeScript"]
draft: true
---

**この記事いらないかも**

はじめに：「メモ化」とは
----

React のコンポーネントは、渡されたプロパティ (prop) が変化したときにだけ描画処理を行うことで、余計な再描画が発生しないように設計されています。
この仕組みを活用すれば効率的な Web サイトのレンダリングを行えますが、不用意に prop の値を変更すると、再描画が発生してしまいます。
