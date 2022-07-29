---
title: "TypeScript を使った Node.js アプリを Docker コンテナ化する"
url: "p/ehxgwt4/"
date: "2022-07-29"
tags: ["Docker", "Node.js"]
---

{{% private %}}
- [Node.js Web アプリケーションを Docker 化する | Node.js](https://nodejs.org/ja/docs/guides/nodejs-docker-webapp/)
- [docker-node/BestPractices.md at main · nodejs/docker-node](https://github.com/nodejs/docker-node/blob/main/docs/BestPractices.md)
{{% /private %}}

何をするか？
----

TypeScript + Node.js で作成したサーバーアプリを、Docker コンテナ化する話です。
サーバーアプリは [Express](https://expressjs.com/) で簡単な Hello レスポンスを返すようなものを用意します。
Docker イメージビルド用の `Dockerfile` ファイルは、マルチステージビルドの構成にして、最終的な実行イメージができるだけ小さくなるようにします（それでも Node.js アプリだと、どうしても 100MB 超えになってしまいますが）。
NPM パッケージの管理には `yarn` を使わず、シンプルに `npm` だけでいきます。


Node.js アプリの用意
----

Node.js アプリは何でもよいのですが、ここでは Express で簡単な Web サーバーを作ることにします。

{{< code lang="console" title="TypeScript プロジェクトのセットアップ" >}}
$ npm init --yes    # package.json を生成
$ npm install express
$ npm install --save-dev typescript @types/express @types/node
$ npx tsc --init    # tsconfig.json を生成
{{< /code >}}

`package.json` に、TypeScript のビルドと、サーバー起動のための NPM スクリプトを追加しておきます。

{{< code lang="js" title="package.json（抜粋）" >}}
  "scripts": {
    "build": "tsc",
    "start": "node out/index.js"
  },
{{< /code >}}

{{< accordion title="package.json（全体）" >}}
{{< code lang="js" >}}
{
  "name": "myapp",
  "version": "1.0.0",
  "author": "maku",
  "private": true,
  "license": "UNLICENSED",
  "scripts": {
    "build": "tsc",
    "start": "node out/index.js"
  },
  "devDependencies": {
    "@types/express": "^4.17.13",
    "@types/node": "^18.6.2",
    "typescript": "^4.7.4"
  },
  "dependencies": {
    "express": "^4.18.1"
  }
}
{{< /code >}}
{{< /accordion >}}

`tsconfig.json` の内容は次のような感じで、__`src`__ ディレクトリ以下の `*.ts` ファイルをトランスパイルして __`out`__ ディレクトリに出力するようにしておきます。
これは、ビルド前の `.ts` ファイルと、ビルド後の `.js` ファイルが混ざらないようにするためです。

{{< code lang="js" title="tsconfig.json" >}}
{
  "compilerOptions": {
    "target": "ESNEXT",
    "module": "commonjs",
    "outDir": "./out",
    "esModuleInterop": true,
    "forceConsistentCasingInFileNames": true,
    "strict": true,
    "skipLibCheck": true
  },
  "includes": [
    "src/**/*"
  ],
}
{{< /code >}}

Express アプリを適当に実装します。
`http://localhost:3000/` にアクセスしたときに Hello と返すだけの簡単な Web サーバーです。

{{< code lang="ts" title="src/index.ts" >}}
import express, { Express, Request, Response } from 'express'

const app: Express = express()
const port = process.env.PORT || 3000

app.get('/', (req: Request, res: Response) => {
  res.send('Hello')
})

app.listen(port, () => {
  console.log(`⚡️ Server is running on port ${port}`)
})
{{< /code >}}

次のようにして、ビルド＆起動できるかを確認しておきます。

```console
$ npm run build
$ npm start
```


Dockerfile の作成
----

Docker イメージをビルドするための __`Dockerfile`__ を作成します。
この `Dockerfile` は [マルチステージビルド構成](https://maku77.github.io/p/z3n4hye/) になっており、前半で TypeScript コードのビルド、後半で Node.js アプリ実行用のイメージをビルドしています。

{{< code lang="docker" title="Dockerfile" >}}
##########################################################
#### ビルドステージ
FROM node:18.7.0-alpine3.15 as builder
WORKDIR /work

# ビルド用の依存パッケージをインストール
COPY package*.json ./
RUN npm install

# TypeScript コードをコピーしてビルド
COPY src tsconfig.json ./
RUN npm run build

##########################################################
#### 実行用イメージの作成
FROM node:18.7.0-alpine3.15 as runner
WORKDIR /work

ENV NODE_ENV production
ENV PORT 3000
EXPOSE 3000

# 本番環境用のパッケージをインストール
COPY package*.json ./
RUN npm install --omit=dev && npm cache clean --force

# builder からビルド結果だけコピー
COPY --from=builder /work/out ./out

# Node.js アプリを起動
CMD ["node", "./out/index.js"]
{{< /code >}}

以下、それぞれのステージの内容を見ていきます。

### ビルドステージ

```docker
#### ビルドステージ
FROM node:18.7.0-alpine3.15 as builder
WORKDIR /work
```

イメージをできるだけ小さくするために、Alpine Linux ベースの node イメージを親イメージとして使用します。
node イメージの具体的なタグ名（バージョン情報）は [Docker Hub](https://hub.docker.com/_/node) で確認してください。
作業ディレクトリは `/work` としています。

```docker
# ビルド用の依存パッケージをインストール
COPY package*.json ./
RUN npm install
```

NPM パッケージをインストールするために、`package.json` や `package-lock.json` ファイルをコピーし、`npm install` しています。
このタイミングでの `COPY` 命令では、アプリのソースコードはコピーしないところがポイントです。
ソースコードを一緒にコピーしてしまうと、実装を少し変更するだけで `npm install` を実行することになってしまいます。

```docker
# TypeScript コードをコピーしてビルド
COPY src tsconfig.json ./
RUN npm run build
```

TypeScript コードのビルドに必要なファイルをコピーし、`npm run build`（実体は `tsc`）でビルドします。
ビルドステージはここまでです。
ビルド結果の `.js` ファイル群が `out` ディレクトリに出力されているので、これらを次のステージで利用します。

### 実行イメージのビルド

```docker
#### 実行用イメージの作成
FROM node:18.7.0-alpine3.15 as runner
WORKDIR /work

ENV NODE_ENV production
ENV PORT 3000
EXPOSE 3000
```

実行イメージ用の親イメージも、同じ Alpine Linux ベースの node イメージを使用します。
このイメージは Docker にキャッシュされているので、イメージが再度ダウンロードされることはありません。
Node.js アプリを本番モードで動作させるために、__`NODE_ENV`__ 環境変数を __`production`__ にセットしておきます。
また、Node.js アプリ内で `PORT` 環境変数を参照しているので、デフォルト値を `ENV PORT 3000` で設定しておきます（コンテナ実行時の `-e` オプションで上書きできます）。
`EXPOSE 3000` は、このイメージが 3000 番ポートを使いますよという単なる印で、あまり重要なものではありません。

```docker
# 本番環境用のパッケージをインストール
COPY package*.json ./
RUN npm install --omit=dev && npm cache clean --force
```

本番環境用の NPM パッケージをインストールするときは、`npm install` に __`--omit=dev`__ オプションを付けて実行します（ver.7 までは `--production` オプションが使われていました）。
同じ `RUN` 命令の中でキャッシュをクリアして、イメージサイズの肥大化を防ぎます。

```docker
# builder からビルド結果だけコピー
COPY --from=builder /work/out ./out
```

ビルドステージでビルドされた `.js` ファイルは、`builder` イメージ側の `out` ディレクトリ内に格納されているので、これを実行イメージの方にコピーします。
これがアプリの本体（エントリポイント）です。

```docker
# Node.js アプリを起動
CMD ["node", "./out/index.js"]
```

最後に、コンテナ起動時のデフォルトコマンド (`CMD`) を定義しておきます。
`node` コマンドで `out/index.js` を起動しています。


イメージをビルドしてコンテナを起動する
----

`Dockerfile` ができたら、あとは `docker image build` でビルドすればよいのですが、先に __`.dockerignore`__ ファイルを作成して、Docker に無視させるディレクトリ／ファイルを列挙しておきます。
これを用意しておかないと、イメージビルド時の `Sending build context to Docker daemon` というプロセスで `node_modules` 以下の大量のファイルを転送しようとしてしまいます（無駄に時間がかかります）。

{{< code title=".dockerignore" >}}
node_modules
npm-debug.log
{{< /code >}}

次のようにビルドすると、`myapp` という名前のイメージができます。

{{< code lang="console" title="Docker イメージをビルド" >}}
$ docker image build -t myapp .
{{< /code >}}

イメージができたらコンテナを起動すればよいのですが、ここでも注意するポイントがあります。
Docker コンテナを起動すると、起動したコマンドのプロセス ID は 1 になります (PID=1)。
一方で、Node.js アプリは PID=1 での起動を想定していないので、そのまま `node` コマンドを起動すると振る舞いがおかしくなってしまいます。
例えば、<kbd>Ctrl + C</kbd> (SIGINT) をハンドルできず、`docker container stop` にも反応しません（10 秒後に SIGKILL で強制終了されます）。
Docker 1.13 以降では、この PID=1 問題に対応するために __`--init`__ オプションが用意されており、[簡易的な init モジュール](https://github.com/krallin/tini) (`/sbin/docker-init`) 経由でアプリを起動することができます。

{{< code lang="console" title="コンテナを起動" >}}
$ docker container run --rm -p 3000:3000 --init --name myapp myapp
{{< /code >}}

コンテナを起動したら、Web ブラウザで `http://localhost:3000` にアクセスするか、`curl` コマンドで次のように実行すれば、Node.js のサーバーアプリが起動していることを確認できます。

```console
$ curl http://localhost:3000
Hello
```


Docker Compose でイメージビルド＆コンテナ起動
----

上記の例では、イメージのビルドと、コンテナの実行を `docker` コマンドで個別に行っていましたが、Docker Compose を使えば、`Dockerfile` を使ったイメージのビルドと、コンテナの起動をワンステップで行えます。
最初からこちらの方法を使った方が早いかもしれません（＾＾；

{{< code lang="yaml" title="docker-compose.yml" >}}
version: '3'
services:
  myapp:
    build: .
    init: true
    ports:
      - '3000:3000'
{{< /code >}}

`Dockerfile` と同じディレクトリに、上記のような Compose ファイルを作成したら次のように実行するだけです。
お手軽！

{{< code lang="console" title="イメージのビルド ＆ コンテナの起動" >}}
$ docker compose up
{{< /code >}}

コンテナを止めたいときは、同じディレクトリで別のターミナルを開いて、次のようにします。

{{< code lang="console" title="コンテナの停止" >}}
# コンテナの停止と削除を別々に実行する場合
$ docker compose stop
$ docker compose rm

# 停止と同時に削除する場合
$ docker compose down
{{< /code >}}

あるいは、コンテナを実行しているターミナル上で <kbd>Ctrl + C</kbd> と入力して、SIGINT シグナルで停止することもできます。
ちゃんと Docker の `init` オプションが効いて、PID=1 問題に対処できていることが分かります。

