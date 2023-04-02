#include "TinyOS.h"

int kern_entry() {
    // 初始化控制台
    console_init();
    printf("Hello TinyOS!\n");

    // // 初始化全局描述符表
    // printf("gdt init ...");
    // gdt_init();
    // printf("done!\n");
    // char *msg = "hello TinyOS";
    // console_write(msg, strlen(msg));
    // 初始化物理内存


    
    return 0;
}