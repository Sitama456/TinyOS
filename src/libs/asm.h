#ifndef _ASM_H_
#define _ASM_H_





#include "type.h"

// 端口写一个字节
void outb(uint16_t port, uint8_t value);
// 端口读一个字节
uint8_t inb(uint16_t port);
#endif // _ASM_H_