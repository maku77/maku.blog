---
title: "Linuxコマンド: curl コマンドのチートシート"
url: "p/2sv4bqw/"
date: "2023-11-28"
tags: ["cheatsheet", "ctf", "curl", "linux"]
---

| コマンド | 説明 |
| ---- | ---- |
| `curl --help all` | 詳細なヘルプを表示 |
| `curl https://example.com` | GET リクエスト |
| `curl -i https://example.com` | レスポンスヘッダーも表示 |
| `curl -I https://example.com` | レスポンスヘッダーだけ表示 |
| `curl -v https://example.com` | 冗長表示（リクエストとレスポンスのヘッダーを確認可能） |
| `curl -H "Authorization: Bearer TOKEN" https://example.com` | リクエストヘッダーの指定 |
| `curl -b "cookie1=value1; cookie2=value2" http://example.com` | クッキーの指定 |
| `curl -A "Mozilla/5.0 (Windows NT 10.0; Win64; x64)" http://example.com` | ユーザーエージェントの指定 |
| `curl -L http://example.com` | リダイレクト (302) レスポンスを自動処理 |
| `curl -O https://example.com/file.txt` | ファイルのダウンロード（同名で保存） |
| `curl -o hoge.txt https://example.com/file.txt` | ファイルのダウンロード（別名で保存） |
| `curl -# -O http://example.com/largefile.zip` | ダウンロードの進捗を表示 |
| `curl -r 0-100 http://example.com/file.txt -o partial.txt` | ファイルの一部だけダウンロード |
| `curl -u user:pass --basic http://example.com` | BASIC 認証（`--basic` は省略可） |
| `curl -u user:pass --digest http://example.com` | Digest 認証 |
| `curl -k https://example.com` | サーバー証明書の検証を無視 |
| `curl --interface eth0 http://example.com` | 使用するネットワークインタフェースを指定 |
| `curl --connect-timeout 0.15 http://example.com` | タイムアウトの指定（秒単位） |
| `curl -X POST http://example.com` | POST リクエスト（データ送信しないときはメソッドの明示が必要） |
| `curl -d "param1=value1&param2=value2" http://example.com` | POST でデータ送信 |
| `curl --data-urlencode "..." http://example.com` | POST でデータ送信（URL エンコード有り） |
| `curl -H "Content-Type: application/json" -d '{"key":"value"}' https://example.com` | POST でデータ送信（JSON 形式） |
| `curl -X POST -F "file=@/path/to/file.txt" http://example.com/upload` | POST でファイルのアップロード |
| `curl -X DELETE http://example.com/resource` | DELETE リクエスト |
| `curl -4 https://example.com` | IPv4 の使用を強制 |
| `curl -6 https://example.com` | IPv6 の使用を強制 |
| `curl -x {proxy}:{port} https://example.com` | プロキシ経由でのアクセス |
| `curl -X CUSTOMMETHOD http://example.com` | 独自の HTTP メソッドを指定 |
| `curl -u user:pass -O ftp://example.com/file.txt` | FTP サーバーからファイルをダウンロード |
| `curl -u user:pass ftp://example.com/dir` | FTP サーバーのディレクトリ内のファイル一覧 |
| `curl -T file.txt -u user:pass --ftp-create-dirs -ssl ftp://example.com` | FTP サーバーへのアップロード |

- テスト用のサーバーとして `https://httpbin.org` を使うと便利です。どのようなリクエストが送信されたかを、JSON レスポンスとして返してくれます。
  - GET メソッド用アドレス: `https://httpbin.org/get`
  - POST メソッド用アドレス: `https://httpbin.org/post`
  - DELETE メソッド用アドレス: `https://httpbin.org/delete`
- Windows のコマンドプロンプトではシングルクォートがうまく扱えないので、入れ子になったダブルクォートをエスケープする必要があります。
  - Linux の場合: `-d '{"key":"value"}'`
  - Windows の場合: `-d "{\"key\":\"value\"}"`

