---
title: "tmux チートシート"
url: "p/hijkfza/"
date: "2024-03-24"
tags: ["cheatsheet", "tmux"]
---

tmux の基本要素
----

{{< image w="700" src="img-001.drawio.svg" title="tmux の構成要素" >}}

クライアント
: tmux の起動元となるターミナルアプリです。
クライアントは tmux サーバーを介してセッションやウィンドウを操作します。
最初に `tmux` コマンドを実行したときに tmux サーバーが立ち上がり、1 つ目のセッションとウィンドウが生成されます。
複数のクライアントが同じセッションに接続できます。

セッション
: セッションは tmux 内での作業単位で、クライアントからセッションにアタッチすることで、セッション内のウィンドウを操作できるようになります。
セッション内には複数のウィンドウを作成することができます。
クライアントからセッションをデタッチしても、tmux サーバーが存在する限り、セッションは生き続けることができます。

ウィンドウ
: ウィンドウはセッション内でのトップレベルの表示領域で、見た目としては Web ブラウザのタブのように見えます。
ウィンドウはセンション間で移動することができます。
ウィンドウは複数のペインに分割することができます。

ペイン
: ペインはウィンドウ内の分割領域を表します。
IDE のようにウィンドウを分割して複数の作業を同時に行えます。


tmux のコマンド入力の仕組み
----

ターミナルアプリなどのクライアントから `tmux` コマンドを実行すると、tmux セッションが立ち上がり、クライアントがアタッチされます。
アタッチされたクライアント上で __`PREFIX`__ キーとの組み合わせによるキーシーケンスを入力することで、tmux セッション内のウィンドウを操作できます。
`PREFIX` キーには、デフォルトで **`Ctrl + b`** が割り当てられています。

tmux の制御コマンドには名前が付けられており、`tmux` コマンドの引数で制御コマンド名を渡すことで実行できるようになっています（内部的には tmux サーバーにコマンドが送られています）。
tmux セッション内で `tmux` コマンドを実行すると、そのセッションに対しての操作になります。
また、**`PREFIX → :`** に続けてコマンド名を入力する方法もあります。

例えば、セッション内に新しいウィンドウを作成したいときは、以下のように複数の方法で実行できます。

- **`PREFIX → c`** と入力する
- **`PREFIX → :new-window`** と入力する（`:neww` でも OK）
- ターミナル上で **`tmux new-window`** を実行する（`neww` でも OK）


セッション (Session)
----

tmux の一連の機能を使用するには、最初にセッションを作成する必要があります。
セッションを作成すると、同時に 1 つのウィンドウが作成されます。

| キーバインド | 説明（パラメーター） |
| ---- | ---- |
| **`tmux`**<br>**`tmux new-session`** | セッションを作成する（セッション名はデフォルトで `0` からの連番になる）（`new-session` は省略形の `new` でも OK）<ul><li>**`tmux new -s <NAME>`** ... セッションを作成してついでに名前を付ける</li><li>**`tmux new -As <NAME>`** ... セッションを作成してついでに名前を付けるが、すでに同名のセッションがあればそこにアタッチする</li><li>**`tmux new -ADs <NAME>`** ... セッションを作成してついでに名前を付けるが、すでに同名のセッションがあればそこにアタッチする（アタッチされている既存のクライアントはデタッチされる）</ul></ul> |
| **`PREFIX` → `d`** | クライアントをセッションからデタッチする |
| **`tmux attach (a)`** | クライアントをセッションにアタッチする<ul><li>**`(パラメーターなし)`** ... 直近のセッションにアタッチする<li>**`-t <NAME>`** ... 既存のセッションにアタッチする（存在しない場合はエラー）</li><li>**`-dt <NAME>`** ... 既存のセッションにアタッチする（存在しない場合はエラー）（現在アタッチされているクライアントはデタッチされる）</li></ul> |
| **`PREFIX → s`** | セッションの一覧を表示して移動する |
| **`PREFIX → $`** | セッションに名前を変更する (`:rename-session`) |
| **`tmux list-sessions`** | セッションの一覧を表示する（省略形: `ls`） |
| **`tmux kill-session [-t <INDEX>]`** | セッションを終了する |


ウィンドウ (Window)
----

1 つのセッション内に複数のウィンドウを配置することができます。
概念的には、macOS や Windows のターミナルアプリにある __タブ機能__ に近いです。

