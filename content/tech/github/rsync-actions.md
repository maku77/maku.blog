---
title: "GitHub Actions で Hugo サイトをビルドして VPS サーバーに rsync デプロイする"
url: "/p/un3gu8m"
date: "2022-02-27"
tags: ["GitHub Actions", "Hugo", "rsync"]
---

何をするか？
----

Web サイトのコンテンツを GitHub で管理し、さくらの VPS や、お名前.com の VPS で Web サーバーを運用している場合、GitHub Actions でビルドとデプロイを自動化すると便利です。
ここでは、Web サイトのビルドに __Hugo__、VPS サーバーへのデプロイに __rsync__ を使う前提で、次のような手順で自動化を進めていきます。

1. SSH 鍵を作成する（自動デプロイのためパスワードは設定しない）
2. VPS 側に SSH 公開鍵を登録する
3. GitHub Actions のシークレットとして SSH 秘密鍵を登録する
4. GitHub Actions のワークフローを作成し、ビルド (Hugo) とデプロイ (rsync) を自動化する

Web サイトのビルドには何を使ってもよいのですが、現時点でおそらく最速の静的サイトジェネレーターである Hugo を例にして説明しています。
GitHub Actions による自動化が完了すると、GitHub の main ブランチに Web サイトコンテンツを push するだけで、Hugo によるビルドと rsync による VPS へのデプロイが自動で行われるようになります。


SSH キーペアを作成して VPS へ公開鍵を登録する
----

`rsync` コマンドで使用する SSH 鍵を `ssh-keygen` コマンドで作成しておきます。
GitHub Actions から `rsync` コマンドを実行するので、SSH 秘密鍵にはパスワードを設定しないようにします。
次の例では、`github-actions` / `github-actions.pub` という名前のキーペアを作成しています。
ファイル名は何でも構いません。

```console
$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/Users/maku/.ssh/id_rsa): github-actions
Enter passphrase (empty for no passphrase): ★そのままEnter
Enter same passphrase again: ★そのままEnter
```

SSH キーぺアが用意できたら、VPS（Web サーバー）へ公開鍵を登録します。
`ssh-id-copy` コマンドを使うと、簡単に `~/.ssh/authorized_keys` にエントリを追加できます。

```console
$ ssh-copy-id -i github-actions.pub user@example.com
...
user@example.com's password: （リモートホスト側のユーザーのパスワードを入力）
...
```

- 参考: [ssh-id-copy で SSH の公開鍵をリモートホストに登録する](/p/2mzbmw8)

`ssh` コマンドで接続できるようになっているか確認しておきます。

```console
$ ssh -i github-actions user@example.com
```

ここまでできたら、作成した SSH キーペアを安全な場所に退避しておきます。
特に秘密鍵 (`github-actions`) の方は厳重に管理してください。


GitHub のシークレットとして SSH 秘密鍵を登録する
----

GitHub Actions から SSH 秘密鍵を参照する必要があるので、先に Repository secret 情報として SSH 秘密鍵の値を登録しておきます。

1. GitHub の対象リポジトリを開き、__`Settings`__ タブ → __`Secrets`__ → __`Actions`__ と選択します。
2. __`New repository secret`__ ボタンを押します。
3. `Name` に __`SSH_PRIVATE_KEY`__ と入力し、`Value` に秘密鍵ファイル（今回は `github-actions` ファイル）の内容を貼り付けます。

    ```
    -----BEGIN OPENSSH PRIVATE KEY-----
    b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAABlwAAAAdzc2gtcn
    NhAAAAAwEAAQAAAYEAqBOVBdo3ZL41PXEqvDY7malbuWPur0grpHLlh/sLqY4Wqm64b4W6

    ...（省略）...

    hLY0Rl3sWcrLswb6/j5tu713d0wL+jyXwTJ2G00nVXOE86wmDEsUEQWazZ166sxGP5U7Co
    RyRi5y7Tx50Z0AAAASbWFrdUBtYWt1bWFjLmxvY2FsAQ==
    -----END OPENSSH PRIVATE KEY-----
    ```

