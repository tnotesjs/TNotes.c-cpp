# [0001. 搭建 C、C++ 本地学习环境](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0001.%20%E6%90%AD%E5%BB%BA%20C%E3%80%81C%2B%2B%20%E6%9C%AC%E5%9C%B0%E5%AD%A6%E4%B9%A0%E7%8E%AF%E5%A2%83)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 推荐用什么 IDE？](#3--推荐用什么-ide)
- [4. 🤔 需要安装哪些 VSCode 插件？](#4--需要安装哪些-vscode-插件)
- [5. 🤔 如何在 macOS 上初始化 C、C++ 的基本学习环境？](#5--如何在-macos-上初始化-cc-的基本学习环境)
- [6. 🤔 如何在 win11 上初始化 C、C++ 的基本学习环境？](#6--如何在-win11-上初始化-cc-的基本学习环境)
  - [6.1. 基础学习环境简介](#61-基础学习环境简介)
  - [6.2. 安装 GCC](#62-安装-gcc)
  - [6.3. 🤔 安装 GCC 有什么用？](#63--安装-gcc-有什么用)
  - [6.4. 📒 安装 VSCode 插件 - Code Runner](#64--安装-vscode-插件---code-runner)
- [7. 📒 查看 Code Runner 运行 C、CPP 的基本原理](#7--查看-code-runner-运行-ccpp-的基本原理)
- [8. 💻 demos.1 - 第一个 C 语言程序 - 在控制台打印 Hello World](#8--demos1---第一个-c-语言程序---在控制台打印-hello-world)
- [9. 🔗 引用](#9--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 在 macOS 和 windows 上搭建 C、C++ 的基本学习环境

## 2. 🫧 评价

这篇笔记主要介绍如何在 Windows 和 macOS 上搭建 C、C++ 的基本学习环境，确保在学习语法及语言核心特性阶段所编写的程序可以编译并执行。

环境搭建完成后，编写第一个 Hello World 程序，并理解 VSCode 中的插件 Code Runner 快速执行 `.c`、`.cpp` 文件的基本原理。

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

如果需要更新或安装完整工具链，建议安装 Homebrew 后安装：

```bash
# 安装 Homebrew（如未安装）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装 GCC 和 Make
brew install gcc make
```

## 6. 🤔 如何在 win11 上初始化 C、C++ 的基本学习环境？

- 安装好 GCC 和 VSCode 插件 - Code Runner；
- 编写一个 Hello World 程序，并理解 Code Runner 快速执行 .c 文件的基本原理；

### 6.1. 基础学习环境简介

- 编译器：GCC
- IDE：VSCode
- 目标：能够运行 C、CPP 程序，学习 C、CPP 语言层面的一些基础知识

### 6.2. 安装 GCC

- https://blog.csdn.net/weixin_64064486/article/details/123940266
  - CSDN 文章 - Windows 下 GCC 安装和使用
  - 作者：丸子爱学习！
  - 跟着文章中提到的步骤完成 GCC 的安装即可。
  - 注意：
    - 环境变量配置好以后，需要重启 VSCode。
    - VS Code 启动后会缓存系统环境变量，如果你在配置了 `PATH` 环境变量后没有重新启动 VS Code，它可能仍然无法找到 `gcc`。
- https://sourceforge.net/projects/mingw/files/
  - mingw 下载链接

### 6.3. 🤔 安装 GCC 有什么用？

- 这玩意儿是 C、C++ 的编译器，C、C++ 和 JS、Python 这些解释型语言不同，是需要编译成可执行程序（`.exe` 文件）执行的，简单来说 GCC 是用来跑咱们写的 C、C++ 的 demo 的。

### 6.4. 📒 安装 VSCode 插件 - Code Runner

- ![img](assets/2025-01-21-11-22-51.png)
- 为了跟方便地直接在 VSCode 中运行 C 程序。
- 注意：
  - 如果不安装 GCC 的话，这玩意儿是没法直接用来跑 C、CPP 程序的。

## 7. 📒 查看 Code Runner 运行 C、CPP 的基本原理

- 打开 VSCode 设置，搜索 `code-runner.executorMap`，在配置文件中查看 `code-runner.executorMap` 的配置。
  - ![img](assets/2025-01-21-11-33-53.png)
  - ![img](assets/2025-01-21-11-34-45.png)
- `"c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt"`
  1. `cd $dir` 切换到对应的目录；
  2. `&& gcc $fileName -o $fileNameWithoutExt` 使用 gcc 对指定文件进行编译，并且输出的可执行文件名称和当前运行的文件名称相同；
  3. `&& $dir$fileNameWithoutExt` 运行编译后的可执行文件；

## 8. 💻 demos.1 - 第一个 C 语言程序 - 在控制台打印 Hello World

```c
#include <stdio.h>

int main()
{
  printf("Hello World\n");
  return 0;
}
```

- 运行：
  - ![img](assets/2025-01-21-13-07-22.png)
- 分析 code runner 的执行原理：

```shell
# cd "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\" && gcc 1.c -o 1 && "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"1
# Hello World

# "c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt"
# 相当于执行了下面这些命令：
# 1. cd "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"
# 2. gcc 1.c -o 1
# 3. "c:\Users\Tdahuyou\Desktop\notes\c-cpp\0001. 在 win11 上搭建 C 的基本学习环境\demos\1\"1
```

- 在理解了原理之后，其实我们也可以手动输入执行命令：

::: swiper

![1](./assets/2025-01-21-13-13-26.png)

![2](./assets/2025-01-21-13-19-06.png)

![3](./assets/2025-01-21-13-22-18.png)

:::

- 也可以直接合并：
  - powershell 环境：`gcc 1.c -o 1; .\1`
  - CMD 环境：`gcc 1.c -o 1 && .\1`
  - 后缀 .exe 可加可不加

## 9. 🔗 引用

- [VSCode 官网][1]
- [C/C++ (Microsoft) 插件][2]
- [Code Runner 插件][3]

[1]: https://code.visualstudio.com/
[2]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
[3]: https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner
