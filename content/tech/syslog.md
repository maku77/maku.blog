---
title: "syslog のログの保存先の設定"
url: "/p/rm78k9i"
date: "2011-02-20"
tags: ["Linux"]
---

`/etc/syslog.conf` に以下のように記述されていれば、debug レベルのログは `/var/log/debug` に、info, notice, warn レベルのログは `/var/log/messages` に記録されます。

```
*.=debug; \
    auth,authpriv.none;\
    news.none;mail.none    -/var/log/debug
*.=info;*.=notice;*.=warn;\
    auth,authpriv.none;\
    cron,daemon.none;\
    mail,news.none    -/var/log/messages
```

