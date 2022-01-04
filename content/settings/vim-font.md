---
title: "VIM のフォントを設定する (gvimrc)"
url: "/p/ba6kwjd"
date: "2009-09-09"
tags: ["PC設定"]
---

Gvim のフォントは、guifont にフォント名と、pt 数を指定することで変更できます。

{{< code lang="vim" >}}
:set guifont Courier:14     "Windows の場合
:set guifont Courier\ 14    "Linux の場合
{{< /code >}}

フォントの候補をカンマで区切って複数指定することができます。

{{< code lang="vim" >}}
:set guifont=Courier\ New:12, Arial:10
{{< /code >}}

フォントの高さと幅を別々に指定することもできます。

{{< code lang="vim" >}}
:set guifont=FixedSys:w10:h8
{{< /code >}}

フォントをダイアログから選んで設定する場合は、次のコマンドを実行します。

{{< code lang="vim" >}}
:set guifont=*
{{< /code >}}

詳しくは、

{{< code lang="vim" >}}
:help 'guifont'
{{< /code >}}


(gvimrc) ファイルタイプ別にフォントを設定する <!-- 2009-09-09 -->
----

{{< code lang="vim" title="例: *.txt ファイルを開いた場合のフォントを設定する" >}}
autocmd BufEnter *.txt set guifont=Arial\ 12
{{< /code >}}

