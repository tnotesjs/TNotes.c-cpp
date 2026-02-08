/**
 * demos/1.c
 */

#include <stdio.h>

int main(void) {
    printf("基本整型的大小（单位：字节）\n");
    printf("sizeof(char)           = %zu\n", sizeof(char));
    printf("sizeof(short)          = %zu\n", sizeof(short));
    printf("sizeof(int)            = %zu\n", sizeof(int));
    printf("sizeof(long)           = %zu\n", sizeof(long));
    printf("sizeof(long long)      = %zu\n", sizeof(long long));
    printf("\n");
    
    return 0;
}

/* 
输出结果：
基本整型的大小（单位：字节）
sizeof(char)           = 1
sizeof(short)          = 2
sizeof(int)            = 4
sizeof(long)           = 8
sizeof(long long)      = 8
*/
