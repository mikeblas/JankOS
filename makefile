

jankos.iso :	jankos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp jankos.bin isodir/boot/jankos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o jankos.iso isodir

jankos.bin :      boot.o kernel.o linker.ld
	i686-elf-gcc -T linker.ld -o jankos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

boot.o :	boot.s
	i686-elf-as boot.s -o boot.o

kernel.o :	kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


clean :
	rm -f boot.o
	rm -f kernel.o
	rm -f jankos.bin
	rm -f jankos.iso
