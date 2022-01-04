---
title: "Unityスクリプト: オブジェクトを回転する (Transform.rotation)"
url: "/p/dx9is3c"
date: "2020-02-10"
tags: ["Unity"]
weight: 1002
---

ゲームオブジェクトに設定された `Transform` コンポーネントを操作することで、ゲームオブジェクトを回転させることができます。
`Transform` コンポーネントのオブジェクトは下記のいずれかの方法で取得できます。

{{< code lang="csharp" >}}
Transform tf = gameObject.GetComponent<Transform>();
Transform tf = gameObject.transform;
{{< /code >}}

[Transform クラス](https://docs.unity3d.com/ja/current/ScriptReference/Transform.html) には、ゲームオブジェクトを回転させるためのプロパティやメソッドが定義されています。


ワールド座標での回転 (Transform.rotation)
----

**`Transform.rotation`** プロパティは、ワールド座標でのオブジェクトの回転角度を示します。
次の例では、Y 軸（上下方向）を中心とした回転角度を 45° に設定しています。

{{< image w="200" src="img-rotation.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube.transform.rotation = Quaternion.Euler(0, 45, 0);
    }
}
{{< /code >}}

`Transform.rotation` プロパティには [Quaternion オブジェクト](https://docs.unity3d.com/ja/current/ScriptReference/Quaternion.html) を設定するのですが、上記のように [Quaternion.Euler() 関数](https://docs.unity3d.com/ja/current/ScriptReference/Quaternion.Euler.html) を使用すると、直感的に Quaternion オブジェクトを生成することができます。


ローカル座標での回転 (Transform.localRotation)
----

オブジェクト自身の向きを考慮した軸（ローカル座標）での回転角度を設定するには、`Transform.rotation` プロパティの代わりに、 **`Transform.localRotation`** プロパティを使用します。

次の例では、親 Cube の上下に子 Cube を配置し、それぞれ Y 軸の回転角度として 30° を設定しています。
ただし、上の Cube は `localRotation` プロパティ、下の Cube は `rotation` プロパティで回転角度を設定しています。

{{< image w="300" src="img-local-rotation.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject parent = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject child1 = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject child2 = GameObject.CreatePrimitive(PrimitiveType.Cube);
        child1.transform.position = new Vector3(0, 1.5f, 0);
        child2.transform.position = new Vector3(0, -1.5f, 0);

        // Set parent-child relations
        child1.transform.parent = parent.transform;
        child2.transform.parent = parent.transform;

        // Rotate objects
        parent.transform.rotation = Quaternion.Euler(0, 30, 0);
        child1.transform.localRotation = Quaternion.Euler(0, 30, 0);
        child2.transform.rotation = Quaternion.Euler(0, 30, 0);
    }
}
{{< /code >}}

親オブジェクトを回転させると、子オブジェクトもそれに追従して回転します。
上の Cube は親オブジェクトの回転角度 30° をベースとして、自分自身の回転角度 (`localRotation`) の 30° で回転しているので、合計 60° の回転角度になっています。
一方、下の Cube はグローバル座標の回転角度 (`rotation`) を 30° に設定しているので、結局、親 Cube と同じ 30° しか回転していません。


相対的に回転させる (Transform.Rotate)
----

現在の回転角度からの回転量を指定して回転させるには、[Transform.Rotate() メソッド](https://docs.unity3d.com/jp/current/ScriptReference/Transform.Rotate.html) を使用します。

{{< code lang="csharp" title="Transform.Rotate メソッド" >}}
void Rotate(Vector3 eulers, Space relativeTo=Space.Self);
void Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo=Space.Self);
void Rotate(Vector3 axis, float angle, Space relativeTo=Space.Self);
{{< /code >}}

`relativeTo` パラメータに `Space.World` を指定すると、ワールド座標の軸方向をベースにして回転角度を指定することができます。
デフォルトは `Space.Self` になっており、自分自身の向き（ローカル座標）を考慮した回転角度を指定します。

次の例では、Cube オブジェクトを Y 軸を中心に 1 秒間に 90° 回転させています。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private GameObject _cube;

    private void Start() {
        _cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
    }

    private void Update() {
        _cube.transform.Rotate(0, 90 * Time.deltaTime, 0);

        // 下記のようにも書けます
        // _cube.transform.Rotate(Vector3.up * 90 * Time.deltaTime);
    }
}
{{< /code >}}

