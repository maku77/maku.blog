---
title: "Go 言語 - ドラフトメモ"
url: "p/z5q8o6m/"
date: "2022-08-05"
tags: ["Go"]
draft: true
---

- Go 1.18 から `go get` はパッケージをビルドしなくなり、`go.mod` ファイルの依存情報を更新するだけになった（`go get` の `-d` フラグが有効化のような振る舞いがデフォルトになりました）。

- コードの import に従って依存モジュールを取得／削除
  ```console
  $ go mod tidy
  ```

- `$GOPATH/pkg/mod` にダウンロードされた外部モジュールのキャッシュをクリアする
  ```console
  $ go clean -modcache
  ```

go get
----

{{< code lang="console" title="パッケージ依存情報の更新" >}}
$ go get example.com/pkg         # 最新のパッケージを使う
$ go get example.com/pkg@latest  # 最新のパッケージを使う
$ go get example.com/pkg@v1.2.3  # 指定したバージョンを使う
$ go get example.com/pkg@none    # 依存情報を削除（→ go mod tidy すればいい）
{{< /code >}}


io.Reader まわりのユーティリティ
----

{{< code lang="go" title="バッファサイズ分だけ読み込む" >}}
// var r io.Reader

buf := make([]byte, 5)
if _, err := io.ReadFull(r, buf); err != nil {
	// error
}
{{< /code >}}

- 一時ファイルを作成する ... `os.CreateTemp()` 第 2 引数の `*` の部分がランダムな文字列になります。
- 一時ディレクトリを作成する ... `os.MkdirTemp()`
- テスト内で一時ディレクトリを作成する ... `(*testing.T).TempDir()`

