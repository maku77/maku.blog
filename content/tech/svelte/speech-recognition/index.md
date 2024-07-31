---
title: "Svelte 実装例: Web ブラウザで音声認識する (SpeechRecognition)"
url: "p/bf4cpjx/"
date: "2024-07-31"
tags: ["Svelte", "音声認識"]
---

何をするか？
----

Web ブラウザーの [SpeechRecognition](https://developer.mozilla.org/en-US/docs/Web/API/SpeechRecognition) API を使って、音声認識をしてみます。
ここでは Svelte アプリケーションとして作成しますが、単純な HTML + JavaScript の組み合わせでもほぼ同様のコードになると思います（参考: [Svelte 関連メモ](/p/td962u6/)）。

<iframe allow="microphone" frameborder="0" style="border: none" width="100%" height="300" src="https://p-bf4cpjx-svelte-speech-recognition.vercel.app" style="resize: both; overflow: auto;"></iframe>

- デモ: https://p-bf4cpjx-svelte-speech-recognition.vercel.app/
- ソースコード: https://github.com/maku77/p-bf4cpjx-svelte-speech-recognition/


プロジェクトの準備
----

Svelte (SvelteKit) のプロジェクトがない場合は最初に作成しておきます。

{{< code lang="console" title="プロジェクトの作成" >}}
$ npm create svelte@latest myapp
(選択肢が表示されたら TypeScript を選択しておく）
$ cd myapp
$ npm install
{{< /code >}}

`SpeechRecognition` はブラウザ標準の API として策定されているものですが、TypeScript の型情報が認識されなかったので DefinitelyTyped で提供されている型情報 __`@types/dom-speech-recognition`__ をインストールしておきます。

{{< code lang="console" title="型情報のインストール" >}}
$ npm install --save-dev @types/dom-speech-recognition
{{< /code >}}

これで、`window.SpeechRecognition` コンストラクタや、`SpeechRecognitionResult` などの各型情報を参照できるようになります。


SpeechRecognition の使い方
----

`SpeechRecognition` による音声認識の基本的な流れは次のようになります。

1. `SpeechRecognition` インスタンスを生成して、各種パラメーターを設定する。
2. `SpeechRecognition.onresult` プロパティに、認識結果を受け取るためのコールバック関数を設定する。
3. `SpeechRecognition.start()` でマイクからの音声キャプチャと音声認識を開始する。


SpeechRecognition インスタンスの生成
----

まず、`SpeechRecognition` インスタンスを生成して、各種パラメータを設定していきます。
次のような感じで関数化しておくと分かりやすいです。

```ts
/**
 * SpeechRecognition インスタンスを生成し基本的なパラメーター設定を行います。
 */
function createSpeechRecognition(): SpeechRecognition {
	// ブラウザによって SpeechRecognition インスタンスの生成方法が異なる
	const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;

	const recognition = new SpeechRecognition();
	recognition.continuous = true; // 発声が途切れてもキャプチャを終了しない
	recognition.interimResults = true; // 認識途中の結果を得る
	recognition.lang = 'ja-JP'; // 認識対象の言語
	recognition.maxAlternatives = 1; // 認識結果の候補の最大数

	return recognition;
}
```

`SpeechRecognition` のコンストラクタは `window` オブジェクトから参照できますが、Chrome ブラウザの場合は `window.webkitSpeechRecognition` だったりするので注意してください。
ここで設定しているプロパティを簡単に説明しておきます。

__`recognition.continuous = true`__
: 発声が途切れても音声認識を継続するかどうかを制御します。
このプロパティを `true` にすると、ユーザーの発話が終了したと認識した後も継続して音声入力を受け付け、音声認識を続けます。
つまり、ずーっと継続して音声認識し続けたいアプリケーションなどでは `true` に設定します。
具体的には、`start()` 実行後に `onresult` プロパティにセットしたコールバック関数が呼び出され続けます。
デフォルトは `false` で、一区切りの発声が終わったときに音声キャプチャと認識が自動的に終了します。

__`recognition.interimResults = true`__
: 音声認識の結果に暫定の認識結果を含めるかを制御します。
このプロパティを `true` にすると、`onresult` コールバックに渡されるイベントオブジェクトの `SpeechRecognitionResultList` に、`isFinal` プロパティが `false` の結果（つまり暫定の認識結果）も含まれるようになります。
このあたりはちょっと難しいけど、この記事を最後まで読めばたぶん分かります。

__`recognition.lang`__
: 音声認識に使用する言語を設定するプロパティです。
デフォルトでは Web サイトに設定された言語属性が使用されます（例: `ja-JP`）。
実際のアプリケーションではユーザーが選択できるようにしておくと親切です。

__`recognition.maxAlternatives`__
: 音声認識の結果候補の最大数を設定あるいは取得するためのプロパティです。
デフォルトは 1 ですが、2 以上の値を設定することで、認識結果の候補を複数取得することができます。
多くのアプリケーションでは 1 のまま使うことになると思います。


result イベントで音声認識の結果を処理する
----

`SpeechRecognition` オブジェクトの __`onresult`__ プロパティにコールバック関数をセットしておくと、音声認識処理が進むたびにその結果を受け取ることができます。
ここが一番難しいところです。

```ts
// let interimTranscript = ''; // 暫定の認識結果（表示用）
// let finalTranscript = ''; // 最終的な認識結果（表示用）

recognition.onresult = (event: SpeechRecognitionEvent) => {
	let interim = '';
	for (let i = event.resultIndex; i < event.results.length; i++) {
		const tr = event.results[i][0].transcript; // i 番目のチャンクの認識結果
		if (event.results[i].isFinal) {
			finalTranscript += tr; // 最終的な認識結果として表示
		} else {
			interim += tr;
		}
	}
	interimTranscript = interim; // 暫定の認識結果として表示
};
```

音声認識の結果は、`SpeechRecognitionEvent` 型のイベントオブジェクトとして渡されます。
音声認識は複数のチャンクが並行して処理されていきます。
各チャンクの認識結果（認識途中のこともある）が、イベントオブジェクトの __`results`__ プロパティの中に __`SpeecRecognitionResult`__ の配列として入っています。
`i` 番目のチャンクが実際にどのようなテキストとして認識されたかは、次のように参照することができます。

```ts
// i 番目の認識結果のテキスト
event.results[i][0].transcript;
```

入れ子の配列になっているのは、認識結果の候補が複数得られる可能性があるからです。
今回は、`maxAlternatives=1` と設定したので、候補は 1 つしか含まれておらず、必ずインデックス 0 で参照します。

あとは、各チャンクの認識結果をループで取り出していけばよいのですが、各チャンクの認識はまだ途中のことがあります。
そのチャンクの認識が完了しているかどうかは、__`event.results[i].isFinal == true`__ で判別することができます。

`onresult` の処理がややこしいのは、音声認識が少し進むたびにこのイベントハンドラーが呼び出されるからです。
つまり、認識が進むたびに、`event.results` の中の結果が更新されながら何度も呼び出されます。
`onresult` が呼び出されるたびに `event.results` を最初からループ処理するのは効率が悪いので、前回の `onresult` 呼び出しでどのチャンクまで認識完了したか（`isFinal=true` になったか）を示すインデックスを __`event.resultIndex`__ で参照できるようになっています。
このインデックスをループの開始インデックスとすることで、更新されたチャンクだけを処理することができます。

{{< image w="700" src="img-001.drawio.svg" title="SpeechRecognition.onresult に渡されるイベントオブジェクト" >}}

アプリケーション内で、音声認識が完了した部分のテキストを逐次処理したいときは、`isFinal=true` になった部分のチャンクを結合して扱えば OK です。
上記の実装例では、`finalTranscript` に格納されたテキストが、音声認識が完了した部分です。


音声認識を開始する
----

`SpeechRecognition` の設定が終わったら、あとは __`start()`__ メソッドを呼び出すことで音声のキャプチャと認識が開始されます。
初回の実行時にはマイクを有効にしてよいかの確認ダイアログが表示されます。

{{< code lang="ts" title="音声認識を開始" >}}
recognition.start();
{{< /code >}}

### その他のメソッド

音声認識の開始・停止にかかわるメソッドとしては次のようなものがあります。

__`start()`__
: 音声のキャプチャと認識を開始します。
  > Starts the speech recognition service listening to incoming audio with intent to recognize grammars associated with the current SpeechRecognition.

__`stop()`__
: 音声のキャプチャと認識を停止します。そこまでに認識した結果は onresult として呼び出されます。
  > Starts the speech recognition service listening to incoming audio with intent to recognize grammars associated with the current SpeechRecognition.

__`abort()`__
: 音声のキャプチャと認識を中断します。そこまでに認識した結果は破棄され、onresult は呼び出されません。
  > Stops the speech recognition service from listening to incoming audio, and doesn't attempt to return a SpeechRecognitionResult.

### その他のイベントハンドラー

前述の実装例では `onresult` イベントハンドラーを使いましたが、他にも `SpeechRecognition` には次のようなイベントハンドラーが用意されています。
必要に応じて画面表示などの制御に使ってください。

__`onstart: () => void`__
: 音声認識が開始されたときに呼び出される関数を設定します。
  Fired when the speech recognition service has begun listening to incoming audio with intent to recognize grammars associated with the current SpeechRecognition.

__`onend: () => void`__
: 音声認識が終了したときに呼び出される関数を設定します。
  Fired when the speech recognition service has disconnected.

__`onaudiostart: () => void`__
: 音声キャプチャが開始されたときに呼び出される関数を設定します。
  Fired when the user agent has started to capture audio.

__`onaudioend: () => void`__
: 音声キャプチャが終了したときに呼び出される関数を設定します。
  Fired when the user agent has finished capturing audio.

__`onresult: (event: SpeechRecognitionEvent) => void`__
: 音声認識の結果があるときに呼び出される関数を設定します。
Fired when the speech recognition service returns a result — a word or phrase has been positively recognized and this has been communicated back to the app.

__`onerror: (event: SpeechRecognitionError) => void`__
: 音声認識が失敗したときに呼び出される関数を設定します。
  Fired when a speech recognition error occurs.

__`onnomatch: () => void`__
: はっきりとした発声を認識できずに終了した場合に呼び出される関数を設定します。
  Fired when the speech recognition service returns a final result with no significant recognition. This may involve some degree of recognition, which doesn't meet or exceed the confidence threshold.

__`onsoundstart: () => void`__
: 音声の入力を開始したときに呼び出される関数を設定します。
  Fired when any sound — recognizable speech or not — has been detected.

__`onsoundend: () => void`__
: 音声の入力を終了したときに呼び出される関数を設定します。
  Fired when any sound — recognizable speech or not — has stopped being detected.

__`onspeechstart: () => void`__
: 発声の開始を認識したときに呼び出される関数を設定します。
  Fired when sound that is recognized by the speech recognition serviceas speech has been detected.

__`onspeechend: () => void`__
: 発声の終了を認識したときに呼び出される関数を設定します。
  Fired when speech recognized by the speech recognition service has stopped being detected.


全体のコード
----

- https://github.com/maku77/p-bf4cpjx-svelte-speech-recognition/
  - [main/src/routes/+page.svelte](https://github.com/maku77/p-bf4cpjx-svelte-speech-recognition/blob/main/src/routes/%2Bpage.svelte)

