

jankos.iso :	jankos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp jankos.bin isodir/boot/jankos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o jankos.iso isodir

jankos.bin :      boot.o kernel.o linker.ld stdio.o terminal.o multiboot_helper.o
	i686-elf-gcc -T linker.ld -o jankos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o stdio.o terminal.o multiboot_helper.o -lgcc

boot.o :	boot.s
	i686-elf-as boot.s -o boot.o

kernel.o :	kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

stdio.o :	stdio.c stdio.h
	i686-elf-gcc -c stdio.c -o stdio.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o :	terminal.c terminal.h
	i686-elf-gcc -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

multiboot_helper.o :	multiboot_helper.c multiboot_helper.h
	i686-elf-gcc -c multiboot_helper.c -o multiboot_helper.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


clean :
	rm -f boot.o
	rm -f kernel.o
	rm -f stdio.o terminal.o
	rm -f jankos.bin
	rm -f jankos.iso
