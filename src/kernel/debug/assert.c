#include "TinyOS.h"

static char buff[1024];

// 强制阻塞
static void spin(char *message) {
    printk("spinning in %s ...\n", message);
    while (TRUE)
        ;
}

void assertion_failure(char *exp, char *file, char *base, int line) {
    printk(
        "\n--> assert(%s) failed!!!\n"
        "--> file: %s \n"
        "--> base: %s \n"
        "--> line: %d \n",
        exp, file, base, line
    );
    spin("assertion_failure()");

    // 不可能走到这里 除非出错
    asm volatile("ud2");
}

void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(buff, fmt, args);
    va_end(args);

    printk("!!! panic !!!\n---> %s \n", buff);
    spin("panic()");

     // 不可能走到这里 除非出错
    asm volatile("ud2");
}