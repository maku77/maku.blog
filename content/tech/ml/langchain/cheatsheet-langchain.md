---
title: "LangChain まとめ（チートシート）"
url: "p/559qmr7/"
date: "2023-12-08"
lastmod: "2023-12-13"
tags: ["LangChain"]
draft: true
---

セットアップ
----

### Python ライブラリのインストール

システムの Python 環境を汚さないように、[venv 仮想環境を作って](https://maku77.github.io/p/wozpogm/) から作業するのがおすすめです。

```console
# 仮想環境の作成とアクティベート
$ python -m venv venv
$ source venv/bin/activate

# langchain および各種モデル用インタフェースのインストール
$ pip install langchain  # LangChain 本体
$ pip install openai     # OpenAI の GPT モデルなどを使う場合
```

### 環境変数

OpenAI のモデルを使用する場合は、OpenAI のアカウントを作成して [API キーを生成](https://platform.openai.com/api-keys) しておく必要があります。
作成した API キーは、__`OPENAI_API_KEY`__ 環境変数で設定しておけば、アプリケーションのコード内でキー設定する必要はありません。

```ini
OPENAI_API_KEY=sk-BWwT3HBlbkF.........省略..........4NqHaQoHqwU7Rf
```

LangChain の主要コンポーネント
----

1. Model I/O
   LLM/Chat models
   : 言語モデルを扱うためのインタフェースは、大きく分けて __LLM__ と __Chat モデル__ の 2 種類があります。LLM は 1 つの文字列（`str`）を受け取り、1 つの文字列を返すモデルです。Chat モデルは一連のメッセージ (`BaseMessage[]`) を受け取り、1 つのメッセージを返すモデルです。例えば、OpenAI 用の LLM として `OpenAI()`、Chat モデルとして `ChatOpenAI()` が用意されています。

   Prompt templates
   : 言語モデルに対して指示を与えるためのプロンプトを構築するためのテンプレートです。単一のテキスト (`str`) を構築するための `PromptTemplate` や、一連のメッセージ (`BaseMessage[]`) を構築するための `ChatPromptTemplate` があります。

   Output parsers
   : 言語モデルからの生の出力を、プログラム内で扱いやすいように（例えばリストデータや JSON などに）変換します。Chat モデルが返した `ChatMessage` オブジェクトを単純なテキストに変換するためにも使われます。
2. Retrieval
3. Agents
4. Chains
5. Memory
6. Callbacks


Chat モデル用のメッセージ
----

Chat モデルには、`BaseMessage` を基底クラスとする一連のメッセージを入力情報として与えます。
どの立場からのメッセージかを、次のようなクラスのインスタンスで表現します。

- `HumanMessage`: A BaseMessage coming from a human/user.
- `AIMessage`: A BaseMessage coming from an AI/assistant.
- `SystemMessage`: A BaseMessage coming from the system.
- `FunctionMessage` / `ToolMessage`: A BaseMessage containing the output of a function or tool call.


スニペット集
----

### LLM

```python
from langchain.llms import OpenAI

llm = OpenAI(model="text-davinci-003", temperature=0.8)
text = "カラフルな靴下を販売する店の名前を考えて。"
print(llm.invoke(text))
```

{{< code title="実行結果" >}}
・カラフルソックス・サプライ
・ソックス・オブ・ザ・レインボー
・カラフル・コネクション
{{< /code >}}

### ChatModel

```python
from langchain.chat_models import ChatOpenAI
from langchain.schema import HumanMessage

chat_model = ChatOpenAI()
text = "What would be a good company name for a company that makes colorful socks?"
messages = [HumanMessage(content=text)]
chat_model.invoke(messages)
# >> AIMessage(content="Socks O'Color")
```

`ChatModel#invoke()` への入力は `BaseMessage` のリストで、戻り値は 1 つの `BaseMessage` です。

### テンプレート - PromptTemplate

```python
from langchain.prompts import PromptTemplate

prompt = PromptTemplate.from_template(
    "What is a good name for a company that makes {product}?"
)
text = prompt.format(product="colorful socks")
```

{{< code title="実行結果（text 変数の内容）" >}}
What is a good name for a company that makes colorful socks?
{{< /code >}}

__`PromptTemplate`__ は、`LLM` へ入力する単純なテキスト (`str`) を生成するためのテンプレートとして使用できます。

### テンプレート - ChatPromptTemplate

```python
from langchain.prompts.chat import ChatPromptTemplate

chat_prompt = ChatPromptTemplate.from_messages([
    ("system", "You are a helpful assistant that translates {input_language} to {output_language}."),
    ("human", "{text}"),
])

messages = chat_prompt.format_messages(
    input_language="English",
    output_language="French",
    text="I love programming."
)
```

{{< code lang="python" title="実行結果（messages 変数の内容）" >}}
[
    SystemMessage(content="You are a helpful assistant that translates English to French.", additional_kwargs={}),
    HumanMessage(content="I love programming.")
]
{{< /code >}}

`ChatModel` に渡す `BaseMessage` のリストを生成するには、__`ChatPromptTemplate`__ を使用します。

### Output パーサー - 独自の Output パーサー

{{< code lang="python" title="カンマ区切りの応答をパースする独自パーサー" >}}
from typing import List

from langchain.schema import BaseOutputParser

class CommaSeparatedListOutputParser(BaseOutputParser[List[str]]):
    """Parse the output of an LLM call to a comma-separated list."""

    def parse(self, text: str) -> List[str]:
        """Parse the output of an LLM call."""
        return text.strip().split(", ")

data = CommaSeparatedListOutputParser().parse("Hello, World!")
{{< /code >}}

{{< code lang="python" title="実行結果（data 変数の内容）" >}}
["Hello", "World!"]
{{< /code >}}

### Chain

```python
from langchain.llms import OpenAI
from langchain.prompts import PromptTemplate

prompt = PromptTemplate.from_template("次の単語の英訳を返して: {word}")
chain = prompt | OpenAI()
text = chain.invoke({"word": "猫"})
print(text)
```

LCEL (LangChain Expression Language) 構文を使って、下記のようなインスタンスをパイプ (`|`) で繋ぐことができます。
接続後のオブジェクトの `invoke()` メソッドを呼び出せば、一連の処理を行った結果を取得できます。

- テンプレート（`PromptTemplate` や `ChatPromptTemplate`）
- LLM/Chat モデル（`OpenAI` や `ChatOpenAI`）
- Output パーサー


主要コンポーネント内のモジュール
----

{{% private %}}
- https://python.langchain.com/
{{% /private %}}

### 1. Model I/O

- Prompts
  - Prompt templates
    - Connecting to a Feature Store
    - Few-shot prompt templates
    - Few-shot examples for chat models
    - ...
  - Example selectors
    - Custom example selector
    - Select by length
    - Select by maximal marginal relevance (MMR)
    - Select by n-gram overlap
    - Select by similarity
- Chat models
  - Caching / Prompts / Streaming / Tracking token usage
- LLMs
  - Async API / Custom LLM / Caching / Serialization / Streaming / Tracking token usage
- Output parsers
  - List parser / Datetime parser / Enum parser / ...

### 2. Retrieval

- Document loaders
  - CSV / File Directory / HTML / JSON / Markdown / PDF
- Document transformers
  - Text splitters / Post retrieval
- Text embedding models
  - CacheBackedEmbeddings
- Vector stores
- Retrievers
  - MultiQueryRetriever / Contextual compression / Ensemble Retriever / ...
- Indexing

### 3. Agents

- Agent Types
  - Conversational / OpenAI assistants / OpenAI functions / ...
- How-to
  - Add Memory to OpenAI Functions Agent
  - Running Agent as an Iterator
  - Returning Structured Output
  - ...
- Tools
  - Toolkits
  - Defining Custom Tools
  - Human-in-the-loop Tool Validation

### 4. Chains

- How to
  - Async API
  - Different call methods
  - Custom chain
  - Adding memory (state)
  - Using OpenAI functions
- Foundational
  - LLM / Router / Sequential / Transformation
- Documents
  - Stuff / Refine / Map reduce / Map re-rank

### 5. Memory

- Chat Messages
- Memory types
  - Conversation Buffer (Buffer Memory)
  - Conversation Buffer Window (Buffer Window Memory)
  - Entity (Entity Memory)
  - Conversation Knowledge Graph (Knowledge Graph Memory)
  - Conversation Summary (Summary Memory)
  - Conversation Summary Buffer (Summary Buffer Memory)
  - Conversation Token Buffer
  - Backed by a Vector Store
- Memory in LLMChain
- Memory in the Multi-Input Chain
- Memory in Agent
- Message Memory in Agent backed by a database
- Customizing Conversational Memory
- Custom Memory
- Multiple Memory classes

### 6. Callbacks

- Async callbacks
- Custom callback handlers
- Callbacks for custom chains
- Logging to file
- Multiple callback handlers
- Tags
- Token counting

### Others

- Text Splitters
  - Character Text Splitter
  - Recursive Character Text Splitter
  - Markdown Text Splitter

