#! Makefile

# 定义目录
BIN= ../bin
OBJS= ../objs
TOOLS= ./tools
SRC= .
INCLUDE_DIR= ./libs ./kernel/driver
INCLUDE= $(addprefix -I, $(INCLUDE_DIR))
# 定义编译工具
CC = gcc
LD = ld 
ASM = nasm

# 定义编译选项
C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-pic 
C_FLAGS += -fno-builtin -fno-stack-protector $(INCLUDE)

LD_FLAGS = -T tools/kernel.ld -m elf_i386 -nostdlib

ASM_FLAGS = -f elf -g -F stabs

DEV_LOOP= /dev/loop4

# 原文件
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst ./%.c, $(OBJS)/%.o, $(C_SOURCES))

S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst ./%.s, $(OBJS)/%.o, $(S_SOURCES))

.PHONY: ECHO
ECHO:
	@echo $(S_SOURCES)
	@echo $(S_OBJECTS)

# 定义目标
all: MASTER_IMG vmdk qemu

$(OBJS)/%.o: $(SRC)/%.c
	$(shell mkdir -p $(dir $@))
	@echo 编译C代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

$(OBJS)/%.o: $(SRC)/%.s
	$(shell mkdir -p $(dir $@))
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $< -o $@

$(BIN)/TinyOS.kernel: $(S_OBJECTS) $(C_OBJECTS)
	$(shell mkdir -p $(dir $@))
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o $@


# 生成虚拟硬盘
$(BIN)/master.img: $(TOOLS)/mk_grub_cfg $(BIN)/TinyOS.kernel
	$(shell mkdir -p $(dir $@))
	dd bs=512 if=/dev/zero of=$@ count=20480
	sudo losetup $(DEV_LOOP) $@
	sudo mkfs.ext4 -q $(DEV_LOOP)
	mkdir -p $(BIN)/hdisk
	sudo mount -o loop $@ $(BIN)/hdisk/
	sudo mkdir -p $(BIN)/hdisk/boot
	sudo grub-install --boot-directory=$(BIN)/hdisk/boot/ --force --allow-floppy $(DEV_LOOP)
# 使用工具写入配置文件信息
	@echo 生成grub.cfg...
	$(shell sudo $(TOOLS)/mk_grub_cfg)
	sudo mv ./grub.cfg $(BIN)/hdisk/boot/grub
	sudo cp $(BIN)/TinyOS.kernel $(BIN)/hdisk/boot/TinyOS.bin
# 取消挂载
	sudo umount -v $(BIN)/hdisk
	sudo losetup -d $(DEV_LOOP) 

.PHONY: MASTER_IMG
MASTER_IMG: $(BIN)/master.img

$(TOOLS)/mk_grub_cfg: $(TOOLS)/mk_grub_cfg.cpp
	$(CC) $< -o $@


# qemu启动和调试
QEMU:= qemu-system-i386
QEMU+= -m 32M	# 内存
QEMU+= -audiodev pa,id=hda	# 音频设备
# QEMU+= -machine pcspk-audiodev=hda #pcspeaker 设备
QEMU+= -rtc base=localtime # 设备本地时间
QEMU+= -drive file=$(BIN)/master.img,if=ide,index=0,media=disk,format=raw # 主硬盘
# QEMU+= -drive file=$(BUILD)/slave.img,if=ide,index=1,media=disk,format=raw # 从硬盘
QEMU_DISK_BOOT:= -boot c
QEMU_DEBUG:= -s -S

.PHONY: qemu
qemu: $(BIN)/master.img
	$(QEMU) $(QEMU_DISK_BOOT)

.PHONY: qemug
qemug: $(IMAGES)
	$(QEMU) $(QEMU_DISK_BOOT) $(QEMU_DEBUG)


# VMWare 磁盘转换
$(BIN)/master.vmdk: $(BIN)/master.img
	qemu-img convert -O vmdk $< $@

.PHONY: vmdk
vmdk: $(BIN)/master.vmdk

.PHONY: clean
clean:
	rm -rf $(BIN)
	rm -rf $(OBJS)