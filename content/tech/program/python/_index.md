---
title: "Python チャレンジ"
url: "p/kvmtuk5/"
date: "2007-11-13"
tags: ["Python"]
draft: true
---

[Python Challenge](http://www.pythonchallenge.com/) の解答メモです。
Python のスキルアップをしたいのであれば、自分で解くまでは答えはみないようにっ。

Python Challenge #0: warming up
----

- 問題:
  - http://www.pythonchallenge.com/pc/def/0.html
  - try to change the URL address.
- 解答: http://www.pythonchallenge.com/pcc/def/map.html

2 の 38 乗を求めるだけです。

```console
$ python -c 'print(2**38)'
274877906944
```

以下のようなバリエーションがあります。

```python
2 ** 38
1 << 38
pow(2, 38)
```


Python Challenge #1: What about making trans?
----

- 問題
  - http://www.pythonchallenge.com/pc/def/274877906944.html
  - http://www.pythonchallenge.com/pc/def/map.html
  - everybody thinks twice before solving this.
- 解答: http://www.pythonchallenge.com/pcc/def/ocr.html

以下のような画像のヒントから、アルファベットを 2 つ後ろにずらせばよいことが分かります。

- K => M
- O => Q
- E => G

### str.maketrans() を使わない方法

{{< code lang="python" title="python-challenge-1a.py" >}}
text = """
g fmnc wms bgblr rpylqjyrc gr zw fylb. rfyrq ufyr amknsrcpq ypc dmp.
bmgle gr gl zw fylb gq glcddgagclr ylb rfyr'q ufw rfgq rcvr gq qm jmle.
sqgle qrpgle.kyicrpylq() gq pcamkkclbcb. lmu ynnjw ml rfc spj.
"""

result = ''
A = ord('a')
Z = ord('z')

for c in text:
    code = ord(c)
    if A <= code <= Z:
        index = ((code - A) + 2) % 26
        result += chr(A + index)
    else:
        result += c

print(result)
{{< /code >}}

{{< code title="結果" >}}
i hope you didnt translate it by hand. thats what computers are for.
doing it in by hand is inefficient and that's why this text is so long.
using string.maketrans() is recommended. now apply on the url.
{{< /code >}}

変換した結果の文で、"now apply on the url."（URL に対して実行しろ）と言っているので、アドレスバーに表示されている "http://www.pythonchallenge.com/pc/def/map.html" の map という部分を同様に置換して ocr にしたものが答えです。

### str.maketrans() を使う方法

問題文を変換した文の中で、__`string.maketrans()`__ を使うのがオススメといっているので、それで書き直すと確かに簡単にできました。
ただし、`import string` して使うのは Python 2 までで、Python 3 からは、組み込みの __`str`__ モジュールの __`str.maketrans()`__ を使用します。

{{< code lang="python" title="python-challenge-1b.py" >}}
text = '''
g fmnc wms bgblr rpylqjyrc gr zw fylb. rfyrq ufyr amknsrcpq ypc dmp.
bmgle gr gl zw fylb gq glcddgagclr ylb rfyr'q ufw rfgq rcvr gq qm jmle.
sqgle qrpgle.kyicrpylq() gq pcamkkclbcb. lmu ynnjw ml rfc spj.
'''

table = str.maketrans('abcdefghijklmnopqrstuvwxyz', 'cdefghijklmnopqrstuvwxyzab')
print(text.translate(table))
{{< /code >}}

さらに、`abcdefghijklmnopqrstuvwxyz` という文字列は、__`string.ascii_lowercase`__ で取得することができるので、上記の `str.maketrans()` のパラメータは以下のように記述できます。

```python
import string

text = ...

seq = string.ascii_lowercase
table = str.maketrans(seq, seq[2:] + seq[:2])
print(text.translate(table))
```


Python Challenge #2: ocr
----

- 問題
  - http://www.pythonchallenge.com/pc/def/ocr.html
  - recognize the characters. maybe they are in the book, but MAYBE they are in the page source.
- 解答: http://www.pythonchallenge.com/pcc/def/equality.html

"MAYBE they are in the page source" と書いてあるので、HTML ファイルのソースを見ると問題が見つかります。
問題文には、「下記テキスト内の頻度の低い文字を見つけよ」と書いてあります。
テキストデータが、`python_challenge.in` ファイルに保存してあるとして、以下のように文字の登場数を数えてみます。

{{< code lang="python" title="python_challenge2.py" >}}
ch_dic = {}

# Count characters
with open('python_challenge2.in') as f:
    for line in f:
        line = line.strip()
        for ch in line:
            ch_dic[ch] = ch_dic.get(ch, 0) + 1

# Display the number of each character
for key, val in ch_dic.items():
    print(key, val)
{{< /code >}}

{{< code title="実行結果" >}}
$ ./python_challenge3.py
@ 6157
i 1
e 1
a 1
[ 6108
^ 6030
_ 6112
] 6152
# 6115
! 6079
& 6043
$ 6046
% 6104
* 6034
+ 6066
( 6154
) 6186
l 1
q 1
t 1
u 1
{ 6046
y 1
} 6105
{{< /code >}}

これで、登場頻度の少ない文字は、`iealqtuy` の 8 文字だということが分かります。
この文字がテキストデータ内にどんな順で出てくるか調べてみます。

{{< code lang="python" >}}
with open('python_challenge2.in') as f:
    for line in f:
        line = line.strip()
        for ch in line:
            if ch in 'ealqiuty':
                print(ch, end='')
{{< /code >}}

```
equality
```


Python Challenge #3: re
----

- 問題
  - http://www.pythonchallenge.com/pc/def/equality.html
  - One small letter, surrounded by EXACTLY three big bodyguards on each of its sides.
- 解答: http://www.pythonchallenge.com/pcc/def/linkedlist.php

問題のテキストデータは、3 問目と同様に HTML ソースの中にありました。
ぱっと見、小文字と大文字で構成されたテキストなので、__surrounded by EXACTLY three big bodyguards__ というのは、大文字 3 字で囲まれた小文字ということでしょう。
タイトルが re であることから、正規表現 (Regular Expression) で解きましょうってことですね。

テキストの中から、正規表現にマッチする文字列を 1 つだけ探すには、__`re.search()`__ が使えます。

{{< code lang="python" title="python_challenge3.py" >}}
import re
text = ''.join([s.strip() for s in open('python_challenge3.in')])
m = re.search(r'[^A-Z][A-Z]{3}([a-z])[A-Z]{3}[^A-Z]', text)
print(m.group(1))
{{< /code >}}

上記のようにして探すと、l が見つかります。
http://www.pythonchallenge.com/pc/def/l.html を開いてみると、__yes. but there are more.__ と書いてあります。
さすがに 1 文字が答えってことはなさそうなので、全部の小文字を見つけて、繋げたものが答えかな。

テキストのなかから、正規表現にマッチする文字列を全て検索するには、__`re.findall()`__ （リスト返し）あるいは、__`re.finditer()`__（イテレータ返し）が使えます。

{{< code lang="python" title="python_challenge3b.py" >}}
import re
text = ''.join([s.strip() for s in open('python_challenge3.in')])
regexp = r'[^A-Z][A-Z]{3}([a-z])[A-Z]{3}[^A-Z]'
matches = re.findall(regexp, text)
print(''.join(matches))
{{< /code >}}

答えは `l i n k e d l i s t` の 10 文字でした。


Python Challenge #4: follow the chain
----

- 問題: http://www.pythonchallenge.com/pc/def/linkedlist.php
- 解答: http://www.pythonchallenge.com/pcc/def/peak.html

画像に以下のようなリンクが貼られています。

- http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=12345

リンクを辿ると、__and the next nothing is 44827__ と書いてあるので、それに従って URL を書き換えてアクセスしていくと、__Your hands are getting tired and the next nothing is 94485__ と表示されます。
この URL を書き換えてアクセスするという操作を自動化しろということでしょう。

ここでは、__`urllib.request.urlopen()`__ を使って HTML ソースを取得して、その中から、__`the next nothing is (.+)`__ というパターンを検索して、連続してアクセスすることにしましょう（Python 2 では `urllib.urlopen()` でした）。

{{< code lang="python" title="python_challenge4.py" >}}
import re
from urllib.request import urlopen

base_url = 'http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing='

def get_next_nothing(nothing):
    """Returns None if there is no more nothing."""

    url = base_url + nothing
    print(url)

    with urlopen(url) as res:
        # Read HTML text.
        # Since readline() returns binary, needs to be decoded.
        line = res.readline()
        line = line.decode('utf-8')
        print(line)

        # search next nothing
        match = re.search(r'the next nothing is (.+)', line)
        if match:  # != None
            return match.group(1)  # 'nothing' value

        return None

if __name__ == '__main__':
    nothing = '12345'
    # nothing = '8022'
    # nothing = '63579'
    while nothing:
        nothing = get_next_nothing(nothing)
{{< /code >}}

{{< code title="実行結果" >}}
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=12345
and the next nothing is 44827
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=44827
and the next nothing is 45439
  :
  :
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=16044
Yes. Divide by two and keep going.
{{< /code >}}

何十回かアクセスを繰り返すと、上記のように表示されて停止するので、
16044 を 2 で割った、8022 でまた同様にプログラムを走らせます。

{{< code title="実行結果（2 回目）" >}}
  :
  :
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=49574
and the next nothing is 82682
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=82682
There maybe misleading numbers in the
{{< /code >}}

最終的に辿り着いたページには以下のように書いてあります（2 行構成なので、上記では途切れて表示されてます）。

```
There maybe misleading numbers in the
text. One example is 82683. Look only for the next nothing and the next nothing is 63579
```

上記のテキスト内に入っている 1 つ目の数字 82683 はフェイクですね。
数字だけで検索していると引っかかるのかも。
最後に `nothing=63579` としてもう一回やってみます。

{{< code title="実行結果（3 度目の正直）" >}}
  :
  :
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=52899
and the next nothing is 66831
http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=66831
peak.html
{{< /code >}}


Python Challenge #5: peak hell
----

- 問題
  - http://www.pythonchallenge.com/pc/def/peak.html
  - pronounce it

peak hell の発音は pickle に似ているので、以下の URI を開いてみます。

- http://www.pythonchallenge.com/pc/def/pickle.html

すると、以下のように書いてあります。

```
yes! pickle!
```

なんのこっちゃ。。。
あ、Python の `pickle` モジュールってことですね。

- https://docs.python.org/3/library/pickle.html

