---
title: "Insertion Sort（挿入ソート）の実装練習"
url: "/p/axg4xms"
date: "2012-04-10"
tags: ["C/C++", "アルゴリズム"]
---

Insertion Sort 実装の練習！
----

{{< code lang="cpp" >}}
void insertion_sort(int vals[], int size) {
    for (int i = 1; i < size; ++i) {
        // Insert vals[i] into the sorted sequence vals[0..i-1].
        int key = vals[i];
        int j = i - 1;
        while (j >= 0 && vals[j] > key) {
            vals[j + 1] = vals[j];
            --j;
        }
        vals[j + 1] = key;
    }
}
{{< /code >}}

降順ソート (descending order) にするには、`vals[j] > key` というところを、`vals[j] < key` にするだけで OK。

### テスト

{{< code lang="cpp" >}}
#include <iostream>
using namespace std;

int main() {
    int vals[] = {5, 2, 4, 6, 1, 3};
    const int N = sizeof(vals) / sizeof(int);

    insertion_sort(vals, N);

    for (int i = 0; i < N; ++i) {
        cout << vals[i] << endl;
    }
}
{{< /code >}}

参考: Introduction to Algorithms

