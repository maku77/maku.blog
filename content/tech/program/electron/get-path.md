---
title: "Electron の app.getData() で取得できる特殊ディレクトリパスの一覧"
url: "/p/77sas8m"
date: "2020-07-01"
tags: ["Electron"]
---

app.getPath について
----
`electron` モジュールの [app.getPath(name)](https://www.electronjs.org/docs/api/app#appgetpathname) 関数を使用すると、OS 固有の特殊フォルダパスを取得することができます。
例えば、__`getPath('userData')`__ とすると、ユーザーの設定ファイルなどを格納するディレクトリを取得することができます。

下記は、いろんな特殊フォルダのパスを取得するテストコードです。

{{< code lang="typescript" >}}
import { app } from 'electron';

export function showSpecialDirs() {
    console.log(`app.getAppPath() = ${app.getAppPath()}`);

    console.log('\n==== Application data');
    console.log(`app.getPath('home')       = ${app.getPath('home')}`);
    console.log(`app.getPath('temp')       = ${app.getPath('temp')}`);
    console.log(`app.getPath('appData')    = ${app.getPath('appData')}`);
    console.log(`app.getPath('cache')      = ${app.getPath('cache')}`);
    console.log(`app.getPath('userData')   = ${app.getPath('userData')}`);
    console.log(`app.getPath('logs')       = ${app.getPath('logs')}`);
    console.log(`app.getPath('crashDumps') = ${app.getPath('crashDumps')}`);

    console.log('\n==== OS multimedia')
    console.log(`app.getPath('desktop')    = ${app.getPath('desktop')}`);
    console.log(`app.getPath('documents')  = ${app.getPath('documents')}`);
    console.log(`app.getPath('downloads')  = ${app.getPath('downloads')}`);
    console.log(`app.getPath('music')      = ${app.getPath('music')}`);
    console.log(`app.getPath('pictures')   = ${app.getPath('pictures')}`);
    console.log(`app.getPath('videos')     = ${app.getPath('videos')}`);

    console.log('\n==== Executables')
    console.log(`app.getPath('exe')        = ${app.getPath('exe')}`);
    console.log(`app.getPath('module')     = ${app.getPath('module')}`);
}
{{< /code >}}

逆に特殊フォルダのパスを設定するための [app.setPath(name, path)](https://www.electronjs.org/docs/api/app#appsetpathname-path) 関数も提供されています。
アプリ起動時に、この関数を使って特殊フォルダのパスをカスタマイズすれば、各モジュールが使用するフォルダのパスを切り替えることができます（そのモジュールが `app.getPath()` でパスを取得していることが前提ですが）。
例えば、ユーザーデータを扱うテストコードを実行する場合は、```app.setPath('userData', `${__dirname}/test-data`)``` のようにテスト用のデータフォルダに切り替えることができます。


各種 OS で app.getPath したときの結果
----

上記の関数を各種 OS で実行すると次のような結果が得られます。

{{< code title="Windows 10 の場合" >}}
app.getAppPath() = D:\gitwork\MyApp

==== Application data
app.getPath('home')       = C:\Users\maku
app.getPath('temp')       = C:\Users\maku\AppData\Local\Temp
app.getPath('appData')    = C:\Users\maku\AppData\Roaming
app.getPath('cache')      = C:\Users\maku\AppData\Roaming
app.getPath('userData')   = C:\Users\maku\AppData\Roaming\MyApp
app.getPath('logs')       = C:\Users\maku\AppData\Roaming\MyApp\Electron\logs
app.getPath('crashDumps') = C:\Users\maku\AppData\Roaming\MyApp\Crashpad

==== OS multimedia
app.getPath('desktop')    = C:\Users\maku\Desktop
app.getPath('documents')  = C:\Users\maku\Documents
app.getPath('downloads')  = C:\Users\maku\Downloads
app.getPath('music')      = C:\Users\maku\Music
app.getPath('pictures')   = C:\Users\maku\Pictures
app.getPath('videos')     = C:\Users\maku\Videos

==== Executables
app.getPath('exe')        = D:\gitwork\MyApp\node_modules\electron\dist\electron.exe
app.getPath('module')     = D:\gitwork\MyApp\node_modules\electron\dist\electron.exe
{{< /code >}}

