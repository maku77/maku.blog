---
title: "TeraTerm マクロのメモ"
url: "/p/kx7myd9"
date: "2011-04-19"
tags: ["アプリ", "TeraTerm"]
---


マクロの起動の仕方
----

### TeraTerm が起動している場合

1. メニューから **Control** → **Macro** でマクロファイル (.ttl) を選択

### TeraTerm を起動してマクロを実行する場合

```
C:\> ttermpro.exe /M=sample.ttl
```

### TeraTerm を起動せずにマクロを実行する場合

```
C:\> ttpmacro.exe sample.ttl
```

マクロの実行中止ダイアログを表示しないようにするには、`/V` オプションをつけます。
ttpmacro.exe でマクロを実行した場合、マクロ中で `connect` コマンドを実行したときに TeraTerm が表示されます。

{{< code title="例1: COM1 ポートでシリアル接続する場合" >}}
connect '/C=1'
{{< /code >}}

{{< code title="例2: TCP/IP (Telnet) 接続する場合" >}}
connect 'host.example.com'
{{< /code >}}

{{< code title="例3: TCP/IP (any port) 接続する場合" >}}
connect 'host.example.com:8080'
{{< /code >}}


文字列を表示する (dispstr)
----

`dispstr` を使えば、TeraTerm アプリケーションの機能として文字列を表示できます。
マクロの実行経過などを表示したい場合などは `dispstr` を使うのがよいでしょう。

```
dispstr 'Hello World'#$0A
```

末尾の `#$0A` は改行コード (LF) を表しています。

間違えやすいものに `sendln` がありますが、これは接続先の端末に対してコマンドを送るものです。
以下のように `echo` コマンドを送信した場合は、リモートの端末上で `echo` コマンドが実行されることになります。

```
sendln 'echo Hello World'
```


メッセージボックスを表示する (messagebox)
----

```
messagebox "Hello TTL!" "It's cool"
```


ビープ音を出す (beep)
----

ビープ音を出すコマンドの書式は以下の通りです。

```
beep [<sound type>]
```

以下のマクロでは、sound type を変えて 1 秒おきにビープ音を鳴らします。
Windows 7 の標準設定では、sound type = 4 の音は鳴りませんでした。

```
dispstr 'A simple beep'#$0A
beep 0
pause 1

dispstr 'Windows Asterisk sound'#$0A
beep 1
pause 1

dispstr 'Windows Exclamation sound'#$0A
beep 2
pause 1

dispstr 'Windows Critical Stop sound'#$0A
beep 3
pause 1

dispstr 'Windows Question sound'#$0A
beep 4
pause 1

dispstr 'Windows Default Beep sound'#$0A
beep 5
pause 1
```

{{< code title="実行結果" >}}
A simple beep
Windows Asterisk sound
Windows Exclamation sound
Windows Critical Stop sound
Windows Question sound
Windows Default Beep sound
{{< /code >}}


特定の文字列が出現するまで待機する (waitln)
----

`waitln` を使用すると、ターミナル上に特定の文字列が表示されるまで、マクロの実行がそこで待機されます。

```
waitln 'Hoge'
```

複数の文字列を指定することもできます。
例えば、以下のようにすると、いずれかの文字列が出現するまで待機します。

```
waitln 'Hoge1', 'Hoge2', 'Hoge3'
```

`timeout` 変数に秒数を代入すると、`waitln` のタイムアウト時間（最大待ち時間）を設定することができます。
0 秒以下を指定すると、`waitln` は指定された文字列が登場するまで永遠に待ち続けます。

```
; Timeout never occurs
timeout = 0
waitln 'Hoge'
```

{{< code title="応用例: ある文字列を含むラインを受信したらメッセージボックスに表示するマクロ" >}}
:LOOP
	waitln "Your Keyword"
	gettime timestr
	messagebox inputstr timestr
	goto LOOP
{{< /code >}}


指定した秒数だけ待機する (pause)
----

```
; Pause for 3 seconds.
pause 3
```


サブルーチンの定義＆呼び出し
----

```
call SubRoutineName
end

:SubRoutineName
	messagebox 'Yah Yah Yah!' ''
	return
```


マクロサンプル: telnet でリモートホストに自動ログインする
----

{{< code title="login_sample.ttl" >}}

```
;;;-----------------------------------------------------------------
;;; TeraTerm マクロのサンプル
;;; telnet でリモートホストに自動ログインする
;;;
;;; Usage:
;;;     ttpmacro.exe login_sample.ttl
;;;-----------------------------------------------------------------

;;; 接続先のホスト名（または IP アドレス）を指定
;;; （TeraTerm 起動してなかったら起動する）
connect 'host.example.com'

;;; "login:" という文字列を受信するまで待機し、
;;; ユーザ名を送信する。
wait 'login:'
sendln 'MyName'

;;; "password:" という文字列を受信するまで待機し、
;;; パスワードを送信する。
wait 'password:'
sendln 'MyPassword'
{{< /code >}}

