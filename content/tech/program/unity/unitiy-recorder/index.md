---
title: "Unityメモ: Unity の動画を撮る (Unity Recorder)"
url: "/p/j8mzbnx"
date: "2020-02-10"
tags: ["Unity"]
weight: 101
---

**Unity Recorder** を使用すると、Unity のゲーム画面を簡単に動画ファイル（mp4 など）にして保存することができます。

Unity Recorder のインストール
----

Unity Recorder は Package Manager からインストールします（Unity 2018 より以前は Asset Store で配布されていましたが、Unity 2019 以降は Package Manager からインストールしたものしか動作しません）。
Package Manager は Unity のメニューから下記のように辿ると起動できます。

1. `Window` → `Package Manager`

Unity Recorder は、Preview Package として配布されているので、まず、Package Manager の `Advanced` というプルダウンメニューから **`Show preview packages`** を選択してください。

{{< image w="500" src="img-001.png" title="Preview Package を表示する" >}}

すると、リストに Unity Recorder が表示されるので、選択して `Install` ボタンを押せばインストールできます。

{{< image w="600" src="img-002.png" title="Unity Recorder のインストール" >}}

Unity Recorder で動画を作成する
----

まず Unity Recorder の設定を行い、mp4 で録画するための設定を行います。
メニューから次のように辿り、Recorder ウィンドウを開きます。

1. `Window` → `General` → `Recorder` → `Recorder Window`

この Recorder ウィンドウは、Inspector ウィンドウの下などにドッキングしておくと使いやすいです。
**`Add New Recorders`** ボタンを押すと、どんな形式のファイルを作成するかのプルダウンメニューが表示されるので、ここでは **`Movie`** を選択します。

{{< image w="500" src="img-settings-001.png" title="Recorder ウィンドウ" >}}

すると、動画ファイルの保存先や、ファイル名などを入力するフィールドが表示されるので、好きなように設定してください。
`File Name` のフィールドで、ワイルドカードを使ったファイル名を設定しておくと、録画するたびに自動的に異なるファイル名を付けてくれます。
例えば、`movie-<Take>` としておくと、`movie-001.mp4`、`movie-002.mp4` と連番を付けてファイルを作成してくれます。

{{< image w="500" src="img-settings-002.png" >}}

あとは、 **左上の録画開始ボタン** を押すと、Unity のシーンが自動的に実行され、同時に録画も開始されます。
シーンの実行を停止すると、録画も自動的に停止されます。

