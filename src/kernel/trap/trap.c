#include "TinyOS.h"

// 定义中断向量表
static gatedesc_t idt[256] = { {0} };

// 告诉cpu中断向量表在哪
static pseudodesc_t idt_pd;

void trap_init() {
    // 将中断描述符与中断处理函数联系起来
    int i;
    extern uint32_t __vectors[];
    for (i = 0; i < sizeof(idt) / sizeof(gatedesc_t); ++i) {
        SETGATE(idt[i], 0, GD_KTEXT, __vectors[i], DPL_KERNEL);
    }
    // 由于系统调用门比较特殊 单独再设置一下
    SETGATE(idt[T_SYSCALL], 1, GD_KTEXT, __vectors[T_SYSCALL], DPL_USER);
    // 加载中断向量表
    idt_pd.pd_base = (uint32_t)idt;
    idt_pd.pd_lim = sizeof(idt) - 1;
    lidt(&idt_pd);
}

bool trap_in_kernel(trapframe_t *tf) {
    return (tf->tf_cs == (uint16_t)KERNEL_CS);
}

static void trap_dispatch(trapframe_t *tf) {

}

void trap(trapframe_t *tf) {
    trap_dispatch(tf);
}