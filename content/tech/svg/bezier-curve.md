---
title: "SVG の path 要素によるベジェ曲線の描画例"
url: "p/ka6xumg/"
date: "2023-09-23"
tags: ["SVG"]
---

`svg` の __`path`__ 要素を使用すると、直線と曲線を組み合わせて多様な図形を描画できます。
図形の形状は、`path` 要素の __`d`__ 属性に一連のコマンドを指定することで定義されます。
しかし、ベジェ曲線の制御点をどのように指定すれば、どのような曲線が描かれるのかが直感的に理解しにくいことがあります。
ここではさまざまな曲線の描画例を紹介します。


C / S コマンドによる 3 次ベジェ曲線
----

__`C` コマンド__ は、2 つの制御点と終点を指定することで、始点（現在座標）と終点を曲線で繋ぎます。
`path` の `d` 属性の内容は、`M始点 C制御点1 制御点2 終点` のような形で指定することになります。

<table>
<tr>
  <td>
    <svg width="210" height="110" viewBox="-20 -20 210 110" style="border: 3px double gray;">
      <path d="M0,0 C5,70 95,70 100,0" stroke="black" stroke-width="2" fill="none"/>
      <g transform="translate(0 0)">
        <circle r="6" fill="blue" />
        <text x="10" y="5">(0, 0)</text>
      </g>
      <g transform="translate(5 70)">
        <circle r="6" fill="red" />
        <text x="10" y="5">(5, 70)</text>
      </g>
      <g transform="translate(95 70)">
        <circle r="6" fill="orange" />
        <text x="10" y="5">(95, 70)</text>
      </g>
      <g transform="translate(100 0)">
        <circle r="6" fill="cyan" />
        <text x="10" y="5">(100, 0)</text>
      </g>
    </svg>
    <div>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="blue" /></svg>始点
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="red" /></svg>制御点1<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="orange" /></svg>制御点2
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="cyan" /></svg>終点
    </div>
  </td>
  <td>
    <code>&lt;path d="M0,0 C5,70 95,70 100,0" stroke="black" stroke-width="2" fill="none" /></code>
  </td>
</tr>
<tr>
  <td>
    <svg width="210" height="240" viewBox="-20 -70 210 240" style="border: 3px double gray;">
      <path d="M0,50 C50,150 50,-50 100,50" stroke="black" stroke-width="2" fill="none"/>
      <g transform="translate(0 50)">
        <circle r="6" fill="blue" />
        <text x="10" y="5">(0, 50)</text>
      </g>
      <g transform="translate(50 150)">
        <circle r="6" fill="red" />
        <text x="10" y="5">(50, 150)</text>
      </g>
      <g transform="translate(50 -50)">
        <circle r="6" fill="orange" />
        <text x="10" y="5">(50, -50)</text>
      </g>
      <g transform="translate(100 50)">
        <circle r="6" fill="cyan" />
        <text x="10" y="5">(100, 50)</text>
      </g>
    </svg>
    <div>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="blue" /></svg>始点
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="red" /></svg>制御点1<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="orange" /></svg>制御点2
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="cyan" /></svg>終点
    </div>
  </td>
  <td>
    <code>&lt;path d="M0,50 C50,150 50,-50 100,50" stroke="black" stroke-width="2" fill="none"/></code>
  </td>
</tr>
</table>

`C` コマンドによる 3 次ベジェ曲線に続けて次の曲線を描く場合、__`S` コマンド__ を使用すると 1 つ目の制御点を省略してなめらかに繋ぐことができます。
この場合、先行する曲線の 2 つ目の制御点の反対側に 1 つ目の制御点を指定したのと同様に振る舞います。
`S` コマンドの引数としては、2 つの座標値を `S制御点2 終点` の形で渡します（`C` コマンドと異なり、制御点 1 の座標値を省略します）。

