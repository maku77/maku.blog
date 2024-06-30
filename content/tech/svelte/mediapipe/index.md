---
title: "Svelte 応用: MediaPipe で Web カメラ映像をジェスチャー認識する"
url: "p/pqmxxqz/"
date: "2024-06-30"
tags: ["MediaPipe", "Svelte"]
---

何をするか？
----

{{< image w="300" border="true" src="img-001.png" title="Svelte + MediaPipe によるジェスチャー認識" >}}

[MediaPipe](https://github.com/google-ai-edge/mediapipe) を使った Svelte アプリを作ってみます。
MediaPipe は様々なメディアの認識処理に対応していますが、ここでは Web カメラからの入力映像を使ったジェスチャー認識を行います。
MediaPipe を使った Web アプリは、Web ブラウザだけで動作するので、静的な Web サイトとしてホスティングすることができます（参考: [Svelte アプリを静的サイトとしてビルドする](/p/4oudmxy/)）。


プロジェクトの作成
----

まずは空っぽの Svelte プロジェクトを新規作成します。

```console
$ npm create svelte@latest myapp
（種類として Skeleton、TypeScript を選択）
```

続いて、必要なライブラリをインストールしておきます。
MediaType ではタスク別に使用するモジュールが分かれており、視覚系のタスクには __`@mediapipe/tasks-vision`__ モジュールを使用します。

- `@mediapipe/tasks-vision` ... 視覚（画像／動画）系タスク __← 今回はコレを使う__
- `@mediapipe/tasks-genai` ... 生成 AI (LLM) 系タスク
- `@mediapipe/tasks-text` ... テキスト系タスク
- `@mediapipe/tasks-audio` ... 音声系タスク

Svelte アプリで依存ライブラリを追加するときは `devDependencies` として追加すれば OK です。

```console
$ cd myapp
$ npm install -D @mediapipe/tasks-vision
```


Web カメラからの入力映像を表示する
----

MediaPipe を使った認識処理の前に、Web カメラからの入力映像を画面上に表示できるようにしておきましょう。
そのためには、HTML の `video` 要素に、Web カメラからの入力 (`MediaStream`) を関連付ける必要があります。

- 参考: [HTML の video 要素で Web カメラ（USB カメラ）の映像を表示する - 天才まくまくノート](https://maku77.github.io/p/vap3zpa/)

ここでは、`WebcamVideo` という Svelte コンポーネントとして実装してみます。

{{< code lang="svelte" title="src/lib/WebcamVideo.svelte" >}}
<script lang="ts">
	/** Webcam による映像キャプチャを開始するためのフラグ */
	export let active: boolean = false;

	/** video 要素の bind 用 */
	export let videoElem: HTMLVideoElement | undefined;

	$: if (active) {
		startCapture();
	}

	function startCapture() {
		// Web カメラのストリームを取得して video 要素に紐付ける
		navigator.mediaDevices
			.getUserMedia({ video: true })
			.then((mediaStream: MediaStream) => {
				if (videoElem) {
					videoElem.srcObject = mediaStream;
					videoElem.play();
				}
			})
			.catch((err) => {
				console.error('Web カメラの取得に失敗しました:', err);
			});
	}
</script>

<video id="webcam" bind:this={videoElem} playsinline>
	<!-- Web カメラの入力映像を表示するのでキャプションは存在しない -->
	<track kind="captions" src="" default />
</video>

<style>
	#webcam {
		width: 640px;
		max-width: 100%;
		transform: scaleX(-1); /* 映像を左右反転 */
	}
</style>
{{< /code >}}

このコンポーネントは、任意の Svelte コンポーネント（`+page.svelte` など）から次のように使えます。

```svelte
<WebcamVideo active={isActive} />
```

`active` プロパティを `true` にすると、Web カメラが有効化されて `video` 要素に入力映像が表示されます（ここでは停止処理は省略しているので、有効化しかできません）。


MediaPipe でジェスチャー認識する
----

Web カメラと関連付けた `video` 要素を用意できたら、MediaPipe を使った認識処理を開始できます。
大まかな流れは次の通りです（参考: [ウェブ用ジェスチャー認識ガイド - Google AI Edge](https://ai.google.dev/edge/mediapipe/solutions/vision/gesture_recognizer/web_js?hl=ja)）。

1. __`GestureRecognizer`__ インスタンスを生成する（モデルのロードなど）。
2. __`GestureRecognizer#recognizeForVideo()`__ メソッドに `video` 要素を渡すことで、最新フレームの認識を行う。
3. 認識結果は __`GestureRecognizerResult`__ オブジェクトとして返されるので、後は煮るなり焼くなり。

`recognizeForVideo()` による認識処理は毎フレーム呼び出す必要があるため、次のような感じで `requestAnimationFrame()` を使って繰り返し呼び出すように実装します。

{{< code lang="ts" hl_lines="4" >}}
const renderLoop = () => {
	const result = recognizer.recognizeForVideo(videoElem, Date.now());
	onFrame(result);
	requestAnimationFrame(renderLoop);
};
{{< /code >}}

また、`recognizeForVideo()` は `video` 要素からの入力データの準備ができてから呼び出さないといけないことに注意してください。
次のような感じで `loadeddata` イベントをハンドルすればよいです。

```ts
videoElem.addEventListener('loadeddata', renderLoop);
```

下記のライブラリは、`GestureRecognizer` の生成と、認識処理をまとめたものです。

{{< code lang="ts" title="src/lib/recognizer.ts" >}}
import {
	GestureRecognizer,
	FilesetResolver,
	type GestureRecognizerResult
} from '@mediapipe/tasks-vision';

/**
 * video 要素からの入力映像の解析を開始します。
 * フレームごとの認識結果はコールバック関数 `onFrame` に渡されます。
 */
export async function startRecognition(
	videoElem: HTMLVideoElement,
	onFrame: (result: GestureRecognizerResult) => void
): Promise<void> {
	const recognizer: GestureRecognizer = await createGestureRecognizer();

	// 入力映像のフレームごとの認識処理
	const renderLoop = () => {
		const result = recognizer.recognizeForVideo(videoElem, Date.now());
		onFrame(result);
		requestAnimationFrame(renderLoop);
	};

	// video 要素からのデータが取得できたらフレーム処理開始
	videoElem.addEventListener('loadeddata', renderLoop);
}

/**
 * MediaPipe のジェスチャー認識器を作成します。
 */
async function createGestureRecognizer(): Promise<GestureRecognizer> {
	const vision = await FilesetResolver.forVisionTasks(
		'https://cdn.jsdelivr.net/npm/@mediapipe/tasks-vision@latest/wasm'
	);
	return await GestureRecognizer.createFromOptions(vision, {
		baseOptions: {
			modelAssetPath:
				'https://storage.googleapis.com/mediapipe-models/gesture_recognizer/gesture_recognizer/float16/1/gesture_recognizer.task',
			delegate: 'GPU'
		},
		numHands: 2,
		runningMode: 'VIDEO'
	});
}
{{< /code >}}

任意の Svelte コンポーネントから、次のように呼び出すだけで認識処理を開始できます。
各フレームの認識結果が `processResult` 関数に渡されます。

```ts
startRecognition(videoElem, processResult);

function processResult(result: GestureRecognizerResult) {
	// ...
}
```


結合する
----

上記で作成した `WebcamVideo` コンポーネントをページコンポーネント (`+page.svelte`) に配置し、`startRecognition()` でジェスチャー認識を開始します。
処理のポイントは、`WebcamVideo` コンポーネント内の `video` 要素の参照を、`startRecognition()` に渡すところくらいです。

他の部分はほとんど認識結果 (`GestureRecognizerResult`) の表示処理です。
ページの下の方に、認識した手の形（Open_Palm や Thumb_Up など）を表示しています。

ここでは、ボタンを 1 つ配置して、ボタンを押したときにキャプチャと認識処理を開始するようにしています。

{{< code lang="svelte" title="src/routes/+page.svelte" >}}
<script lang="ts">
	import type { GestureRecognizerResult } from '@mediapipe/tasks-vision';
	import WebcamVideo from '$lib/WebcamVideo.svelte';
	import { startRecognition } from '$lib/recognizer';

	let videoElem: HTMLVideoElement;
	let buttonElem: HTMLButtonElement;
	let isWebcamEnabled = false;

	$: if (isWebcamEnabled) {
		startRecognition(videoElem, processResult);
		buttonElem.disabled = true; // ボタンを無効化
		//buttonElem.style.display = 'none'; // ボタンを消す
	}

	// 以下は認識結果の表示用
	let categoryName1: string = '?'; // 1 つ目の手の形状（カテゴリ）
	let categoryName2: string = '?'; // 2 つ目の手の形状（カテゴリ）
	let score1 = 0.0; // 1 つ目のカテゴリのスコア
	let score2 = 0.0; // 2 つ目のカテゴリのスコア

	function processResult(result: GestureRecognizerResult) {
		const category1 = result.gestures.at(0)?.at(0);
		const category2 = result.gestures.at(1)?.at(0);
		categoryName1 = category1?.categoryName ?? '?';
		categoryName2 = category2?.categoryName ?? '?';
		score1 = category1?.score ?? 0.0;
		score2 = category2?.score ?? 0.0;
	}
</script>

<h1>MediaPipe Demo</h1>
<div>
	<WebcamVideo bind:videoElem active={isWebcamEnabled} />
</div>
<button bind:this={buttonElem} on:click={() => (isWebcamEnabled = true)}
	>Web カメラによる認識を開始</button
>
<ul>
	<li>手の形1: {categoryName1} (信頼度: {score1.toFixed(2)})</li>
	<li>手の形2: {categoryName2} (信頼度: {score2.toFixed(2)})</li>
</ul>
{{< /code >}}


全体のコードとデモページ
----

- 全体のコード: https://github.com/maku77/p-pqmxxqz-svelte-mediapipe
- デモページ: https://p-pqmxxqz-svelte-mediapipe.vercel.app/

