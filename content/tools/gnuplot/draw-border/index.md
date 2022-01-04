---
title: "gnuplot: グラフの枠を表示・非表示する"
url: "/p/3mgr3db"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 306
---

枠 (border) の表示・非表示
----

{{< code lang="gnuplot" title="Syntax" >}}
set [no]border
{{< /code >}}

グラフの枠はデフォルトでは表示されるので、特に枠を消したい場合のみ下記のように実行します。

{{< code lang="gnuplot" title="例: グラフの枠を消す" >}}
set noborder
plot sin(x)
{{< /code >}}

{{< image src="20040103-noborder.gif" >}}


指定した軸だけ表示する
----

{{< code lang="gnuplot" title="Syntax" >}}
set border <枠番号の論理和>
{{< /code >}}

<TABLE align="center" border="1">
	<TR>
		<TH>&nbsp;</TH><TH>plot border</TH><TH>splot</TH><TH>splot</TH>
	</TR>
	<TR>
		<TH>Side</TH><TH>splot base</TH><TH>verticals</TH><TH>top</TH>
	</TR>
	<TR>
		<TD>bottom (south)</TD><TD class="c">1</TD><TD class="c">16</TD><TD class="c">256</TD>
	</TR>
	<TR>
		<TD>left (west)</TD><TD class="c">2</TD><TD class="c">32</TD><TD class="c">512</TD>
	</TR>
	<TR>
		<TD>top (north)</TD><TD class="c">4</TD><TD class="c">64</TD><TD class="c">1024</TD>
	</TR>
	<TR>
		<TD>right (east)</TD><TD class="c">8</TD><TD class="c">128</TD><TD class="c">2048</TD>
	</TR>
</TABLE>

