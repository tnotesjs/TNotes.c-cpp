# [0015. 控制语句](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0015.%20%E6%8E%A7%E5%88%B6%E8%AF%AD%E5%8F%A5)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 C 语言有哪些基本控制语句？](#3--c-语言有哪些基本控制语句)
- [4. 🤔 if-else 语句的嵌套和悬空 else 问题如何避免？](#4--if-else-语句的嵌套和悬空-else-问题如何避免)
- [5. 🆚 if-else vs switch 在多分支选择中如何取舍？](#5--if-else-vs-switch-在多分支选择中如何取舍)
- [6. 🤔 for 循环的三个表达式分别起什么作用？](#6--for-循环的三个表达式分别起什么作用)
- [7. 🤔 while 和 do-while 循环的本质区别是什么？](#7--while-和-do-while-循环的本质区别是什么)
- [8. 🤔 break 和 continue 在循环中的行为有何不同？](#8--break-和-continue-在循环中的行为有何不同)
- [9. 🤔 goto 语句是否应该被禁止？它在哪些场景下仍有价值？](#9--goto-语句是否应该被禁止它在哪些场景下仍有价值)
- [10. 🤔 如何避免控制语句中的常见逻辑错误？](#10--如何避免控制语句中的常见逻辑错误)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 条件分支
- `if`、`else if`、`else`
- `switch` 与 `case`、`default`
- `for`、`while`、`do-while`
- `break`、`continue`
- `goto` 的边界用法
- 悬空 `else`
- 常见控制流陷阱

## 2. 🫧 评价

- 控制语句是最基础的语法，但错误往往来自边界条件和可读性。
- 写得“聪明”的控制流通常更难维护，优先用清晰的结构和早返回。

## 3. 🤔 C 语言有哪些基本控制语句？

C 语言的控制语句可以按“分支”和“循环”两大类理解：

- 分支：`if`、`else if`、`else`，以及多分支的 `switch`。
- 循环：`for`、`while`、`do-while`。
- 控制跳转：`break`、`continue`、`goto`、`return`。

其中 `return` 虽然不被所有教材归为“控制语句”，但它在工程里是最常见的控制流工具之一，常用于提前结束函数。

::: code-group

```c [✅ 早返回让分支更清晰]
int parse_port(const char* s) {

  int port;

  if (s == NULL) {
    return -1;
  }

  if (sscanf(s, "%d", &port) != 1) {
    return -1;
  }

  if (port < 0 || port > 65535) {
    return -1;
  }

  return port;
}
```

:::

注意：上面用到了 `sscanf`，在真实工程中通常会配合更严格的校验与错误处理。

## 4. 🤔 if-else 语句的嵌套和悬空 else 问题如何避免？

`if-else` 的嵌套很常见，但容易出现两个问题：

1. 嵌套过深导致可读性差。
2. 悬空 `else`（dangling else）导致 `else` 绑定的 `if` 不是你以为的那个。

所谓悬空 `else` 指的是：在没有使用花括号 `{}` 的情况下，`else` 总是与“最近的、尚未匹配 `else` 的 `if`”配对。

::: code-group

```c [❌ 悬空 else]
int a = 1;
int b = 0;

if (a > 0)
  if (b > 0)
    puts("both positive");
  else
    puts("a positive, b not positive");
// ❌ 这里的 else 绑定的是 if (b > 0)，而不是 if (a > 0)。
```

```c [✅ 用花括号明确结构]
int a = 1;
int b = 0;

if (a > 0) {
  if (b > 0) {
    puts("both positive");
  } else {
    puts("a positive, b not positive");
  }
}
```

:::

避免嵌套过深的常用技巧：

- 用“早返回”或“提前 continue”把异常路径先处理掉。
- 把复杂判断提取成布尔变量或独立函数，让条件更短。
- 统一使用花括号，即使单行语句也加。

## 5. 🆚 if-else vs switch 在多分支选择中如何取舍？

当你需要处理多分支选择时，`if-else` 和 `switch` 都能实现，但适用范围不同。

| 特性         | `if-else`                | `switch`                     |
| ------------ | ------------------------ | ---------------------------- |
| 条件类型     | 任意布尔表达式           | 整型表达式或可转为整型的枚举 |
| 分支表达能力 | 强，可以写区间、组合条件 | 主要是等值匹配               |
| 可读性       | 分支多时容易变长         | 多个 `case` 时更清晰         |
| 易错点       | 条件重复、漏写边界       | 忘写 `break`、穿透行为       |

`switch` 的典型使用场景是命令、状态机、协议字段等离散值分发。

::: code-group

```c [✅ switch 处理离散值]
typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
} Op;

int apply(int a, int b, Op op, int* out) {
  if (out == NULL) {
    return -1;
  }

  switch (op) {
    case OP_ADD:
      *out = a + b;
      return 0;
    case OP_SUB:
      *out = a - b;
      return 0;
    case OP_MUL:
      *out = a * b;
      return 0;
    case OP_DIV:
      if (b == 0) {
        return -1;
      }
      *out = a / b;
      return 0;
    default:
      return -1;
  }
}
```

```c [⚠️ switch 的穿透行为]
int level = 2;
switch (level) {
  case 1:
    puts("level 1");
    break;
  case 2:
    puts("level 2");
    // ⚠️ 如果这里忘了 break，会继续执行 case 3。
  case 3:
    puts("level 3");
    break;
  default:
    puts("unknown");
    break;
}
```

:::

工程建议：

- `switch` 的每个 `case` 通常都写 `break`，除非你明确要穿透，并在穿透处写清楚注释。
- 如果分支逻辑非常复杂，优先用 `if-else` 或拆成函数，不要强行塞进一个巨大 `switch`。

## 6. 🤔 for 循环的三个表达式分别起什么作用？

`for` 循环的语法是：

`for (init; cond; step) { ... }`

三个表达式的含义分别是：

- `init`：初始化，只执行一次。
- `cond`：循环条件，每轮迭代开始前检查，结果为假则退出循环。
- `step`：步进表达式，每轮迭代结束后执行一次。

这三个位置都可以为空，但分号不能省略。

::: code-group

```c [for 的基本形态]
for (int i = 0; i < 10; i++) {
  printf("%d\n", i);
}
```

```c [⚠️ 多表达式与逗号运算符]
for (int i = 0, j = 9; i < j; i++, j--) {
  printf("i=%d，j=%d\n", i, j);
}
```

```c [空表达式的 for]
int i = 0;
for (; i < 10; ) {
  i++;
}
```

:::

经验上，`for` 更适合“计数型循环”，`while` 更适合“不确定循环次数的循环”。

## 7. 🤔 while 和 do-while 循环的本质区别是什么？

`while` 和 `do-while` 的本质区别在于：条件检查发生在循环体之前还是之后。

- `while`：先判断条件，再决定是否执行循环体。
- `do-while`：先执行一次循环体，再判断条件。

因此，`do-while` 至少会执行一次。

::: code-group

```c [while：可能一次都不执行]
int n = 0;
while (n > 0) {
  puts("run");
}
```

```c [do-while：至少执行一次]
int n = 0;
do {
  puts("run once");
} while (n > 0);
```

:::

常见使用场景：

- `while`：读取输入直到 EOF，或轮询直到条件满足。
- `do-while`：菜单交互，让用户至少输入一次。

## 8. 🤔 break 和 continue 在循环中的行为有何不同？

`break` 和 `continue` 都用于改变循环控制流，但作用不同：

- `break`：直接结束当前循环。
- `continue`：结束本轮迭代，进入下一轮迭代。

它们在 `for` 循环中的细微差别是：执行 `continue` 后仍会执行 `step` 表达式。

::: code-group

```c [break 与 continue]
for (int i = 0; i < 10; i++) {
  if (i == 2) {
    continue;
  }
  if (i == 5) {
    break;
  }
  printf("%d\n", i);
}
```

:::

工程建议：

- `continue` 可以让“异常分支”提前结束，减少嵌套。
- 但过度使用 `break` 和 `continue` 也会让控制流变难读，保持克制。

## 9. 🤔 goto 语句是否应该被禁止？它在哪些场景下仍有价值？

`goto` 在大多数业务代码里不推荐使用，因为它容易制造“意大利面式控制流”。

但在 C 语言的某些场景里，`goto` 仍然有工程价值，最典型的是统一错误处理与资源释放。

在没有异常机制、又需要在多个失败点释放多种资源时，用 `goto` 可以避免层层嵌套与重复的清理代码。

::: code-group

```c [✅ goto 做统一清理]
#include <stdio.h>
#include <stdlib.h>

int work(size_t n) {
  int ret = -1;
  int* a = NULL;
  int* b = NULL;

  a = (int*)malloc(n * sizeof(int));
  if (a == NULL) {
    goto cleanup;
  }

  b = (int*)malloc(n * sizeof(int));
  if (b == NULL) {
    goto cleanup;
  }

  ret = 0;

cleanup:
  free(b);
  free(a);
  return ret;
}
```

:::

使用 `goto` 的底线原则：

- 只向前跳到统一出口，不要在函数中随意来回跳。
- 标签命名清晰，例如 `cleanup`、`fail`、`out`。
- 不要用 `goto` 替代循环与分支，它应该是“最后的工具”。

## 10. 🤔 如何避免控制语句中的常见逻辑错误？

控制语句的逻辑错误通常不是语法错误，而是边界条件、分支覆盖和状态变化导致的错误。

常见问题与建议如下：

1. 条件写反或漏写边界：对区间判断优先写成“显式上下界”，并添加单元测试。
2. 混淆赋值与比较：把编译告警打开，并在条件中避免直接写赋值。
3. `switch` 忘写 `break`：默认每个 `case` 都写 `break`，需要穿透时写明注释。
4. 循环变量越界：数组下标循环优先写成 `i < n`，不要写 `i <= n - 1`。
5. 死循环与无法退出：把退出条件写在循环头，并在循环体内避免隐式修改关键变量。

::: code-group

```c [⚠️ 赋值与比较混淆]
int x = 0;

// ⚠️ 这不是语法错误，但通常是逻辑错误。
if (x = 1) {
  puts("always true");
}
```

```c [✅ 更清晰的写法]
int x = 0;
if (x == 1) {
  puts("x is 1");
}
```

:::

最终建议是：让控制流“可视化”。只要表达式复杂到需要停下来推理，就把它拆开。

## 11. 🔗 引用

- [cppreference：Statements，C 语句概览][1]
- [cppreference：Selection statements，`if` 与 `switch`][2]
- [cppreference：Iteration statements，`for`、`while`、`do-while`][3]
- [GCC：Warning Options，`-Wparentheses` 等告警][4]

[1]: https://en.cppreference.com/w/c/language/statements
[2]: https://en.cppreference.com/w/c/language/if
[3]: https://en.cppreference.com/w/c/language/for
[4]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
