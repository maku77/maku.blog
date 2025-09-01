---
title: "Claude Code の使い方（指示の出し方のコツ）"
url: "p/qj9wydo/"
date: "2025-08-29"
tags: ["claude", "claude code"]
---

Claude Code を使っていく中で、効率的な活用方法がいろいろと見えてきたので、そのノウハウをまとめてみました。
Claude Code と効率的に共同作業するためのコツです。

Claude Code を使い始めた頃は、「指示を出す → 作業が終わるまで見守る → 新しい指示を出す」というパターンで使っていました。
しかし、この方法だと自分がツールを操作しているだけで、結局ずっと画面に張り付いていなければなりません。
Claude Code をもっと有効活用するには、**人に仕事を依頼する感覚** で使うのがコツだと気づきました。
以下のような機能を知っていると、**単なる壁打ちから脱却した本格的な共同作業** ができるようになります。

- 作業中に割り込んで方向性を修正する（`ESC` 割り込み）
- 最初に詳細なプランを立ててもらう（`Shift + TAB` で Plan mode へ）
- 作業が終わったら通知してもらう（hooks）
- Claude Code と並行してリポジトリ内で別の作業をする（`git worktree` など）


作業中に割り込む (ESC)
----

{{< image w="300" src="img-matta.png" alt="待った！" >}}

**`ESC` キーによる「待った」** 機能はとても重要です。
Claude Code に複雑な作業を任せていると、途中で「あ、やっぱりこうしてほしい」と思うことがよくあります。
例えば、ビルド構成を最適化してもらっているときに、Claude が想定外のファイルまで解析しようとしていることに気づいたとします。
そんなときは **`ESC`** キーを押して作業に割り込み、「そのファイルは対象外です」と教えてあげましょう。

{{< image border="true" w="700" src="img-interrupt-1.png" title="ESC で割り込み指示" >}}

すると、新しい方針に従って作業を続けてくれます。
Claude が考えている最中だけでなく、Claude から追加の情報を求められたとき（ユーザーからの入力待ちのとき）にも割り込みできます。
間違った方向に進んでいることに気づいたら、すぐに `ESC` キーで割り込めば、トークンも時間も無駄になりません。


先に詳細なプランを立ててもらう (Plan mode)
----

仕事で部下や協力会社の人に複雑な作業をお願いするときは、いきなり作業を始めてもらうのではなく、まず「作業プラン」を立ててもらって確認しますよね。
Claude Code でも同じように、最初に詳細なプランを立ててもらい、それを確認してから作業してもらうと手戻りが少なくなります。
Claude Code はデフォルトだといきなり作業を始めてしまうので、**`Shift + TAB`** キーを2回押してPlan mode に切り替えます。

{{< image border="true" w="700" src="img-plan-mode-1.png" title="plan mode へ切り替え" >}}

入力エリアの下に **plan mode on** と表示されていることを確認してください。
この状態で、やってもらいたいことを指示すると、最初に詳細なプランを立ててくれます。

{{< image border="true" w="700" src="img-plan-mode-2.png" title="詳細なプランを立ててもらう" >}}

提示されたプランでよければ、そのまま作業を開始してもらえばOKです。
少し修正したいところがあれば、先ほど説明した **`ESC`** キーで割り込んで修正指示を出します。
ここでは、難しいオプション機能は必要ないことを伝えてみましょう。

{{< image border="true" w="700" src="img-plan-mode-3.png" title="プランの修正指示" >}}

そうすると、次のような感じで修正されたプランを提示してくれます。

{{< image border="true" w="700" src="img-plan-mode-4.png" title="修正された実装プラン" >}}

プランを立てずにいきなりコードを作成してもらう方法もありますが、複雑な作業の場合は、できあがったコードをレビューするより実装プランをレビューする方がずっと楽です。
全体のプランに納得できたら、安心して作業を任せられます。


作業が終わったら知らせてもらう (hooks)
----

