---
title: "プロコンメモ: C++ で vector<pair<int, int> > をソートするテクニック"
url: "p/k3evguf/"
date: "2012-03-24"
tags: ["c/c++", "プロコン"]
---

下記のような `pair<int, int>` の格納された `vector` があるとします。

```cpp
vector<pair<int, int> > v;
```

これを単純にソートすると、`first` メンバが等しい場合は、`second` メンバの値を元に昇順ソートされます。

```cpp
sort(v.begin(), v.end());
```

ここで、`second` メンバに関してだけ逆順（降順）にソートしたい場合はどうすればよいでしょうか？
すぐに思いつくのは、比較関数を用意して以下のように実装することです。

```cpp
#include <algorithm>
#include <iostream>
#include <utility>  // pair, make_pair
#include <vector>
using namespace std;

bool comp(const pair<int, int>& a, const pair<int, int>& b) {
    if (a.first == b.first) {
        return a.second > b.second;  // descending order
    }
    return a.first < b.first;  // ascending order
}

int main() {
    vector<pair<int, int> > v;
    v.push_back(make_pair(2, 20));
    v.push_back(make_pair(2, 10));
    v.push_back(make_pair(1, 20));
    v.push_back(make_pair(1, 30));
    sort(v.begin(), v.end(), comp);

    for (int i = 0; i < v.size(); ++i) {
        cout << v[i].first << " " << v[i].second << endl;
    }
}
```

{{< code title="実行結果" >}}
1 30
1 20
2 20
2 10
{{< /code >}}

でも、こんな比較関数を実装するのは面倒なので、ちょっとした技を使います。
コンテナ要素が数値の場合は、-1 を掛けた値を格納しておくことで、昇順ソートを降順ソートの代わりに使うことができます。
今回の場合は、`pair` の `second` 要素の符号を反転させて `vector` に入れておきます。

```cpp
for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    v.push_back(make_pair(x, -y));
}
sort(v.begin(), v.end());
```

このテクニックを使ったときは、ソート後に内容を参照するときに -1 を掛けて参照しなければいけないことに注意してください。

