#include <limits.h>
#include <stdio.h>

int main(void) {
  printf("char 类型的符号性演示\n");
  char c = 0xFF;
  signed char sc = 0xFF;
  unsigned char uc = 0xFF;

  printf("char c = 0xFF:          %d\n", c);
  printf("signed char sc = 0xFF:  %d\n", sc);
  printf("unsigned char uc = 0xFF: %u\n", uc);

  if (CHAR_MIN < 0) {
    printf("当前平台 char 是有符号类型\n");
  } else {
    printf("当前平台 char 是无符号类型\n");
  }
  printf("\n");

  return 0;
}

/*
输出结果：
char 类型的符号性演示
char c = 0xFF:          -1
signed char sc = 0xFF:  -1
unsigned char uc = 0xFF: 255
当前平台 char 是有符号类型
*/
