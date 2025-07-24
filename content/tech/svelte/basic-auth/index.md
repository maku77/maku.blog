---
title: "Svelte 入門: サーバー Hooks で Basic 認証を実装する"
url: "p/mem8u54/"
date: "2025-02-11"
tags: ["Svelte", "Basic 認証"]
---

Hooks とは
----

SvelteKit には [Hooks の仕組み](https://svelte.dev/docs/kit/hooks)が提供されており、特にサーバー Hooks を定義することで、任意のリクエストに割り込んで処理を行うことができます。
例えば、各ルートのハンドラーが呼び出される前に、次のような処理を行うことができます。

- リクエストのログを取る
- リクエストの内容を変更する（リダイレクトなど）
- リクエストを拒否する（アクセス制御）
- 後続の処理のためにデータを付加する（Cookie に基づくユーザー情報など）

この記事では、サーバー Hooks の利用例として、簡単な **Basic 認証** を実装する方法を解説します。

{{< image w="310" src="img-001.png" title="Chrome の Basic 認証ダイアログ" >}}

管理者にしかアクセスできないページを手っ取り早く作りたい場合などに便利ですが、Basic 認証はセキュアな認証方式ではないので、あくまで Hooks の使い方の例と考えてください。
本番環境に認証・認可の仕組みを導入する場合は、OAuth などのよりセキュアな方式を検討してください。


サーバー Hooks の基本
----

サーバー Hooks を定義するのはとても簡単で、SvelteKit のプロジェクト内に **`src/hooks.server.ts`**（あるいは `.js`）を作成し、**`handle`** 関数をエクスポートするだけです。
ファイル名を間違えると、Hooks が正しく動作しないので注意してください（特に `hook` でなく `hooks` であることに注意）。

次の サーバー Hooks 実装では、リクエストイベントの内容をコンソールに出力しています。

{{< code lang="ts" title="src/hooks.server.ts" >}}
import type { Handle } from '@sveltejs/kit';

export const handle: Handle = async ({ event, resolve }) => {
	console.log(JSON.stringify(event, undefined, 2));
	const response = await resolve(event);
	return response;
};
{{< /code >}}

{{< accordion title="出力例" >}}
{{< code lang="json" >}}
{
	"cookies": {},
	"locals": {},
	"params": {},
	"request": {},
	"route": {
		"id": "/"
	},
	"url": "http://localhost:5173/",
	"isDataRequest": false,
	"isSubRequest": false
}
{{< /code >}}
{{< /accordion >}}

`handle` 関数には、**`event`** オブジェクト ([RequestEvent](https://svelte.dev/docs/kit/@sveltejs-kit#RequestEvent)) と、**`resolve`** 関数が渡されます。
`resolve` 関数は、各ルートに対応するレンダラーを呼び出して、その結果を `Response` オブジェクトとして返す関数です。
よって、上記のコードのように単純に `resolve(event)` とすると、デフォルトのレンダリング処理を行った結果を返すことになります。

`resolve` 関数を呼び出さずに、独自の `Response` オブジェクトを作成して返すこともできます。
以下の例では、URL パスの最初のセグメントが `hello` の場合には `Hello!` というテキストを返し、`admin` の場合には `403 Forbidden` エラーを返しています。

{{< code lang="ts" title="src/hooks.server.ts" >}}
import { error, type Handle } from '@sveltejs/kit';

export const handle: Handle = async ({ event, resolve }) => {
	// URL パスの最初のセグメントを取得
	const url = new URL(event.request.url);
	const firstSegment = url.pathname.split('/')[1];

	// "/hello" 以下にアクセスした場合
	if (firstSegment === 'hello') {
		return new Response('Hello!');
	}

	// "/admin" 以下にアクセスした場合
	if (firstSegment === 'admin') {
		error(403, 'Forbidden');
	}

	return await resolve(event);
};
{{< /code >}}

{{% note %}}
よく次のように URL パスのプレフィックスだけを見て処理を分岐させている例を見かけますが、このようにすると、`/hello` だけではなく、`/helloworld` などにもヒットしてしまいます。

{{< code lang="ts">}}
if (event.url.pathname.startsWith('/hello')) {
	return new Response('Hello!');
}
{{< /code >}}

ちゃんとパスをセグメント（コンポーネント）に分割してから比較するようにしましょう。
{{% /note %}}


サーバー Hooks による Basic 認証
----

### Basic 認証の仕組み

Basic 認証の仕組みはとてもシンプルで、次のような流れでブラウザーとサーバーが連携します。

1. ブラウザーがサーバーにリクエストを送信する。
2. サーバーがレスポンスに `WWW-Authenticate` ヘッダーを付加して、認証が必要であることを通知する。
3. ブラウザーがユーザー名とパスワードを入力するダイアログを表示する。
4. ブラウザーが `<username>:<password>` というテキストを Base64 エンコードして、`Authorization` ヘッダーに付加して再度リクエストを送信する（例: `Authorization: Basic XXXXXX`）。
5. サーバーが `Authorization` ヘッダーを検証して、認証が成功した場合にはリクエストを処理する。

### Basic 認証の実装

以下は、SvelteKit のサーバ Hooks を使った Basic 認証の実装例です。
管理者アクセス用のクレデンシャル情報（ユーザー名とパスワード）は、Private な環境変数として、`.env` ファイル（開発用）、あるいはサーバーの環境変数（本番用）で設定しておきます。
Private な環境変数の扱い方はこちらの記事を参考にしてください → [Svelte 入門: 外部 API 用のキーを Private な環境変数で定義する ($env)](/p/nmdte68/)

{{< code lang="ini" title=".env" >}}
ADMIN_CREDENTIALS="user:pass"
{{< /code >}}

{{< code lang="ts" title="src/hooks.server.ts" >}}
import { type Handle } from '@sveltejs/kit';
import { createUnauthorizedResponse, validateBasicAuth } from '$lib/basic-auth';

export const handle: Handle = async ({ event, resolve }) => {
	// Basic 認証用のリクエストヘッダーを確認
	if (!validateBasicAuth(event.request)) {
		// 認証に失敗した場合は 401 Unauthorized レスポンスを返す
		return createUnauthorizedResponse();
	}

	// 認証に成功した場合は処理を続行
	return resolve(event);
};
{{< /code >}}

{{< code lang="ts" title="src/lib/basic-auth.ts" >}}
import { error } from '@sveltejs/kit';
import { env } from '$env/dynamic/private';

/** Basic 認証用の正しいリクエストヘッダーが含まれているか検証します。 */
export function validateBasicAuth(request: Request): boolean {
	const authHeader = request.headers.get('Authorization');
	if (!authHeader || !authHeader.startsWith('Basic ')) {
		return false;
	}
	const base64Credentials = authHeader.split(' ')[1];
	const credentials = atob(base64Credentials).split(':');
	const [username, password] = credentials;
	return isAdmin(username, password);
}

/** Basic 認証用のダイアログを表示するためのレスポンスを生成します。 */
export function createUnauthorizedResponse(): Response {
	return new Response('Not authorized', {
		status: 401,
		headers: { 'WWW-Authenticate': 'Basic realm="Member Only"' }
	});
}

/** ユーザー名とパスワードが正しいか検証します。 */
function isAdmin(username: string, password: string): boolean {
	const cred = env.ADMIN_CREDENTIALS;
	if (!cred) {
		error(500, 'Server is not properly set up');
	}
	return `${username}:${password}` === cred;
}
{{< /code >}}

`pnpm dev` コマンドなどで Svelte の開発サーバーを立ち上げて、`http://localhost:5173` にアクセスすると、次のように Basic 認証のダイアログが表示されるはずです。

{{< image w="310" src="img-001.png" title="Chrome の Basic 認証ダイアログ" >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ

