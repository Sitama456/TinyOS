#ifndef _GDT_H_
#define _GDT_H_

#include "type.h"

/* Application segment type bits */
#define STA_X           0x8         // Executable segment
#define STA_E           0x4         // Expand down (non-executable segments)
#define STA_C           0x4         // Conforming code segment (executable only)
#define STA_W           0x2         // Writeable (non-executable segments)
#define STA_R           0x2         // Readable (executable segments)
#define STA_A           0x1         // Accessed

/* System segment type bits */
#define STS_T16A        0x1         // Available 16-bit TSS
#define STS_LDT         0x2         // Local Descriptor Table
#define STS_T16B        0x3         // Busy 16-bit TSS
#define STS_CG16        0x4         // 16-bit Call Gate
#define STS_TG          0x5         // Task Gate / Coum Transmitions
#define STS_IG16        0x6         // 16-bit Interrupt Gate
#define STS_TG16        0x7         // 16-bit Trap Gate
#define STS_T32A        0x9         // Available 32-bit TSS
#define STS_T32B        0xB         // Busy 32-bit TSS
#define STS_CG32        0xC         // 32-bit Call Gate
#define STS_IG32        0xE         // 32-bit Interrupt Gate
#define STS_TG32        0xF         // 32-bit Trap Gate


// 全局描述符类型
typedef struct segdesc {
    unsigned sd_lim_15_0 : 16;      // low bits of segment limit
    unsigned sd_base_15_0 : 16;     // low bits of segment base address
    unsigned sd_base_23_16 : 8;     // middle bits of segment base address
    unsigned sd_type : 4;           // segment type (see STS_ constants)
    unsigned sd_s : 1;              // 0 = system, 1 = application
    unsigned sd_dpl : 2;            // descriptor Privilege Level
    unsigned sd_p : 1;              // present
    unsigned sd_lim_19_16 : 4;      // high bits of segment limit
    unsigned sd_avl : 1;            // unused (available for software use)
    unsigned sd_rsv1 : 1;           // reserved
    unsigned sd_db : 1;             // 0 = 16-bit segment, 1 = 32-bit segment
    unsigned sd_g : 1;              // granularity: limit scaled by 4K when set
    unsigned sd_base_31_24 : 8;     // high bits of segment base address
} segdesc_t;

// 全局描述符表结构体
typedef struct pseudodesc {
    uint16_t pd_lim;            // 表界限
    uint32_t pd_base;           // 表基地址
} pseudodesc_t;

// 设置空描述符
#define SEG_NULL                                            \
    (struct segdesc) {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

// brief: 设置全局描述符
// type: 用户段类型，可读、写、执行
// base: 段基址
// lim: 段界限
// dpl: 段权限
#define SEG(type, base, lim, dpl)                           \
    (struct segdesc) {                                      \
        ((lim) >> 12) & 0xffff, (base) & 0xffff,            \
        ((base) >> 16) & 0xff, type, 1, dpl, 1,             \
        (unsigned)(lim) >> 28, 0, 0, 1, 1,                  \
        (unsigned) (base) >> 24                             \
    }

// brief: 设置TSS描述符
// type: 系统段
// base: 段基址
// lim: 段界限
// dpl: 段权限
#define SEGTSS(type, base, lim, dpl)                        \
    (struct segdesc) {                                      \
        (lim) & 0xffff, (base) & 0xffff,                    \
        ((base) >> 16) & 0xff, type, 0, dpl, 1,             \
        (unsigned) (lim) >> 16, 0, 0, 1, 0,                 \
        (unsigned) (base) >> 24                             \
    }

/* task state segment format (as described by the Pentium architecture book) */
// 任务状态段
typedef struct taskstate {
    uint32_t ts_link;       // old ts selector
    uint32_t ts_esp0;      // stack pointers and segment selectors
    uint16_t ts_ss0;        // after an increase in privilege level
    uint16_t ts_padding1;
    uint32_t ts_esp1;
    uint16_t ts_ss1;
    uint16_t ts_padding2;
    uint32_t ts_esp2;
    uint16_t ts_ss2;
    uint16_t ts_padding3;
    uint32_t ts_cr3;       // page directory base
    uint32_t ts_eip;       // saved state from last task switch
    uint32_t ts_eflags;
    uint32_t ts_eax;        // more saved state (registers)
    uint32_t ts_ecx;
    uint32_t ts_edx;
    uint32_t ts_ebx;
    uint32_t ts_esp;
    uint32_t ts_ebp;
    uint32_t ts_esi;
    uint32_t ts_edi;
    uint16_t ts_es;         // even more saved state (segment selectors)
    uint16_t ts_padding4;
    uint16_t ts_cs;
    uint16_t ts_padding5;
    uint16_t ts_ss;
    uint16_t ts_padding6;
    uint16_t ts_ds;
    uint16_t ts_padding7;
    uint16_t ts_fs;
    uint16_t ts_padding8;
    uint16_t ts_gs;
    uint16_t ts_padding9;
    uint16_t ts_ldt;
    uint16_t ts_padding10;
    uint16_t ts_t;          // trap on task switch
    uint16_t ts_iomb;       // i/o map base address
} __attribute__((packed)) taskstate_t;


void gdt_init();
#endif // _GDT_H_