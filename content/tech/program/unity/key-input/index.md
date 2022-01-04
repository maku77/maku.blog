---
title: "Unityスクリプト: キー入力を取得する (UnityEngine.GetKey)"
url: "/p/xitjt3c"
date: "2020-02-10"
tags: ["Unity"]
---

{{< video w="400" src="movie-001.mp4" >}}

キーが押されているか調べる (GetKey)
----

**`UnityEngine.GetKey()`** 関数を使用すると、指定したキーが現在押されているかどうかを調べることができます。
次の例では、`Update` メソッド内で上カーソルキーと下カーソルキーが押されているかどうかを調べ、入力状態によって `Cube` オブジェクトを上下に動かしています。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private GameObject _cube;
    private void Start() {
        _cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
    }

    private void Update() {
        float amount = 0;
        if (Input.GetKey(KeyCode.UpArrow)) {
            amount = 3;
        } else if (Input.GetKey(KeyCode.DownArrow)) {
            amount = -3;
        }
        _cube.transform.Translate(0, amount * Time.deltaTime, 0);
    }
}
{{< /code >}}

継続的にオブジェクトの位置を動かす場合、上記のように移動量に **`Time.deltaTime`** をかけることで、1 秒あたりの移動量を示すことができます。


単発のキー入力 (GetKeyDown/ GetKeyUp)
----

`UnityEngine.GetKey()` 関数の代わりに、`GetKeyDown()` や `GetKeyUp()` 関数を使うと、指定したキーが押された瞬間、あるいは離された瞬間を調べることができます。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private GameObject _cube;
    private void Start() {
        _cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
    }

    private void Update() {
        float amount = 0;
        if (Input.GetKeyDown(KeyCode.UpArrow)) {
            amount = 1;
        } else if (Input.GetKeyDown(KeyCode.DownArrow)) {
            amount = -1;
        }
        _cube.transform.Translate(0, amount, 0);
    }
}
{{< /code >}}

単発のキー入力の場合は、移動量に `Time.deltaTime` をかけて調整する必要はないことに注意してください。