| キーバインド | 説明 |
| ---- | ---- |
| **`PREFIX → c`** | 新しいウィンドウを作成する。ウィンドウには連番のインデックス (`0`, `1`, `2`, ...) が割り当てられます。<ul><li>**`:neww -n <NAME>`** ... 作成するウィンドウの名前を指定する。</li><li>**`:neww -t <INDEX>`** ... 作成するウィンドウのインデックスを指定する。</ul> |
| **`PREFIX → n`** | 次のウィンドウへ移動する (`:next`, `:next-window`, `:select-window -n`) |
| **`PREFIX → p`** | 前のウィンドウへ移動する (`:prev`, `:previous-window`, `:select-window -l`) |
| **`PREFIX → l`** | 直近のウィンドウへ移動する (`:last`, `:last-window`, `:select-window -l`) |
| **`PREFIX → 0`** | 0 番のウィンドウへ移動する (`:select-window -t0`, `:selectw -t0`) |
| **`PREFIX → 1`** | 1 番のウィンドウへ移動する (`:select-window -t1`, `:selectw -t1`) |
| **`PREFIX → '`** | 移動先のウィンドウを指定するプロンプトを表示 |
| **`PREFIX → w`** | ウィンドウの一覧を表示して移動する (`:choose-tree`) |
| **`PREFIX → ,`** | ウィンドウ名を変更する (`:rename-window`) |
| **`PREFIX → .`** | 現在のウィンドウの番号を変更する (`:move-window`, `:movew`) |
| **`tmux list-windows`** | ウィンドウの一覧を表示する（省略形: `lsw`） |
| **`exit`** | ウィンドウを閉じる |


ペイン (Pane)
----

ウィンドウを分割することでペイン (Pane) を作成できます。

| キーバインド | 説明（パラメーター） |
| ---- | ---- |
| **`PREFIX → %`** | ウィンドウを水平方向（左右）にペイン分割する<ul><li>**`:splitw -hb`** ... 新しいペインを左に作成する</li><li>**`:splitw -hd`** ... 分割後にペインを切り替えない</li></ul> |
| **`PREFIX → "`** | ウィンドウを垂直方向（上下）にペイン分割する<ul><li>**`:splitw -vb`** ... 新しいペインを上に作成する</li><li>**`:splitw -vd`** ... 分割後にペインを切り替えない</li></ul> |
| **`PREFIX → o`** | 次のペインへ移動する |
| **`PREFIX → ;`** | 直前のペインへ移動する |
| **`PREFIX → Up`** | 上のペインへ移動する (`:select-pane -U`) |
| **`PREFIX → Down`** | 下のペインへ移動する (`:select-pane -D`) |
| **`PREFIX → Left`** | 左のペインへ移動する (`:select-pane -L`) |
| **`PREFIX → Right`** | 右のペインへ移動する (`:select-pane -R`) |
| **`PREFIX → q`** | 各ペインの番号とサイズを表示する (`:display-panes`)<br>**続けて数字キーを入力することでそのペインへ移動できます** |
| **`PREFIX → {`** | 上のペインと入れ替える |
| **`PREFIX → }`** | 下のペインと入れ替える |
| **`PREFIX → Ctrl+o`** | ウィンドウ内でペインの位置を回転する (`:rotatew`, `:rotate-window`) |
| **`exit`** | ペインを閉じる |

ターミナル上の `tmux` コマンドでも分割できるところが面白いです。
例えば、`tmux sp -h` で左右に分割できます（`sp` は `splitw` の省略形）。


その他（ヘルプなど）
----

| キーバインド | 説明 |
| ---- | ---- |
| **`PREFIX → ?`** | キーバインドの一覧を確認する（迷ったらコレ）（`q` で終了） |
| **`PREFIX → /`** | 入力したキーに割り当てられた機能を表示する |
| **`PREFIX → :`** | コマンド名を入力してコマンドを実行する |
| **`tmux kill-server`** | tmux サーバーを終了する（すべてのセッションが破棄されます） |
| **`tmux list-clients`** | クライアントの一覧を表示する（省略形: `lsc`） |
| **`PREFIX → t`** | ペイン内に現在時刻を表示する |

### Alacritty

tmux と相性のよいモダンなターミナルエミュレーターとして、Alacritty があります。

- 参考: [Alacritty で快適なターミナル環境を手にいれる（macOS/Windows/Linux 対応）](/p/p9ppdqy/)

### シェル起動時に tmux セッションへ接続

tmux のセッションにアタッチしているときは、シェル上で **`TMUX`** 環境変数に値が入ります。
これを利用すると、シェルスクリプト内で tmux セッションにアタッチされているかどうかを判別することができます。
次のスクリプトでは、シェルの起動時に `tmux` コマンドの存在を確認し、`tmux` コマンドが存在するなら `$TMUX` の値を見て、値がセットされていなければ既存のセッション 0 にアタッチする、既存のセッションが存在しなければ新しくセッション 0 を作成する、ということをしています。

{{< code lang="zsh" title="~/.zshrc" >}}
# tmux セッションにアタッチしていないときはアタッチ（必要があればセッション作成）
if which tmux >/dev/null 2>&1 && [[ ! -n $TMUX ]]; then
  tmux new-session -As 0
fi

# 単純に下記の 1 行だけでもいいかも（エラー表示は完全に無視されちゃうけどシンプル）
# tmux new -As 0 >/dev/null 2>&1
{{< /code >}}

