---
title: "macOS で JAVA_HOME に設定すべきパスを調べる（java_home コマンド）"
url: "/p/7jkgzd4"
date: "2014-07-19"
tags: ["java", "mac"]
---

複数の JDK がインストールされている場合にバージョンを切り替えて `java` (`javac`) コマンドを使用するには、`JAVA_HOME` 環境変数を設定します。
`JAVA_HOME` に設定すべきパスは、**`java_home`** コマンドで調べることができます。
下記は、Apple の JDK1.6、Oracle の JDK1.8 がインストールされている場合の出力例です。

{{< code >}}
$ /usr/libexec/java_home -v 1.6
/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home

$ /usr/libexec/java_home -v 1.8
/Library/Java/JavaVirtualMachines/jdk1.8.0_11.jdk/Contents/Home
{{< /code >}}

普段使用する JDK バージョンは、下記のように .bash_profile で設定しておけばよいでしょう。

{{< code lang="bash" title="~/.bash_profile" >}}
# JDK 1.6 を使用する場合
export JAVA_HOME=$(/usr/libexec/java_home -v 1.6)

# JDK 1.8 を使用する場合（デフォルト）
export JAVA_HOME=$(/usr/libexec/java_home -v 1.8)
{{< /code >}}

