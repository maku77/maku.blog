---
title: "Linux の system 関数で任意のプログラムを実行する"
url: "/p/neegpac"
date: "2011-06-22"
tags: ["Linux", "プログラム"]
---

Linux の `system` 関数は、プログラムの起動に失敗すると -1 を返します。
また、戻り値を `WEXITSTATUS` マクロにかけると、呼び出したプログラム自体の終了コード（正常時は 0）を取得することができます。

{{< code lang="cpp" >}}
#include <stdio.h>
#include <stdlib.h>  // system(), WEXITSTATUS

bool doSystem(const char* command) {
    int ret = system(command);
    if (ret == -1) {
        fprintf(stderr, "ERROR: Cannot execute [%s]\n", command);
        return false;
    }

    int exitStatus = WEXITSTATUS(ret);
    if (exitStatus != 0) {
        fprintf(stderr, "ERROR: [%s] returns error %d\n", command, exitStatus);
        return false;
    }

    return true;
}
{{< /code >}}

