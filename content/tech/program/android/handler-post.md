---
title: "Android開発: Handler.post した Runnable タスクがメインスレッドで実行される仕組み"
linkTitle: "Handler.post した Runnable タスクがメインスレッドで実行される仕組み"
url: "/p/9ne93oc"
date: "2011-08-08"
tags: ["Android"]
---

Android プログラムで UI 操作を行う場合は、その処理はアプリのメインスレッドから行わなければいけません（Android に限らずほとんどの OS で同じような感じです）。
`Activity` の `onCreate` メソッドなどを呼び出しているのがそのアプリのメインスレッドであり、Android ではイコール UI スレッドです。
タイマー処理などで、UI を更新する場合はこのメインスレッドから行う必要があり、新しく立ち上げた別のスレッドから UI を更新することはできません。

このようなケースで利用できるのが __`android.os.Handler`__ クラスです。
`Handler#post` メソッドに `Runnable` オブジェクトを渡すと、最終的にその `Handler` オブジェクトを生成したスレッドで `Runnable` オブジェクトの `run` メソッドが実行されるようになっています。
つまり、`Handler` オブジェクトを `Application` のメインスレッドから（例えば `Activity` のフィールドとして）生成しておけば、そこに `post` した `Runnable` タスクは、メインスレッドで実行されることが保証されます。

`Handler#post` メソッドは、内部的にはスレッドローカルな `Looper` に結び付けられた `MessageQueue` オブジェクトに処理をキューイングしています。
`Handler` オブジェクトをメインスレッドから生成すれば、メインスレッド上の `MessageQueue` と結び付けられ、その `MessageQueue` が `Looper` によってメインスレッド上で処理されるという流れになっています。

別の言い方をすれば、メインスレッドの `MessageQueue` に対して、`Runnable` タスクを投入するためのインタフェースとなるのが `Handler` オブジェクトというわけです。

