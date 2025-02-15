---
title: "VS Code で Kaggle 用の勉強環境を作る（Jupyter 拡張）"
url: "p/ma86izp/"
date: "2025-02-10"
lastmod: "2025-02-15"
tags: ["Jupyter", "Kaggle", "VSCode"]
changes:
  - 2025-02-15: Python インタープリターの詳しい設定方法を追記
---

何をするか？
----

機械学習のコンペティションを開催している [Kaggle](https://www.kaggle.com/) のサイトでは、Web ブラウザから利用可能な Notebook 機能が提供されています。
これを使うとブラウザ上で直接 Python のコードを書いて実行できるのですが、やっぱり使い慣れた Visual Studio Code 環境上で作業したいと思ったので、VS Code 上に Jupyter Notebook の環境を作っていきます。

Python の実行環境についてはインストール済みであることを前提とします。
Python がまだインストールされていない場合は、[`pyenv` などでインストール](https://maku77.github.io/p/x4z298a/) するとよいです。


Jupyter 拡張のインストール
----

まずは、Microsoft 純正の **Python 拡張** と **Jupyter 拡張** をインストールします。
下記サイトの Install ボタンを押すか、VS Code の拡張機能画面から検索してインストールできます。

- [Python - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=ms-python.python)
- [Jupyter - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=ms-toolsai.jupyter)


Kaggle 用の仮想環境 (venv)
----

### venv の作成

VS Code 上の Jupyter Notebook で Python コードを実行するには、`pip` コマンドで Python カーネル (**`ipykernel`**) をインストールしておく必要があります。
また、機械学習系の実装では **`numpy`** や **`pandas`** などのライブラリも必要になってきます。
ここでは、ホームディレクトリに Python の仮想環境ディレクトリ (**`~/.venv/kaggle`**) を作成し、そこに各ライブラリをインストールすることにします。

{{< code lang="console" title="Kaggle 用の venv 環境を作成" >}}
$ python -m venv ~/.venv/kaggle
{{< /code >}}

プロジェクトのルートディレクトリに `.venv` ディレクトリを作る方法も一般的ですが、上記のようにホームディレクトリに作っておくと、他のプロジェクトからも使いまわせて便利です。

### venv へのライブラリインストール

仮想環境を作成したら、仮想環境をアクティベートして、必要になりそうなライブラリをインストールしておきます。
少なくとも `ipykernel` は Jupyter Notebook 上での Python 実行のために必須です。

{{< code lang="console" title="Kaggle 環境用のライブラリをインストール" >}}
# venv をアクティベート
$ source ~/.venv/kaggle/bin/activate

# Jupyter で必須のライブラリ
$ pip install ipykernel

# あとは必要に応じて
$ pip install numpy pandas matplotlib scikit-learn
{{< /code >}}

{{% note title="activate 用エイリアス" %}}
毎回 `source ~/.venv/kaggle/bin/activate` と入力するのが面倒な場合は、シェルの設定ファイル（`.zshrc` など）にエイリアスを設定しておくと便利です。

{{< code lang="bash" >}}
alias venv-kaggle="source ~/.venv/kaggle/bin/activate"
{{< /code >}}
{{% /note %}}

### venv の検索パスを追加

前述のように、プロジェクトの外に venv 環境を作成した場合は、VS Code の Python 拡張が見つけられるように検索パスを追加しておく必要があります。
**<kbd><kbd>Ctrl/Cmd</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd></kbd> ⇨ <samp>Preferences: Open User Settings (JSON)</samp>** で設定ファイルを開いて、次のような行を追加しておきます（参考: [Where the extension looks for environment](https://code.visualstudio.com/docs/python/environments#_where-the-extension-looks-for-environments)）。

{{< code lang="js" title="settings.json" >}}
{
  // Python の venv 検索パスを追加（~/.venv/kaggle などを検索するため）
  "python.venvPath": "~/.venv",

  // あるいは下記でも可能（ホームディレクトリ以下のディレクトリ名を列挙）
  // "python.venvFolders": [".venv"],
{{< /code >}}

**`python.venvPath`** あるいは **`python.venvFolders`** のいずれかで設定すれば大丈夫です。
プロジェクト（ワークスペース）のルートディレクトリ内に `.venv` ディレクトリを配置する場合は、この設定は不要です。


Python 拡張に venv を認識させる
----

VS Code 上の Python 拡張は、デフォルトではシステムにインストールされた Python インタープリター（`python` コマンド）を使おうとするので、上記で作成した仮想環境 (venv) 内の Python インタープリターを使うように設定しておきます。
この作業は、プロジェクト内で一度だけ実行すれば OK です。

まず、適当にプロジェクト用のディレクトリを作成して、VS Code を起動します。

{{< code lang="console" title="VS Code の起動" >}}
$ mkdir ~/kaggle
$ code ~/kaggle
{{< /code >}}

VS Code 上で Python ファイルを開いて、右下に表示された Python バージョンをクリックするか、コマンドパレット (**<kbd>Ctrl/Cmd</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd>**) から **`Python: Select Interpreter`** を選択し、先ほど作成した Python 仮想環境 (`~/.venv/kaggle`) を選択します。
これで、このプロジェクトでは仮想環境内の Python インタープリターが使われるようになります。

{{% note title="Python インタープリター設定はどこに保存されるか？" %}}
`Python: Select Interpreter` で選択した Python インタープリターの設定は、VS Code 内部の保存領域に保存されるため、JSON 形式の設定ファイルとしては確認できないようです（参考: [Setting descriptions](https://github.com/microsoft/vscode-python/wiki/Setting-descriptions)）。
この情報をクリアするには、コマンドパレットから **`Python: Clear Workspace Interpreter Setting`** を実行します（システムの Python を使う状態に戻ります）。
次のように、デフォルトの Python インタープリターを設定しておくこともできます。

{{< code lang="js" >}}
{
  // ...
  "python.defaultInterpreterPath": "~/.venv/kaggle/bin/python",
  // ...
}
{{< /code >}}

この設定は、プロジェクト内でまだ一度も Python インタープリターを選択していないときに使われます（あるいは、前述の方法で設定をクリアした場合）。
ただ、Python インタープリターの設定はプロジェクト内で一度だけ行えばよいので、この設定はあまり使わないかもしれません。
{{% /note %}}


Jupyter Notebook を起動する
----

**<kbd>Ctrl/Cmd</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd>** でコマンドパレットを開いて、**`Create: New Jupyter Notebook`** を選択すると、新しい Jupyter Notebook (`Untitled-1.ipynb`) ファイルが作成されます。

こちらも、どの Python 実行環境を使うかを選択しておく必要があります。
Jupyter Notebook では「カーネル」と呼ばれ、Python 拡張が参照する「Python インタープリター」とは別に設定する必要があります。
Jupyter Notebook 画面上の **<samp>カーネル選択</samp>** ボタンを押すか、コマンドパレットから **`Notebook: Select Notebook Kernel`** を選択し、使用する Python 仮想環境 (`~/.venv/kaggle`) を選択します。

これで、Jupyter Notebook 上で Python コードを実行する準備が整いました。
`pandas` などのライブラリもインポートできるようになっているはずです。


Kaggle CLI のインストール
----

### kaggle コマンドのインストールと設定

これは VS Code とは直接関係ありませんが、Kaggle 公式の **`kaggle`** コマンドをインストールしておくと、コマンドラインからコンペティションのデータセットのダウンロードや、解答の提出ができて便利です。
簡単にセットアップできるので入れておきましょう。

{{< code lang="console" title="Kaggle CLI のインストール" >}}
$ pip install kaggle
{{< /code >}}

`kaggle` コマンドを実行するには、Kaggle のサイトで API トークンを取得して、**`~/.kaggle/kaggle.json`** に保存しておく必要があります。
[Kaggle のアカウント設定](https://www.kaggle.com/settings/account) を開き、**`Create New Token`** ボタンをクリックすると、`kaggle.json` ファイルをダウンロードできます。
OS によって保存先が異なるので注意してください（`kaggle` コマンドのエラー表示を見て確認するのでもよいです）。

| OS | 保存先 |
| ---- | ---- |
| Windows | `C:\Users\<ユーザー名>\.kaggle\kaggle.json` |
| Linux | `~/.config/kaggle/kaggle.json` |
| macOS / その他 | `~/.kaggle/kaggle.json` |

### kaggle コマンドの使い方

#### 開催中のコンペティションの一覧 (kaggle competitions list)

```console
$ kaggle c list                            # 基本的な使い方
$ kaggle c list --help                     # ヘルプ
$ kaggle c list --search titanic           # 検索 (Titanic)
$ kaggle c list --category gettingStarted  # カテゴリ (Getting Started)
$ kaggle c list --category playground      # カテゴリ (Playground)
$ kaggle c list --sort-by recentlyCreated  # ソート (Recently Created)
```

{{< accordion title="実行例" >}}
{{< code lang="console" >}}
$ kaggle c list --sort-by recentlyCreated
ref                                                                                deadline             category                reward  teamCount  userHasEntered
---------------------------------------------------------------------------------  -------------------  ---------------  -------------  ---------  --------------
https://www.kaggle.com/competitions/playground-series-s5e2                         2025-02-28 23:59:00  Playground                Swag       1363           False
https://www.kaggle.com/competitions/konwinski-prize                                2025-03-12 23:59:00  Featured         1,225,000 Usd        306           False
https://www.kaggle.com/competitions/lux-ai-season-3                                2025-03-10 23:59:00  Featured            50,000 Usd        463           False
https://www.kaggle.com/competitions/equity-post-HCT-survival-predictions           2025-03-05 23:59:41  Research            50,000 Usd       2459           False
https://www.kaggle.com/competitions/llms-you-cant-please-them-all                  2025-03-04 23:59:00  Featured            50,000 Usd       1317           False
https://www.kaggle.com/competitions/fide-google-efficiency-chess-ai-challenge      2025-02-11 23:59:00  Featured            50,000 Usd       1120           False
https://www.kaggle.com/competitions/wsdm-cup-multilingual-chatbot-arena            2025-03-10 23:59:00  Featured            50,000 Usd        950           False
https://www.kaggle.com/competitions/ai-mathematical-olympiad-progress-prize-2      2025-04-01 23:59:00  Featured         2,117,152 Usd       1356           False
https://www.kaggle.com/competitions/llm-classification-finetuning                  2030-07-01 23:59:00  Getting Started      Knowledge        125           False
https://www.kaggle.com/competitions/jane-street-real-time-market-data-forecasting  2025-07-12 23:59:00  Featured           120,000 Usd       3757           False
https://www.kaggle.com/competitions/spaceship-titanic                              2030-01-01 00:00:00  Getting Started      Knowledge       1962           False
https://www.kaggle.com/competitions/store-sales-time-series-forecasting            2030-06-30 23:59:00  Getting Started      Knowledge        676           False
https://www.kaggle.com/competitions/gan-getting-started                            2030-07-01 23:59:00  Getting Started      Knowledge        139           False
https://www.kaggle.com/competitions/contradictory-my-dear-watson                   2030-07-01 23:59:00  Getting Started      Knowledge         32           False
https://www.kaggle.com/competitions/tpu-getting-started                            2030-06-03 23:59:00  Getting Started      Knowledge         49           False
https://www.kaggle.com/competitions/connectx                                       2030-01-01 00:00:00  Getting Started      Knowledge        176           False
https://www.kaggle.com/competitions/nlp-getting-started                            2030-01-01 00:00:00  Getting Started      Knowledge        553           False
https://www.kaggle.com/competitions/home-data-for-ml-course                        2030-01-01 23:59:00  Getting Started      Knowledge       5897           False
https://www.kaggle.com/competitions/house-prices-advanced-regression-techniques    2030-01-01 00:00:00  Getting Started      Knowledge       3506           False
https://www.kaggle.com/competitions/titanic                                        2030-01-01 00:00:00  Getting Started      Knowledge      12781            True
{{< /code >}}
{{< /accordion >}}

#### データセットのダウンロード (kaggle competitions download)

上記のコマンドで各コンペティションの URL が分かったら、その URL の末尾部分（最後の `/` 以降）を使ってデータセットをダウンロードできます。
次の例では、コンペティション名 `playground-series-s5e2` のデータセットをダウンロードしています（先に [Kaggle のサイト](https://www.kaggle.com/competitions)で対象のコンペに参加しておく必要があります）。

{{< code lang="console" title="コンペのデータセットをダウンロード" >}}
$ kaggle c download playground-series-s5e2
{{< /code >}}

zip 形式でダウンロードされるので、`unzip` コマンドなどで展開すればコーディングの準備完了です。

{{< code lang="console" title="zip ファイルを data ディレクトリに展開" >}}
$ unzip playground-series-s5e2.zip -d data
Archive:  playground-series-s5e2.zip
  inflating: data/sample_submission.csv
  inflating: data/test.csv
  inflating: data/train.csv
  inflating: data/training_extra.csv
{{< /code >}}

できたー ٩(๑❛ᴗ❛๑)۶ わーぃ

