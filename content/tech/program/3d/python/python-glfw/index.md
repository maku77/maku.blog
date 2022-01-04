---
title: "Python で OpenGL (1) pyGLFW のインストール"
url: "/p/sfpwow9"
date: "2019-08-16"
tags: ["OpenGL", "Python"]
---

Python で簡単に Open GL のプログラムを開発できるように、GLFW (pyGLFW) というライブラリを使用するための環境構築をしてみます。

GLFW とは
----

Khronos グループが作成している OpenGL や OpenGL ES、Vulkan などの API を使用すると、クラスプラットフォームな 3D プログラムを作成することができます。
ただし、OS ごとにウィンドウの作成方法やキーハンドリングの方法は異なっているため、その部分は個別に作成する必要があります。

[GLFW](https://www.glfw.org/) は、そのあたりの OS 依存の処理を抽象化するためラッパーライブラリで、Windows、macOS、Linux などのデスクトップ環境に対応しています。
GLFW 本家は C 言語用のライブラリとして作成していますが、Python 用のラッパーライブラリ (pyGLFW) も公開されています。

昔は GLUT というライブラリがよく使われていましたが、今はもうメンテナンスされていないので、GLFW を使います。


pyGLFW (glfw) のインストール
----

ここでは、Python 用の GLFW ラッパーライブラリである、[pyGLFW (glfw) パッケージ](https://pypi.org/project/glfw/) をインストールします。

### Python 用ライブラリ glfw のインストール

Python 用の `glfw` パッケージは `pip` コマンドで簡単にインストールできます。
システム全体の環境に影響しないようにするには、[virtualenv を使って仮想環境を構築](https://maku77.github.io/python/env/virtualenv.html) してからインストールするのがよいでしょう。

{{< code title="virtualenv で仮想環境に切り替える場合" >}}
$ mkdir sample
$ virtualenv ENV
$ source ENV/bin/activate
{{< /code >}}

{{< code title="glfw のインストール" >}}
$ pip install glfw
{{< /code >}}

### ランタイムライブラリのインストール

`glfw` を使用したプログラムを実行するには、各 OS ごとに用意されたランタイムライブラリ (Shared library) もインストールしておく必要があります。

{{< code title="macOS の場合" >}}
$ brew install glfw3
{{< /code >}}

{{< code title="Ubuntu/Debian の場合" >}}
$ sudo apt-get install -y libglfw3-dev
{{< /code >}}

{{< code title="Fedora/Red Hat の場合" >}}
$ sudo yum install -y libglfw3-dev
{{< /code >}}

Windows の場合は、[本家のサイト](https://www.glfw.org/download.html) から DLL ファイルを含んだアーカイブをダウンロードし、`GLFW_LIBRARY` 環境変数で DLL のパスを指定します。

ランタイムライブラリがインストールされていない状態で、`glfw` を使用したプログラムを実行しようとすると、下記のようなエラーが発生します。

{{< code >}}
ImportError: Failed to load GLFW3 shared library.
{{< /code >}}


GLFW で Hello World
----

下記は、GLFW (pyGLFW) を使ってウィンドウを表示するだけの簡単なプログラムです。

{{< code lang="python" title="main.py" >}}
import glfw

def main():
    # Initialize glfw
    if not glfw.init():
        raise RuntimeError('Could not initialize GLFW3')

    # Create a windowed mode window and its OpenGL context
    window = glfw.create_window(320, 240, 'Hello World', None, None)
    if not window:
        glfw.terminate()
        raise RuntimeError('Could not create an window')

    # Make the window's context current
    glfw.make_context_current(window)

    # Loop until the user closes the window
    while not glfw.window_should_close(window):
        # Render here, e.g. using pyOpenGL

        # Swap front and back buffers
        glfw.swap_buffers(window)

        # Poll for and process events
        glfw.poll_events()

    # Proper shutdown
    glfw.terminate()

if __name__ == "__main__":
    main()
{{< /code >}}

次のように実行すると、空っぽのウィンドウが表示されます。

{{< code >}}
$ python main.py
{{< /code >}}

{{< image w="320" src="python-glfw-001.png" title="GLFW でウィンドウ表示" >}}

