---
title: "PowerShell のプロファイルを作成して独自コマンドを定義する"
url: "p/v7bkitw/"
date: "2024-05-08"
tags: ["PowerShell"]
---

プロファイルとは
----

PowerShell は起動時に __`$PROFILE`__ のパスに格納されたプロファイルファイルを読み込むようになっています。
このファイルで関数の定義や初期化処理を行うことで、PowerShell を便利に使えるようになります。
`$PROFILE` には次のようなパスが格納されています。

```powershell
PS C:\> $profile
C:\Users\maku\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1
```


プロファイルの作成
----

デフォルトでは `$PROFILE` が示すファイルは存在しない可能性があるので、__`New-Item`__ コマンドレットなどを使って、エディタで開く前にファイルを生成しておく必要があります。

{{< code lang="powershell" title="プロファイルを作成してエディタで開く" >}}
New-Item -Path $profile -ItemType file -Force
notepad $profile
{{< /code >}}

このファイルを作成したあとに新しく PowerShell を開こうとすると、[実行ポリシー](https://go.microsoft.com/fwlink/?LinkID=135170) 関連のセキュリティエラーが発生します。
これは、デフォルトで `ps1` ファイルの実行が許可されていないからです。

{{< code title="セキュリティエラー" >}}
. : ファイル C:\Users\maku\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1 を読み込めません。
ファイル C:\Users\maku\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1 はデジタル署名されていません。
このスクリプトは現在のシステムでは実行できません。
スクリプトの実行および実行ポリシーの設定の詳細については、「about_Execution_Policies」(https://go.microsoft.com/fwlink/?LinkID=135170) を参照してください。
{{< /code >}}

現在の実行ポリシーの一覧は、__`Get-ExecutionPolicy`__ コマンドレットで確認できます。

```powershell
PS C:\> Get-ExecutionPolicy -List

        Scope ExecutionPolicy
        ----- ---------------
MachinePolicy       Undefined
   UserPolicy       Undefined
      Process       Undefined
  CurrentUser       Undefined
 LocalMachine       AllSigned
```

カレントユーザーによるスクリプトの実行を許可するには、次のように __`Set-ExecutionPolicy`__ コマンドレットを実行します。

```powershell
PS C:\> Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```

ここで指定している `RemoteSigned` ポリシーは、「インターネット上の署名されたスクリプト」および「ローカルコンピューター上のスクリプト」の実行を許可するポリシーです。


プロファイルの設定例
----

### 例）ディレクトリ移動用のコマンドを定義

下記は、ディレクトリ移動のための関数（`cd-gitwork` や `u`）を定義する例です。
ついでに PowerShell の起動時にその関数を呼び出して、ディレクトリを移動しています。
よく使う作業ディレクトリに自動的に移動するようにしておくと便利です（Windows ターミナルの設定でも初期ディレクトリは設定できますが）。

{{< code lang="powershell" title="Microsoft.PowerShell_profile.ps1" >}}
Function cd-gitwork {
  cd D:\y\gitwork  # Set-Location -Path D:\y\gitwork でも OK
}
Function u { cd ..  }
Function uu { cd ..\..  }
Function uuu { cd ..\..\..  }

# PowerShell 起動時にディレクトリを移動
cd-gitwork
{{< /code >}}

独自コマンドの定義方法としては、`Set-Alias` コマンドレットによるエイリアス定義もありますが、`Set-Alias` は柔軟なパラメーター設定ができないので、`Function` を使って関数として定義するのがよいです。

### 例）他のスクリプトを読み込む

複数の PC で設定を共有したいときは、Dropbox などに共通スクリプトを置いておいて、__`.`__ で読み込むようにします。
各 PC のプロファイル (`$profile`) には、次のような 1 行だけを記述しておきます。

{{< code lang="powershell" title="Microsoft.PowerShell_profile.ps1" >}}
. D:\Dropbox\share\config\powershell\alias.ps1
{{< /code >}}

### 例）プロファイル設定ファイルを簡単に開けるようにする

プロファイル設定ファイルのパスは PowerShell の中で `$profile` で簡単に参照できる（例: `notepad $profile`）のですが、`.ps1` ファイルの中で下記のようなコマンドエイリアスを設定しておくと、その `.ps1` ファイル自身を簡単に開いて編集できるようになります。
前述のように別の `.ps1` ファイルをインクルードしているようなケースでは、`$profile` が指すファイルを開いて、さらに別の `.ps1` ファイルを辿る (Vim の `gt` など）といった操作が必要になってくるので、このようなショートカットを用意しておくと便利です。

{{< code lang="powershell" title="D:\Dropbox\share\config\powershell\alias.ps1" >}}
# 自分自身の .ps1 ファイルをエディタで開く
Function m-edit-alias { nvim-qt $PSCommandPath }
{{< /code >}}

ちなみに、**`m-`** というプレフィックスは、標準コマンドと自分で定義したコマンドが混ざらないようにするために付けてます。
`my` とか `maku` の略です ٩(๑❛ᴗ❛๑)۶

