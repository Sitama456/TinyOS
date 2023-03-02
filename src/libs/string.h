#ifndef _STRING_H_
#define _STRING_H_
#include "type.h"

size_t strlen(char *str);

// 按字节为单位拷贝内存
void memcpy(void *dest, void *src, size_t n);

#endif // _STRING_H_