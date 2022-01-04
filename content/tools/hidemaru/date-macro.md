---
title: "秀丸: 日時挿入マクロ"
url: "/p/a9u6j7f"
date: "2005-01-31"
tags: ["エディタ", "秀丸"]
---

カーソル位置に、`2005-01-31 14:07:33` といった現在の日時を挿入するマクロです。

{{< code title="my_insert_datetime.mac" >}}
//disableinvert;
//disabledraw;

call DayOfWeekStr;

// 挿入用文字列を作成
#i = -1;
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day;
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day + " " + hour + ":" + minute;
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day + " (" + $$return + ")";
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day + " (" + $$return + ")" + " " + hour + ":" + minute;
#i = #i + 1; $str[#i] = year + "-" + month + "-" + day + " (" + $$return + ")" + " " + hour + ":" + minute + ":" + second;

// 表示用文字列を作成
#j = 0;
while (#j <= #i) {
    $showstr[#j] = "&" + str(#j + 1) + "  " + $str[#j];
    #j = #j + 1;
}

menuarray $showstr, #i+1;
#n = result;  // result は 1 回しか参照できないので保存 (Ver 4.15 -- 2005/07/08)
if (#n != 0) {
    insert $str[#n - 1];
}

//enabledraw;
//enableinvert;
endmacro;

// 曜日の文字列作成
DayOfWeekStr:
    ##n = dayofweeknum;
    if (##n == 0) {
        return "Sun";
    } else if (##n == 1) {
        return "Mon";
    } else if (##n == 2) {
        return "Tue";
    } else if (##n == 3) {
        return "Wed";
    } else if (##n == 4) {
        return "Thu";
    } else if (##n == 5) {
        return "Fri";
    } else if (##n == 6) {
        return "Sat";
    }
    return "";
{{< /code >}}

