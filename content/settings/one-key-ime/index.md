---
title: "左手小指だけで IME 操作 (AutoHotkey, Karabiner-Elements)"
url: "p/s3subpp/"
date: "2025-05-01"
tags: ["PC設定", "Karabiner-Elements", "AutoHotkey"]
---

何をやるか？
----

{{< image w="400" src="img-001.png" title="小指で IME の ON/OFF" >}}

Mac や Windows の日本語入力のオン／オフ (IME ON/OFF) 切り替えを、左手の小指だけでできるようにしてみます。

これまでずっと、`Cmd + Space` などのキーコンビネーションで IME の ON/OFF をしていたのですが、やはり **IME ON/OFF 切り替えは別々のキーに割り当てた方が望ましい** と思い、このたびがんばって設定することにしました。

新しい入力方法に慣れれば、「今 IME ON だっけ？ OFF だっけ？」と悩むことがなくなるので、毎日のストレスがほんの少しだけ軽減すると思います。
また、`Cmd + Space` などのキーコンビネーションが、アプリケーションのショートカットキーとバッティングしてしまうトラブルも避けられます。

前提条件は次の通りです。

* macOS と Windows の両方で同じようにキー操作できること
* キーコンビネーションではなく、異なる単独キーで IME ON/OFF できること
* US（英字配列）キーボードを使うので、「かな」「英数」などのキーがなくてもよいこと
* 無料のソフトで設定できること

最終的には、次のように設定することにしました。

* 使用するアプリケーション
  * macOS: [Karabiner-Elements](https://karabiner-elements.pqrs.org/)
  * Windows: [AutoHotkey](https://www.autohotkey.com/)
* キー設定
  * **`CapsLock` キーの単押し** → IME OFF
  * **左 `Shift` キーの単押し** → IME ON

今回は左小指で押せるキーを使いましたが、スペースキーの左右にある `Cmd` キーや `Alt` キーに割り当てるのもありだと思います。
最初は左右の `Shift` キーに割り当ててみたのですが、どうしても右側の `Shift` キーが押しにくく、上記のようなキー割り当てに落ち着きました。


macOS の設定
----

macOS では [Karabiner-Elements](https://karabiner-elements.pqrs.org/) を使って設定します。
メニューから、**`Complex Modifications`** ⇨ **`Add your own rule`** を選択し、下記のように設定して保存すれば完了です。

{{< code lang="json" title="Karabiner-Elements 用の設定ファイル" >}}
{
    "description": "CapsLockキー単押しでIMEオフ、左Shiftキー単押しでIMEオン",
    "manipulators": [
        {
            "from": {
                "key_code": "caps_lock",
                "modifiers": { "optional": ["any"] }
            },
            "parameters": {
                "basic.to_if_alone_timeout_milliseconds": 250,
                "basic.to_if_held_down_threshold_milliseconds": 250
            },
            "to": [
                {
                    "key_code": "caps_lock",
                    "lazy": true
                }
            ],
            "to_if_alone": [{ "key_code": "japanese_eisuu" }],
            "to_if_held_down": [{ "key_code": "caps_lock" }],
            "type": "basic"
        },
        {
            "from": {
                "key_code": "left_shift",
                "modifiers": { "optional": ["any"] }
            },
            "parameters": {
                "basic.to_if_alone_timeout_milliseconds": 250,
                "basic.to_if_held_down_threshold_milliseconds": 250
            },
            "to": [
                {
                    "key_code": "left_shift",
                    "lazy": true
                }
            ],
            "to_if_alone": [{ "key_code": "japanese_kana" }],
            "to_if_held_down": [{ "key_code": "left_shift" }],
            "type": "basic"
        }
    ]
}
{{< /code >}}

Karabiner-Elements は[本当に色々な設定ができる](https://ke-complex-modifications.pqrs.org/)ので、使いこなせるようになったらすごく便利そうです。
でもやりすぎは禁物。


Windows の設定
----

Windows の場合は、[AutoHotkey](https://www.autohotkey.com/) というソフトウェアを使います。
macOS のときより、若干手順は複雑です。

### 1. CapsLock キーと Control キーと入れ替える

まず、`A` キーの左にある `CapsLock` キーを `Control` キーに入れ替えておきます。
Happy Hacking Keyboard など、ハードウェアスイッチがあるキーボードの場合はそれで設定すれば OK です。

キーの入れ替えは、Microsoft が提供している下記のいずれかのツールで簡単に設定できます。

- [Ctrl2Cap.exe](https://learn.microsoft.com/ja-jp/sysinternals/downloads/ctrl2cap)
  - キー入れ替え専用のツールです。
- [PowerToys](https://learn.microsoft.com/ja-jp/windows/powertoys/install)
  - いろんな便利ツールが入ってるので、この中の Keyboard Manager という機能を使います。

### 2. IME の設定を変更す

次に、IME の設定を変更し、`無変換` キーを「IME-オフ」、`変換` キーを「IME-オン」に割り当てておきます。

1. **`Windows`** キーでスタートメニューを開いて **`IME`** で検索
2. **`日本語 IME 設定`** → **`キーとタッチのカスタマイズ`** を選択

次のように設定します。

{{< image border="true" src="img-002.png" title="Windows の IME 設定" >}}

### 3. AutoHotkey のスクリプトの作成

最後に、AutoHotkey 用のスクリプトファイルを作成します。
**`.ahk`** という拡張子を付けて適当なディレクトリに保存してください。

{{< code lang="ahk" title="C:\config\maku-keyconfig.ahk" >}}
#Requires AutoHotkey v2.0
; https://maku.blog/p/s3subpp/

; 左Controlキーが単独で離されたときの処理
~LControl Up::
{
    if (A_PriorKey = "LControl") {
        Send "{vk1Dsc07B}"  ; 「無変換」キーをエミュレート (IME OFF)
    }
}

; 左Shiftキーが単独で離されたときの処理
~LShift Up::
{
    if (A_PriorKey = "LShift") {
        Send "{vk1Csc079}"  ; 「変換」キーをエミュレート (IME ON)
    }
}
{{< /code >}}

ここでは、左 `Control` キーを単独で押しときに `無変換` キーを押したかのように振る舞い、左 `Shift` キーを押したときに `変換` キーを押したかのように振る舞うように設定しています。

AutoHotkey がインストールされた環境でこのスクリプトファイルを **ダブルクリックして起動** すると、キーの入れ替えが有効になります。
無事に起動すると、タスクトレイに {{< image-inline w="30" src="img-003.png" >}} アイコンが表示されます。

Windows の起動時にこの設定を有効にしたいときは、この `.ahk` ファイルのショートカットを、Windows のスタートアップフォルダに作成しておきます。
スタートアップフォルダは、**`Win + R`** ⇨ **`shell:startup`** で開くことができます。

