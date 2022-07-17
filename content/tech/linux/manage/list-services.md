---
title: "Linux で起動しているサービスの一覧を表示する"
url: "p/co7n5jy/"
date: "2022-07-17"
tags: ["Linux"]
---

pstree コマンドを使う方法
----

```console
$ pstree
systemd─┬─ModemManager───2*[{ModemManager}]
        ├─acpid
        ├─2*[agetty]
        ├─cron
        ├─dbus-daemon
        ├─fail2ban-server───4*[{fail2ban-server}]
        ├─irqbalance───{irqbalance}
        ├─multipathd───6*[{multipathd}]
        ├─networkd-dispat
        ├─ntpd───{ntpd}
        ├─packagekitd───2*[{packagekitd}]
        ├─polkitd───2*[{polkitd}]
        ├─rsyslogd───3*[{rsyslogd}]
        ├─snapd───10*[{snapd}]
        ├─sshd───sshd───sh───pstree
        ├─sshd───sshd───sshd
        ├─systemd───(sd-pam)
        ├─systemd-journal
        ├─systemd-logind
        ├─systemd-network
        ├─systemd-resolve
        ├─systemd-udevd
        ├─udisksd───4*[{udisksd}]
        └─unattended-upgr───{unattended-upgr}
```

`systemd` で各種サービスが動作していることを確認できます。


systemctl コマンドを使う方法
----

{{< code lang="console" >}}
$ systemctl list-unit-files --type=service
UNIT FILE                                  STATE           VENDOR PRESET
acpid.service                              enabled         enabled
apparmor.service                           enabled         enabled
apport-autoreport.service                  static          -
apport-forward@.service                    static          -
apport.service                             generated       -
apt-daily-upgrade.service                  static          -
apt-daily.service                          static          -
autovt@.service                            alias           -
blk-availability.service                   enabled         enabled
bolt.service                               static          -
cloud-config.service                       enabled         enabled
cloud-final.service                        enabled         enabled
cloud-init-hotplugd.service                static          -
...（省略）...
{{< /code >}}

```console
$ systemctl status
```


service コマンドを使う方法
----

```console
$ service --status-all
...（省略）...
 [ + ]  resolvconf
 [ - ]  rsync
 [ - ]  screen-cleanup
 [ + ]  ssh
 [ + ]  udev
 [ + ]  ufw
...（省略）...
```

記号の意味:

- `+` ... サービスが起動しています
- `–` ... サービスが停止しています
- `?` ... 状態を把握できません

