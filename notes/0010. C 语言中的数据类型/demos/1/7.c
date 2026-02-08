/**
 * demos/7.c
 */

#include <stdio.h>

int main(void) {
    printf("void 类型的应用\n");
    printf("void* 指针大小: %zu 字节\n", sizeof(void*));
    printf("void* 可以指向任意类型，使用前需要转换\n");
    
    int num = 42;
    void* ptr = &num;
    printf("通过 void* 访问整数: %d\n", *(int*)ptr);
    printf("\n");
    
    return 0;
}

/* 
输出结果：
void 类型的应用
void* 指针大小: 8 字节
void* 可以指向任意类型，使用前需要转换
通过 void* 访问整数: 42
*/
