---
title: "Unityスクリプト: デバッグログを表示する (Debug.Log)"
url: "/p/bxcp2dp"
date: "2020-01-01"
tags: ["Unity"]
---

デバッグログの基本
----

[Debug.Log()](https://docs.unity3d.com/ja/current/ScriptReference/Debug.Log.html) 系の関数を使用すると、Unity の Console ビュー内にデバッグ用ログを出力することができます。

{{< code lang="csharp" title="Debug クラス" >}}
public static void Log(object message);
public static void LogWarning(object message);
public static void LogError(object message);
{{< /code >}}

デフォルトのレイアウトでは、Conosle ビューは画面の下の方に配置されています。

{{< image w="400" border="true" src="log-001.png" >}}

`Debug.Log()` の代わりに、`Debug.LogWarning()` で警告レベルのログ、`Debu.LogError()` でエラーレベルのログを出力できます。
Console ビュー上に表示されるアイコンも変化します。

{{< code lang="csharp" title="Debug.Log() の使用例" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    void Start() {
        Debug.Log("Normal Log");
        Debug.LogWarning("Warning Log");
        Debug.LogError("Error Log");
    }
}
{{< /code >}}


デバッグログをゲームオブジェクトに関連づける
----

{{< code lang="csharp" title="Debug クラス" >}}
public static void Log(object message, Object context);
public static void LogWarning(object message, Object context);
public static void LogError(object message, Object context);
{{< /code >}}

{{< image w="400" border="true" src="log-002.png" >}}

`Debug.Log()` 系の関数で、2 つ目のパラメータに任意のゲームオブジェクトを設定しておくと、Console ビュー上に出力されたログをクリックすることで、そのゲームオブジェクトをハイライト表示することができます。

スクリプトをアタッチしたゲームオブジェクトを指定するには、次のように `gameObject` を指定するか、`this` を指定します。
Main Camera にスクリプトがアタッチされている場合は、上記イメージのように Hierarchy ビュー上の Main Camera がハイライトされます。

{{< code lang="csharp" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    void Start() {
        Debug.Log("Hello", gameObject);
    }
}
{{< /code >}}

