# [0026. 二重指针](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0026.%20%E4%BA%8C%E9%87%8D%E6%8C%87%E9%92%88)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是二重指针？它的声明和语义是什么？](#3--什么是二重指针它的声明和语义是什么)
- [4. 🤔 为什么需要二重指针？典型使用场景有哪些？](#4--为什么需要二重指针典型使用场景有哪些)
- [5. 🤔 如何通过二重指针在函数中修改指针本身？](#5--如何通过二重指针在函数中修改指针本身)
- [6. 🆚 传入指针 vs 传入二重指针：对函数内修改的影响有何不同？](#6--传入指针-vs-传入二重指针对函数内修改的影响有何不同)
- [7. 🤔 二重指针如何用于动态分配二维数组？常见布局有哪些？](#7--二重指针如何用于动态分配二维数组常见布局有哪些)
  - [7.1. 指针数组加多次分配](#71-指针数组加多次分配)
  - [7.2. 连续内存加行指针](#72-连续内存加行指针)
- [8. 🤔 二重指针在命令行参数 `char *argv[]` 中如何体现？它与指针数组有何关系？](#8--二重指针在命令行参数-char-argv-中如何体现它与指针数组有何关系)
- [9. 🤔 常见的二重指针误用和陷阱有哪些？如何安全地解引用二重指针？](#9--常见的二重指针误用和陷阱有哪些如何安全地解引用二重指针)
- [10. 🤔 二重指针与多级指针（如三重指针）有何关系？](#10--二重指针与多级指针如三重指针有何关系)
- [11. 🤔 如何用二重指针实现链表的头节点修改？](#11--如何用二重指针实现链表的头节点修改)
- [12. 🆚 二重指针 vs 指向数组的指针：语法和用途有何不同？](#12--二重指针-vs-指向数组的指针语法和用途有何不同)
- [13. 🤔 为什么某些系统 API（如 POSIX）使用二重指针作为输出参数？](#13--为什么某些系统-api如-posix使用二重指针作为输出参数)
- [14. 🔗 引用](#14--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 二重指针声明与语义
- 二重指针修改指针本身
- 指针传参与二重指针传参对比
- 动态二维数组的两种布局
- `argv` 与指针数组
- 二重指针安全解引用与常见陷阱
- 多级指针与链表头节点修改
- 二重指针与指向数组的指针对比

## 2. 🫧 评价

- 二重指针的语法不复杂，复杂的是你要清楚你在改谁的值，是改数据还是改指针本身。
- 一旦涉及动态二维数组或链表，二重指针几乎是绕不开的工具，但同样也会放大生命周期与边界错误。

## 3. 🤔 什么是二重指针？它的声明和语义是什么？

二重指针就是指向指针的指针。

如果 `int *p` 是指向 `int` 的指针，那么 `int **pp` 就是指向 `int*` 的指针。

你可以把它理解为两层间接访问：

- `pp` 保存的是 `p` 的地址
- `*pp` 得到的是 `p`
- `**pp` 得到的是 `p` 指向的 `int`

::: code-group

```c [二重指针的层级关系]
#include <stdio.h>

int main(void) {
  int x = 10;
  int *p = &x;
  int **pp = &p;

  printf("x=%d\n", x);
  printf("*p=%d\n", *p);
  printf("**pp=%d\n", **pp);
  return 0;
}
```

:::

二重指针的核心价值是：它让你可以在函数内部修改调用者的指针变量。

## 4. 🤔 为什么需要二重指针？典型使用场景有哪些？

典型场景主要有这几类。

1. 函数需要为调用者分配内存并返回指针
2. 函数需要把指针改为指向别处，例如重定向缓冲区
3. 修改链表头指针
4. 表达字符串数组或参数列表，例如 `argv`

::: code-group

```c [输出参数分配内存]
#include <stdlib.h>

static int alloc_int(int **out) {
  if (out == NULL) {
    return 0;
  }

  *out = (int*)malloc(sizeof(int));
  if (*out == NULL) {
    return 0;
  }

  **out = 123;
  return 1;
}
```

:::

只要你想让函数改变调用者手里的那个指针变量本身，二重指针就会出现。

## 5. 🤔 如何通过二重指针在函数中修改指针本身？

思路是把指针变量的地址传进去。

下面用一个最小例子说明。

::: code-group

```c [修改指针本身]
#include <stdio.h>

static void redirect(int **pp, int *target) {
  if (pp == NULL) {
    return;
  }
  *pp = target;
}

int main(void) {
  int a = 1;
  int b = 2;
  int *p = &a;

  printf("%d\n", *p);
  redirect(&p, &b);
  printf("%d\n", *p);
  return 0;
}
```

:::

注意：如果你只把 `p` 传进去，函数里改的是副本，调用者的 `p` 不会变。

## 6. 🆚 传入指针 vs 传入二重指针：对函数内修改的影响有何不同？

核心差异在于你能不能修改调用者的指针变量。

| 特性                 | 传入指针 `T*`    | 传入二重指针 `T**`             |
| -------------------- | ---------------- | ------------------------------ |
| 修改 `*p` 指向的数据 | 可以             | 可以                           |
| 修改 `p` 的指向      | 不行，改的是副本 | 可以，改的是调用者的 `p`       |
| 常见用途             | 读写数据缓冲区   | 输出参数、重定向指针、改头指针 |

::: code-group

```c [传入指针无法修改调用者指向]
#include <stdio.h>

static void wrong_redirect(int *p, int *target) {
  p = target;
}

static void right_redirect(int **pp, int *target) {
  if (pp == NULL) {
    return;
  }
  *pp = target;
}

int main(void) {
  int a = 1;
  int b = 2;
  int *p = &a;

  wrong_redirect(p, &b);
  printf("%d\n", *p);

  right_redirect(&p, &b);
  printf("%d\n", *p);
  return 0;
}
```

:::

这个对比是理解二重指针最关键的一步。

## 7. 🤔 二重指针如何用于动态分配二维数组？常见布局有哪些？

动态二维数组最常见的两种布局是：

1. 指针数组加多次分配，也就是每行单独 `malloc`
2. 一次性分配连续大块内存，然后建立行指针

### 7.1. 指针数组加多次分配

这种方式直观，缺点是内存不连续，分配与释放需要循环。

::: code-group

```c [指针数组形式的二维数组]
#include <stdlib.h>

static int alloc_matrix_rows(int rows, int cols, int ***out) {
  if (out == NULL || rows <= 0 || cols <= 0) {
    return 0;
  }

  int **m = (int**)malloc((size_t)rows * sizeof(int*));
  if (m == NULL) {
    return 0;
  }

  for (int i = 0; i < rows; i++) {
    m[i] = (int*)malloc((size_t)cols * sizeof(int));
    if (m[i] == NULL) {
      for (int k = 0; k < i; k++) {
        free(m[k]);
      }
      free(m);
      return 0;
    }
  }

  *out = m;
  return 1;
}

static void free_matrix_rows(int rows, int **m) {
  if (m == NULL) {
    return;
  }
  for (int i = 0; i < rows; i++) {
    free(m[i]);
  }
  free(m);
}
```

:::

### 7.2. 连续内存加行指针

这种方式数据连续，缓存友好，释放也更简单。

::: code-group

```c [连续内存形式的二维数组]
#include <stdlib.h>

static int alloc_matrix_contig(int rows, int cols, int ***out) {
  if (out == NULL || rows <= 0 || cols <= 0) {
    return 0;
  }

  int **m = (int**)malloc((size_t)rows * sizeof(int*));
  if (m == NULL) {
    return 0;
  }

  int *data = (int*)malloc((size_t)rows * (size_t)cols * sizeof(int));
  if (data == NULL) {
    free(m);
    return 0;
  }

  for (int i = 0; i < rows; i++) {
    m[i] = data + (size_t)i * (size_t)cols;
  }

  *out = m;
  return 1;
}

static void free_matrix_contig(int **m) {
  if (m == NULL) {
    return;
  }
  free(m[0]);
  free(m);
}
```

:::

⚠️ 两种布局都需要二重指针，是因为你要把分配得到的 `int**` 传回给调用者。

## 8. 🤔 二重指针在命令行参数 `char *argv[]` 中如何体现？它与指针数组有何关系？

在 `main` 的常见签名里：

```c
int main(int argc, char *argv[])
```

`argv` 是一个指针数组，每个元素是 `char*`，指向一个字符串。

在函数形参位置，`char *argv[]` 会退化成 `char **argv`。

因此下面两种写法在参数层面等价。

::: code-group

```c [argv 的两种等价形参写法]
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return 0;
}

int main2(int argc, char **argv) {
  (void)argc;
  (void)argv;
  return 0;
}
```

:::

`argv[i]` 是第 `i` 个字符串的指针，`argv[i][j]` 是该字符串的第 `j` 个字符。

::: code-group

```c [argv 的下标访问]
#include <stdio.h>

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  return 0;
}
```

:::

## 9. 🤔 常见的二重指针误用和陷阱有哪些？如何安全地解引用二重指针？

最常见的坑有：

1. 只检查 `pp`，不检查 `*pp`
2. 把不匹配的类型强转成 `T**`
3. 忘记初始化输出指针，导致调用者误用旧值
4. 动态二维数组释放顺序错误

安全解引用的基本套路是分层判空。

::: code-group

```c [分层判空再解引用]
#include <stdio.h>

static void print_int_pp(int **pp) {
  if (pp == NULL) {
    return;
  }
  if (*pp == NULL) {
    return;
  }
  printf("%d\n", **pp);
}
```

:::

⚠️ 另外一个容易误用的点是把二维数组当成 `int**`。

::: code-group

```c [❌ 二维数组不是 int**]
static void f(int **pp) {
  (void)pp;
}

int main(void) {
  int a[2][3] = { 0 };
  /* ❌ 类型不匹配 */
  /* f((int**)a); */
  return 0;
}
```

:::

这里的关键是类型系统，不要用强转掩盖错误。

## 10. 🤔 二重指针与多级指针（如三重指针）有何关系？

多级指针就是在二重指针基础上继续加一层间接。

例如 `char ***p` 常用于表示一个可变的字符串数组的地址，或者一个数组的地址再被输出参数包装一层。

实际开发中不鼓励滥用多级指针。

- 如果层级超过两层，经常意味着抽象不清
- 很多场景可以用结构体把相关信息打包起来

但你仍然需要能读懂它。

::: code-group

```c [多级指针的读法]
/* p 是指向 char** 的指针 */
char ***p;
```

:::

## 11. 🤔 如何用二重指针实现链表的头节点修改？

链表头指针在插入与删除时经常会变化。

如果你希望一个函数完成头插并让调用者的 `head` 更新，通常需要二重指针。

::: code-group

```c [用二重指针实现头插]
#include <stdlib.h>

struct Node {
  int value;
  struct Node *next;
};

static int push_front(struct Node **head, int v) {
  if (head == NULL) {
    return 0;
  }

  struct Node *n = (struct Node*)malloc(sizeof(struct Node));
  if (n == NULL) {
    return 0;
  }

  n->value = v;
  n->next = *head;
  *head = n;
  return 1;
}
```

:::

`head` 是调用者变量的地址，`*head` 才是当前链表头指针。

## 12. 🆚 二重指针 vs 指向数组的指针：语法和用途有何不同？

二重指针和指向数组的指针都长得像带两个星号或带括号的复杂类型，但它们表达的对象完全不同。

| 特性         | 二重指针 `T**`                   | 指向数组的指针 `T (*)[N]`  |
| ------------ | -------------------------------- | -------------------------- |
| 指向对象     | 一个指针变量                     | 一个数组对象               |
| 典型用途     | 输出参数、改指针本身、链表头操作 | 二维数组传参、保持列数信息 |
| 内存布局假设 | 不保证连续                       | 约定按数组布局连续         |
| 常见误区     | 以为可以接收二维数组             | 以为可以当成 `T**`         |

::: code-group

```c [指向数组的指针用于二维数组]
#include <stdio.h>

static int sum2x3(const int (*a)[3]) {
  int s = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      s += a[i][j];
    }
  }
  return s;
}

int main(void) {
  int m[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
  printf("%d\n", sum2x3(m));
  return 0;
}
```

:::

如果你要传二维数组，优先考虑 `T (*)[cols]` 或 C99 变长数组，而不是 `T**`。

## 13. 🤔 为什么某些系统 API（如 POSIX）使用二重指针作为输出参数？

系统 API 使用二重指针作为输出参数通常是为了两件事。

1. 在函数内部创建或获取资源，把指针返回给调用者
2. 让调用者明确自己需要负责释放资源

这种设计还能把错误处理集中在返回值上，例如返回 0 或 -1，资源通过输出参数返回。

::: code-group

```c [输出参数模式]
#include <stdlib.h>

static int make_buf(int n, char **out) {
  if (out == NULL || n <= 0) {
    return 0;
  }

  char *p = (char*)malloc((size_t)n);
  if (p == NULL) {
    return 0;
  }

  *out = p;
  return 1;
}
```

:::

⚠️ 这种模式的关键是文档必须写清楚所有权与释放方式，否则调用者很容易泄漏或重复释放。

## 14. 🔗 引用

- [cppreference：Pointers][1]
- [cppreference：Array to pointer decay][2]
- [cppreference：Pointer arithmetic][3]
- [cppreference：malloc][4]
- [cppreference：free][5]

[1]: https://en.cppreference.com/w/c/language/pointer
[2]: https://en.cppreference.com/w/c/language/array#Array_to_pointer_decay
[3]: https://en.cppreference.com/w/c/language/operator_arithmetic#Pointer_arithmetic
[4]: https://en.cppreference.com/w/c/memory/malloc
[5]: https://en.cppreference.com/w/c/memory/free
