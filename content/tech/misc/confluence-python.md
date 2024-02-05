---
title: "Python で Confluence の情報を取得する"
url: "p/wgsz2wn/"
date: "2024-02-05"
tags: ["confluence", "python"]
---

Atlassian のドキュメンテーション管理ツールである Confluence は、REST API を使って様々な操作を自動化することができます。
各言語用にラッパーライブラリが公開されているので、通常は REST API を直接呼び出す必要はありません。
ここでは、__Atlassian Python API (`atlassian-python-api`)__ を使って、Python から Confluence の情報を取得してみます。

- [Atlassian Python API documentation](https://atlassian-python-api.readthedocs.io/)
- [(GitHub) atlassian-api/atlassian-python-api](https://github.com/atlassian-api/atlassian-python-api)


atlassian-python-api のインストール
----

`pip` コマンドで __`atlassian-python-api`__ パッケージをインストールします。
システムを汚さないように、[venv 環境を作成してインストールする](https://maku77.github.io/p/wozpogm/) のがおすすめです。


{{< code lang="console" title="atlassian-python-api パッケージのインストール" >}}
## venv 仮想環境を使う場合（Linux/macOS の場合）
$ python3 -m venv venv       # 仮想環境の作成
$ source venv/bin/activate   # 仮想環境に入る

## venv 仮想環境を使う場合（Windows の場合）
$ py -m venv venv            # 仮想環境の作成
$ venv\Scripts\activate.bat  # 仮想環境に入る

## atlassian-python-api パッケージのインストール
$ pip install atlassian-python-api
{{< /code >}}

ついでに、__`python-dotenv`__ を入れて、[`.env` ファイル](https://maku77.github.io/p/gzo8d7y/) でアクセストークンを設定できるようにしておきます。

{{< code lang="console" title="dotenv パッケージのインストール" >}}
$ pip install python-dotenv
{{< /code >}}

アクセストークンの発行
----

API アクセスのための認証方法はいくつかありますが、今回は、Confluence 上で生成できるパーソナルアクセストークンを使うことにします。
Confluece 画面から次のようにして発行できます。

1. Confluence 画面右上の `ユーザーアイコン` をクリック → `設定` を選択
2. `個人用アクセストークン` を選択し、`トークンの作成` をクリック

__`.env`__ ファイルを作成し、Confluence のサーバーアドレスと、上記のアクセストークンを記述しておきます。
このファイルは Git にコミットしないように注意してください。
OS の環境変数に設定するのでも OK です。

{{< code lang="inf" title=".env" >}}
CONFLUENCE_URL = "https://<server>"
CONFLUENCE_TOKEN = "NTc3MTI2...（省略）...L6Haih/B"
{{< /code >}}


config モジュールの作成
----

Confluence のサーバーアドレスとトークンを参照するための `config` モジュールを作成しておきます。
こういった設定を別モジュール化しておくことで、メインロジックをシンプルに記述できます。

{{< code lang="python" title="config.py" >}}
import os
import sys
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

def load_env_or_exit(env_name: str) -> str:
    """
    Get the value of the environment variable.
    If it is not set, an error message is displayed and the program is terminated.
    """
    env_val = os.getenv(env_name)
    if not env_val:
        sys.exit(
            f"Error: {env_name} not set. Please consider adding a .env file with {env_name}."
        )
    return env_val

# Export configuration variables
CONFLUENCE_URL = load_env_or_exit("CONFLUENCE_URL")
CONFLUENCE_TOKEN = load_env_or_exit("CONFLUENCE_TOKEN")
{{< /code >}}


Confluence クラスを使って情報を取得する
-----

次のように生成した `atlassian.Confluence` インスタンスを使って、Confluence サーバーから様々な情報を取得することができます。

```python
from atlassian import Confluence
import config

confluence = Confluence(url=config.CONFLUENCE_URL, token=config.CONFLUENCE_TOKEN)
```

### スペースの一覧を取得する

```python
res = confluence.get_all_spaces(start=0, limit=10)
for s in res["results"]:
    print("{}, {}, {}".format(s["id"], s["key"], s["name"]))
```

### ページの内容を取得する（ID 指定）

```python
page_id = "1830728929"
res = confluence.get_page_by_id(
    page_id=page_id,
    expand="space,body.view"  # スペース情報やページの本文を取得
)

print(res["space"]["key"])
print(res["id"])
print(res["title"])
print(res["body"]["view"]["value"][:100])  # 本文 (HTML) の先頭 100 文字
```

ページの本文部分の HTML を取得するには、`expand` 引数で `body.view` を指定しておく必要があります。
`body.view` の代わりに `body.storage` を使うと、Confluence 独自の形式（HTML + 独自タグ）で本文を取得できます。

### ページ内のテーブルの内容を取得する

```python
# import json

page_id = "6349290133"
res = confluence.get_tables_from_page(page_id)
tables = json.loads(res)
print(tables["tables_content"][0])
```

{{< code lang="python" title="実行結果（3行3列のテーブルが含まれている場合）" >}}
[['Col-A', 'Col-B', 'Col-C'], ['値A-1', '値B-1', '値C-1'], ['値A-2', '値B-2', '値C-2']]
{{< /code >}}

これはちょっと面白い機能で、ページ内に記述したテーブルの内容を配列データとして取得できます。
Confluence のページを簡易的なデータベースとして使うことができます。
内部的に HTML/XML パーサーを利用するらしく、`pip install lxml` が必要です。

### その他の API

`atlassian` モジュールには他にもいろいろな API がありますが、網羅的なドキュメントはないようです。
下記のモジュールドキュメントや、GitHub 上のサンプルコード、Atlassian の REST API ドキュメントなどを参考にして試行錯誤する必要があります。
特に、各 API のレスポンス内容は、REST API ドキュメントを見ないと分からなかったりします。

- [atlassian モジュールの API ドキュメント](https://atlassian-python-api.readthedocs.io/)
- [atlassian モジュールのサンプルコード](https://github.com/atlassian-api/atlassian-python-api/tree/master/examples/confluence)
- [The Confluence Cloud REST API](https://developer.atlassian.com/cloud/confluence/rest/v2/)

