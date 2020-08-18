disk=sdj
mount=/media/dmitry-us/Vertux
Version=0.1
arhive=/VertuxOS_arhive/build_$(Version)/
skip=90
CFLAGS  = -Wall -fno-builtin -nostdinc -nostdlib -std=c99 -pedantic-errors
Source=./VertuxOS/source/

Driver=mgr
#remote default
ip=192.168.1.103
user=dmitry-us
port=3045
scp=/usr/bin/scp
ssh=/usr/bin/ssh

all: fasm_c gcc unlock finish qemu 
remote_all: fasm_c gcc finish finish_remote qemu_remote
debug_remote: fasm gcc finish finish_remote
dd_remote:
	@sudo dd if=/dev/$(disk) of=.$(arhive)Image.mmcblk0
Boot_remote: fasm_c fasm gcc unlock finish dd_remote finish_remote
finish_remote: 
	$(scp) -P$(port) .$(arhive)Image.mmcblk0 $(user)@$(ip):/home/dmitry-us/OS/Image.mmcblk0
sync:
	$(scp) -r -P$(port) ./* $(user)@$(ip):/home/dmitry-us/OS
#----------------------------- Local
unlock:
	@/bin/echo 'Разблокировка носителя'
	@sudo chmod 777 /dev/$(disk)
	@sudo chmod 777 $(mount)
qemu:
	@/bin/echo 'Запуск'
	@qemu-system-x86_64 -no-reboot  -d in_asm,pcall -hda /dev/$(disk)
fasm:
	@/bin/echo 'Сборка Загрузчика'
	@/bin/fasm ./VertuxOS/Boot/MBR.asm 
	@/bin/fasm ./VertuxOS/Boot/Boot.asm 
fasm_c:
	@/bin/echo 'Компиляция *.asm'
	@find  ./VertuxOS -type f -name '*.asm' -exec /bin/fasm {} \;
debug_local: fasm fasm_c gcc unlock finish
	@qemu-system-x86_64 -no-reboot -d in_asm,pcall  -D ./log/kernel.log -m 1024M -monitor stdio -S -s -hdb /dev/$(disk) #-S -s
dd_:
	@sudo dd if=./VertuxOS/Boot/MBR.bin of=/dev/$(disk) bs=1 count=512 seek=$(skip)
Boot: fasm unlock dd_ finish qemu
format:
	@/bin/echo 'Форматирование носителя'
	@sudo umount /dev/$(disk)
	@sudo mkfs.vfat -n Vertux /dev/$(disk)
	@/bin/sleep 5
	@sudo mount -a /dev/$(disk) $(mount)
finish:
	@/bin/echo 'Сборка'
	
	@/bin/rm -rf $(Source)*
	@/bin/rm -rf $(mount)/*
	@/bin/cp -f ./VertuxOS/Boot/Boot.bin $(Source)BOOT
	@/bin/cp -f ./VertuxOS/KERNEL $(Source)KERNEL

	@find  ./VertuxOS/mgr/ -type f -iname 'makefile' -exec make -f {} +
	@/bin/cp -fR ./VertuxOS/mgr/ $(Source)$(Driver)

	@find  $(Source) -type f -name '*.tmp' -exec /bin/rm {} +
	@find  $(Source) -type f -name '*.asm' -exec /bin/rm {} +
	@find  $(Source) -type f -name '*.o' -exec /bin/rm {} +
	@find  $(Source) -type f -name '*.c' -exec /bin/rm {} +
	@find  $(Source) -type f -name '*.cpp' -exec /bin/rm {} +
	@find  $(Source) -type f -iname 'makefile' -exec /bin/rm {} +
	@find  $(Source) -type f -name 'make.ld' -exec /bin/rm {} +
	@find  $(Source) -type f -name '*.h' -exec /bin/rm {} +

	@/bin/sleep 3
	@sudo /bin/cp -r $(Source)* .$(arhive)
	@sudo /bin/cp -r $(Source)* $(mount)

	#@find  .$(arhive) -type f -name '*.asm' -exec /bin/rm {} +
	#@find  .$(arhive) -type f -name '*.o' -exec /bin/rm {} +
	#@find  .$(arhive) -type f -name '*.c' -exec /bin/rm {} +
	#@find  .$(arhive) -type f -name '*.cpp' -exec /bin/rm {} +
	#@find  .$(arhive) -type f -name '*.h' -exec /bin/rm {} +
	
gcc:
	@/bin/echo 'Дело за GCC...'
	@gcc -Iinclude $(CFLAGS) -c -m32 -ffreestanding -o ./VertuxOS/kernel.o ./VertuxOS/kernel.c
	@ld --oformat=binary -melf_i386 -T LD.script -o ./VertuxOS/KERNEL ./VertuxOS/kernelasm.o ./VertuxOS/kernel.o
	@ld -melf_i386 -T DebugLD.script -o ./VertuxOS/KERNEL.gdb ./VertuxOS/kernelasm.o ./VertuxOS/kernel.o 
	@/usr/bin/objdump -D ./VertuxOS/KERNEL.gdb > Debug
Format_z:
	@/bin/echo 'Отчистка носителя'
	@sudo dd if=/dev/zero of=/dev/$(disk) bs=512 count=128M
clear:
	@/bin/sleep 5
	@find  ./VertuxOS -type f -name '*.o' -exec rm {} +



