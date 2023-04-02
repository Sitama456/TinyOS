#include "TinyOS.h"



int kern_entry() {
    // 初始化控制台
    console_init();
    printk("Hello TinyOS!\n");

    // 初始化全局描述符表
    printk("gdt init ...");
    gdt_init();
    printk("done!\n");
    
    // 初始化物理内存
    while (1)
    {
        /* code */
    }
    

    
    return 0;
}