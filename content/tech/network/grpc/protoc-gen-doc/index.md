---
title: "Protocol Buffers の .proto ファイルから API ドキュメントを自動生成する (protoc-gen-doc)"
url: "p/ikw7gpz/"
date: "2022-11-29"
tags: ["gRPC", "Protocol Buffers"]
---

protoc-gen-doc とは？
----

David Muto 氏が公開している [protoc-gen-doc](https://github.com/pseudomuto/protoc-gen-doc) という `protoc` コマンドのプラグインを使用すると、`.proto` ファイルから、HTML 形式や Markdown 形式のドキュメントを自動生成することができます。
複数の `.proto` ファイルの内容をまとめて 1 つのページとして出力してくれるので、シンプルで見通しのよいドキュメントになります。

生成されたドキュメントの例: [HTML 形式](https://rawgit.com/pseudomuto/protoc-gen-doc/master/examples/doc/example.html) / [Markdown 形式](https://github.com/pseudomuto/protoc-gen-doc/blob/master/examples/doc/example.md) / [JSON 形式](https://github.com/pseudomuto/protoc-gen-doc/blob/master/examples/doc/example.json)

`protoc-gen-doc` は `protoc` コマンドのプラグインとして動作するのですが、実行環境が Docker イメージとして提供されているので、__docker コマンド一発で、簡単に .proto ファイルからドキュメントを生成できます__。
もちろん、`protoc` コマンドと `protoc-gen-doc` を両方インストールして実行することもできますが、Docker を使った方が断然お手軽です。

Protocol Buffers Compiler（`protoc` コマンド）に関しては、[こちらを参考にしてください](/p/37e6uck/)。


.proto ファイルからのドキュメント生成
----

`protoc-gen-doc` の Docker イメージを使って、`.proto` ファイルからドキュメントを生成してみます。
Docker の実行環境は、[Docker Desktop](https://www.docker.com/products/docker-desktop/) などでインストールしてください。
`.proto` ファイルが手元になければ、とりあえず下記のファイルをダウンロードして試せます。

- {{< file src="Vehicle.proto" caption="Vehicle.proto（サンプル .proto ファイル）" >}}

`proto/Vehicle.proto` のように、__`proto`__ ディレクトリに格納すれば準備完了です。
あとは次のように実行すると、`docs` ディレクトリに `index.html` ファイルが生成されます。

{{< code title="HTML 形式のドキュメント (docs/index.html) を生成" >}}
$ docker container run --rm \
    -v $(pwd)/docs:/out \
    -v $(pwd)/proto:/protos \
    pseudomuto/protoc-gen-doc --doc_opt=html,index.html
{{< /code >}}

`protoc-gen-doc` は、デフォルトでコンテナ内の __`/protos`__ ディレクトリにある `.proto` ファイルを読み込んで、コンテナ内の __`/out`__ ディレクトリに出力しようとします。
なので、`docker container run` コマンドに指定しているマウントオプション (__`-v`__) は、次のような意味になります。

- __`-v $(pwd)/docs:/out`__ ... ローカルの `docs` ディレクトリを出力先にする
- __`-v $(pwd)/proto:/protos`__ ... ローカルの `proto` ディレクトリ内の `*.proto` を読み込む

出力形式を変えたい場合などは、__`--doc_opt`__ オプションを変更します。

| `--doc_opt` オプションの例 | 説明 |
| ---- | ---- |
| `--doc_opt=markdown,api.md` | 出力形式を Markdown (`.md`) にする |
| `--doc_opt=:google/*,somepath/*` | 対象外にする `.proto` 指定する（`:` の後ろに記述） |

GitHub で `.proto` ファイルを管理している場合は、[GitHub Actions でドキュメントを自動生成](https://maku77.github.io/p/f2eggno/) するように設定しておくと、開発メンバーが幸せになれます。

