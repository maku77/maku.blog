---
title: "各種 Linux のファイアウォール設定ツール"
linkTitle: "ファイアウォール"
url: "p/ceow5cj/"
date: "2022-07-09"
tags: ["Linux", "セキュリティ"]
---

従来 Linux のパケットフィルタリングの設定には、iptables が使われていましたが、現在ではより直感的な操作が可能な ufw、firewalld、nftables といったツールが使われています。

| ディストリビューション | ファイアウォール設定ツール |
| ---- | ---- |
| Ubuntu 8.04 LTS 以降 | ufw |
| CentOS 7 以降 | firewalld |
| Fedora 18 以降 | firewalld |
| Rocky Linux | firewalld および nftables |

- 参考: [ufw (Uncomplicated Firewall) によるファイアウォール設定](/p/drar8o4/)
- 参考: [firewalld によるファイアウォール設定](/p/ij6kxeq/)

どのツールも下回りとしては Linux カーネルの Netfilter サブシステムの仕組みを利用しています。

```
      ufw ---+
             |
firewalld ---+---> Netfilter (Linux kernel)
             |
 iptables ---+
```

