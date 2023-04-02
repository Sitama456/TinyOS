#include "TinyOS.h"

// 定义任务状态段
// static taskstate_t ts = { 0 };

// 定义全局描述符表
static segdesc_t gdt[] = {
    SEG_NULL,
    [SEG_KTEXT] = SEG(STA_X | STA_R, 0x0, 0xFFFFFFFF, DPL_KERNEL),
    [SEG_KDATA] = SEG(STA_W, 0x0, 0xFFFFFFFF, DPL_KERNEL),
    [SEG_UTEXT] = SEG(STA_X | STA_R, 0x0, 0xFFFFFFFF, DPL_USER),
    [SEG_UDATA] = SEG(STA_W, 0x0, 0xFFFFFFFF, DPL_USER),
    [SEG_TSS] = SEG_NULL
};

// 定义全局描述符表的寄存器指针
static pseudodesc_t gdt_pd;


void gdt_init() {
    // 从 0 开始 所以要 -1
    gdt_pd.pd_lim = sizeof(gdt) - 1;
    gdt_pd.pd_base = (uint32_t)gdt;
    // 加载寄存器 并刷新地址
    lgdt(&gdt_pd);

}

