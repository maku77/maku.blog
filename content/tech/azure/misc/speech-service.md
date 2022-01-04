---
title: "Azure Speech Service を使って音声をテキストに変換する (STT)"
url: "/p/5zx3ozz"
date: "2019-07-24"
tags: ["Azure", "Python", "音声認識"]
description: "Azure Speech Service を使った音声認識 (STT: Speech to Text) を試してみます。"
---

Microsoft の Cognitive Services のひとつとして提供されている [Speech Service](https://azure.microsoft.com/ja-jp/services/cognitive-services/speech-services/) を使用すると、音声をテキストに変換したり、逆にテキストを音声に変換したりすることができます。

ここでは、Python から Speech Service の機能を利用してみます（Windows 10 で動作確認済）。
実行するにはマイクのついた PC が必要です
マイクのついていない PC で実行すると `SPXERR_MIC_NOT_AVAILABLE` エラーが発生します。


準備
----

### Speech Service の準備

[Azure Portal](https://portal.azure.com/) から <samp>Speech</samp> のリソースを作成し、Subscription Key を取得しておいてください。

### Speech SDK のインストール

Python の `azure-cognitiveservices-speech` パッケージをインストールします。

```
$ pip install azure-cognitiveservices-speech
```

### Visual Studio C++ Redistributable のインストール

必要があれば、Visual Studio C++ の再頒布可能パッケージをインストールします。

- [Visual C++ 再頒布可能パッケージ](https://support.microsoft.com/ja-jp/help/2977003/the-latest-supported-visual-c-downloads) ([vc_redist.x64.exe](https://aka.ms/vs/16/release/vc_redist.x64.exe))


Python コード
----

### 一回だけ変換して終わるバージョン

{{< code lang="python" title="stt.py" >}}
import azure.cognitiveservices.speech as speechsdk

# この設定は適宜変更してください
subscription = "e1b5f0964ab743133b7de4f892741c7a"
region = "japaneast"
language = "ja-JP"
# proxy = ("proxy.example.com", 8888, "", "")

# SpeechConfig オブジェクトを生成します
speech_config = speechsdk.SpeechConfig(
    subscription=subscription,
    region=region,
    speech_recognition_language=language)
if 'proxy' in locals():
    speech_config.set_proxy(*proxy)

# SpeechRecognizer インスタンスを生成します
recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config)

print("何かしゃべってください")

result = recognizer.recognize_once()
if result.reason == speechsdk.ResultReason.RecognizedSpeech:
    print("「{}」".format(result.text))
elif result.reason == speechsdk.ResultReason.NoMatch:
    print("No speech could be recognized: {}".format(result.no_match_details))
elif result.reason == speechsdk.ResultReason.Canceled:
    cancellation_details = result.cancellation_details
    print("Speech Recognition canceled: {}".format(cancellation_details.reason))
    if cancellation_details.reason == speechsdk.CancellationReason.Error:
        print("Error details: {}".format(cancellation_details.error_details))
{{< /code >}}


### 繰り返し入力を受け付けるバージョン

こちらのバージョンは、<kbd>Ctrl+C</kbd> でプログラムを停止するまで、繰り返しユーザーの入力（発話）を受け付けます。

{{< code lang="python" title="stt-loop.py" >}}
import azure.cognitiveservices.speech as speechsdk

# この設定は適宜変更してください
subscription = "e1b5f0964ab743133b7de4f892741c7a"
region = "japaneast"
language = "ja-JP"
# proxy = ("proxy.example.com", 8888, "", "")

# SpeechConfig オブジェクトを生成します
speech_config = speechsdk.SpeechConfig(
    subscription=subscription,
    region=region,
    speech_recognition_language=language)
if 'proxy' in locals():
    speech_config.set_proxy(*proxy)

# SpeechRecognizer インスタンスを生成します
recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config)

# 非同期に返された認識結果を出力します
recognizer.recognized.connect(lambda evt: print('「{}」'.format(evt.result.text)))
recognizer.session_started.connect(lambda evt: print('SESSION STARTED: {}'.format(evt)))
recognizer.session_stopped.connect(lambda evt: print('SESSION STOPPED {}'.format(evt)))

try:
    print("何かしゃべってください")
    recognizer.start_continuous_recognition()
    import time
    time.sleep(100000)
except KeyboardInterrupt:
    print("バイバイ")
    recognizer.recognized.disconnect_all()
    recognizer.session_started.disconnect_all()
    recognizer.session_stopped.disconnect_all()
{{< /code >}}


実行
----

スクリプトを起動すると、「何かしゃべってください」と表示されるので、マイクに向かってしゃべると、その言葉が STT によりテキストに変換されて表示されます。
下記は、「こんにちは」としゃべった場合の出力例です。

{{< code >}}
C:\> python stt.py
何かしゃべってください
「こんにちは。」
{{< /code >}}


参考資料
----------------------------------------------------------------

ここの解説は下記のサンプルコードを参考にしています。

- [cognitive-services-speech-sdk/quickstart/python at master · Azure-Samples/cognitive-services-speech-sdk](https://github.com/Azure-Samples/cognitive-services-speech-sdk/tree/master/quickstart/python)

