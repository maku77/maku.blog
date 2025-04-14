---
title: "MCP サーバーを作ってみる"
url: "p/5c9y6vt/"
date: "2025-04-14"
tags: ["mcp", "python"]
---

MCP とは
----

[MCP (Model Context Protocol)](https://modelcontextprotocol.io/) は、AI アプリケーションが外部のツールと連携するためのプロトコルで、Anthropic によってオープンソース化されました。
この標準化されたプロトコルに従って MCP サーバーを作っておくと、Claude Desktop や VS Code（GitHub Copilot 拡張）などの AI アプリケーション (MCP host) から簡単に接続できるようになります。
2025 年に入った頃から MCP はエージェント連携のためのプロトコルとしてデファクトスタンダードとみなされており、今後はさまざまな MCP サーバー、およびそれらをサポートする AI アプリケーションが登場してくると思われます。

有名どころの MCP サーバー実装は、[Awesome MCP Servers](https://github.com/punkpeye/awesome-mcp-servers) などのサイトで紹介されています。

MCP サーバーは Python や Typescript 用のライブラリを使うと簡単に実装することができます。
MCP がどのような仕組みで動いているのかを理解するには、**Hello World 的な MCP サーバーを作ってしまうのが一番早いです**。
ここでは、Python で簡単な MCP サーバーを作って、AI アプリ（Claude Desktop と VS Code）と連携させてみます。


プロジェクトの作成
----

まず、Python のプロジェクトを作成します。
ここでは、[プロジェクト管理ツールとして uv を使用します](https://maku77.github.io/p/fjsfjpw/)。
`uv` は Python スクリプト実行時に必要なライブラリを自動でインストールしてくれるので、複数の MCP サーバーを立ち上げるようなケースでは特に便利です（MCP サーバーごとに手動で venv 環境に入らなくて済みます）。

```console
$ uv init my-mcp-server
$ cd my-mcp-server
```

次に、必要な MCP 関連ライブラリをインストールします。
`mcp[cli]` と指定することで、基本ライブラリに加えて開発用の `mcp` コマンド（CLI ツール）が使えるようになります。

```console
$ uv add "mcp[cli]"
```


MCP サーバーの実装
----

下記は、2つの整数の足し算を行うだけの簡単なツール (`add`) を提供する MCP サーバーの実装例です。

{{< code lang="main.py" >}}
from mcp.server.fastmcp import FastMCP

mcp = FastMCP("Demo")

# ツールの実装
@mcp.tool()
def add(a: int, b: int) -> int:
    """Add two numbers."""
    return a + b

if __name__ == "__main__":
    # MCPサーバーを起動
    mcp.run(transport="stdio")
{{< /code >}}

簡単ですね！

**`@mcp.tool()`** デコレーターは、この MCP サーバー（`mcp` オブジェクト）が `add` というツール（≒関数）を提供することを示しています。
1 つの MCP サーバーで複数のツールを公開することができます。
`@mcp.tool()` というデコレーターを使うには、先に `mcp` オブジェクトを生成しておく必要があることに注意してください。

`add` 関数の `Add two numbers.` という **docstring は、AI アプリがこのツールの役割を把握するために参照します**。
できるだけ簡潔に分かりやすく記述するようにしてください。
今回の例くらいの簡単な関数では必要ありませんが、通常は各引数の説明も書いておくとよいです。


Claude Desktop からの使用
----

AI アプリ（= MCP ホスト）としては、まずは本家 Authropic の [Claude Desktop](https://claude.ai/download) を使ってみるのがよいでしょう。
Claude Desktop の設定ファイル **`claude_desktop_config.json`** を開いて、上記の MCP サーバーの起動方法を記述するだけでツール連携できるようになります。
設定ファイルは下記のパスを直接エディタで開くか、Claude Desktop の「設定」メニューから開きます。

- macOS/Linux: `~/Library/Application\ Support/Claude/claude_desktop_config.json`
- Windows: `$env:AppData\Claude\claude_desktop_config.json`

以下の設定例では、"my-mcp-server" という名前の MCP サーバーを `uv run main.py` コマンドで起動できるということを Claude Desktop に教えています。

{{< code lang="json" title="claude_desktop_config.json" >}}
{
    "mcpServers": {
        "my-mcp-server": {
            "command": "uv",
            "args": [
                "--directory",
                "/Users/maku/my-mcp-server",
                "run",
                "main.py"
            ]
        }
    }
}
{{< /code >}}

`/Users/maku/my-mcp-server` の部分は、実際のプロジェクトのディレクトリパスに置き換えてください。
設定ファイルを編集したら、Claude Desktop を再起動して反映させます。

{{% note title="絶対パスでの指定" %}}
プロジェクトのディレクトリパスは上記のように絶対パスで指定する必要があるようです（Claude バージョン 0.9.2 で確認）。
ホームディレクトリを示す `~/` なども使えません。
Claude Desktop の起動時に `Spawn uv ENOENT` のようなエラーが出た場合は、`uv` コマンドを見つけられていません。
この場合は、`uv` コマンドの絶対パスを `which uv` で調べて `command` プロパティに指定してください（例: `/Users/maku/.cargo/bin/uv`）。
{{% /note %}}

Claude Desktop が起動したら、`123 + 321` の計算をお願いしてみます。
最近の LLM は賢くなっていて、単純な計算は自力で計算してしまうことが多いので、ここでは `MCP サーバーを使って` と明示しています。

{{< image w="567" src="img-001.png" title="Claude Desktop で MCP サーバーを使用する" >}}

応答の中のアコーディオンを開くと、MCP サーバー (`my-mcp-server`) にどのような入力をして、どのような出力が得られたかを確認することができます。


VS Code（GitHub Copilot 拡張）からの使用
----

Visual Studio Code の GitHub Copilot 拡張機能でも MCP サーバーを使用することができます。

- [Use agent mode in VS Code](https://code.visualstudio.com/docs/copilot/chat/chat-agent-mode)

下記のような設定ファイルを VS Code 上で作成し、エディタ上に表示される「起動」ボタンを押すと MCP サーバーが起動します。

{{< code lang="json" title=".vscode/mcp.json" >}}
{
  "servers": {
    "my-mcp-server": {
      "command": "uv",
      "args": [
        "--directory",
        "/Users/maku/my-mcp-server",
        "run",
        "main.py"
      ]
    }
  }
}
{{< /code >}}

これで、GitHub Copilot 拡張のチャットウィンドウから MCP サーバーの機能にアクセスできるようになります。
ちなみに、プロンプト内に **`#add`** のようにツール名を明記すると確実にそのツールを使ってくれます。

{{< image w="558" src="img-002.png" title="VS Code（GitHub Copilot 拡張）で MCP サーバーを使用する" >}}

できた！ ٩(๑❛ᴗ❛๑)۶ わーぃ

