---
title: "Unicode、フォント関連用語"
url: "/p/hrhozs9"
date: "2012-04-24"
tags: ["フォント", "Unicode"]
---

- [Unicode.org の Unicode 関連用語集](http://unicode.org/glossary/)

用語まとめ
----

font family
: Arial とか Courier とか。

font face
: font family に、Italic とか Bold とかの区別を加えたもの（ノーマルなものは Regular）。

font collection
: 複数の face を 1 つのファイルに含んだもの。

glyph
: 文字を描画したときの形。フォントファイルは、ビットマップで glyph を持つこともあるし、ベクターデータで glyph を持つこともある。

ligature
: 合字。２文字以上をくっつくて１文字を表現したもの。

character map (charmap)
: 特定の encoding の文字コードを、glyph index にマッピングするための情報。１つの Font face は複数の charmap を含んでいることが多い。Mac 用の charmap、Unicode（Windows）の charmap など。

スクリプト
: スクリプト（書体）は言語の文字情報を表す記号の集まりです。スクリプトの例には、ラテン文字、アラビア文字、漢字、ギリシャ文字があります（参考: http://unicode.org/reports/tr24/ ）。

