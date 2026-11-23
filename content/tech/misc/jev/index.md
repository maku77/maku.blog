---
title: "Jev を使おう"
url: "p/67c6rck/"
date: "2026-11-23"
tags: ["jev"]
---

**Jev** は使いこなすと色々なことができそうなので、ブームの発端となった [TypeSafe AI](https://typesafe.ai/) で課金して試してみることにします。

アカウントを作成してクレジットを購入すると、すぐに API を呼び出せるようになります。
クレジットは最低 5 ドルから購入でき、有効期限は 1 年です。
Jev は非常に安いので 5 ドルのクレジットでかなり遊べそうです。
Jev 1.14 の場合、入力 100 万トークンあたり $0.042 しかかかりません。


Jev とは？
----

Jev は入力に対して何らかの判断をさせるための AI モデルです。
例えば、ユーザーからの何らかの報告に対して、それが「不具合の報告」であるかどうかを判断させることができます。

- 入力：「送信ボタンを 2 回押さないと反応しないようです」
- 出力： 不具合の確率: 0.95

これと比べて、GPT や Claude などの大規模言語モデル（LLM）では柔軟な文章を生成することができますが、Jev はシンプルな定型のアウトプットに特化させることで、**圧倒的に速くて安い** という特徴があります。
つまり、AI に任せたいタスクが何らかの判断処理であれば、LLM ではなく Jev を使った方が効率的です。
現在 LLM だけで構成されているエージェントアプリケーションも、Jev による判断と LLM による文章生成を組み合わせることで、より高速かつ安価に動作させられる可能性があります。

ちなみに、Jev のように超高速に判断を行うモデルを「**System 1 モデル**」、LLM のようによく考えて比較的ゆっくりと文章を生成するモデルを「**System 2 モデル**」と呼ぶようです。

1. ユーザー入力を System 1 モデル (Jev) で処理し、素早く分類やスコア付けを行う。
2. その結果に応じた処理を System 2 モデル (LLM) で行いユーザーへの応答とする。

のような流れで使えるということですね。

System 1 と System 2 の元ネタは、ダニエル・カーネマンの『Thinking, Fast and Slow』で示された意思決定モデルです。
これは 2011 年に発売された書籍ですが、現在ベストセラーになっているようです。

{{< amazon
  itemId="0374533555"
  title="Thinking, Fast and Slow"
  author="Daniel Kahneman"
  publisher="FSG Adult"
  imageUrl="https://m.media-amazon.com/images/W/BW_MEDIAX_AVIF_MEASUREMENT_1306696-T4/images/I/61fdrEuPJwL._SL1500_.jpg"
>}}

↑とりあえずアフィリンクを張っておきました。買うべし ٩(๑❛ᴗ❛๑)۶


Python から Jev を使う（typesafe-sdk パッケージ）
----

TypeSafe AI の System 1 モデル API を呼び出すには、`https://api.typesafe.ai/v1/systemone` というエンドポイントに POST リクエストを送ります。
`curl` コマンドなどで直接呼び出すこともできますが、ここでは Python の **`typesafe-sdk`** パッケージを使って呼び出してみます。

### API キーを発行

API キーがないと始まらないので、[TypeSafe AI のコンソール](https://console.typesafe.ai) から発行しておきます。
API キーの名前はテスト用であれば何でもよいのですが、作りたいサービスが決まっているのであれば、それに合わせた名前にしておくことをお勧めします。
そうしないと、将来 API キーを整理したくなったときに、どのキーを消してよいかが分からなくなってしまいます。

{{< image border="true" src="img-api-key.png" title="TypeSafe AI での API キーの発行" >}}

API キーを発行したら、プログラムから参照できるように環境変数 **`TYPESAFE_API_KEY`** に設定しておきます。
後述の Python クライアントは、デフォルトでこの名前の環境変数を参照するようになっています。

{{< code lang="zsh" title="~/.zshrc などで環境変数を設定" >}}
export TYPESAFE_API_KEY=apikey_..........
{{< /code >}}

### Python プロジェクトの作成

Python コードから Jev を使いたいときは、TypeSafe AI が提供している **`typesafe-sdk`** パッケージを使うのが簡単です。
ここでは、`uv` を使ってプロジェクトを作成し、パッケージを追加します（参考：[Python プロジェクト用の爆速パッケージマネージャ uv を導入する | まくまく Python ノート](https://maku77.github.io/p/fjsfjpw/)）。

{{< code lang="zsh" title="uv プロジェクトの作成" >}}
uv init --bare jevtest
cd jevtest
uv add typesafe-sdk
{{< /code >}}

次のような **`pyproject.toml`** ができていれば準備完了です。

{{< code lang="toml" title="pyproject.toml" hl_lines="6" >}}
[project]
name = "jevtest"
version = "0.1.0"
requires-python = ">=3.12"
dependencies = [
    "typesafe-sdk>=0.7.5",
]
{{< /code >}}

### Python から Jev を呼び出す

System 1 モデルの API を呼び出すには、その名の通り **`system_one()`** という名前のメソッドを使います。
次のサンプルコードでは、「営業利益 (operating profit)」というフレーズが、どのようなカテゴリと関連があるのかを調べています。
例えば、辞書系のサービスを作ったときに、各項目をカテゴリ分類する用途に使えます。

{{< code lang="python" title="main.py" hl_lines="6-18" >}}
import os
from typesafe_sdk import Noul, TypeSafeClient


def main(client: TypeSafeClient) -> None:
    response = client.system_one(
        state={
            "ja": "営業利益",
            "en": "operating profit",
        },
        questions={
            "金融": Noul(instructions="Is this phrase related to finance?"),
            "技術": Noul(instructions="Is this phrase related to technology?"),
            "動物": Noul(instructions="Is this phrase related to animals?"),
            "食べ物": Noul(instructions="Is this phrase related to food?"),
            "文": Noul(instructions="Is this phrase a sentence?"),
        },
    )

    print(f"金融: {response.nouls['金融'].noul}")
    print(f"技術: {response.nouls['技術'].noul}")
    print(f"動物: {response.nouls['動物'].noul}")
    print(f"食べ物: {response.nouls['食べ物'].noul}")
    print(f"文: {response.nouls['文'].noul}")


if __name__ == "__main__":
    api_key = os.getenv("TYPESAFE_API_KEY")
    if not api_key:
        raise RuntimeError("TYPESAFE_API_KEY が設定されていません。")

    with TypeSafeClient(api_key=api_key) as client:
        main(client)
{{< /code >}}

解析したいデータは、`system_one()` メソッドの **`state`** 引数で渡します。
単純な文字列だけでなく、オブジェクトや配列を渡すことができます。
オブジェクトを渡すときは、各プロパティの名前からその意味を推測できるようにしておく必要があります。
ここでは `en` と `ja` というプロパティを持つ辞書オブジェクトを渡しています（`english` と `japanese` の方がよいかも）。

**`questions`** 引数には、各質問のキーと内容を辞書形式で渡します。
質問の型（[Primitives](https://docs.typesafe.ai/primitives)）は何種類かあるのですが、ここでは最もシンプルな **`Noul`** 型だけ使っています。
`Noul` 型は yes/no で答えられる質問を表し、0.0 〜 1.0 の値を返します（1.0 に近いほど yes の確率が高い）。

このプログラムを実行すると、次のような結果が得られます。

{{< code lang="console" title="実行結果" >}}
$ uv run main.py
金融: 0.98
技術: 0.08
動物: 0.02
食べ物: 0.03
文: 0.03
{{< /code >}}

「金融」に関するフレーズの可能性が高いという結果が得られました。

うまく動いた！ ٩(๑❛ᴗ❛๑)۶ わーぃ


応用: 複数の入力をまとめて処理する
----

Jev では複数の入力や質問を並列処理することができます。
1 回の API 呼び出しで複数の判断をまとめて行うことができるため、うまく使えば短時間で大量のデータを処理することができます。

次の例では、ユーザーの書き込み（レビューやコメントなど）を複数まとめて処理し、それぞれの書き込みが「肯定的」「否定的」「中立的」のどれに分類されるかを判定しています。
分類処理には **`Choice`** 型の Question (Primitive) を使用します。
指示 (`instructions`) の中で **`` `posts[0]` ``** のように記述すると、`state` で渡した配列データの特定位置の要素を参照できます。
この仕組みを利用して、複数の判断をまとめて処理しています。


{{< code lang="python" title="main.py" >}}
from typesafe_sdk import Choice, TypeSafeClient

# ユーザーの書き込み
POSTS = [
    "対応が丁寧で、すぐ問題を解決してくれました。満足しています。",
    "デザインはきれいですが、読み込みが遅いのが気になります。",
    "何度問い合わせても返信がなく、とてもがっかりしました。",
    "普通に使えます。特に問題はありません。",
]

# 感情の分類
SENTIMENT_OPTIONS = {
    "positive": "満足、感謝、推奨、好意的な評価を表している",
    "negative": "不満、問題、失望、批判、改善要求、否定的な評価を表している",
    "neutral": "事実の報告、質問、曖昧な意見が書かれており、明確な感情表現がない",
}


def main():
    state = {"posts": POSTS}
    questions = {
        f"sentiment_{i}": Choice(
            instructions=f"ユーザーによる書き込み `posts[{i}]` の感情を判定してください。",
            criteria=SENTIMENT_OPTIONS,
        )
        for i in range(len(POSTS))
    }

    with TypeSafeClient() as client:
        response = client.system_one(state=state, questions=questions)

        for i in range(len(POSTS)):
            sentiment = response.choices[f"sentiment_{i}"]
            print(POSTS[i])
            print("  choice:", sentiment.choice)
            print("  confidence:", sentiment.confidence)
            print("  probabilities:", sentiment.probabilities)
            print()


if __name__ == "__main__":
    main()
{{< /code >}}

{{< code lang="console" title="実行結果" >}}
$ uv run main.py
対応が丁寧で、すぐ問題を解決してくれました。満足しています。
  choice: positive
  confidence: 1.0
  probabilities: {'positive': 1.0, 'negative': 0.0, 'neutral': 0.0}

デザインはきれいですが、読み込みが遅いのが気になります。
  choice: negative
  confidence: 0.61
  probabilities: {'positive': 0.02, 'negative': 0.74, 'neutral': 0.24}

何度問い合わせても返信がなく、とてもがっかりしました。
  choice: negative
  confidence: 1.0
  probabilities: {'positive': 0.0, 'negative': 1.0, 'neutral': 0.0}

普通に使えます。特に問題はありません。
  choice: neutral
  confidence: 0.82
  probabilities: {'positive': 0.12, 'negative': 0.0, 'neutral': 0.88}
{{< /code >}}

各書き込みの感情をまとめて分類することができました。
`confidence` プロパティの値が 1.0 に近いほど、分類の確信度が高いことを示しています。
`probabilities` の合計値は必ず 1.0 (100%) になるように出力されますが、`confidence` が低い場合は分類結果をそのまま使わないように気をつけます。


結局いくらかかった？
----

上記のようなサンプルコードを何十回も動かしても、クレジットは $5.00 から全然減りませんでした。
数万トークンしか消費していないので **10 銭** くらいしかかかっていない計算になります。
安くて速い！

{{< shout "2万トークン * $0.042 / 100万トークン = $0.00084" >}}

٩(๑❛ᴗ❛๑)۶ これは使いやすそうだっ

