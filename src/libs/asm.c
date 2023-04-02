#include "TinyOS.h"

// 端口写一个字节
inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// 端口读一个字节
inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN" (port));
    return ret;
}

inline void lgdt(pseudodesc_t* pd) {
    asm volatile("lgdt (%0)"::"r"(pd));
    asm volatile("movw %%ax, %%gs"::"a"(USER_DS));
    asm volatile("movw %%ax, %%fs"::"a"(USER_DS));
    asm volatile("movw %%ax, %%ds"::"a"(KERNEL_DS));
    asm volatile("movw %%ax, %%ss"::"a"(KERNEL_DS));
    asm volatile("movw %%ax, %%es"::"a"(KERNEL_DS));

    // 重新加载cs
    asm volatile("ljmp %0, $1f\n 1:\n"::"i"(KERNEL_CS));
}

inline void lidt(pseudodesc_t* pd) {
    asm volatile("lidt (%0)"::"r"(pd):"memory");
}