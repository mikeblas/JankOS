#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "multiboot_helper.h"

#include "terminal.h"
#include "stdio.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(unsigned long magic, multiboot_info_t* mbi)
{

	/* Initialize terminal interface */
	terminal_initialize();

	/* Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		return;
	}

	/* write a few lines */
	/*
	printf("Hello, kernel World!\nSecond line\nThird Line\n");
	printf("Fourth Line\nFifth line\nSixth Line\n");
	printf("Seventh Line\nEigth Line\nNinth Line\n");
	*/

	/* scroll up one line, then keep writing */
	terminal_scroll_up(1);
	printf("After scroll up\n");

	printf("magic number is 0x%x\n", (unsigned) magic);
	printf("pointer is 0x%x\n", (unsigned) mbi);

	multiboot_dump(mbi);
}

