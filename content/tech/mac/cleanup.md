---
title: "macOS で不要なファイルを削除してストレージ容量を確保する"
url: "/p/ygpw6dk"
date: "2021-05-30"
tags: ["mac"]
---

macOS のストレージ容量が足りなくなってきたときのために、各種キャッシュの削除方法などをまとめておきます。


Homebrew (brew) 関連
----

### 古いバージョンの削除

`brew` コマンドでいろいろコマンドをインストールしていると、古いバージョンのモジュールが `/usr/local/Cellar` ディレクトリの中にどんどん増えていきます。
`brea cleanup` コマンドで、古いモジュールを削除できます。

{{< code >}}
# ドライラン（何が削除されて、何MBくらい減らせるか確認）
$ brew cleanup -n

# 実際に削除
$ brew cleanup
{{< /code >}}

各モジュールでどのようなバージョンがインストールされているかは、次のように確認することができます。

{{< code >}}
$ brea ls-versions
...
libxext 1.3.4
libxrender 0.9.10
libyaml 0.1.6_1 0.2.5 0.1.7
...
{{< /code >}}

### ダウンロードキャッシュの削除

Homebrew のダウンロードキャッシュが格納されているディレクトリは次のコマンドで確認できます。

{{< code >}}
$ brew --cache
/Users/maku/Library/Caches/Homebrew
{{< /code >}}

次のようにしてキャッシュディレクトリごとまとめて削除できます。

{{< code >}}
$ rm -rf `brew --cache`
{{< /code >}}


ライブラリフォルダ内のキャッシュを削除
----

{{< code >}}
$ rm -rf ~/Library/Caches/*
{{< /code >}}


Ruby 関連
----

### gem

{{< code >}}
# インストールされている Gem のバージョン一覧
$ gem list

# 古いバージョンの削除（ドライラン）
$ gem cleanup --dryrun

# 古いバージョンの削除（実行）
$ gem cleanup
{{< /code >}}