VPS の「ユーザー名」や「接続先アドレス」も念のため隠しておきたいので、次のようなシークレットも追加で登録しておきます。

- Name: __`SSH_USER`__、Value: VPSのユーザ名 （例: `maku`）
- Name: __`SSH_HOST`__、Value: VPSのアドレス （例: `www9999xx.sakura.ne.jp`）
- Name: __`DST_PATH`__、Value: デプロイ先のホームディレクトリからの相対パス （例: `webroot`）


GitHub Actions のワークフローを作成する
----

Web サイトのコンテンツが格納されている GitHub リポジトリに、GitHub Actions のワークフローを作成します。
ワークフローでは、特定のイベントが発生したときに実行する一連のジョブを定義します。

1. `Actions` タブを開き、__`set up a workflow yourself`__ を選択します。
2. 後述のような内容のワークフローファイルをコミットします。デフォルトのファイル名は `main.yml` ですが、`.github/workflows` ディレクトリ以下であれば、ファイル名は何でもかまいません。

これで、`main` ブランチに Web サイトのコンテンツを push（あるいはトピックブランチからのマージ）するだけで、Hugo によるビルドと rsync によるデプロイが自動で行われるようになります。

{{< code lang="yaml" title="<Repo>/.github/workflows/main.yml" >}}
on:
  push:
    branches: [ main ]

jobs:
  build-and-deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
        with:
          submodules: true  # Fetch Hugo themes (true OR recursive)
          fetch-depth: 0    # Fetch all history for .GitInfo and .Lastmod

      - name: Setup Hugo
        uses: peaceiris/actions-hugo@v2
        with:
          hugo-version: '0.92.1'
          extended: true  # Enable scss

      - name: Build
        run: hugo --minify

      - name: Generate ssh key
        run: echo "$SSH_PRIVATE_KEY" > ${{ runner.temp }}/key && chmod 600 ${{ runner.temp }}/key
        env:
          SSH_PRIVATE_KEY: ${{ secrets.SSH_PRIVATE_KEY }}

      - name: Deploy with rsync
        run: >
          rsync -e 'ssh -i ${{ runner.temp }}/key -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null'
          -av --delete public/ ${SSH_USER}@${SSH_HOST}:${DST_PATH}
        env:
          SSH_USER: ${{ secrets.SSH_USER }}
          SSH_HOST: ${{ secrets.SSH_HOST }}
          DST_PATH: ${{ secrets.DST_PATH }}
{{< /code >}}

以下、このワークフローファイルの内容について細かく見ていきます。

```yaml
on:
  push:
    branches: [ main ]
```

まず、この `on` プロパティで、`main` ブランチへの push が行われたときに、このワークフローが実行されるように設定しています。
残りは、`build-and-deploy` ジョブの各ステップの説明になります（今回定義しているジョブは 1 つだけです）。
ジョブ内の各ステップは上から順番に実行されていき、どこかでエラーが発生すると、そこでジョブの実行は停止します。
なので、ビルドに失敗したら、後続のデプロイ処理は実行されません。

```yaml
- uses: actions/checkout@v2
  with:
    submodules: true  # Fetch Hugo themes (true OR recursive)
    fetch-depth: 0    # Fetch all history for .GitInfo and .Lastmod
```

最初のステップでは、定番の `actions/checkout` アクションを使って、リポジトリ内のコードをワークスペースにチェックアウトします。
Hugo プロジェクトでは、通常 Git submodules で外部テーマを取り込んで使用するので、`submodules: true` オプションを指定して、サブモジュールを一緒に取得するようにしています。

```yaml
- name: Setup Hugo
  uses: peaceiris/actions-hugo@v2
  with:
    hugo-version: '0.92.1'
    extended: true  # Enable scss
```

`peaceiris/actions-hugo` で Hugo をインストールしています。
Web サイトのスタイル定義に SCSS などのプリプロセッサを使っている場合は、extended バージョンの Hugo が必要になるので、`extended: true` オプションを指定しています。

