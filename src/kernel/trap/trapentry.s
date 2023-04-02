section .text
GD_KDATA equ 2 << 3
[GLOBAL __alltraps]
[EXTERN trap]
__alltraps:
    ; 保存寄存器
    push ds
    push es
    push fs
    push gs
    pusha

    ; 设置数据段和扩展段寄存器位内核代码段
    mov eax, GD_KDATA
    mov ds, ax
    mov es, ax

    ; 保存 esp
    push esp

    ; 真正的处理函数
    call trap

 
    pop esp
; 处理结束返回
[GLOBAL __trapret]
__trapret:
    popa

    pop gs
    pop fs
    pop es
    pop ds

    ; 丢弃中断向量号和错误码
    add esp, 0x8
    iret

    