# [0031. 函数参数](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0031.%20%E5%87%BD%E6%95%B0%E5%8F%82%E6%95%B0)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 C 语言中函数参数是如何传递的？](#3--c-语言中函数参数是如何传递的)
- [4. 🤔 什么是按值传递？它对实参和形参有何影响？](#4--什么是按值传递它对实参和形参有何影响)
- [5. 🤔 为什么不能直接通过形参修改调用者的变量？](#5--为什么不能直接通过形参修改调用者的变量)
- [6. 🆚 传递普通变量 vs 传递指针作为参数：效果有何不同？](#6--传递普通变量-vs-传递指针作为参数效果有何不同)
- [7. 🤔 如何通过参数让函数返回多个值？](#7--如何通过参数让函数返回多个值)
- [8. 🤔 数组作为函数参数时会发生什么？为什么长度信息会丢失？](#8--数组作为函数参数时会发生什么为什么长度信息会丢失)
- [9. 🤔 什么是 const 参数？它如何提升函数的安全性？](#9--什么是-const-参数它如何提升函数的安全性)
- [10. 🤔 可变参数函数（如 printf）是如何实现的？](#10--可变参数函数如-printf是如何实现的)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 按值传递的语义
- 指针作为参数的效果
- 输出参数返回多个值
- 数组形参退化与长度传递
- `const` 参数与只读约束
- 可变参数与 `stdarg` 基础

## 2. 🫧 评价

- C 语言参数传递只有按值传递这一种语义，但指针会让很多人产生按引用的错觉。
- 只要你把输入输出约定写清楚，并固定输出参数与长度参数的模式，函数接口会更稳更不易误用。

## 3. 🤔 C 语言中函数参数是如何传递的？

C 语言的函数参数传递只有一种。

- 按值传递

也就是把实参的值拷贝给形参。

如果实参是指针，那么被拷贝的是地址值。

::: code-group

```c [按值传递的直观例子]
#include <stdio.h>

static void inc(int x) {
  x += 1;
}

int main(void) {
  int a = 10;
  inc(a);
  printf("%d\n", a);
  return 0;
}
```

:::

上例里 `a` 不会改变，因为 `inc` 里改的是形参副本。

## 4. 🤔 什么是按值传递？它对实参和形参有何影响？

按值传递的关键影响是两个。

1. 函数内部对形参的修改不会影响调用者的实参变量
2. 如果形参是大对象，拷贝成本可能比较高

在 C 语言里，大对象通常指结构体。

::: code-group

```c [结构体按值传递会拷贝]
struct Big {
  int a[1024];
};

static int sum_first(struct Big b) {
  return b.a[0];
}
```

```c [用指针避免拷贝]
struct Big {
  int a[1024];
};

static int sum_first2(const struct Big *b) {
  if (b == NULL) {
    return 0;
  }
  return b->a[0];
}
```

:::

第二种写法更常见，因为它清晰地表达了。

- 不想拷贝
- 允许参数为空
- 不修改输入

## 5. 🤔 为什么不能直接通过形参修改调用者的变量？

因为形参只是实参的副本。

你在函数里写 `x = 123`，修改的是副本 `x`。

要修改调用者的变量，你需要让函数拿到这个变量的地址。

::: code-group

```c [❌ 修改副本]
static void set_to_100_wrong(int x) {
  x = 100;
}
```

```c [通过地址修改调用者变量]
static void set_to_100(int *x) {
  if (x == NULL) {
    return;
  }
  *x = 100;
}
```

:::

⚠️ 如果你要修改调用者的指针变量本身，而不是修改它指向的数据，那么需要二重指针。

## 6. 🆚 传递普通变量 vs 传递指针作为参数：效果有何不同？

| 特性               | 传递普通变量 | 传递指针                     |
| ------------------ | ------------ | ---------------------------- |
| 传递内容           | 数据副本     | 地址副本                     |
| 能否修改调用者变量 | 不行         | 可以修改指向的数据           |
| 是否需要判空       | 不需要       | 通常需要                     |
| 常见用途           | 纯计算       | 输出参数，修改对象，避免拷贝 |

::: code-group

```c [普通变量参数]
#include <stdio.h>

static int add1(int x) {
  x += 1;
  return x;
}

int main(void) {
  int a = 10;
  int b = add1(a);
  printf("%d %d\n", a, b);
  return 0;
}
```

```c [指针参数修改调用者数据]
#include <stdio.h>

static void add1_inplace(int *x) {
  if (x == NULL) {
    return;
  }
  *x += 1;
}

int main(void) {
  int a = 10;
  add1_inplace(&a);
  printf("%d\n", a);
  return 0;
}
```

:::

## 7. 🤔 如何通过参数让函数返回多个值？

最常见的做法是输出参数。

函数返回状态码，数据通过指针形参返回。

::: code-group

```c [输出参数返回多个值]
static int div_mod(int a, int b, int *q, int *r) {
  if (q == NULL || r == NULL || b == 0) {
    return 0;
  }
  *q = a / b;
  *r = a % b;
  return 1;
}
```

:::

如果返回的值很多，也可以把输出集中到结构体。

::: code-group

```c [返回结构体聚合结果]
struct DivResult {
  int q;
  int r;
  int ok;
};

static struct DivResult div_mod2(int a, int b) {
  struct DivResult res = { 0, 0, 0 };
  if (b == 0) {
    return res;
  }
  res.q = a / b;
  res.r = a % b;
  res.ok = 1;
  return res;
}
```

:::

两种方式都可以，输出参数更贴近 C 的传统接口风格。

## 8. 🤔 数组作为函数参数时会发生什么？为什么长度信息会丢失？

数组作为函数参数时，形参会退化为指针。

例如 `int a[]` 在形参位置等价于 `int *a`。

因此数组长度不会随参数自动传递。

::: code-group

```c [数组形参退化为指针]
#include <stdio.h>

static void f(int a[]) {
  printf("%zu\n", sizeof(a));
}

int main(void) {
  int x[10] = { 0 };
  f(x);
  return 0;
}
```

:::

上例里 `sizeof(a)` 得到的是指针大小，不是数组大小。

解决方式是显式传长度。

::: code-group

```c [数组加长度参数]
static int sum_ints(const int *a, size_t n) {
  if (a == NULL) {
    return 0;
  }
  int s = 0;
  for (size_t i = 0; i < n; i++) {
    s += a[i];
  }
  return s;
}
```

:::

⚠️ 这是 C 接口设计的基本模式。

- 缓冲区指针
- 缓冲区长度

## 9. 🤔 什么是 const 参数？它如何提升函数的安全性？

`const` 参数用于表达只读约束。

常见形式是把指针写成 `const T *`。

它带来的好处。

1. 编译器能阻止你在函数内意外修改输入数据
2. 调用者更容易理解函数不会改动传入对象
3. 接口更通用，例如可以接收常量对象或只读内存

::: code-group

```c [const 输入参数]
static int len_str(const char *s) {
  if (s == NULL) {
    return 0;
  }
  int n = 0;
  while (s[n] != '\0') {
    n++;
  }
  return n;
}
```

```c [❌ 试图修改 const 数据会报错]
static void bad(const char *s) {
  (void)s;
  /* ❌ s[0] = 'x'; */
}
```

:::

⚠️ `const` 只约束通过该标识符的写操作，不是安全魔法。

## 10. 🤔 可变参数函数（如 printf）是如何实现的？

可变参数函数依赖 `stdarg.h` 提供的宏。

- `va_list`
- `va_start`
- `va_arg`
- `va_end`

最常见的实现方式是。

1. 固定参数里有一个用来描述可变参数的格式信息
2. 函数内部按约定从可变参数列表里逐个取出值

::: code-group

```c [可变参数求和示例]
#include <stdarg.h>

static int sum_n(int n, ...) {
  va_list ap;
  va_start(ap, n);

  int s = 0;
  for (int i = 0; i < n; i++) {
    s += va_arg(ap, int);
  }

  va_end(ap);
  return s;
}
```

:::

⚠️ 可变参数的风险很高。

- 编译器无法检查参数类型是否匹配
- 取参类型写错就是未定义行为

能用结构体或数组加长度表达的场景，尽量不要用可变参数。

## 11. 🔗 引用

- [cppreference：Function call][1]
- [cppreference：Function declaration][2]
- [cppreference：Array to pointer decay][3]
- [cppreference：const type qualifier][4]
- [cppreference：Variadic arguments][5]

[1]: https://en.cppreference.com/w/c/language/function_call
[2]: https://en.cppreference.com/w/c/language/function_declaration
[3]: https://en.cppreference.com/w/c/language/array#Array_to_pointer_decay
[4]: https://en.cppreference.com/w/c/language/const
[5]: https://en.cppreference.com/w/c/variadic
