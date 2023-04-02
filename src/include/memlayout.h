#ifndef _MEMLAYOUT_H_
#define _MEMLAYOUT_H_

#include "type.h"

/* global segment number */
#define SEG_KTEXT       1       // 内核代码段
#define SEG_KDATA       2       // 内核数据段
#define SEG_UTEXT       3       // 用户代码段
#define SEG_UDATA       4       // 用户数据段
#define SEG_TSS         5       // 任务状态段

/* global descrptor numbers */
#define GD_KTEXT        ((SEG_KTEXT) << 3)
#define GD_KDATA        ((SEG_KDATA) << 3)
#define GD_UTEXT        ((SEG_UTEXT) << 3)
#define GD_UDATA        ((SEG_UDATA) << 3)
#define GD_TSS          ((GD_TSS) << 3)

// 段权限
#define DPL_KERNEL  (0)
#define DPL_USER    (3)

// 段选择子
#define KERNEL_CS   ((GD_KTEXT) | DPL_KERNEL)
#define KERNEL_DS   ((GD_KDATA) | DPL_KERNEL)
#define USER_CS     ((GD_UTEXT) | DPL_USER)
#define USER_DS     ((GD_UDATA) | DPL_USER)

#endif // _MEMLAYOUT_H_