---
title: "Python で OpenGL (2) PyOpenGL のインストール"
url: "/p/665rua3"
date: "2019-08-16"
tags: ["OpenGL", "Python"]
---

PyOpenGL とは
----

[前回の記事](/p/sfpwow9) では、`pyGLFW` を使ってウィンドウを表示するところまで行きました。
ただし、`pyGLFW` がやってくれるのは、ウィンドウの作成や、そのサーフェスに描画するための OpenGL コンテキストを生成するところまでです。
実際に OpenGL コンテキストを使ってレンダリングするには、OpenGL の API が必要です。

そのためのライブラリが [PyOpenGL パッケージ](https://pypi.org/project/PyOpenGL/) です。


PyOpenGL のインストール
----

`PyOpenGL` パッケージも `pyGLFW` と同様、`pip` コマンドで簡単にインストールすることができます。

{{< code >}}
$ pip install PyOpenGL
{{< /code >}}


PyOpenGL で Hello World
----

下記のサンプルコードは、pyGLFW (glfw) パッケージを使ってウィンドウの初期化を行い（`main()`）、PyOpenGL (OpenGL) パッケージを使って OpenGL の API を呼び出しています（`render()`）。

{{< code lang="python" title="main.py" >}}
import glfw
from OpenGL.GL import *

def render():
    glClearColor(0, 0, 0.5, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)

def main():
    if not glfw.init():
        raise RuntimeError('Could not initialize GLFW3')

    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, GL_TRUE)
    glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    window = glfw.create_window(320, 240, 'Hello World', None, None)
    if not window:
        glfw.terminate()
        raise RuntimeError('Could not create an window')

    glfw.make_context_current(window)

    while not glfw.window_should_close(window):
        render()
        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == "__main__":
    main()
{{< /code >}}

ここでは、`glClear()` しているだけなので、背景がクリアされるだけです。
背景色は `glClearColor()` で暗めの青に設定しています。

{{< code >}}
$ python main.py
{{< /code >}}

{{< image w="320" src="python-pyopengl-001.png" >}}

