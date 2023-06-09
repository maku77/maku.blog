---
title: "HMAC-SHA256 コードを生成する"
url: "/p/uqhbb5p"
date: "2013-08-2"
lastmod: "2023-06-09"
changes:
  - 2023-06-09: Python の hashlib の使い方ページへのリンクを追加
tags: ["Python", "セキュリティ"]
---

Python で HMAC-SHA256 を生成する
----

下記の `siggen.py` スクリプトは、コマンドライン引数で渡された「秘密鍵テキスト」と「メッセージ」をもとに [HMAC (Hash-based Message Authentication Code)](https://ja.wikipedia.org/wiki/HMAC) 署名を生成します。

{{< code lang="python" title="siggen.py" >}}
import sys
import hashlib
import hmac

def usage():
    print('python ' + sys.argv[0] + ' <key> <message>')
    sys.exit(1)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        usage()

    key = sys.argv[1]
    msg = sys.argv[2]
    sig = hmac.new(key.encode('ascii'), msg.encode('ascii'), hashlib.sha256)
    print(sig.hexdigest())
{{< /code >}}

{{< code title="使用例" >}}
$ python siggen.py 'SecretKey' 'YourMessage'
8aff2951003c218bd26ee43c99e30527a0c30e06042008a60935ef1ab28891ec
{{< /code >}}

ここでは、SHA256 ハッシュ関数を使用していますが (HMAC-SHA256)、`hashlib.sha256` の部分を変更すれば、他のハッシュ関数を適用することができます。


openssl コマンドで HMAC-SHA256 を生成する
----

Linux や macOS などの、`openssl` コマンドを使用できる環境では、下記のようにして簡単に HMAC-SHA256 を求めることができます。

{{< code >}}
$ echo -n 'YourMessage' | openssl dgst -sha256 -hmac 'SecretKey'
8aff2951003c218bd26ee43c99e30527a0c30e06042008a60935ef1ab28891ec
{{< /code >}}


おまけ（ランダムで秘密鍵テキストを生成するスクリプト）
----

下記の `random-password.py` スクリプトは、HMAC 計算に使用可能な 20 桁のランダムな文字列（秘密鍵）を生成します。
上記の `siggen.py` スクリプトに入力する秘密鍵として使用することができます。

{{< code lang="python" title="ramdom-password.py" >}}
from random import randint

CHARS = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#$-=?@[]_'
password = ''
for i in range(20):
    password += CHARS[randint(0, len(CHARS)-1)]
print(password)
{{< /code >}}

{{< code title="使用例" >}}
$ python random-password.py
GAFJ[XHrAClx_#ZPfE$O
{{< /code >}}


関連リンク
----

- [Python でハッシュ値 (MD5/SHA-1/SHA-256/SHA-512) を求める (`hashlib`) - まくまく Python ノート](https://maku77.github.io/p/gn4cn3s/)

