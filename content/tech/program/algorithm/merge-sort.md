---
title: "Merge Sort（マージソート）の実装練習 ★途中 マージ部分だけ"
url: "/p/g4n3b9h"
date: "2012-05-01"
tags: ["C/C++", "アルゴリズム"]
draft: true
---

マージ処理では、2 つのソート済みシーケンスの先頭要素のうち、大きいほうを選んでいくだけの処理になるので、処理時間としては O(n) で済むところがポイントです。

STL の algorithm とか使いたくなるけど、アルゴリズムの説明なので、for ループで我慢 (^-^;

{{< code lang="cpp" >}}
#include <iostream>
#include <limits>  // numeric_limits
using namespace std;

/**
 * Merge two sorted sequence, arr[left .. mid] and arr[mid+1 .. right].
 * (left <= mid < right)
 * A result will be written back to the original sequence arr[left .. right].
 */
template<class T>
void merge(T arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;  // The size of the sub array 1.
    int n2 = right - mid;     // The size of the sub array 2.

    // Create temporary arrays before merging.
    T* a1 = new T[n1 + 1];
    T* a2 = new T[n2 + 1];
    for (int i = 0; i < n1; ++i) a1[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) a2[i] = arr[mid + 1 + i];

    // Add a sentinel value (infinity) to simplify the combine step.
    a1[n1] = numeric_limits<T>::max();
    a2[n2] = numeric_limits<T>::max();

    // Combine two sequences.
    int x = 0, y = 0;
    for (int i = left; i <= right; ++i) {
        if (a1[x] < a2[y]) {
            arr[i] = a1[x++];
        } else {
            arr[i] = a2[y++];
        }
    }
}

int main() {
    int arr[] = {1, 3, 5, 2, 4, 6};
    merge(arr, 0, 2, 5);  // merge {1,3,5} and {2,4,6}

    const int N = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < N; ++i) {
        cout << arr[i] << endl;
    }
}
{{< /code >}}

