---
title: "Unityスクリプト: オブジェクトを拡大縮小する (Transform.localScale)"
url: "/p/neuatgr"
date: "2020-02-11"
tags: ["Unity"]
weight: 1003
---

ゲームオブジェクトに設定された `Transform` コンポーネントを操作することで、ゲームオブジェクトを拡大縮小することができます。
`Transform` コンポーネントのオブジェクトは下記のいずれかの方法で取得できます。

{{< code lang="csharp" >}}
Transform tf = gameObject.GetComponent<Transform>();
Transform tf = gameObject.transform;
{{< /code >}}


オブジェクトの拡大率を指定する (Transform.localScale)
----

オブジェクトの拡大縮小の設定は **`Transform.localScale`** プロパティで行います。
[位置の設定 (position)](/p/53o7p8p) や [回転角度の設定 (rotation)](/p/dx9is3c) には、ワールド座標とローカル座標の区別がありますが、拡大縮小には、`localScale` の一種類しかないのでシンプルです。

次の例では、Cube オブジェクトを X 軸方向に 4 倍、Y 軸方向に 0.5 倍、Z 軸方向に 2 倍に拡大縮小しています。

{{< image w="300" src="img-local-scale-001.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube.transform.localScale = new Vector3(4, 0.5f, 2);
    }
}
{{< /code >}}

全ての軸に同じ比率だけ拡大縮小したいのであれば、`Vector3(1, 1, 1)` を示す定数 `Vector3.one` を使って次のように記述できます。

{{< code lang="csharp" >}}
cube.transform.localScale = Vector3.one * 2;
{{< /code >}}


拡大率は親の拡大率を引き継ぐ
----

オブジェクトが親子関係があるとき、`Transform.localScale` による拡大率は、必ず親オブジェクトの拡大率を継承します。
次の例では、3 つの Cube オブジェクトに `cube1` ← `cube2` ← `cube3` という親子関係を持たせ、それぞれ X 軸の拡大率を 2 倍に設定しています。

{{< image w="300" src="img-local-scale-002.png" >}}

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private void Start() {
        GameObject cube1 = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject cube2 = GameObject.CreatePrimitive(PrimitiveType.Cube);
        GameObject cube3 = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube2.transform.position = new Vector3(0, -1, 0);
        cube3.transform.position = new Vector3(0, -2, 0);

        // Set parent-child relations
        cube2.transform.parent = cube1.transform;
        cube3.transform.parent = cube2.transform;

        // Scale objects
        cube1.transform.localScale = new Vector3(2, 1, 1);
        cube2.transform.localScale = new Vector3(2, 1, 1);
        cube3.transform.localScale = new Vector3(2, 1, 1);
    }
}
{{< /code >}}

拡大率が親子で引き継がれるため、`cube1`、`cube2`、`cube3` の表示上の拡大率は、2倍、4倍、8倍となっていることが分かります。

{{< note >}}
X、Y、Z 軸のいずれかの拡大率を 0 にしてしまうと、オブジェクトの大きさが 0 になって見えなくなってしまうことに注意してください。
{{< /note >}}


ワールド座標におけるグローバルスケールを取得する (lossyScale)
----

上記の例で、`cube1`、`cube2`、`cube3` の表示上の拡大率は 2倍、4倍、8倍になっていると説明しました。
このような、ワールド座標におけるグローバルな拡大率は、[Transform.lossyScale プロパティ](https://docs.unity3d.com/ja/current/ScriptReference/Transform-lossyScale.html) で取得することができます。
この `lossyScale` プロパティは読み取り専用のため、拡大率の指定はあくまで `localScale` プロパティの方で行う必要があります。

{{< code lang="csharp" >}}
private void Start() {

    // ...省略...

    Debug.Log("cube1 localScale=" + cube1.transform.localScale);
    Debug.Log("cube2 localScale=" + cube2.transform.localScale);
    Debug.Log("cube3 localScale=" + cube3.transform.localScale);

    Debug.Log("cube1 lossyScale=" + cube1.transform.lossyScale);
    Debug.Log("cube2 lossyScale=" + cube2.transform.lossyScale);
    Debug.Log("cube3 lossyScale=" + cube3.transform.lossyScale);
}
{{< /code >}}

{{< code title="実行結果" >}}
cube1 localScale=(2.0, 1.0, 1.0)
cube2 localScale=(2.0, 1.0, 1.0)
cube3 localScale=(2.0, 1.0, 1.0)
cube1 lossyScale=(2.0, 1.0, 1.0)
cube2 lossyScale=(4.0, 1.0, 1.0)
cube1 lossyScale=(8.0, 1.0, 1.0)
{{< /code >}}

