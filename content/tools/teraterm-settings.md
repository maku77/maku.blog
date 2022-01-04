---
title: "TeraTerm でログを自動保存する"
url: "/p/oi9yak2"
date: "2009-07-17"
tags: ["アプリ", "TeraTerm"]
---


TeraTerm 起動時にログを自動でファイルに保存するようにする
----

（Version: TeraTerm 4.62 で確認）

1. メニューから、<kbd><samp>Setup</samp></kbd> → <kbd><samp>Additional settings...</samp></kbd>
2. Log のタブを開き、例えば以下のように設定
    - Default Log File Name (strftime format): <kbd>%Y%m%dT%H%M%S-log.txt</kbd>
    - Default Log Save Folder: <kbd>D:\y\log\teraterm</kbd>
    - Auto start loggin: <kbd>Check</kbd>
3. 最後に <kbd><samp>Setup</samp></kbd> → <kbd><samp>Save setup</samp></kbd> で設定を保存

これで、TeraTerm 起動時に自動で `20090717T201506-log.txt` のような名前で、ログファイルを作成してくれるようになります。


ログファイルの各行に Windows システム時刻を付加する
----

以下のように設定しておくと、

1. メニューから <kbd><samp>File</samp></kbd> → <kbd><samp>Log...</samp></kbd> （ログ取得中の場合はいったんログ取得を停止してから）
2. Option から <kbd><samp>Timestamp</samp></kbd> にチェックを入れて <kbd><samp>Save</samp></kbd>
3. 最後に <kbd><samp>Setup</samp></kbd> → <kbd><samp>Save setup</samp></kbd> で設定を保存

ログファイルの各行に以下のような、時刻のプレフィックスが付くようになります。

```
[Tue Jun 28 13:37:27.890 2011] ...
[Tue Jun 28 13:37:30.590 2011] ...
[Tue Jun 28 13:37:30.590 2011] ...
```


ログ取得のダイアログを非表示する
----

ログ取得中のダイアログが邪魔な場合は、以下のように消すことができます。

1. メニューから <kbd><samp>File</samp></kbd> → <kbd><samp>Log...</samp></kbd> （ログ取得中の場合はいったんログ取得を停止してから）
2. Option から <kbd><samp>Hide dialog</samp></kbd> にチェックを入れて <kbd><samp>Save</samp></kbd>
3. 最後に <kbd><samp>Setup</samp></kbd> → <kbd><samp>Save setup</samp></kbd> で設定を保存

一時的に表示させたい場合は、以下のようにします。

1. メニューから <kbd><samp>File</samp></kbd> → <kbd><samp>Show Log dialog...</samp></kbd>

