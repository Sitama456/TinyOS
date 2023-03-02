; -----------------------------------------
;	
; boot.s 内核从这里开始，从GRUB中接过CPU执行权
;
; --------------------------------------------
; Multiboot 魔术，由规范决定的
MBOOT_HEADER_MAGIC equ	0x1BADB002

; 0 号位表示所有的引导模块将按页(4KB)边界对齐
MBOOT_PAGE_ALIGN equ 1 << 0

; 1号位通过Multiboot的信息结构的mem_*域包括可用内存的信息
; 告诉GRUB把内存空间的信息包含在Multiboot信息结构中
MBOOT_MEM_INFO equ 1 << 1

; 定义适用的Multiboot的标记
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; 域checksum是一个32位的无符号值，要求magic+flags+checknum = 0
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; Multiboot规范要求这样一个magic Multiboot头
; -----------------------------------------------
; 偏移量 类型 名称  被指
;
; 0	u32	magic	必需
; 4	u32	flags 	必需
; 8	u32	checksum 必需
;
; 我们只需要了解这些就够了
;-------------------------------------------------
[BITS 32]	; 所有代码以32-bit的方式编译
section .text	; 代码段从这里开始

; 设置Multiboot头信息

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start] ;向外部声明内核代码入口，此处声明提供给链接器
[GLOBAL glb_mboot_ptr] ;向外部声明struct multiboot* 变量
[GLOBAL bootstack]
[GLOBAL bootstacktop]
[EXTERN kern_entry] ;引入内核C代码的入口函数

start:
    cli             ; 关中断、

    mov esp, bootstacktop       ; 设置内核栈指针
    mov ebp, 0                  ; 设置帧指针位0
    ; and esp, 0xFFFFFFF0       ; 栈地址按照16字节对齐
    mov [glb_mboot_ptr], ebx    ; 将ebx中存储的指针存入全局变量中
    call kern_entry             ; 调用内核入口函数
stop:
    hlt                         ; 停机指令，可以降低CPU功耗
    jmp stop                    ; 到这里结束


; --------------------------------------------------
section .bss                    ; 数据段
bootstack:
    resb 32768                  ; 内核栈 32K Byte
bootstacktop:
glb_mboot_ptr:                  ; 全局的multiboot指针
    resb 4
	

