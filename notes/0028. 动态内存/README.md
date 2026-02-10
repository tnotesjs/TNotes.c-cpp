# [0028. 动态内存](https://github.com/tnotesjs/TNotes.c/tree/main/notes/0028.%20%E5%8A%A8%E6%80%81%E5%86%85%E5%AD%98)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是动态内存？它与静态/自动内存有何区别？](#3--什么是动态内存它与静态自动内存有何区别)
- [4. 🤔 如何使用 malloc 和 free 分配与释放内存？](#4--如何使用-malloc-和-free-分配与释放内存)
- [5. 🤔 calloc 和 malloc 有什么不同？何时使用 calloc？](#5--calloc-和-malloc-有什么不同何时使用-calloc)
- [6. 🤔 realloc 的作用是什么？如何安全地调整已分配内存大小？](#6--realloc-的作用是什么如何安全地调整已分配内存大小)
- [7. 🆚 malloc vs calloc vs realloc：功能和使用场景如何区分？](#7--malloc-vs-calloc-vs-realloc功能和使用场景如何区分)
- [8. 🤔 什么是内存泄漏？如何避免和检测？](#8--什么是内存泄漏如何避免和检测)
- [9. 🤔 什么是野指针和悬空指针？它们与动态内存有何关联？](#9--什么是野指针和悬空指针它们与动态内存有何关联)
- [10. 🤔 动态分配结构体或数组时需要注意什么？](#10--动态分配结构体或数组时需要注意什么)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 动态内存与生命周期
- `malloc` 与 `free`
- `calloc` 与零初始化
- `realloc` 安全扩容与缩容
- 内存泄漏与资源清理套路
- 野指针与悬空指针
- 动态数组与结构体分配要点

## 2. 🫧 评价

- 动态内存的关键不是会用 `malloc`，而是能把所有权、生命周期、异常路径的清理逻辑写清楚。
- 只要你开始手动管理堆内存，就要默认自己会犯错，用规范的分配与释放模式降低出错概率。

## 3. 🤔 什么是动态内存？它与静态/自动内存有何区别？

动态内存通常指堆上分配的内存，通过 `malloc`、`calloc`、`realloc` 获取，通过 `free` 释放。

它与静态内存和自动内存的核心差异是生命周期与管理方式。

- 静态内存：全局变量、静态局部变量，程序启动时创建，进程结束时释放
- 自动内存：函数栈上的局部变量，进入作用域创建，离开作用域自动释放
- 动态内存：由你显式分配与释放，生命周期跨作用域，灵活但更容易出错

动态内存的典型用途包括。

- 运行时才能确定大小的数组或缓冲区
- 需要跨函数返回或跨模块共享的数据
- 链表、树、哈希表等动态数据结构

## 4. 🤔 如何使用 malloc 和 free 分配与释放内存？

使用 `malloc` 分配内存的要点。

1. 用 `sizeof *p` 写法避免类型改动导致的错配
2. 用 `size_t` 表示字节数与元素数量
3. 分配失败时 `malloc` 返回 `NULL`，必须检查
4. 用完后调用 `free` 释放，并把指针置为 `NULL` 减少悬空指针风险

::: code-group

```c [malloc 与 free 的基本用法]
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  size_t n = 5;
  int *a = malloc(n * sizeof *a);
  if (a == NULL) {
    return 1;
  }

  for (size_t i = 0; i < n; i++) {
    a[i] = (int)(i + 1);
  }

  for (size_t i = 0; i < n; i++) {
    printf("%d\n", a[i]);
  }

  free(a);
  a = NULL;
  return 0;
}
```

```c [❌ 释放后继续使用会产生悬空指针]
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof *p);
  if (p == NULL) {
    return 1;
  }

  free(p);
  /* ❌ p 变成悬空指针 */
  /* *p = 1; */
  return 0;
}
```

:::

⚠️ `free(NULL)` 是安全的，但重复释放同一块内存是未定义行为。

## 5. 🤔 calloc 和 malloc 有什么不同？何时使用 calloc？

`calloc` 的特点是分配并把内存清零。

如果你需要一个全部为零的数组或结构体数组，`calloc` 往往更直接。

::: code-group

```c [calloc 分配并清零]
#include <stdlib.h>

int main(void) {
  size_t n = 10;
  int *a = calloc(n, sizeof *a);
  if (a == NULL) {
    return 1;
  }

  free(a);
  return 0;
}
```

:::

⚠️ 清零不等于初始化为有效业务值，只是把字节置零。

## 6. 🤔 realloc 的作用是什么？如何安全地调整已分配内存大小？

`realloc` 用于调整已分配内存的大小。

- 扩容时可能搬家到新地址
- 缩容时可能返回同一地址
- 失败时返回 `NULL`，并且原指针仍然有效

安全用法的关键是用临时指针接住返回值，成功后再更新原指针。

::: code-group

```c [✅ 安全的 realloc 用法]
#include <stdlib.h>

static int grow_int_array(int **a, size_t old_n, size_t new_n) {
  (void)old_n;
  if (a == NULL) {
    return 0;
  }
  if (new_n == 0) {
    free(*a);
    *a = NULL;
    return 1;
  }

  int *tmp = realloc(*a, new_n * sizeof **a);
  if (tmp == NULL) {
    return 0;
  }

  *a = tmp;
  return 1;
}
```

```c [❌ 直接覆盖原指针会导致泄漏]
#include <stdlib.h>

int main(void) {
  int *p = malloc(4);
  if (p == NULL) {
    return 1;
  }

  /* ❌ realloc 失败返回 NULL，原地址丢失，发生泄漏 */
  /* p = realloc(p, 8); */

  free(p);
  return 0;
}
```

:::

## 7. 🆚 malloc vs calloc vs realloc：功能和使用场景如何区分？

| 特性 | `malloc` | `calloc` | `realloc` |
| --- | --- | --- | --- |
| 作用 | 分配内存 | 分配并清零 | 调整已分配内存大小 |
| 是否清零 | 否 | 是 | 不保证 |
| 典型场景 | 普通缓冲区、结构体 | 数组、需要零值默认状态 | 动态数组增长、缓冲区扩容 |
| 失败返回 | `NULL` | `NULL` | `NULL` 且原指针仍有效 |

选型的简单经验。

- 默认用 `malloc`
- 明确需要零字节初始化时用 `calloc`
- 需要增长或缩小已分配缓冲区时用 `realloc`，并遵循临时指针模式

## 8. 🤔 什么是内存泄漏？如何避免和检测？

内存泄漏是指分配的动态内存丢失了最后一个可达指针，导致无法释放。

常见原因。

1. `malloc` 后忘记 `free`
2. 多处返回路径，某条路径漏掉清理
3. `realloc` 错误用法覆盖原指针
4. 深层结构只释放外层，内部成员未释放

避免泄漏的实用套路是统一出口清理。

::: code-group

```c [统一出口清理]
#include <stdlib.h>

static int work(void) {
  int ok = 0;
  char *buf = NULL;
  int *a = NULL;

  buf = malloc(128);
  if (buf == NULL) {
    goto cleanup;
  }

  a = malloc(10 * sizeof *a);
  if (a == NULL) {
    goto cleanup;
  }

  ok = 1;

cleanup:
  free(a);
  free(buf);
  return ok;
}
```

:::

检测手段通常包括。

- 使用内存检查工具，例如 Valgrind
- 开启编译器运行时检查，例如 AddressSanitizer

## 9. 🤔 什么是野指针和悬空指针？它们与动态内存有何关联？

野指针通常是指未初始化的指针，值是随机的。

悬空指针是指指向的内存已经释放，但指针仍然保留旧地址。

两者都可能在解引用时触发崩溃或数据破坏。

::: code-group

```c [❌ 野指针]
int main(void) {
  int *p;
  /* ❌ 未初始化就使用 */
  /* *p = 1; */
  return 0;
}
```

```c [✅ 释放后置空减少悬空指针误用]
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof *p);
  if (p == NULL) {
    return 1;
  }
  free(p);
  p = NULL;
  return 0;
}
```

:::

## 10. 🤔 动态分配结构体或数组时需要注意什么？

动态分配结构体或数组的常见注意点。

1. 使用 `sizeof *p` 避免写错类型大小
2. 注意元素数量乘法的溢出风险
3. 结构体里如果还有动态成员，需要做深度释放

::: code-group

```c [动态分配结构体]
#include <stdlib.h>

struct User {
  int id;
  int age;
};

int main(void) {
  struct User *u = malloc(sizeof *u);
  if (u == NULL) {
    return 1;
  }

  u->id = 1001;
  u->age = 18;

  free(u);
  u = NULL;
  return 0;
}
```

```c [动态分配数组并检查溢出]
#include <stdlib.h>
#include <stdint.h>

static int alloc_ints(size_t n, int **out) {
  if (out == NULL) {
    return 0;
  }
  if (n > SIZE_MAX / sizeof **out) {
    return 0;
  }

  int *a = malloc(n * sizeof *a);
  if (a == NULL) {
    return 0;
  }

  *out = a;
  return 1;
}
```

:::

⚠️ 释放策略建议写成成对的接口，例如 `create` 搭配 `destroy`，避免调用者记不清释放顺序。

## 11. 🔗 引用

- [cppreference：malloc][1]
- [cppreference：calloc][2]
- [cppreference：realloc][3]
- [cppreference：free][4]
- [Valgrind Documentation][5]
- [Clang AddressSanitizer][6]

[1]: https://en.cppreference.com/w/c/memory/malloc
[2]: https://en.cppreference.com/w/c/memory/calloc
[3]: https://en.cppreference.com/w/c/memory/realloc
[4]: https://en.cppreference.com/w/c/memory/free
[5]: https://valgrind.org/docs/
[6]: https://clang.llvm.org/docs/AddressSanitizer.html