Claude Code に時間のかかる作業を依頼した後は、その間に別の作業を進めたいですよね。
メールの返信や机上の書類整理など、他にもやるべきことは山積みだからです。
このような並行作業を行う場合、Claude Code 側の処理が完了したタイミングで通知を受け取れると非常に便利です。

幸い、Claude Code には **hooks** という仕組みがあり、Claude Code が入力待ちになったときや作業が完了したタイミングで、任意のコマンドを実行できます。
ここでは、以下のように通知してもらうよう設定します。

- Claude Code が入力待ちになったときに「Waiting」と音声で知らせる (OS の通知も表示)
- Claude Code の作業が完了したときに「Completed」と音声で知らせる (OS の通知も表示)

OS ごとにコマンドを書き分けるのは面倒なので、[めもらばさん](https://www.memory-lovers.blog/entry/2025/07/15/163625) の [ccnoti](https://www.npmjs.com/package/ccnoti) というツールを使わせてもらいました。
Node.js がインストールされている環境なら、`npx` コマンドだけで実行できます。
Claude Codeの設定ファイル（**`~/.claude/settings.json`**）で以下のように設定するだけで OK です。

{{< code lang="js" title="~/.claude/settings.json" >}}
{
  // ...
  "hooks": {
    "Notification": [
      {
        "matcher": "",
        "hooks": [
          {
            "type": "command",
            "command": "npx ccnoti@0.2.0 -v -d -m Waiting"
          }
        ]
      }
    ],
    "Stop": [
      {
        "matcher": "",
        "hooks": [
          {
            "type": "command",
            "command": "npx ccnoti@0.2.0 -v -d -m Completed"
          }
        ]
      }
    ]
  }
}
{{< /code >}}

これで、Claude Code の作業が完了したときに通知してもらえるようになります。
ちょうど同僚から「作業が終わったよ」と声をかけてもらうような感覚ですね。


Claude Code と並行してリポジトリ内で別の作業をする
----

開発対象のアプリケーションに実装すべき機能が多数ある場合、効率的な開発を進めるために並行して作業したくなります。
例えば、Claude Code に一つの機能を任せている間に、自分は別の機能を実装するといったアプローチです。
あるいは、複数の Claude Code インスタンスを同時に起動して、異なる作業を並行して進めたい場合もあるでしょう。
このように同じコードベースで異なる作業を並行して行う場合、作業内容が競合しないよう注意が必要です。

例えば、Git リポジトリに関する作業なら、[**`git worktree`**](https://git-scm.com/docs/git-worktree) コマンドが便利です。
通常、Git でチェックアウトできるブランチは1つだけですが、`git worktree` を使うと、別のブランチを別の作業ディレクトリにチェックアウトして並行作業できます。

次の例では、`myproject` プロジェクト内で `feature1` ブランチを作成し、そのブランチ用の作業ディレクトリを `../myproject-feature1` として作成しています。

{{< code lang="console" title="feature1 ブランチと作業ディレクトリを作成する" >}}
$ cd myproject
$ git worktree add ../myproject-feature1 -b feature1
{{< /code >}}

これで、`myproject` と `myproject-feature1` の 2 つのディレクトリで、別々のブランチに対する作業を並行して行えるようになります。


その他
----

以下の内容は AI ツールの進化により将来変わる可能性がありますが、現時点で有効な方法として簡潔にまとめておきます。

### LLM が苦手な定型作業は MCP 化しておく

例えば「ランダムな ID を生成して」みたいな依頼をすると、Claude Code は毎回シェルスクリプトや Python スクリプトを作ろうとします。
これはトークンの無駄遣いですし、余計なコード確認の手間も増えてしまいます。
同様の処理を頻繁に行う場合は、**独自の MCP サーバーを作ってツールとして提供** してしまうのがおすすめです。
Python の `fastmcp` ライブラリを使えば、関数を定義するくらいの感覚で MCP サーバーを簡単に作れます。

- 参考: [MCP サーバーを作ってみる｜まくろぐ](/p/5c9y6vt/)

{{< accordion title="簡単な MCP サーバーの例（generate_random_id ツール）" >}}
{{< code lang="python" title="main.py" >}}
#!/usr/bin/env python3
"""
MCP Maku Tools - A Model Context Protocol server with useful tools
"""

import random

from fastmcp import FastMCP


def create_server() -> FastMCP:
    """MCPサーバーを生成します"""
    server = FastMCP("mcp-maku-tools")

    @server.tool()
    def generate_random_id(length: int = 7) -> str:
        """指定された文字数のランダムID生成ツールです

        Args:
            length: 生成するIDの長さ（デフォルト: 7）

        Returns:
            生成されたランダムID
        """
        chars = "23456789abcdefghijkmnopqrstuvwxyz"
        return "".join(random.choice(chars) for _ in range(length))

    return server


if __name__ == "__main__":
    # MCPサーバーを起動
    mcp = create_server()
    mcp.run(transport="stdio")
{{< /code >}}
{{< /accordion >}}

{{< accordion title="Claude Code での MCP 設定例" >}}
{{< code lang="js" title="~/.claude.json" >}}
{
  // ...
  "mcpServers": {
    "mcp-maku-tools": {
      "type": "stdio",
      "command": "uv",
      "args": [
        "run",
        "--directory",
        "/Users/maku/gitwork/maku77/mcp-maku-tools",
        "main.py"
      ],
      "env": {}
    }
  },
  // ...
}
{{< /code >}}
{{< /accordion >}}

自分用の MCP サーバーを 1 つ作ってしまえば、あとはそこに関数を追加していくだけで Claude Code から使えるツールが増えていきます。
MCP サーバー自体をたくさん作る必要はありません。

### 複数の解決方法を提案してもらう

Claude Code は一つの解決方法しか提示しないことが多く、それをそのまま受け入れてしまいがちです。
でも、他にもっと良い方法があるかもしれません。
ある問題を解決するためのコードを作成してもらうときに、以下のように **複数のアプローチを提示してもらうよう指示** してみてください。

```
複数の解決方法を提案してください。
```

### テストにパスするまで修正を繰り返してとお願いする

Claude Code がコードを一発で完璧に書けることは稀です。
テストコードも一緒に作成してもらい、テストにパスするまで **修正を繰り返してもらうよう指示** します。

```
テストコードも一緒に作成し、テストにパスするまで修正を繰り返してください。
```

簡単なテストに関しては、多くの場合、成功するまで修正を繰り返してくれるようになりましたが、途中で諦めてしまうような作業があったら、明示的に繰り返し修正するようお願いするのが効果的です。

### think harder で深く考えてもらう

Claude Code の隠しコマンド (正確には Sonnet モデルの機能) に **`think hard`**、**`think harder`** というものがあります。
プロンプト内にこれらの文字列を含めておくと、より深く考えてもらえるようになるので、ここぞというときに使ってみてください。
カスタムスラッシュコマンドでも使えます (例: `/maku:analyze-project think harder`)。

### 汎用的なカスタムスラッシュコマンドは GitHub で管理する

プロジェクト内の Claude 関連ファイルは Git リポジトリなどでバージョン管理するのが基本ですが、ユーザーディレクトリ以下に配置した汎用的なカスタムスラッシュコマンド (`~/.claude/commands/*.md`) も **GitHub リポジトリで管理** するようにしましょう。
PC 故障などで紛失するのを防げますし、複数の PC やユーザー間で共有したい場合にも便利です。
以下のページでは、Windows のジャンクション機能や、macOS/Linux のシンボリックリンクを使って、Git リポジトリや Dropbox と連携する方法を紹介しています。

- 参考: [Claude Code のカスタムコマンドを Git や Dropbox で共有する｜まくろぐ](/p/xpwvvfz/)

（2025年8月）Claude Code の新しい仕組みとして、**`~/.claude/plugins/`** ディレクトリを使って、GitHub リポジトリと自動連携する機能が検討されているようです。

