# [0018. 循环结构](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0018.%20%E5%BE%AA%E7%8E%AF%E7%BB%93%E6%9E%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是循环结构？](#3--什么是循环结构)
- [4. 🤔 for 循环的三个组成部分分别起什么作用？](#4--for-循环的三个组成部分分别起什么作用)
- [5. 🤔 while 和 do-while 循环的本质区别是什么？](#5--while-和-do-while-循环的本质区别是什么)
- [6. 🆚 for vs while：如何选择合适的循环形式？](#6--for-vs-while如何选择合适的循环形式)
- [7. 🤔 循环中使用 break 和 continue 会带来什么影响？](#7--循环中使用-break-和-continue-会带来什么影响)
- [8. 🤔 什么是无限循环？如何安全地编写和退出它？](#8--什么是无限循环如何安全地编写和退出它)
- [9. 🤔 嵌套循环的执行顺序和性能开销如何？](#9--嵌套循环的执行顺序和性能开销如何)
- [10. 🤔 循环变量的作用域和生命周期需要注意什么？](#10--循环变量的作用域和生命周期需要注意什么)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- `for`、`while`、`do-while` 的语义与选择
- `break`、`continue` 的控制效果
- 无限循环的安全写法与退出方式
- 嵌套循环的执行顺序与复杂度
- 循环变量作用域与常见坑

## 2. 🫧 评价

- 循环语法不难，难在边界与约定。
- 写循环时把不变量和退出条件先想清楚，代码会更短也更稳。
- 尽量用可读性更强的写法表达意图，例如用 `for` 表达“计数”，用 `while` 表达“直到条件满足”。

## 3. 🤔 什么是循环结构？

循环结构用于重复执行一段语句块，直到满足退出条件。

在 C 语言里常见的循环有三类：

- `for`：更适合“已知次数”或“按步进计数”。
- `while`：更适合“先判断条件再执行”，循环次数通常不固定。
- `do-while`：更适合“先执行一次再判断”，例如菜单交互或至少执行一次的重试逻辑。

工程上写循环时，优先把以下三件事说清楚：

- 循环不变量：每次迭代都应保持成立的事实。
- 退出条件：什么情况下必须停止。
- 状态推进：每次迭代如何让状态更接近退出条件。

## 4. 🤔 for 循环的三个组成部分分别起什么作用？

`for` 的基本形式是：

::: code-group

```c [for 的结构]
for (init; condition; step) {
  /* body */
}
```

:::

三个部分的作用分别是：

- `init`：初始化语句，通常只执行一次，例如定义并初始化计数变量。
- `condition`：每次进入循环体之前都会判断，结果为假就结束循环。
- `step`：每次循环体执行完后都会执行一次，用来推进状态，例如 `i++`。

注意：三部分都可以省略，但省略后含义要更谨慎地表达。

::: code-group

```c [⚠️ 省略 condition 会形成无限循环]
for (;;) {
  /* ... */
}
```

:::

常见的计数循环写法：

::: code-group

```c [✅ 典型计数循环]
#include <stdio.h>

int main(void) {
  int sum = 0;
  for (int i = 1; i <= 100; i++) {
    sum += i;
  }
  printf("%d\n", sum);
  return 0;
}
```

:::

## 5. 🤔 while 和 do-while 循环的本质区别是什么？

核心区别是判断时机：

- `while`：先判断，再执行，可能一次都不执行。
- `do-while`：先执行一次，再判断，至少执行一次。

::: code-group

```c [while：可能一次都不执行]
int x = 0;
while (x > 0) {
  x--;
}
```

```c [do-while：至少执行一次]
int x = 0;
do {
  x--;
} while (x > 0);
```

:::

一个典型场景是读取用户输入并做合法性校验，通常希望用户至少输入一次：

::: code-group

```c [do-while：输入校验循环]
#include <stdio.h>

int main(void) {
  int n;
  do {
    printf("请输入一个正整数：");
  } while (scanf("%d", &n) != 1 || n <= 0);

  printf("n = %d\n", n);
  return 0;
}
```

:::

## 6. 🆚 for vs while：如何选择合适的循环形式？

选择依据通常不是“哪个好”，而是“哪个更能表达意图”。

| 特性         | `for`                       | `while`                        |
| ------------ | --------------------------- | ------------------------------ |
| 典型用途     | 计数与遍历                  | 条件驱动的重复执行             |
| 退出条件位置 | 固定在循环头部              | 固定在循环头部                 |
| 状态推进位置 | 固定在循环头部的 `step`     | 通常写在循环体内               |
| 可读性优势   | 初始化、条件、步进集中展示  | 更贴近“直到条件满足”的自然语言 |
| 常见风险     | 边界条件写错导致 off-by-one | 忘记推进状态导致死循环         |

两条实用经验：

1. 如果循环变量就是“迭代计数器”，优先用 `for`。
2. 如果循环的停止条件来自外部事件或输入，优先用 `while`。

::: code-group

```c [✅ for：遍历数组]
#include <stdio.h>

int main(void) {
  int a[] = { 1, 2, 3, 4 };
  int n = (int)(sizeof(a) / sizeof(a[0]));

  for (int i = 0; i < n; i++) {
    printf("%d\n", a[i]);
  }
  return 0;
}
```

```c [✅ while：按输入驱动]
#include <stdio.h>

int main(void) {
  int x;
  while (scanf("%d", &x) == 1) {
    printf("read: %d\n", x);
  }
  return 0;
}
```

:::

## 7. 🤔 循环中使用 break 和 continue 会带来什么影响？

- `break`：直接跳出“当前这一层”循环。
- `continue`：跳过本次循环体后续语句，进入下一次迭代。

::: code-group

```c [break：遇到 0 就停止读取]
#include <stdio.h>

int main(void) {
  int x;
  while (scanf("%d", &x) == 1) {
    if (x == 0) {
      break;
    }
    printf("%d\n", x);
  }
  return 0;
}
```

```c [continue：跳过非法数据]
#include <stdio.h>

int main(void) {
  int x;
  while (scanf("%d", &x) == 1) {
    if (x < 0) {
      continue;
    }
    printf("%d\n", x);
  }
  return 0;
}
```

:::

注意：在嵌套循环中，`break` 只会跳出最内层循环。

::: code-group

```c [⚠️ break 只跳出内层]
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    if (j == 1) {
      break;
    }
    /* 只会打印 j == 0 */
  }
}
```

:::

## 8. 🤔 什么是无限循环？如何安全地编写和退出它？

无限循环指循环条件永远为真，例如 `while (1)` 或 `for (;;)`。

无限循环本身不是问题，问题是没有明确的退出路径。

安全写法的要点：

- 退出条件必须清晰，最好集中在少数几个分支里。
- 避免把退出逻辑散落在循环体的很多地方。
- 在可能阻塞的场景中考虑超时或错误分支。

::: code-group

```c [✅ for(;;) 搭配显式退出条件]
#include <stdio.h>

int main(void) {
  int x;
  for (;;) {
    if (scanf("%d", &x) != 1) {
      break;
    }
    if (x == 0) {
      break;
    }
    printf("%d\n", x);
  }
  return 0;
}
```

:::

::: code-group

```c [⚠️ 忘记推进状态很容易死循环]
int i = 0;
while (i < 10) {
  /* ... */
  /* ❌ 忘记 i++ */
}
```

:::

## 9. 🤔 嵌套循环的执行顺序和性能开销如何？

嵌套循环通常是“外层控制轮次，内层完成一轮工作”。执行顺序是：外层每迭代一次，内层会完整执行一遍。

::: code-group

```c [嵌套循环的执行顺序]
for (int i = 0; i < 2; i++) {
  for (int j = 0; j < 3; j++) {
    /* (i, j) 的组合依次为 (0,0)(0,1)(0,2)(1,0)(1,1)(1,2) */
  }
}
```

:::

复杂度上，如果外层运行 $n$ 次，内层运行 $m$ 次，总步数大约是 $n \cdot m$。

工程上常见的性能问题：

- 把“可提前终止”的条件写在内层末尾，导致多做大量无用迭代。
- 内层循环里做了昂贵操作，例如频繁调用 I/O 或重复计算不变量。

优化方向通常是：

1. 把不变量提到循环外。
2. 尽可能早地 `break`。
3. 用更合适的数据结构减少内层扫描次数。

## 10. 🤔 循环变量的作用域和生命周期需要注意什么？

循环变量的作用域取决于它声明的位置。

::: code-group

```c [✅ C99：在 for 初始化部分声明变量，作用域限制在循环内]
for (int i = 0; i < 3; i++) {
  /* i 只在这里可见 */
}
/* 这里访问 i 会编译失败 */
```

```c [⚠️ 在循环外声明变量，循环后仍可见，容易被误用]
int i;
for (i = 0; i < 3; i++) {
  /* ... */
}
/* i 在这里仍然可见，值为 3 */
```

:::

两个常见坑：

- 下标越界：边界条件写错，例如把 `< n` 写成 `<= n`。
- 有符号与无符号混用：例如 `int i` 与 `size_t n` 比较时，可能产生隐式转换。

::: code-group

```c [⚠️ off-by-one：会访问 a[n] 越界]
int a[3] = { 1, 2, 3 };
for (int i = 0; i <= 3; i++) {
  /* ❌ i == 3 时越界 */
  (void)a[i];
}
```

:::

如果你遍历的是数组或容器长度，建议统一使用同一种“计数类型”，并把边界写成更常见的半开区间形式：`[0, n)`。

## 11. 🔗 引用

- [cppreference：Iteration statements，`for`、`while`、`do-while`][1]
- [cppreference：break statement][2]
- [cppreference：continue statement][3]

[1]: https://en.cppreference.com/w/c/language/for
[2]: https://en.cppreference.com/w/c/language/break
[3]: https://en.cppreference.com/w/c/language/continue
