#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char* argv[]) {
    const char* filename = "./grub.cfg";
    FILE* pfile = fopen(filename, "w");
    if (pfile == NULL) {
        printf("error in open %s\n", filename);
        return -1;
    }
    
    fprintf(pfile, "menuentry 'TinyOS' {\n");
    fprintf(pfile, "insmod part_msdos\n");
    fprintf(pfile, "insmod ext2\n");
    fprintf(pfile, "set root='hd0'                 #我们的硬盘只有一个分区所以是'hd0'\n");
    fprintf(pfile, "multiboot /boot/TinyOS.bin     #加载boot目录下的HelloOS.bin文件\n");
    fprintf(pfile, "boot                            #引导启动\n");
    fprintf(pfile, "}\n");
    fprintf(pfile, "set timeout_style=menu\n");
    fprintf(pfile, "if [ \"${timeout}\" = 0 ]; then\n");
    fprintf(pfile, "  set timeout=10                # 等待10秒钟自动启动\n");
    fprintf(pfile, "fi\n");

    fclose(pfile);

    return 0;
}


