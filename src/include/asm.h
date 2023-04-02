#ifndef _ASM_H_
#define _ASM_H_

#include "type.h"
#include "gdt.h"

/// @brief 往端口写一个字节的数据
/// @param port 端口
/// @param value 数据
void outb(uint16_t port, uint8_t value);

/// @brief 从端口读一个字节的数据
/// @param port 端口
/// @return 数据
uint8_t inb(uint16_t port);

/// @brief 加载gdt全局描述符表
/// @param pd 结构指针
void lgdt(pseudodesc_t* pd);

void lidt(pseudodesc_t* pd);
#endif // _ASM_H_