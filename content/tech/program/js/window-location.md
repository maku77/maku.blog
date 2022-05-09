---
title: "JavaScript で現在のページの URL の構成要素を取得する (window.location)"
url: "/p/j6iu6gs"
date: "2020-09-03"
tags: ["JavaScript"]
---

Web サイト上で実行される JavaScript から __`window.location`__ を参照すると、カレントページの URL の構成要素をパーツごとに取得することができます。

{{< code lang="js" title="JavaScript" >}}
const loc = window.location;

console.log('location.href = ' + loc.href);
console.log('location.origin = ' + loc.origin);
console.log('location.host = ' + loc.host);
console.log('location.protocol = ' + loc.protocol);
console.log('location.hostname = ' + loc.hostname);
console.log('location.port = ' + loc.port);
console.log('location.pathname = ' + loc.pathname);
console.log('location.search = ' + loc.search);
console.log('location.hash = ' + loc.hash);
{{< /code >}}

例えば、次のようなアドレスにアクセスした場合は、

{{< code >}}
https://example.com:8042/over/there?key1=val1&key2=val2#nose
{{< /code >}}

次のような結果を取得することができます。

{{< code lang="ini" >}}
location.href = https://example.com:8042/over/there?key1=val1&key2=val2#nose
location.origin = https://example.com:8042
location.host = example.com:8042
location.protocol = https:
location.hostname = example.com
location.port = 8042
location.pathname = /over/there
location.search = ?key1=val1&key2=val2
location.hash = #nose
{{< /code >}}

