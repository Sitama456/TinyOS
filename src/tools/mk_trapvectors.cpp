#include <stdio.h>
#include <set>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("please input the filename");
        return -1;
    }
    const char* name = argv[1];
    FILE *fp = fopen(name, "w");
    if (fp == NULL) {
        printf("error with open file %s", name);
        return -1;
    }
    std::set<int> special = {8, 10, 11, 12, 13, 14, 17};

    fprintf(fp, ";中断处理函数\n");
    fprintf(fp, "[BITS 32]\n");\
    fprintf(fp, "section .text\n");
    fprintf(fp, "[GLOBAL __alltraps]\n");
    for (int i = 0; i < 256; ++i) {
        fprintf(fp, "[GLOBAL vector%d]\n", i);
        fprintf(fp, "vector%d:\n", i);
        if (special.count(i) == 0) {
            // 用压入错误码的
            fprintf(fp, "   push 0\n");
        }
        // 压入中断号
        fprintf(fp, "   push %d\n", i);
        fprintf(fp, "   jmp __alltraps\n");
    }
    fprintf(fp, "\n");
    fprintf(fp, "; vector table\n");
    fprintf(fp, "section .data\n");
    fprintf(fp, "[GLOBAL __vectors]\n");
    fprintf(fp, "__vectors:\n");
    for (int i = 0; i < 256; ++i) {
        fprintf(fp, "   dd vector%d\n", i);
    }
    fclose(fp);
}