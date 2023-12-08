---
title: "LangChain まとめ（チートシート）"
url: "p/559qmr7/"
date: "2023-12-08"
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
2. Retrieval
3. Agents
4. Chains
5. Memory
6. Callbacks


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


サンプルコード
----

### Model I/O - LLMs

```python
from langchain.llms import OpenAI

llm = OpenAI(model="text-davinci-003", temperature=0.8)
text = "カラフルな靴下を販売する店の名前を考えて。"
print(llm.invoke(text))
```

{{< code title="実行結果" >}}
・カラフルソックス・サプライ
・ソックス・ソニック
・クール・ソックス
・ソックス・オブ・ザ・レインボー
・カラフル・コネクション
{{< /code >}}
