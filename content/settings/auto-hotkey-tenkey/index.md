---
title: "テンキーを特定の URL を開くショートカットキーとして使う (AutoHotkey)"
url: "p/7v5nd8x/"
date: "2025-09-22"
tags: ["AutoHotkey", "PC設定"]
---

昔購入した USB の外付けテンキーボードが余っていたので、任意の URL をブラウザで開くショートカットキーとして活用することにしました（Blender の操作用に買ったのだけど結局使わなかった🙄）。

{{< image w="400" title="テンキーをアプリランチャーにする" src="img-001.png" >}}

最近は様々な AI Chat サービスを使うことが多いので、テンキーの `1`〜`6` キーに AI Chat 系サービスを割り当て、`7`～`9` によく見る社内のページを割り当ててみました。
ランチャーアプリを使えばいいじゃんと言われればそれまでなのですが、実際使ってみると思ったより物理的な起動ボタンは便利です。
ランチャーアプリよりも直感的で早いです。
もうこれのためにテンキーボードを買ってもいいくらい（言い過ぎ）。

下記は、Windows の [AutoHotkey](https://ahkscript.github.io/) というソフトウェアを使ってテンキーに処理を割り当てる設定例です。

{{< code lang="ahk" title="maku-keyconfig.ahk" >}}
#Requires AutoHotkey v2.0

; 指定されたURLを既定のブラウザで開き、Edge または Chrome をアクティブにする
OpenUrl(url) {
    Run url
    if WinExist("ahk_exe msedge.exe")
        WinActivate "ahk_exe msedge.exe"
    else if WinExist("ahk_exe chrome.exe")
        WinActivate "ahk_exe chrome.exe"
}

Numpad1::OpenUrl "https://www.perplexity.ai"
Numpad2::OpenUrl "https://m365.cloud.microsoft/chat/?auth=2"
Numpad3::OpenUrl "https://chat.openai.com/"
Numpad7::OpenUrl "https://..."
Numpad8::OpenUrl "https://..."
Numpad9::OpenUrl "https://..."
{{< /code >}}

この `.ahk` ファイルをダブルクリックして実行しておけば、テンキーの `1`〜`9` キーを押すだけで指定した URL を既定のブラウザで開きます。
さらに、上のスクリプトでは、Edge または Chrome が起動していれば、そのウィンドウをアクティブにするようにしています（`OpenUrl` 関数を調整すれば、他のブラウザにも対応できます）。

例えば、Perplexity に何か聞いてみようと思ったら、おもむろにテンキーの `1` キーを押せば、ブラウザで Perplexity のページが開いて入力欄がアクティブになるので、すぐに質問を入力できます。

٩(๑❛ᴗ❛๑)۶ 最強最速！わーぃ

