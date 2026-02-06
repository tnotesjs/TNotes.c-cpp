# [0017. 选择结构](https://github.com/tnotesjs/TNotes.c-cpp/tree/main/notes/0017.%20%E9%80%89%E6%8B%A9%E7%BB%93%E6%9E%84)

<!-- region:toc -->

- [1. 🎯 本节内容](#1--本节内容)
- [2. 🫧 评价](#2--评价)
- [3. 🤔 什么是选择结构？](#3--什么是选择结构)
- [4. 🤔 if 语句的基本形式和执行逻辑是什么？](#4--if-语句的基本形式和执行逻辑是什么)
- [5. 🤔 如何避免 if-else 的悬空 else 问题？](#5--如何避免-if-else-的悬空-else-问题)
- [6. 🤔 多重 if-else 和 if-else if 链有何区别？](#6--多重-if-else-和-if-else-if-链有何区别)
- [7. 🆚 if-else 链 vs switch 语句：何时该用哪种？](#7--if-else-链-vs-switch-语句何时该用哪种)
- [8. 🤔 switch 语句中的 case 标签必须满足什么条件？](#8--switch-语句中的-case-标签必须满足什么条件)
- [9. 🤔 为什么 switch 语句容易遗漏 break 导致穿透（fall-through）？](#9--为什么-switch-语句容易遗漏-break-导致穿透fall-through)
- [10. 🤔 如何安全地使用 switch 的 fall-through 特性？](#10--如何安全地使用-switch-的-fall-through-特性)
- [11. 🔗 引用](#11--引用)

<!-- endregion:toc -->

## 1. 🎯 本节内容

- 选择结构
- `if` 与 `if-else`
- `else if` 链
- 悬空 `else`
- `switch` 与 `case`、`default`
- `break` 与穿透
- `case` 常量表达式
- 分支覆盖与可读性

## 2. 🫧 评价

- 选择结构的难点不在语法，而在分支覆盖、可读性和边界条件。
- 工程里最常见的坑是复杂 `if` 条件和 `switch` 忘写 `break`，靠习惯与告警可以大幅减少。

## 3. 🤔 什么是选择结构？

选择结构指的是：程序根据条件的真假，选择执行不同的语句块。

在 C 语言中，选择结构主要由两类语句实现：

- `if` 系列：适用于任意布尔表达式。
- `switch`：适用于离散值的等值分发，通常是整型或枚举。

选择结构的本质是：同一段代码在不同输入下可能走不同路径，因此需要你更关注“哪些路径会执行，哪些路径不会执行”。

## 4. 🤔 if 语句的基本形式和执行逻辑是什么？

`if` 的基本形式是：当条件表达式为真时，执行对应语句块。

在 C 语言里，条件并不要求是 `0` 或 `1`，而是遵循“零为假，非零为真”的规则。

::: code-group

```c [if 与 if-else 基本形式]
#include <stdio.h>

int main(void) {

  int x = 3;

  if (x > 0) {
    puts("positive");
  }

  if (x % 2 == 0) {
    puts("even");
  } else {
    puts("odd");
  }

  return 0;
}
```

:::

工程建议：即使 `if` 体只有一行，也建议统一使用花括号 `{}`，减少后续修改引入错误的概率。

## 5. 🤔 如何避免 if-else 的悬空 else 问题？

悬空 `else` 指的是：在省略花括号的写法中，`else` 总是与最近的、尚未匹配 `else` 的 `if` 配对。

::: code-group

```c [❌ 悬空 else 示例]
int a = 1;
int b = 0;

if (a > 0)
  if (b > 0)
    puts("both positive");
  else
    puts("a positive, b not positive");
// ❌ else 绑定的是 if (b > 0)，而不是 if (a > 0)。
```

```c [✅ 统一加花括号]
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

避免悬空 `else` 的最可靠方式只有一个：统一使用花括号。

## 6. 🤔 多重 if-else 和 if-else if 链有何区别？

这两个说法常被混用，但建议你把它们区分开：

- 多重 `if`：多个独立 `if`，每个条件都会被检查，可能执行多个分支。
- `if-else if` 链：同一链条里只会命中一个分支，命中后就跳过后续条件。

::: code-group

```c [多重 if：可能执行多个分支]
int x = 6;

if (x % 2 == 0) {
  puts("even");
}
if (x % 3 == 0) {
  puts("divisible by 3");
}
```

```c [else if 链：只会命中一个分支]
int score = 85;

if (score >= 90) {
  puts("A");
} else if (score >= 60) {
  puts("B");
} else {
  puts("C");
}
```

:::

如果你希望多个条件都能生效，用多重 `if`。如果你希望“互斥分支”，用 `else if` 链，并注意条件的顺序要从严格到宽松或从高到低。

## 7. 🆚 if-else 链 vs switch 语句：何时该用哪种？

`if-else` 链和 `switch` 都能做多分支选择，但适用范围不同。

| 特性         | `if-else` 链           | `switch`           |
| ------------ | ---------------------- | ------------------ |
| 条件表达能力 | 强，可写区间、组合条件 | 主要做等值匹配     |
| 典型输入     | 任意布尔表达式         | 整型或枚举         |
| 可读性       | 分支多时容易冗长       | 多 `case` 时更直观 |
| 常见坑       | 条件重叠、边界漏写     | 忘写 `break`、穿透 |

::: code-group

```c [✅ switch 适合离散值分发]
typedef enum {
  CMD_START = 1,
  CMD_STOP = 2,
  CMD_STATUS = 3,
} Command;

void handle(Command cmd) {
  switch (cmd) {
    case CMD_START:
      puts("start");
      break;
    case CMD_STOP:
      puts("stop");
      break;
    case CMD_STATUS:
      puts("status");
      break;
    default:
      puts("unknown");
      break;
  }
}
```

:::

如果你的条件涉及范围、多个变量、复杂逻辑判断，优先用 `if-else`。如果你的条件是一个“枚举或离散整数值”，优先用 `switch`。

## 8. 🤔 switch 语句中的 case 标签必须满足什么条件？

`switch` 的选择表达式会先被求值，然后与每个 `case` 标签进行匹配。

关键规则是：`case` 后面必须是“整型常量表达式”，也就是编译期可确定的常量。

常见的合法形式包括：

- 字面量常量，例如 `case 0:`、`case 10:`。
- `enum` 枚举值，例如 `case CMD_START:`。
- 宏常量，例如 `#define MAX 10` 后的 `case MAX:`。

::: code-group

```c [✅ case 使用常量表达式]
enum Mode {
  MODE_A = 1,
  MODE_B = 2,
};

void f(int mode) {
  switch (mode) {
    case MODE_A:
      puts("A");
      break;
    case MODE_B:
      puts("B");
      break;
    default:
      puts("unknown");
      break;
  }
}
```

:::

注意：`case` 不是条件表达式，它不支持区间判断，例如 `case 1 ... 5` 不是标准 C 语法。

## 9. 🤔 为什么 switch 语句容易遗漏 break 导致穿透（fall-through）？

`switch` 的行为是：进入某个 `case` 后，会从该位置开始顺序执行，除非遇到 `break`、`return` 或跳出语句。

因此，若你忘记写 `break`，就会继续执行下一个 `case`，这就是穿透。

::: code-group

```c [⚠️ 忘写 break 的典型问题]
int level = 2;

switch (level) {
  case 1:
    puts("level 1");
    break;
  case 2:
    puts("level 2");
    // ⚠️ 忘了 break，会继续执行 case 3。
  case 3:
    puts("level 3");
    break;
  default:
    puts("unknown");
    break;
}
```

:::

工程实践里，建议把编译告警打开，并养成每个 `case` 都写 `break` 的习惯。

## 10. 🤔 如何安全地使用 switch 的 fall-through 特性？

穿透并不总是错误，有些场景下它能减少重复代码，例如多个 `case` 共享同一段处理逻辑。

安全使用穿透的关键是：明确表达意图，并避免“无意穿透”。

::: code-group

```c [✅ 多个 case 共享逻辑]
int is_weekend(int day) {
  switch (day) {
    case 6:
    case 7:
      return 1;
    default:
      return 0;
  }
}
```

```c [✅ 显式标注有意穿透]
void print_level(int level) {
  switch (level) {
    case 3:
      puts("high");
      /* fall through */
    case 2:
      puts("mid");
      /* fall through */
    case 1:
      puts("low");
      break;
    default:
      puts("unknown");
      break;
  }
}
```

:::

如果一个 `case` 逻辑已经足够复杂，宁可拆成函数，也不要依赖多层穿透来复用逻辑。

## 11. 🔗 引用

- [cppreference：Selection statements，`if` 与 `switch`][1]
- [cppreference：switch 语句细节与 `case` 规则][2]
- [GCC：Warning Options，`-Wimplicit-fallthrough` 等告警][3]

[1]: https://en.cppreference.com/w/c/language/if
[2]: https://en.cppreference.com/w/c/language/switch
[3]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
