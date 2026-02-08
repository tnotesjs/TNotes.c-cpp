/**
 * demos/6.c
 */

#include <stdio.h>

typedef unsigned long size_type;
typedef unsigned char byte_t;

int main(void) {
    printf("typedef 类型别名演示\n");
    size_type size = 1024;
    byte_t byte = 255;
    printf("size_type (unsigned long) 值: %lu\n", size);
    printf("byte_t (unsigned char) 值:    %u\n", byte);
    printf("\n");
    
    return 0;
}

/* 
输出结果：
typedef 类型别名演示
size_type (unsigned long) 值: 1024
byte_t (unsigned char) 值:    255
*/
