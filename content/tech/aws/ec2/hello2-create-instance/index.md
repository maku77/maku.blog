---
title: "Amazon EC2 で Hello World (2) EC2 インスタンスの起動と接続"
url: "/p/whu7hr3"
date: "2021-03-02"
tags: ["AWS"]
weight: 102
---

EC2 インスタンスの起動
----

EC2 インスタンスに接続するための [キーペアを作成](/p/pcp3doy) したら、実際に EC2 インスタンスを起動してみます。
ここでは、マシンイメージ (AMI) として EC2 用に最適化された Amazon Linux 2 を選択することにします。

1. [EC2 マネージメントコンソール](https://console.aws.amazon.com/ec2/v2/home) を開く
1. サイドバーの `インスタンス` を選択し、__`インスタンスを起動`__ ボタンを押す
1. Amazon マシンイメージ (AMI) の選択画面で、__`Amazon Linux 2`__ を選択
1. インスタンスタイプの選択画面で、一番安い __`t2.nano`__ や、一年間の無料枠がある __`t2.micro`__ を選択（料金の参考: [EC2Instances.info](https://instances.vantage.sh/?selected=t2.nano)）
1. __`確認と作成`__ ボタンを押す
1. 使用するキーペアを選択する画面が表示されるので、[あらかじめ設定しておいた]() キーペアを選択して、__`インスタンスの作成`__ ボタンを押す
   {{< image w="500" border="true" title="キーペアの選択" src="img-001.png" >}}

あとは数分待てば、EC2 インスタンスが起動します。


EC2 インスタンスへの SSH 接続
----

EC2 インスタンスが無事起動したら、SSH で接続してみます。
接続用のユーザー名やアドレスは、次のようにして確認することができます。

1. [EC2 マネージメントコンソール](https://console.aws.amazon.com/ec2/v2/home) を開く
1. サイドバーの `インスタンス` を選択し、対象の EC2 インスタンスを選択
1. __`接続`__ ボタンを押す

すると、次のような感じで接続先のユーザー名（`ec2-user`）や DNS アドレス（`ec2-XXXXX`) を確認することができます。

{{< image w="600" border="true" title="EC2 インスタンスの接続情報" src="img-002.png" >}}

あとは、この情報を使って `ssh` コマンドで接続するだけです。
`EC2 Instance Connect` タブで `接続` ボタンを押すと、ブラウザ上で動作する端末画面から接続することもできます。

{{< code >}}
$ ssh -i ~/.ssh/ec2key.pem ec2-user@ec2-52-193-255-XXX.ap-northeast-1.compute.amazonaws.com

       __|  __|_  )
       _|  (     /   Amazon Linux 2 AMI
      ___|\___|___|

https://aws.amazon.com/amazon-linux-2/
[ec2-user@ip-52-193-255-XXX ~]$
{{< /code >}}

上記では、DNS アドレス (FQDN) を使っていますが、IP アドレスでも接続できます。

{{< code >}}
$ ssh -i ~/.ssh/ec2key.pem ec2-user@52.193.255.XXX
{{< /code >}}

これらのアドレスは、EC2 インスタンスを再起動すると変化することに注意してください。


EC2 インスタンスの停止
----

EC2 インスタンスは立ち上がっているかぎり従量課金で使用料がかかるので、しばらく使わない場合は忘れずに停止しておきます。
EC2 コンソール上でインスタンスを選択し、__`インスタンスの状態`__ プルダウンから、__`インスタンスの停止`__ を選択することで停止できます。

{{< image border="true" title="EC2 インスタンスの停止" src="img-003.png" >}}

これで、EC2 の Hello World は完了です。

