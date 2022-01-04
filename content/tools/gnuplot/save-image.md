---
title: "gnuplot: 画像ファイルへの保存"
url: "/p/ojm4was"
date: "2004-01-03"
tags: ["gnuplot"]
weight: 106
---

GIF で出力
----

{{< code title="Syntax" >}}
set terminal gif [transparent] [interlace] [font_size]
                 [size x, y] [bg_color] [border_color]
                 [axes_color] [line1_color line2_color ...]
{{< /code >}}

<TABLE align="center" border="1">
	<CAPTION>set terminal gif のオプション</CAPTION>
	<TR>
		<TH>option</TH><TH>value</TH><TH>Default</TD><TH>description</TH>
	</TR>
	<TR>
		<TD class="namec">transparent</TD><TD>transparent | なし</TD><TD>なし</TD><TD>背景を透過するか</TD>
	</TR>
	<TR>
		<TD class="namec">interlace</TD><TD>interlace | なし</TD><TD>なし</TD><TD>インターレースにするか</TD>
	</TR>
	<TR>
		<TD rowspan="3" class="namec">font_size</TD><TD>small</TD><TD rowspan="3">small</TD><TD>6x12</TD>
	</TR>
	<TR>
		<TD>medium</TD><TD>7x13 (Bold)</TD>
	</TR>
	<TR>
		<TD>large</TD><TD>8x16</TD>
	</TR>
	<TR>
		<TD class="namec">size</TD><TD>size &lt;x&gt;, &lt;y&gt;</TD><TD>640, 480</TD><TD>画像の幅と高さ</TD>
	</TR>
	<TR>
		<TD class="namec">bg_color</TD><TD>&lt;xRRGGBB&gt;</TD><TD>xFFFFFF (白)</TD><TD>背景色</TD>
	</TR>
	<TR>
		<TD class="namec">border_color</TD><TD>&lt;xRRGGBB&gt;</TD><TD>x000000 (黒)</TD><TD>枠の色</TD>
	</TR>
	<TR>
		<TD class="namec">axes_color</TD><TD>&lt;xRRGGBB&gt;</TD><TD>xCCCCCC (灰)</TD><TD>軸の色</TD>
	</TR>
	<TR>
		<TD class="namec">lineX_color</TD><TD>&lt;xRRGGBB&gt;</TD><TD>xFF0000 (赤)<BR>x00FF00 (緑)<BR>x0000FF (青)</TD><TD>線の色を順に指定</TD>
	</TR>
</TABLE>

{{< code lang="gnuplot" title="例: test.gif に出力する設定" >}}
set terminal gif size 400, 300
set output 'test.gif'
{{< /code >}}

{{< code lang="gnuplot" title="例: test.gif に出力する設定（白色を透過）" >}}
set terminal gif size 400, 300 transparent xffffff
set output 'test.gif'
{{< /code >}}

これに続けて `plot` すれば、カレントディレクトリ (`pwd` で確認できます) に GIF ファイルが作成されます。
画面で出力を確認しておいてから出力先をファイルに変更し、`replot` を実行するのがおすすめです。


PostScript 形式で出力
----

{{< code title="Syntax" >}}
set terminal postscript {<mode>}
                        {<enhancement>}
                        {<color>}
                        {<lineStyle>}
                        {<duplexing>}
                        {"<fontName>"}
                        {<fontSize>}
{{< /code >}}

<TABLE align="center" border="1">
	<CAPTION>set terminal postscript のオプション</CAPTION>
	<TR>
		<TH>Option</TH><TH>Value</TH><TH>Default</TH><TH>Description</TH>
	</TR>
	<TR>
		<TD class="namec">&lt;mode&gt;</TD>
		<TD>landscape | portrait | eps | default</TD>
		<TD>landscape</TD>
		<TD>グラフのモード。向きなどを指定する。</TD>
	</TR>
	<TR>
		<TD class="namec">&lt;enhancement&gt;</TD>
		<TD>enhanced | noenhanced | plus | noplus</TD>
		<TD>noenhanced</TD>
		<TD>拡張ポストスクリプトを有効にするか無効にするか。</TD>
	</TR>
	<TR>
		<TD class="namec">&lt;color&gt;</TD>
		<TD>color | monochrome</TD>
		<TD>monochrome</TD>
		<TD>カラーで出力するかモノクロで出力するか。</TD>
	</TR>
	<TR>
		<TD class="namec">&lt;lineStyle&gt;</TD>
		<TD>dashed | solid</TD>
		<TD>dashed</TD>
		<TD>グラフのラインに破線を使うか全て実線にするか。</TD>
	</TR>
	<TR>
		<TD class="namec">&lt;duplexing&gt;</TD>
		<TD>defaultplex | simplex | duplex</TD>
		<TD>defaultplex</TD>
		<TD>片面印刷か両面印刷か。</TD>
	</TR>
	<TR>
		<TD class="namec">&quot;&lt;fontName&gt;&quot;</TD>
		<TD>&quot;Times-Roman&quot; など</TD>
		<TD>&quot;Helvetica&quot;</TD>
		<TD>PS で使用できるフォントを指定。</TD>
	</TR>
	<TR>
		<TD class="namec">&lt;fontSize&gt;</TD>
		<TD>12 など</TD>
		<TD>14</TD>
		<TD>フォントのポイント数。</TD>
	</TR>
</TABLE>


EPS で出力
----

{{< code lang="gnuplot" title="例: 白黒で EPS ファイルに出力する設定" >}}
set term postscript eps enhanced
set output 'test.eps'
{{< /code >}}

{{< code lang="gnuplot" title="例: カラーで EPS ファイルに出力する設定" >}}
set term postscript eps enhanced color
set output 'test.eps'
{{< /code >}}

{{< code lang="gnuplot" title="例: 論文用の EPS ファイルを出力する設定" >}}
# set term postscript eps plus
set term postscript eps enhanced
set size 0.6,0.6
set lmargin 8
set bmargin 3
set rmargin 2
set tmargin 1
set output "fig1.eps"
{{< /code >}}

論文用の EPS ファイルを作成する場合は、マージンやサイズを変更しないといけないかもしれません。
LaTeX に `epsfig` で EPS ファイルを貼るときは、普通に `plot` すると文字が小さくて読めないので、LaTeX のほうでは `\epsfig{file=data.eps, scale=1.5}` くらいにするといいです。
思い通りの大きさにするには、gnuplot のほうでも `set size` を指定してください。

### コラム: EPS ファイルを直接編集してポイント（記号）の大きさを変える

記号の横と縦のサイズは **`/hpt_`** と **`/vpt_`** で定義されています。
サイズを大きくした場合は、ここの数字を大きい値に変更します。

{{< code title="EPS ファイルの 17, 18 行目くらい" >}}
/hpt_ 31.5 def    % 横のサイズ
/vpt_ 31.5 def    % 縦のサイズ
{{< /code >}}

