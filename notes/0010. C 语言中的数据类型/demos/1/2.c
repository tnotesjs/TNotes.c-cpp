#include <stdio.h>

int main(void) {
  printf("有符号与无符号整型\n");
  printf("sizeof(signed int)     = %zu\n", sizeof(signed int));
  printf("sizeof(unsigned int)   = %zu\n", sizeof(unsigned int));
  printf("sizeof(unsigned long)  = %zu\n", sizeof(unsigned long));
  printf("\n");

  return 0;
}

/*
输出结果：
有符号与无符号整型
sizeof(signed int)     = 4
sizeof(unsigned int)   = 4
sizeof(unsigned long)  = 8
*/
