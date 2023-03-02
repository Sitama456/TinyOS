#include "TinyOS.h"

#define CRT_ADDR_REG    0x3D4       // CRT(6845) 索引寄存器
#define CRT_DATA_REG    0x3D5       // CRT(6845) 数据寄存器

#define CRT_START_ADDR_H    0xC     // 显示内存起始位置 - 高位
#define CRT_START_ADDR_L    0xD     // 显示内存起始位置 - 地位
#define CRT_CURSOR_H        0xE     // 光标位置 - 低位
#define CRT_CURSOR_L        0xF     // 光标位置 - 高位

#define MEM_BASE    0xB8000         // 显卡内存起始位置
#define MEM_SIZE    0x4000          // 显卡内存大小
#define MEM_END (MEM_BASE + MEM_SIZE)   // 显卡内存结束位置

#define WIDTH 80                    // 屏幕文本列数
#define HEIGHT 25                   // 屏幕文本行数
#define ROW_SIZE (WIDTH * 2)        // 每行字节数
#define SCR_SIZE (ROW_SIZE * HEIGHT) // 屏幕字节数

#define ASCII_NUL 0x00
#define ASCII_ENQ 0x05
#define ASCII_BEL 0x07 // \a  响铃 自导扬声器
#define ASCII_BS 0x08  // \b  退格符 
#define ASCII_HT 0x09  // \t  制表符
#define ASCII_LF 0x0A  // \n  换行符
#define ASCII_VT 0x0B  // \v
#define ASCII_FF 0x0C  // \f
#define ASCII_CR 0x0D  // \r
#define ASCII_DEL 0x7F

// 屏幕抽象位一个80*25的二维数组
static uint32_t screen; // 当前屏幕显示器开始的内存位置
static uint32_t cursor_pos; // 记录当前光标的内存位置

// 当前光标的(逻辑坐标)坐标
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;


static uint8_t attr = vc_black << 4 | vc_green;        // 字符样式
static uint16_t erase = 0x0720; // 空格

// 获取屏幕的开始位置
static void get_screen() {
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);       // 开始位置高地址
    screen = inb(CRT_DATA_REG) << 8;            // 开始位置的高八位

    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    screen |= inb(CRT_ADDR_REG);

    // screen 相当于第几个元素 每个元素有两个字节
    screen <<= 1;
    screen += MEM_BASE;
}

// 设置当前显示器开始的位置
static void set_screen() {
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    outb(CRT_ADDR_REG, ((screen - MEM_BASE) >> 1) & 0xff);
}

// 获得当前光标的位置
static void get_cursor() {
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    cursor_pos = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    cursor_pos |= inb(CRT_DATA_REG);

    get_screen();

    cursor_pos <<= 1;
    cursor_pos += MEM_BASE;

    uint32_t delta = (cursor_pos - screen) >> 1;
    cursor_x = delta % WIDTH;
    cursor_y = delta / WIDTH;
}

static void set_cursor() {
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    outb(CRT_DATA_REG, ((cursor_pos - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, ((cursor_pos - MEM_BASE) >> 1) & 0xff);
}

void console_clear() {
    // 屏幕起始地址直接等于显存起始地址
    screen = MEM_BASE;
    cursor_pos = MEM_BASE;
    cursor_x = cursor_y = 0;
    set_cursor();
    set_screen();

    uint16_t *ptr = (uint16_t *)MEM_BASE;
    while (ptr < (uint16_t)MEM_END) {
        *ptr++ = erase;
    }
}

// 屏幕向上滚动
static void scroll() {
    // 滚到显存最后了 没法再滚了
    if (screen + SCR_SIZE + ROW_SIZE >= MEM_END) {
        memcpy((void*)MEM_BASE, (void*)screen, SCR_SIZE);
        cursor_pos -= (screen - MEM_BASE);
        screen = MEM_BASE;
    }
    // 先清空要下滚的一行
    uint16_t *ptr = (uint16_t)(screen + SCR_SIZE); 
    for (size_t i = 0; i < WIDTH; ++i) {
        *ptr++ = erase;
    }
    // 向上滚一行，相当于screen向后加一个ROW_SIZE
    screen += ROW_SIZE;
    cursor_pos += ROW_SIZE;
    set_screen();
}

static void command_bs() {
    // cursor_x 大于0 则回退一个
    if (cursor_x) {
        cursor_x--;
        cursor_pos -= 2;
        *(uint16_t *)cursor_pos = erase;
    }
}

static void command_del() {
    *(uint16_t *)cursor_pos = erase;
}

// 换行 \n
static void command_lf() {
    if (cursor_y + 1 < HEIGHT) {
        cursor_y++;
        cursor_pos += ROW_SIZE;
        return;
    }
    // 需要滚屏
    scroll();
}

// 光标回到开始位置 \r
static void command_cr() {
    cursor_pos -= cursor_x << 1;
    cursor_x = 0;
}

// 制表符号 \t 通常宽度相当于8个空格
static void command_ht() {
    int offset = 8 - (cursor_x & 7);
    cursor_x += offset;
    cursor_pos += offset << 1;
    // 超出一行的宽度了 就需要换行
    if (cursor_x >= WIDTH) {
        cursor_x -= WIDTH;
        cursor_pos -= ROW_SIZE;
        command_lf();
    }
}

// 输出字符
static void chr(char ch) {
    if (cursor_x >= WIDTH) {
        cursor_x -= WIDTH;
        cursor_pos -= ROW_SIZE;
        command_lf();
    }
    *((char *)cursor_pos) = ch;
    cursor_pos++;
    *((char *)cursor_pos) = attr;
    cursor_pos++;
    cursor_x++;
}

size_t console_write(char *buf, size_t count) {
    char ch;
    size_t nr = 0;
    // 一个一个地写
    while (nr++ < count) {
        ch = *buf++;
        switch (ch)
        {
        case ASCII_NUL:
            break;
        case ASCII_BS:
            command_bs();
            break;
        case ASCII_HT:
            command_ht();
            break;
        case ASCII_LF:
            command_lf();
            command_cr();
            break;
        case ASCII_VT:
        case ASCII_FF:
            command_lf();
            break;
        case ASCII_DEL:
            command_del();
            break;
        default:
            chr(ch);
            break;
        }
    }
    set_cursor();
    return nr;
}

void console_init() {
    console_clear();
}