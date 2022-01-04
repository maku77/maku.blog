---
title: "Groovy をインストールする"
url: "/p/vtfddwb"
date: "2014-05-30"
tags: ["Groovy", "プログラム"]
---

MacOSX に Groovy をインストールする
----

**GVM: Groovy enVironment Manager** を使うと複数バージョンの Groovy を管理することができます。
ここでは、GVM を使って Groovy の環境をインストールする手順を示します。

### 1. GVM のインストール

```
$ curl -s get.gvmtool.net | bash
```

これで、`.bash_profile` に以下のように追加されて `gvm` コマンドにパスが通ります。

```
#THIS MUST BE AT THE END OF THE FILE FOR GVM TO WORK!!!
[[ -s "/Users/maku/.gvm/bin/gvm-init.sh" ]] && source "/Users/maku/.gvm/bin/gvm-init.sh"
```

### 2. GVM で groovy のインストール

```
$ gvm list groovy           # インストール可能なバージョン確認
$ gvm install groovy        # 最新版のインストール
$ gvm install groovy 2.3.2  # バージョンを指定してインストール
```

これで、`groovy` や `groovysh` コマンドが使用できるようになります。
使用中の `groovy` のバージョンの確認、切り替えは以下のように行います。

```
$ gvm current           # GVM で管理しているコマンドとそのバージョンを確認
$ gvm use groovy 2.3.1  # 使用する groovy コマンドのバージョンを切り替え
```


Windows に Groovy をインストールする
----

Windows では Groovy のインストーラを使用するのが簡単です。

1. Groovy を http://groovy.codehaus.org/Download のインストーラでインストール（環境変数 `GROOVY_JAVA` はインストーラから設定できます）。
3. JDK を http://www.oracle.com/technetwork/java/javase/downloads/ のインストーラでインストール。
4. 環境変数 `JAVA_HOME` を `C:\Program Files\Java\jdk1.8.0_05` などに設定（`bin` ディレクトリまで指定しないことに注意）。

これで、以下のように Groovy の処理系を使用できるようになります。

```
C:\> groovy   （ファイルを実行）
C:\> groovysh （対話型の実行）
```

{{% note title="dynamic library (jvm.dll) 関連のエラーが出る場合" %}}
Windows 用には、`groovy.exe` と `groovy.bat` が入っていますが、`groovy.bat` の方を実行するとうまく起動できます。
拡張子を省略して `groovy` と実行すると、`groovy.exe` の方が優先的に実行されてしまうので、`groovy.exe` を `__groovy.exe` などにリネームしておくとよいです。
{{% /note %}}

