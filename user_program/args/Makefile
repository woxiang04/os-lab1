# **** GNU make manual: http://www.gnu.org/software/make/manual/make.html ****

CC      = gcc
LD      = ld
CFLAGS  = -m32 -static -ggdb -MD -Wall -Werror -I../../include -I../ -O0 \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer -fno-align-functions -Wno-main
LDFLAGS = -melf_i386 -e _start

CFILES  = $(shell find ./ -maxdepth 1 -name "*.c")
SFILES  = $(shell find ./ -maxdepth 1 -name "*.S")
SYS		= $(shell find ../sys_call/ -name "*.o")
LIB     = ../../src/user_c_lib/*.o ../../src/kernel/manager/error.o ../start.o
OBJS    = $(CFILES:.c=.out) $(SFILES:.S=.out)
DUMP  = $(OBJS:.out=.dump)


compile: $(OBJS)

#xxd: $(ARRAYS)

$(OBJS): %.out : %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LIB) $(SYS)
	xxd -c 4 -s 0 -i $@ > $(subst out,ar,$@)

.PHONY: debug test
debug: $(OBJS) $(DUMP)

$(DUMP): %.dump : %.out
	objdump -D $< > debug/$@
	readelf -a $< > debug/$(subst dump,elf,$@)


clean:
	rm -f $(OBJS) $(CFILES:.c=.o) $(CFILES:.c=.d) *.dump *.elf
