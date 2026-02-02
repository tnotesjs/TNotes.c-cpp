# [0001. 搭建 C、C++ 本地学习环境](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0001.%20%E6%90%AD%E5%BB%BA%20C%E3%80%81C%2B%2B%20%E6%9C%AC%E5%9C%B0%E5%AD%A6%E4%B9%A0%E7%8E%AF%E5%A2%83)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 推荐用什么 IDE？](#3--推荐用什么-ide)
- [4. 🤔 需要安装哪些 VSCode 插件？](#4--需要安装哪些-vscode-插件)
- [5. 🤔 如何在 macOS 上初始化 C、C++ 的基本学习环境？](#5--如何在-macos-上初始化-cc-的基本学习环境)
- [6. 🤔 Code Runner 一键运行 C、CPP 的原理是？](#6--code-runner-一键运行-ccpp-的原理是)
- [7. 🤔 如何让 git 忽略编译产物？](#7--如何让-git-忽略编译产物)
- [8. 🤔 如何在 Windows 上初始化 C、C++ 的基本学习环境？](#8--如何在-windows-上初始化-cc-的基本学习环境)
- [9. 💻 demos.1 - 第一个 C 语言程序 - 在控制台打印 Hello World](#9--demos1---第一个-c-语言程序---在控制台打印-hello-world)
- [10. 🔗 引用](#10--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 在 macOS 和 windows 上搭建 C、C++ 的基本学习环境

## 2. 🫧 评价

这篇笔记主要介绍如何在 Windows 和 macOS 上搭建 C、C++ 的基本学习环境，确保在学习语法及语言核心特性阶段所编写的程序可以编译并执行。

搭建学习环境，核心就是安装 C、C++ 编译器（如 gcc、g++、clang 等），以及选择一个合适的代码编辑器（如 VSCode）来编写和运行代码。

环境搭建完成后，会编写第一个 Hello World 程序，并简单介绍 VSCode 中的插件 Code Runner 快速执行 `.c`、`.cpp` 文件的基本原理。

## 3. 🤔 推荐用什么 IDE？

目前（26.02）使用的代码编辑器是 VSCode，有需要的可自行前往 [VSCode 官网][1] 下载。

## 4. 🤔 需要安装哪些 VSCode 插件？

- [C/C++ (Microsoft) 插件][2]
- [Code Runner 插件][3]

## 5. 🤔 如何在 macOS 上初始化 C、C++ 的基本学习环境？

macOS 自带 clang 编译器，可直接使用：

```bash
# 检查是否已安装：
gcc --version
# 或：
clang --version

# 输出示例：
gcc --version
# Apple clang version 15.0.0 (clang-1500.3.9.4)
# Target: arm64-apple-darwin24.5.0
# Thread model: posix
# InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

clang --version
# Apple clang version 15.0.0 (clang-1500.3.9.4)
# Target: arm64-apple-darwin24.5.0
# Thread model: posix
# InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

选中需要运行的文件，然后右键，选择 `Run Code` 即可运行 C、C++ 程序。

![img](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-01-23-59-31.png)

输出：

![img](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-00-05.png)

## 6. 🤔 Code Runner 一键运行 C、CPP 的原理是？

打开 VSCode 设置，搜索 `code-runner.executorMap`，在配置文件中查看 `code-runner.executorMap` 的配置。

::: swiper

![1](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-17-43.png)

![2](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-17-50.png)

:::

`"c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt"`

1. `cd $dir` 切换到对应的目录；
2. `&& gcc $fileName -o $fileNameWithoutExt` 使用 gcc 对指定文件进行编译，并且输出的可执行文件名称和当前运行的文件名称相同；
3. `&& $dir$fileNameWithoutExt` 运行编译后的可执行文件；

由此可见，如果你本地没有安装 gcc 编译器，那么 Code Runner 插件就无法正常工作。

## 7. 🤔 如何让 git 忽略编译产物？

::: info 备注

这主要是开源的 TNotes.c-cpp 需要解决的一个问题 —— 避免将不必要的编译产物推送到仓库中，和 C、C++ 的学习环境搭建没有直接关系。

:::

可以通过编写 `.vscode/settings.json` 文件，配置 Code Runner 插件的编译输出目录为 `build`，然后在项目根目录下创建 `.gitignore` 文件，添加配置 `notes/**/demos/**/build`。

VSCode Code Runner 插件配置示例：

```json
{
  "code-runner.executorMap": {
    "c": "cd $dir && mkdir -p build && gcc $fileName -o build/$fileNameWithoutExt && $dir/build/$fileNameWithoutExt",
    "cpp": "cd $dir && mkdir -p build && g++ $fileName -o build/$fileNameWithoutExt && $dir/build/$fileNameWithoutExt"
  },
  "code-runner.preserveFocus": false,
  "code-runner.clearPreviousOutput": true
}
```

配置解释：

- `cd $dir`：切换到当前文件所在目录
- `mkdir -p build`：创建 build 目录用于存放编译产物，-p 参数确保目录存在时不报错
- `gcc $fileName -o build/$fileNameWithoutExt`：编译源文件并将可执行文件输出到 build 目录
- `$fileNameWithoutExt`：获取不含扩展名的文件名作为可执行文件名
- `$dir/build/$fileNameWithoutExt`：运行 build 目录下的可执行文件

这样配置后，所有编译产物都会保存在 build 目录中，便于 Git 忽略，保持源码目录整洁。

---

或者更直接一些：

```json
{
  "code-runner.executorMap": {
    "c": "cd $dir && gcc $fileName -o temp_executable && ./temp_executable && rm temp_executable",
    "cpp": "cd $dir && g++ $fileName -o temp_executable && ./temp_executable && rm temp_executable"
  }
}
```

配置解释：

- `cd $dir`：切换到当前文件所在目录
- `gcc $fileName -o temp_executable`：编译源文件并将可执行文件输出为临时可执行文件
- `./temp_executable`：运行临时可执行文件
- `rm temp_executable`：运行后删除临时可执行文件，保持目录整洁

## 8. 🤔 如何在 Windows 上初始化 C、C++ 的基本学习环境？

安装编译器（推荐两种方案）：

- 方案一：使用 MinGW-w64（简单推荐）
  1. 下载 [MinGW-w64][4]
  2. 安装时选择：
     - Architecture：x86_64
     - Threads：posix
     - Exception：seh
  3. 将 `mingw64\bin` 添加到系统 PATH
- 方案二：使用 MSYS2（功能更全）

```bash
# 安装后运行
pacman -Syu
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

验证编译器是否安装完成：

```bash
gcc --version
```

## 9. 💻 demos.1 - 第一个 C 语言程序 - 在控制台打印 Hello World

::: code-group

<<< ./demos/1/1.c

<<< ./demos/1/1.cpp

:::

使用 Code Runner 运行：

![img](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-23-53.png)

通过输出内容分析 Code Runner 的执行原理：

```shell
# cd "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\" && gcc 1.c -o 1 && "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"1
# Hello World

# "c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt"
# 相当于执行了下面这些命令：
# 1. cd "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"
# 2. gcc 1.c -o 1
# 3. "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"1
```

在理解了原理之后，其实我们也可以手动输入执行命令：

::: swiper

![1](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-24-55.png)

![2](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-25-05.png)

![3](https://cdn.jsdelivr.net/gh/tnotesjs/imgs-2026@main/2026-02-02-00-25-12.png)

:::

也可以直接合并：

- powershell 环境：`gcc 1.c -o 1; .\1`
- CMD 环境：`gcc 1.c -o 1 && .\1`
- 后缀 .exe 可加可不加

## 10. 🔗 引用

- [VSCode 官网][1]
- [C/C++ (Microsoft) 插件][2]
- [Code Runner 插件][3]
- [Mingw-w64][4]

[1]: https://code.visualstudio.com/
[2]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
[3]: https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner
[4]: https://sourceforge.net/projects/mingw-w64/