```yaml
- name: Build
  run: hugo --minify
```

Hugo による Web サイトのビルドを行います。
オプションは適宜修正します。
例えば、`-F` オプションを追加すれば、将来の日付が設定されたコンテンツをビルド対象にできます。

```yaml
- name: Generate ssh key
  run: echo "$SSH_PRIVATE_KEY" > ${{ runner.temp }}/key && chmod 600 ${{ runner.temp }}/key
  env:
    SSH_PRIVATE_KEY: ${{ secrets.SSH_PRIVATE_KEY }}
```

この後の `rsync` コマンド実行時に、SSH 秘密鍵の「ファイル」が必要になるので、ここで作成しています。
前述の手順で `SSH_PRIVATE_KEY` というシークレットに、SSH 秘密鍵の「値」を格納しておいたので、この値を `key` という名前の「ファイル」として出力します。
`${{ runner.temp }}` は、ビルド時にテンポラリディレクトリ名に置換されます。

```yaml
- name: Deploy with rsync
  run: >
    rsync -e 'ssh -i ${{ runner.temp }}/key -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null'
    -r -h --delete public/ ${SSH_USER}@${SSH_HOST}:${DST_PATH}
  env:
    SSH_USER: ${{ secrets.SSH_USER }}
    SSH_HOST: ${{ secrets.SSH_HOST }}
    DST_PATH: ${{ secrets.DST_PATH }}
```

最後は `rsync` によるデプロイ処理ですが、ここは若干複雑です。
`rsync` の `-e` オプションでは、SSH 秘密鍵ファイルの指定と、`known_hosts` 関連のエラー対策を行っています。

```
rsync -e 'ssh -i ${{ runner.temp }}/key -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null'
```

SSH で新しいホストに接続しようとすると、`known_hosts` への接続先ホスト情報の書き込みが行われるのですが、この処理はデフォルトでユーザーに確認を求めるという挙動になっているので、`-o StrictHostKeyChecking=no` というオプションで、ユーザー確認なしで書き込むようにします。
さらに、`-o UserKnownHostsFile=/dev/null` という指定で、実際には `known_hosts` ファイルには出力せずに、出力を破棄するという振る舞いにしています。
これらを指定しておかないと、`rsync` コマンド実行時に、`Host key verification failed.` というエラーが出て、ジョブが停止してしまいます。
このあたりは、おまじないのように入れておけばよいと思います。

`rsync` の残りの部分では、具体的なファイルのコピー方法などを指定しています。

```
    -av --delete public/ ${SSH_USER}@${SSH_HOST}:${DST_PATH}
  env:
    SSH_USER: ${{ secrets.SSH_USER }}
    SSH_HOST: ${{ secrets.SSH_HOST }}
    DST_PATH: ${{ secrets.DST_PATH }}
```

ここも用途によって要調整ですが、大体次のような指定を行っています。

- `-a` ... アーカイブモードでコピーする（ディレクトリを再帰的に処理し、タイムスタンプやパーミッション情報を維持する）
- `-v` ... ログを詳細に表示する
- `--delete` ... コピー元にないファイルをコピー先から削除する
- `public/` ... コピー元のディレクトリパス
  - Hugo のデフォルトの出力ディレクトリは `public` です。`public` ディレクトリそのものではなく、ディレクトリ内のファイルだけをコピーしたい場合は、`public/` のように末尾にスラッシュが必要なことに注意してください。
- `${SSH_USER}@${SSH_HOST}:${DST_PATH}` ... VPN のユーザー名、ホスト名、ディレクトリ名
  - それぞれのプレースホルダーには、GitHub の Repository secret で設定した値が入ります。コピー先のディレクトリ名は、VPN ユーザーのホームディレクトリからの相対パスで指定します。

`rsync` コマンドの詳細に関しては、下記の記事を参考にしてください。

- 参考: [rsync コマンドで2つのディレクトリを同期する](/p/c3s7wyx)

