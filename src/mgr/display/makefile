dirname=display
CFLAGS  = -O3 -Wall -fno-builtin -nostdinc -nostdlib -std=c99 -pedantic-errors -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables
LDFLAGS = -melf_i386 
sqr=./VertuxOS/mgr/$(dirname)
setld=make.ld
all: Sgcc
#OUTPUT_FORMAT("elf")
#     . = ALIGN(4096);
Sgcc:
	@/bin/echo 'Сборка Дисплей-менеджера'
	@gcc -Iinclude $(CFLAGS) -c -m32 -ffreestanding -o $(sqr)/init.o $(sqr)/init.c
	@gcc -Iinclude $(CFLAGS) -c -m32 -ffreestanding -o $(sqr)/header.o $(sqr)/header.c
	@ld $(LDFLAGS) -T $(sqr)/$(setld) -o $(sqr)/init.mgr $(sqr)/header.o $(sqr)/init.o
	@hexdump $(sqr)/init.mgr > $(sqr)/HexInitMgr.tmp
	@/usr/bin/objdump -x $(sqr)/init.mgr > init.Debug
	@echo '-----------' >> init.Debug
	@/usr/bin/objdump -D $(sqr)/init.mgr >> init.Debug
	@echo '-----------' >> init.Debug
	@/usr/bin/objdump -s $(sqr)/init.mgr >> init.Debug