<table>
<tr>
  <td>
    <svg width="310" height="160" viewBox="-20 -20 310 160" style="border: 3px double gray;">
      <path d="M0,50 C20,100 80,100 100,50 S180,0 200,50" stroke="black" stroke-width="2" fill="none"/>
      <g transform="translate(0 50)">
        <circle r="6" fill="blue" />
        <text x="10" y="5">(0, 50)</text>
      </g>
      <g transform="translate(20 100)">
        <circle r="6" fill="red" />
        <text x="-30" y="25">(20, 100)</text>
      </g>
      <g transform="translate(80 100)">
        <circle r="6" fill="orange" />
        <text x="10" y="5">(80, 100)</text>
      </g>
      <g transform="translate(100 50)">
        <circle r="6" fill="cyan" />
        <text x="10" y="5">(100, 50)</text>
      </g>
      <g transform="translate(120 0)">
        <circle r="6" fill="none" stroke="red" />
      </g>
      <g transform="translate(180 0)">
        <circle r="6" fill="brown" />
        <text x="10" y="5">(180, 0)</text>
      </g>
      <g transform="translate(200 50)">
        <circle r="6" fill="green" />
        <text x="10" y="5">(200, 50)</text>
      </g>
    </svg>
    <div>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="blue" /></svg>始点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="red" /></svg>Cコマンドの制御点1<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="orange" /></svg>Cコマンドの制御点2<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="cyan" /></svg>Cコマンドの終点 = 2つ目の始点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="none" stroke="red" /></svg>Sコマンドの（仮想的な）制御点1<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="brown" /></svg>Sコマンドの制御点2<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="green" /></svg>Sコマンドの終点<br/>
    </div>
  </td>
  <td>
    <code>&lt;path d="M0,50 C20,100 80,100 100,50 S180,0 200,50" stroke="black" stroke-width="2" fill="none"/></code>
  </td>
</tr>
</table>


Q / T による 2 次ベジェ曲線
----

__`Q` コマンド__ を使った 2 次ベジェ曲線は、制御点を 1 つだけ指定するので `C` コマンド（3 次ベジェ曲線）よりも簡単です。

<table>
<tr>
  <td>
    <svg width="210" height="140" viewBox="-20 -20 210 140" style="border: 3px double gray;">
      <path d="M0,0 Q50,100 100,0" stroke="black" stroke-width="2" fill="none"/>
      <g transform="translate(0 0)">
        <circle r="6" fill="blue" />
        <text x="10" y="5">(0, 0)</text>
      </g>
      <g transform="translate(50 100)">
        <circle r="6" fill="red" />
        <text x="10" y="5">(50, 100)</text>
      </g>
      <g transform="translate(100 0)">
        <circle r="6" fill="cyan" />
        <text x="10" y="5">(100, 0)</text>
      </g>
    </svg>
    <div>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="blue" /></svg>始点
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="red" /></svg>制御点
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="cyan" /></svg>終点
    </div>
  </td>
  <td>
    <code>&lt;path d="M0,0 Q50,100 100,0" stroke="black" stroke-width="2" fill="none"/></code>
  </td>
</tr>
</table>

`Q` コマンドの曲線に続けて次の曲線を描画するときは、__`T` コマンド__ を使うことで制御点の指定を省略できます。
その場合、先行する曲線の制御点の反対側に制御点を指定したのと同様に振る舞います。

<table>
<tr>
  <td>
    <svg width="270" height="140" viewBox="-20 -20 270 140" style="border: 3px double gray;">
      <path d="M0,50 Q40,100 80,50 T160,50" stroke="black" stroke-width="2" fill="none"/>
      <g transform="translate(0 50)">
        <circle r="6" fill="blue" />
        <text x="10" y="5">(0, 50)</text>
      </g>
      <g transform="translate(40 100)">
        <circle r="6" fill="red" />
        <text x="10" y="5">(40, 100)</text>
      </g>
      <g transform="translate(80 50)">
        <circle r="6" fill="cyan" />
        <text x="0" y="22">(80, 50)</text>
      </g>
      <g transform="translate(120 0)">
        <circle r="6" fill="none" stroke="red" />
      </g>
      <g transform="translate(160 50)">
        <circle r="6" fill="green" />
        <text x="10" y="5">(160, 50)</text>
      </g>
    </svg>
    <div>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="blue" /></svg>始点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="red" /></svg>Qコマンドの制御点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="cyan" /></svg>Qコマンドの終点 = 2つ目の始点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="none" stroke="red" /></svg>Tコマンドの（仮想的な）制御点<br/>
      <svg width="17" height="14"><circle cx="8" cy="8" r="6" fill="green" /></svg>Tコマンドの終点<br/>
    </div>
  </td>
  <td>
    <code>&lt;path d="M0,50 Q40,100 80,50 T160,50" stroke="black" stroke-width="2" fill="none"/></code>
  </td>
</tr>
</table>

