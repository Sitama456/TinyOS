#ifndef _ASM_H_
#define _ASM_H_





#include "type.h"

// 端口写一个字节
void outb(uint16_t port, uint8_t value);
// 端口读一个字节
uint8_t inb(uint16_t port);

static inline int __strcmp(const char *s1, const char *s2) __attribute__((always_inline));
static inline char *__strcpy(char *dst, const char *src) __attribute__((always_inline));
static inline void *__memset(void *s, char c, size_t n) __attribute__((always_inline));
static inline void *__memmove(void *dst, const void *src, size_t n) __attribute__((always_inline));
static inline void *__memcpy(void *dst, const void *src, size_t n) __attribute__((always_inline));

#ifndef __HAVE__ARCH_STRCMP
#define __HAVE__ARCH_STRCMP
static inline int __strcmp(const char *s1, const char *s2) {
    int d0, d1, ret;
    asm volatile (
        "1: lodsb;"
        "scasb;"
    )
  
}
#endif

#endif // _ASM_H_