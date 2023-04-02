#ifndef _ELF_H_
#define _ELF_H_
#include "type.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

#define ELF_MAGIC   0x464C457FU

/*elf 文件头*/
typedef struct elfhdr {
    uint32_t e_magic;           // 魔数
    uint8_t e_elf[12];
    uint16_t e_type;            // 1=relocatable, 2=executable, 3=shared object, 4=core image
    uint16_t e_machine;         // 3=x86, 4=68K, etc.
    uint32_t e_version;         // file version, always 1
    uint32_t e_entry;           // entry point if executable
    uint32_t e_phoff;           // file position of program header or 0
    uint32_t e_shoff;           // file position of program header or 0
    uint32_t e_flags;           // architecture-specific flags, usually 0
    uint16_t e_ehsize;          // size of this elf header
    uint16_t e_phentsize;       // size of an entry in program header
    uint16_t e_phnum;           // number of entries in program header or 0
    uint16_t e_shentsize;       // size of an entry in section header
    uint16_t e_shnum;           // number of entries in section header or 0
    uint16_t e_shstrndx;        // section number that contains section name strings
} elfhdr_t;

/* 程序节头 */
typedef struct elf_program_header {
    uint32_t p_type;   // loadable code or data, dynamic linking info,etc.
    uint32_t p_offset; // file offset of segment
    uint32_t p_va;     // virtual address to map segment
    uint32_t p_pa;     // physical address, not used
    uint32_t p_filesz; // size of segment in file
    uint32_t p_memsz;  // size of segment in memory (bigger if contains bss）
    uint32_t p_flags;  // read/write/execute bits
    uint32_t p_align;  // required alignment, invariably hardware page size
} proghdr_t;

typedef struct elf_section_header {
    uint32_t name;      
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} elf_section_header_t;

/* elf 格式符号 */
typedef struct elf_symbol {
    uint32_t name;      // 名字
    uint32_t value;     // 值
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;     // 属于哪个节
} elf_symbol_t;

typedef struct elf
{
    elf_symbol_t *symtab;   // 符号表
    uint32_t symtabsz;  // 符号表大小
    const char *strtab; // 字符串表
    uint32_t strtabsz;  // 字符串表大小
} elf_t;

// 解析 multiboot 中elf的符号表
elf_t elf_from_multiboot(multiboot_t *mb);

// 通过地址给出函数名字
const char *elf_lookup_symbol(uint32_t addr, elf_t *elf);

#endif // _ELF_H_