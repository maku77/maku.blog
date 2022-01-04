---
title: "Android開発: 3種類のタイムスタンプの使い分け"
linkTitle: "3種類のタイムスタンプの使い分け"
url: "/p/sfvqxtr"
date: "2010-11-09"
tags: ["Android"]
---

{{% private %}}
- 参考: http://developer.android.com/reference/android/os/SystemClock.html
{{% /private %}}

Android において long 型のタイムスタンプを取得する方法には、以下の 3 種類があり、それぞれ意味が異なるため、用途に応じて使い分ける必要があります。

- System. __currentTimeMillis()__
    - Epoch (1970-01-01) を基準にしたシステムタイムまでの経過ミリ秒。
    - ユーザがシステムの現在時刻を変更すると、過去にジャンプしたりするので、タイムスタンプベースのタイマーなどに使用するべきではない。時計の UI を表示するときに使用する。
- android.os.SystemClock. __elapsedRealtime()__
    - Android 端末をブートしてからの経過ミリ秒。`uptimeMillis()` とは異なり、スリープ中にもカウントされる。
    - AlarmManager はスリープ中の時間もカウントして発火するので、内部で `elapsedRealtime()` ベースのタイマを使用する。これは、AlarmManager のタイプで `ELAPSED_REALTIME (ELAPSED_REALTIME_WAKEUP)` を指定した場合の振る舞いで、`RTC (RTC_WAKEUP)` を指定すれば、`System.currentTimeMillis()` ベースのタイマで発火するように指定することもできる。
- android.os.SystemClock. __uptimeMillis()__
    - Android 端末をブートしてからの経過ミリ秒。スリープ中にはカウントされない。
    - タイムスタンプベースのタイマーに使用できる。例えば `Handler` クラスは、`uptimeMillis()` ベースのタイマを使用して非同期コールバックを発火している。

| | AlarmManager | Handler.<br>postAtTime | Handler.<br>sendMessageAtTime |
| ---- | :--: | :--: | :--: |
| <b>System.<wbr>currentTimeMillis</b><br><br>UTC<wbr>（システム時刻設定で変化） | ○<br>RTC<wbr>(_WAKEUP) | × | × |
| <b>SystemClock.<wbr>elapsedRealtime</b><br><br>ブートからの経過時間<wbr>（スリープ時間含む） | ○<br>ELAPSED_REALTIME<wbr>(_WAKEUP) | × | × |
| <b>SystemClock.<wbr>uptimeMillis</b><br><br>ブートからの経過時間<wbr>（スリープ時間含まず） | ×<br>（スリープ時カウントできないため） | ○ | ○ |

