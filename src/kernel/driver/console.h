#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "TinyOS.h"

typedef enum vga_color {
    vc_black = 0,
    vc_blue = 1,
    vc_green = 2,
    vc_cyan = 3,
    vc_red = 4,
    vc_magenta = 5,
    vc_brown = 6,
    vc_light_grey = 7,
    vc_dark_grey = 8,
    vc_light_blue = 9,
    vc_light_green = 10,
    vc_light_cyan = 11,
    vc_light_red = 12,
    vc_light_magenta = 13,
    vc_light_brown = 14,
    vc_white = 15
} vga_color_t;

// 清屏操作
void console_clear();

// 写入操作
size_t console_write(char *buf, size_t count);

// 初始化
void console_init();


#endif // _CONSOLE_H_