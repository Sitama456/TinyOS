#include <string.h>

size_t strlen(char *str) {
    size_t n = 0;
    char* ptr = str;
    while (*ptr++ != '\0') {
        n++;
    }
    return n;
}


void memcpy(void *dest, void *src, size_t n) {
    char* p1 = (char *)dest;
    char* p2 = (char *)src;

    // 要从后面拷贝
    if (p2 + n > p1) {
        p1 += n;
        p2 += n;
        while (n--) {
            *(--p1) = *(--p2);
        }
        return;
    }
    // 从前面拷贝
    while (n--) {
        *(p1++) = *(p2++);
    }
    return;
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 == *str2) {
        if (*str1 == '\0') {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}