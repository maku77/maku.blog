---
title: "Linux のバージョンの調べ方まとめ"
url: "p/vdweubq/"
date: "2022-07-09"
tags: ["Linux"]
---

{{% private %}}
Docker で各種環境を試すときの起動方法。

```console
# Debian 系
$ docker container run --rm -it ubuntu
$ docker container run --rm -it debian

# Red Hat 系
$ docker container run --rm -it rockylinux
$ docker container run --rm -it almalinux
```
{{% /private %}}

Linux ディストリビューションのバージョン情報を調べる方法はいくつかありますが、__`/etc/os-release`__ ファイルの内容を見るのが簡単です。


/etc/os-release ファイル
----

{{< code lang="console" title="Ubuntu 22.04 の場合" >}}
$ cat /etc/os-release
PRETTY_NAME="Ubuntu 22.04 LTS"
NAME="Ubuntu"
VERSION_ID="22.04"
VERSION="22.04 (Jammy Jellyfish)"
VERSION_CODENAME=jammy
ID=ubuntu
ID_LIKE=debian
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
UBUNTU_CODENAME=jammy
{{< /code >}}

{{< code lang="console" title="Debian 11 の場合" >}}
$ cat /etc/os-release
PRETTY_NAME="Debian GNU/Linux 11 (bullseye)"
NAME="Debian GNU/Linux"
VERSION_ID="11"
VERSION="11 (bullseye)"
VERSION_CODENAME=bullseye
ID=debian
HOME_URL="https://www.debian.org/"
SUPPORT_URL="https://www.debian.org/support"
BUG_REPORT_URL="https://bugs.debian.org/"
{{< /code >}}

{{< code lang="console" title="Rocky Linux 8.6 の場合" >}}
$ cat /etc/os-release
NAME="Rocky Linux"
VERSION="8.6 (Green Obsidian)"
ID="rocky"
ID_LIKE="rhel centos fedora"
VERSION_ID="8.6"
PLATFORM_ID="platform:el8"
PRETTY_NAME="Rocky Linux 8.6 (Green Obsidian)"
ANSI_COLOR="0;32"
CPE_NAME="cpe:/o:rocky:rocky:8:GA"
HOME_URL="https://rockylinux.org/"
BUG_REPORT_URL="https://bugs.rockylinux.org/"
ROCKY_SUPPORT_PRODUCT="Rocky Linux"
ROCKY_SUPPORT_PRODUCT_VERSION="8"
REDHAT_SUPPORT_PRODUCT="Rocky Linux"
REDHAT_SUPPORT_PRODUCT_VERSION="8"
{{< /code >}}

{{< code lang="console" title="AlmaLinux 8.6 の場合" >}}
$ cat /etc/os-release
NAME="AlmaLinux"
VERSION="8.6 (Sky Tiger)"
ID="almalinux"
ID_LIKE="rhel centos fedora"
VERSION_ID="8.6"
PLATFORM_ID="platform:el8"
PRETTY_NAME="AlmaLinux 8.6 (Sky Tiger)"
ANSI_COLOR="0;34"
LOGO="fedora-logo-icon"
CPE_NAME="cpe:/o:almalinux:almalinux:8::baseos"
HOME_URL="https://almalinux.org/"
DOCUMENTATION_URL="https://wiki.almalinux.org/"
BUG_REPORT_URL="https://bugs.almalinux.org/"
ALMALINUX_MANTISBT_PROJECT="AlmaLinux-8"
ALMALINUX_MANTISBT_PROJECT_VERSION="8.6"
REDHAT_SUPPORT_PRODUCT="AlmaLinux"
REDHAT_SUPPORT_PRODUCT_VERSION="8.6"
{{< /code >}}


/etc/system-release ファイル
----

Red Hat 系 Linux において簡潔なバージョン情報が格納されたファイルです。
__`/etc/system-release`__ ファイルは、ディストリビューションごとに次のようなファイルへのシンボリックリンクになっています。

- `/etc/almalinux-release` ... AlmaLinux の場合の実体
- `/etc/centos-release` ... CentOS の場合の実体
- `/etc/redhat-release` ... RHEL の場合の実体
- `/etc/rocky-release` ... Rocky Linux の場合の実体

{{< code lang="console" title="Rocky Linux 8.6 の場合" >}}
$ cat /etc/system-release
Rocky Linux release 8.6 (Green Obsidian)
{{< /code >}}

{{< code lang="console" title="AlmaLinux 8.6 の場合" >}}
$ cat /etc/system-release
AlmaLinux release 8.6 (Sky Tiger)
{{< /code >}}


/etc/issue ファイル
----

__`/etc/issue`__ ファイルには、ユーザーがログインする前に表示されるメッセージが定義されています。
この内容を見ることでも、ディストリビューションのバージョンを把握できることがあります。

{{< code lang="console" title="Ubuntu 22.04 の場合" >}}
$ cat /etc/issue
Ubuntu 22.04 LTS \n \l
{{< /code >}}

{{< code lang="console" title="Debian 11 の場合" >}}
$ cat /etc/issue
Debian GNU/Linux 11 \n \l
{{< /code >}}

{{< code lang="console" title="Rocky Linux 8.6 の場合（バージョン判別できず）" >}}
$ cat /etc/issue
\S
Kernel \r on an \m
{{< /code >}}

{{< code lang="console" title="AlmaLinux 8.6 の場合（バージョン判別できず）" >}}
$ cat /etc/issue
\S
Kernel \r on an \m
{{< /code >}}

ちなみに、`\n` などのエスケープシーケンスは、実際にはホスト名などのテキストに置換されて表示されます。


lsb_release コマンド
----

{{< code lang="console" title="Ubuntu 22.04 の場合" >}}
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 22.04 LTS
Release:	22.04
Codename:	jammy
{{< /code >}}

{{< code lang="console" title="Debian 11 の場合" >}}
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Debian
Description:	Debian GNU/Linux 11 (bullseye)
Release:	11
Codename:	bullseye
{{< /code >}}

Ubuntu や Debian で `lsb_release` コマンドが見つからない場合は、`apt install -y lsb-release` でインストールできます（インストールするパッケージ名は、ハイフンで単語が区切られていることに注意）。

{{< code lang="console" title="Rocky Linux 8.6 の場合" >}}
$ lsb_release -a
LSB Version:	:core-4.1-aarch64:core-4.1-noarch
Distributor ID:	Rocky
Description:	Rocky Linux release 8.6 (Green Obsidian)
Release:	8.6
Codename:	GreenObsidian
{{< /code >}}

{{< code lang="console" title="AlmaLinux 8.6 の場合" >}}
$ lsb_release -a
LSB Version:    :core-4.1-amd64:core-4.1-noarch
Distributor ID: AlmaLinux
Description:    AlmaLinux release 8.6 (Sky Tiger)
Release:        8.6
Codename:       SkyTiger
{{< /code >}}

Rocky Linux や AlmaLinux で `lsb_release` コマンドが見つからない場合は、`dnf install -y redhat-lsb-core` でインストールできます。

