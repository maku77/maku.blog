---
title: "TypeScriptサンプル: 文字列内のひらがなとカタカナを変換する (hiraToKana, kanaToHira)"
linkTitle: "文字列内のひらがなとカタカナを変換する (hiraToKana, kanaToHira)"
url: "/p/tzjvcad"
date: "2020-02-17"
tags: ["TypeScript"]
---

{{< code lang="ts" title="util.ts" >}}
export class Util {
  /**
   * 文字列に含まれているすべてのひらがなをカタカナに変換した文字列を作成します。
   * ただし、半角文字は変換しません。
   * @param str ひらがなを含む文字列
   */
  static hiraToKata(str: string): string {
    return str.replace(/[\u3041-\u3096]/g, ch =>
      String.fromCharCode(ch.charCodeAt(0) + 0x60)
    );
  }

  /**
   * 文字列に含まれているすべてのカタカナをひらがなに変換した文字列を作成します。
   * ただし、半角文字は変換しません。
   * @param str カタカナを含む文字列
   */
  static kataToHira(str: string): string {
    return str.replace(/[\u30A1-\u30FA]/g, ch =>
      String.fromCharCode(ch.charCodeAt(0) - 0x60)
    );
  }
}
{{< /code >}}

{{< code lang="ts" title="使用例" >}}
import { Util } from './util';

console.log(Util.hiraToKata('あいうアイウ'));  //=> アイウアイウ
console.log(Util.kataToHira('あいうアイウ'));  //=> あいうあいう
{{< /code >}}

{{< reference >}}
- [ひらがなとカタカナを正規表現で表す](/p/kkaq7q5)
{{< /code >}}

