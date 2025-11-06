---
title: "プロコンメモ: C++ で標準入力から vector<int> を作る技"
url: "p/qzxqy3m/"
date: "2012-04-16"
tags: ["c/c++", "プロコン"]
---

C++ で標準入力 `cin` から数値を読み込みながら `vector<int>` に詰めて行くとき、`push_back()` を使うと、以下のようにテンポラリ変数が必要になってしまいます。

```cpp
vector<int> v;
int temp;
for (int i=0; i<N; ++i) {
    cin >> temp;
    v.push_back(temp);
}
```

読み出す回数が `N` 回、のように決まっているのであれば、`vector` のサイズを先に初期化してしまうことで、インデックスを使って格納していくことができます。

```cpp
vector<int> v(N);
for (int i=0; i<N; ++i) cin >> v[i];
```

