---
title: "Android アプリのテンプレートコード（空っぽのフラグメント）"
url: "/p/vdnv5dm"
date: "2021-02-25"
tags: ["Android"]
---

Android のアプリを作るときは、大体まっさらな Activity + Fragment の組み合わせで作り始めるんですが、そのようなクリーンなテンプレートコード（土台）を作るのが意外と大変だったりするので、ここにメモしておきます。

{{< image w="500" border="true" src="img-001.png" title="空っぽの Android アプリ" >}}

下記は、Android TV アプリ用のテンプレートです。

- [maku77/Template-AndroidTv: Template of Android TV App](https://github.com/maku77/Template-AndroidTv)

主なコードを抜粋。

{{< code lang="kotlin" title="app/src/main/java/com/example/myapp/MainActivity.kt" >}}
package com.example.myapp

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.commit // androidx.fragment:fragment-ktx

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        showMainFragment()
    }

    private fun showMainFragment() {
        supportFragmentManager.commit {
            add(R.id.fragment_container, MainFragment.newInstance())
            //addToBackStack(null)  // 戻るキー用にスタック
        }
    }
}
{{< /code >}}

{{< code lang="kotlin" title="app/src/main/java/com/example/myapp/MainFragment.kt" >}}
package com.example.myapp

import androidx.fragment.app.Fragment

class MainFragment : Fragment(R.layout.fragment_main) {
    companion object {
        fun newInstance() = MainFragment()
    }
}
{{< /code >}}

{{< code lang="xml" title="app/src/main/AndroidManifest.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.myapp">

    <uses-permission android:name="android.permission.INTERNET" />

    <uses-feature
        android:name="android.hardware.touchscreen"
        android:required="false" />
    <uses-feature
        android:name="android.software.leanback"
        android:required="true" />

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:supportsRtl="true"
        android:theme="@style/Theme.AppCompat">

        <activity
            android:name=".MainActivity"
            android:banner="@drawable/app_icon"
            android:icon="@drawable/app_icon"
            android:label="@string/app_name"
            android:logo="@drawable/app_icon"
            android:screenOrientation="landscape">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LEANBACK_LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
{{< /code >}}

{{< code lang="xml" title="app/src/main/res/layout/activity_main.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<FrameLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/fragment_container"
    android:layout_width="match_parent"
    android:layout_height="match_parent" />
</FrameLayout>
{{< /code >}}

{{< code lang="xml" title="app/src/main/res/layout/fragment_main.xml" >}}
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
</LinearLayout>
{{< /code >}}
