---
title: "エミュレーター: RetroArch のメモ"
linkTitle: "RetroArch"
url: "/p/n9nybnx"
date: "2020-08-14"
tags: ["エミュレーター"]
---

設定ファイルの場所
----

キーコンフィグなどの設定ファイルは下記のディレクトリに格納されています。

{{< code title="macOS の場合" >}}
/Users/ユーザー名/Library/Application Support/RetroArch
{{< /code >}}

キー設定を間違えて、にっちもさっちもいかなくなってしまった場合は、この中の

- `config/retroarch.cfg`

というファイルを削除して RetroArch を再起動すれば OK です。

スキャンした ROM のリストや、セーブファイルなどは下記のディレクトリに格納されています。

{{< code title="macOS の場合" >}}
/Users/ユーザー名/Documents/RetroArch
{{< /code >}}


おすすめ設定
----

スクリーンショット (`F8`) やセーブファイルの保存先は、Dropbox などのクラウド上に保存するようにしておくと安心です。

- `設定` → `ディレクトリ`
  - `録画`
    - `$DROPBOX/fun-game/retro-records`
- `スクリーンショット`
    - `$DROPBOX/fun-game/retro-screenshots`
- `セーブファイル`
    - `$DROPBOX/fun-game/retro-saves`
- `ステートセーブファイル`
    - `$DROPBOX/fun-game/retro-states`


スクリーンキャプチャの解像度
----

- `Nintendo - NES / Famicom` の場合 → `256x224`

