---
title: "Unityスクリプト: オブジェクトを移動する (Transform.position)"
url: "/p/53o7p8p"
date: "2020-02-10"
tags: ["Unity"]
weight: 1001
---

ゲームオブジェクトに設定された `Transform` コンポーネントを操作することで、ゲームオブジェクトを移動させることができます。
`Transform` コンポーネントのオブジェクトは下記のいずれかの方法で取得できます。

{{< code lang="csharp" >}}
Transform tf = gameObject.GetComponent<Transform>();
Transform tf = gameObject.transform;
{{< /code >}}

[Transform クラス](https://docs.unity3d.com/ja/current/ScriptReference/Transform.html) には、ゲームオブジェクトを移動させるためのプロパティやメソッドが定義されています。


ワールド座標での位置 (Transform.position)
----

`Transform.position` プロパティは、ワールド座標でのオブジェクトの位置を示します。
次の例では、3 つのオブジェクトの X 座標を -2、0、2 に設定しています。

{{< image w="400" src="img-001.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject sphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        GameObject capsule = GameObject.CreatePrimitive(PrimitiveType.Capsule);
        cube.transform.position = new Vector3(-2, 0, 0);
        sphere.transform.position = new Vector3(0, 0, 0);
        capsule.transform.position = new Vector3(2, 0, 0);
    }
}
{{< /code >}}

`+=` 演算子や `-=` 演算子を使用すれば、指定した数値分だけワールド座標での位置を移動できます。

{{< code lang="csharp" >}}
cube.transform.position += new Vector3(-2, 0, 0);
{{< /code >}}

X、Y、Z 軸方向にだけ動かしたい場合は、`Vector3` クラスに用意されている次のような定数を利用できます。

| 定数 | 値 |
| ---- | ---- |
| `Vector3.left` | `Vector3(-1, 0, 0)` |
| `Vector3.right` | `Vector3(1, 0, 0)` |
| `Vector3.up` | `Vector3(0, 1, 0)` |
| `Vector3.down` | `Vector3(0, -1, 0)` |
| `Vector3.forward` | `Vector3(0, 0, 1)` |
| `Vector3.back` | `Vector3(0, 0, -1)` |

{{< code lang="csharp" >}}
cube.transform.position += Vector3.left * 2;
{{< /code >}}


ローカル座標での位置 (Transform.localPosition)
----

親オブジェクトからの相対位置（ローカル座標）で位置を指定したい場合は、`Transform.position` プロパティの代わりに、**`Transform.localPosition`** プロパティを指定します。

次の例では、親オブジェクト (Cube) に対して、子オブジェクト (Sphere と Capsule) を、それぞれローカル座標とグローバル座標で位置指定した場合の違いを示しています。

{{< image w="400" src="img-local-position.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject parent = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject child1 = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        GameObject child2 = GameObject.CreatePrimitive(PrimitiveType.Capsule);

        // Set parent-child relation
        child1.transform.parent = parent.transform;
        child2.transform.parent = parent.transform;

        // Move objects
        parent.transform.position = Vector3.up * 2;
        child1.transform.localPosition = Vector3.right * 2;
        child2.transform.position = Vector3.right * 2;
    }
}
{{< /code >}}

子 Sphere (`child1`) はローカル座標で位置を指定しているので、親 Cube からの相対的な座標で右に表示されています。
一方、子 Capsule (`child2`) はグローバル座標で位置を指定しているので、親 Cube の位置に関係なく、X = 2 の位置に表示されています。


相対的に移動させる (Transform.Translate)
----

現在の位置から移動量を指定して移動するには、[Transform.Translate() メソッド](https://docs.unity3d.com/ja/current/ScriptReference/Transform.Translate.html) を使用します。

{{< code lang="csharp" title="Transform.Translate メソッド" >}}
void Translate(Vector3 translation, Space relativeTo=Space.Self);
void Translate(Vector3 translation, Transform relativeTo);
void Translate(float x, float y, float z, Space relativeTo=Space.Self);
void Translate(float x, float y, float z, Transform relativeTo);
{{< /code >}}

`Space` 型の `relativeTo` パラメータでは、ローカル座標 (`Space.Self`) かワールド座標 (`Space.World`) のどちらで移動させるかを指定します。
デフォルトではローカル座標 (`Space.Self`) になっているので、自分自身のオブジェクトが向いている方向に応じた移動を行います。

次のサンプルコードでは、Cube オブジェクトを左右に行ったり来たりさせています。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private GameObject _cube;
    private Vector3 _direction = Vector3.right;

    private void Start() {
        _cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
    }

    private void Update() {
        // 移動方向を切り替える
        float x = _cube.transform.position.x;
        if (x > 5) {
            _direction = Vector3.left;
        } else if (x < -5) {
            _direction = Vector3.right;
        }

        // 移動方向に従って少しだけ移動する
        _cube.transform.Translate(_direction * 10 * Time.deltaTime);
    }
}
{{< /code >}}

