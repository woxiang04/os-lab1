CC      = gcc
LD      = ld
CFLAGS  = -m32 -static -ggdb -MD -Wall -Werror -I./include -O0 \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer
ASFLAGS = -m32 -MD -I./include
LDFLAGS = -melf_i386
QEMU    = qemu-system-i386

CFILES  = $(shell find src/ -name "*.c")
SFILES  = $(shell find src/ -name "*.S")
OBJS    = $(CFILES:.c=.o) $(SFILES:.S=.o)

run: hardDisk
	$(QEMU) -no-shutdown -serial stdio harddisk/harddisk.img

debug: hardDisk
	gnome-terminal -e "bash -c \"gdb -s kernel; exec bash\""
	$(QEMU) -serial stdio -s -S harddisk/harddisk.img

hardDisk: disk.img
	@cd harddisk; make run

disk.img: kernel
	@cd boot; make
	cat boot/bootblock kernel > disk.img

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -e os_init -Ttext 0xC0100000 -o kernel $(OBJS)
	objdump -D kernel > code.txt	# disassemble result
	readelf -a kernel > elf.txt		# obtain more information about the executable

harddisk.img: disk.img
	python3 harddisk/makeimg.py
	cat harddisk/harddisk disk.img > disk.img

-include $(OBJS:.o=.d)

clean:
	@cd boot; make clean
	rm -f kernel disk.img $(OBJS) $(OBJS:.o=.d)
	make disk.img

user_pro: lib_comp
	@cd user_program; make

lib_comp:
	@cd user_program; make systemcall

