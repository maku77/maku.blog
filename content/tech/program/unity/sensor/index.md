---
title: "Unityスクリプト: 加速度センサー／ジャイロスコープの値を取得する"
url: "/p/m6hs3dn"
date: "2020-02-24"
tags: ["Unity"]
---

加速度センサ (Input.acceleration)
----

加速度センサからの入力を取得するには、[Input.acceleration](https://docs.unity3d.com/jp/current/ScriptReference/Input-acceleration.html) プロパティを参照します。
`Vector3` 型で、X/Y/Z 軸の加速度を -1.0 〜 +1.0 の範囲で取得できます。

{{< code lang="cs" >}}
Vector3 accel = Input.acceleration;
{{< /code >}}

次の例では、`Update()` のタイミングで加速度を読み取り、画面上のテキストで X, Y, Z 各軸の加速度を表示しています。

{{< image w="400" src="img-001.png" >}}

{{< code lang="cs" title="Sample.cs" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private Vector3 m_accel;

    void Update() {
        m_accel = Input.acceleration;
    }

    private void OnGUI() {
        var rect = new Rect(30, 30, 500, 50);
        GUI.skin.label.fontSize = 30;
        GUI.Label(rect, string.Format("X={0:F2}, Y={1:F2}, Z={2:F2}",
            m_accel.x, m_accel.y, m_accel.z));
    }
}
{{< /code >}}


ジャイロスコープ (Input.gyro)
----

ジャイロスコープからの入力を取得するには、[Input.gyro](https://docs.unity3d.com/jp/current/ScriptReference/Input-gyro.html) プロパティを参照し、[Gyroscope](https://docs.unity3d.com/jp/current/ScriptReference/Gyroscope.html) オブジェクトを取得します。

{{< code lang="cs" >}}
Gyroscope gyro = Input.gyro;
{{< /code >}}

`Gyroscope` の機能を使用するには、`enabled` プロパティを `true` に設定しておく必要があります。

{{< code lang="cs" >}}
Input.gyro.enabled = true;
{{< /code >}}

`Gyroscope` オブジェクトからは、次のような値を取得できます。

- `Gyroscope.attitude` ... デバイスの傾き具合 (Quaternion)
- `Gyroscope.rotationRate` ... デバイスの回転率 (Vector3)

`attitude` プロパティは、デバイスの傾きを `Quaternion` で取得できるので、そのままゲームオブジェクトの `transform.rotation` プロパティに設定してやれば、デバイスの傾きとオブジェクトの傾きを一致させることができます。

次の例では、デバイスの傾き具合と回転率を画面上に表示しています。
また、傾きに応じて、スクリプトをアタッチしたオブジェクトを回転させています。

{{< image w="400" src="img-002.png" >}}

{{< code lang="cs" title="Sample.cs" >}}
using UnityEngine;

public class Sample : MonoBehaviour {
    private Gyroscope m_gyro;

    private void Start() {
        Input.gyro.enabled = true;
    }

    void Update() {
        m_gyro = Input.gyro;
        transform.rotation = m_gyro.attitude;
    }

    private void OnGUI() {
        var rect1 = new Rect(30, 30, 600, 50);
        var rect2 = new Rect(30, 60, 600, 50);
        GUI.skin.label.fontSize = 30;
        GUI.Label(rect1, "attitude=" + m_gyro.attitude);
        GUI.Label(rect2, "rotationRate=" + m_gyro.rotationRate);
    }
}
{{< /code >}}

