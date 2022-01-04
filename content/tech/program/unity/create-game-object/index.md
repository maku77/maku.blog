---
title: "Unityスクリプト: 空のゲームオブジェクトを作成する (GameObject)"
url: "/p/j4fqyaj"
date: "2020-01-01"
tags: ["Unity"]
---

{{< code lang="csharp" title="GameObject クラス" >}}
public GameObject();
public GameObject(string name);
public GameObject(string name, params Type[] components);
{{< /code >}}

Unity のシーンは、様々なゲームオブジェクトを配置することで構築していきます。
[GameObject クラスのコンストラクタ](https://docs.unity3d.com/ja/current/ScriptReference/GameObject-ctor.html) を使用して、空の `GameObject` インスタンスを生成することができます。

`GameObject` インスタンスを生成した後は、そこにコンポーネントを追加することで、オブジェクトの性質を付け加えていくことができます。

次の例では、`MyGameObject` という名前を付けて空のゲームオブジェクトを作成しています。
作成した `GameObject` インスタンスはフィールドで保持していませんが、明示的に削除するまではシーン上に残ります。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    void Start() {
        var obj = new GameObject("MyGameObject");
    }
}
{{< /code >}}

動的に作成されたゲームオブジェクトは、Hierarchy ビュー上で確認することができます。

{{< image w="500" border="true" src="create-game-object-001.png" >}}

空のゲームオブジェクトであっても、`Transform` コンポーネントだけはデフォルトで設定されているため、座標を移動するための矢印が画面上に表示されます。

