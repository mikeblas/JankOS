
#include "stdio.h"
#include "multiboot.h"

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void multiboot_dump(multiboot_info_t* mbi)
{
	/* Print out the flags. */
	printf("flags = 0x%x\n", (unsigned) mbi->flags);

	/* Are mem_* valid? */
	if (CHECK_FLAG(mbi->flags, 0))
	printf("mem_lower = %uKB, mem_upper = %uKB\n",
		(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_FLAG(mbi->flags, 1))
		printf("boot_device = 0x%x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_FLAG(mbi->flags, 2))
		printf("cmdline = %s\n", (char *) mbi->cmdline);

	/* Are mods_* valid? */
	if (CHECK_FLAG(mbi->flags, 3))
	{
		multiboot_module_t *mod;
		multiboot_uint32_t i;

		printf("mods_count = %d, mods_addr = 0x%x\n",
			(int) mbi->mods_count, (int) mbi->mods_addr);
		for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
			i < mbi->mods_count;
			i++, mod++) {
				printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
					(unsigned) mod->mod_start,
					(unsigned) mod->mod_end,
					(char *) mod->cmdline);
		}
	}

	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
	{
		printf("Both bits 4 and 5 are set.\n");
		return;
	}

	/* Is the symbol table of a.out valid? */
	if (CHECK_FLAG(mbi->flags, 4))
	{
		multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

		printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
			"strsize = 0x%x, addr = 0x%x\n",
			(unsigned) multiboot_aout_sym->tabsize,
			(unsigned) multiboot_aout_sym->strsize,
			(unsigned) multiboot_aout_sym->addr);
	}

	/* Is the section header table of ELF valid? */
	if (CHECK_FLAG(mbi->flags, 5))
	{
		multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

		printf("multiboot_elf_sec: num = %u, size = 0x%x,"
			" addr = 0x%x, shndx = 0x%x\n",
			(unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
			(unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
	}

	/* Are mmap_* valid? */
	if (CHECK_FLAG(mbi->flags, 6))
	{
		multiboot_memory_map_t *mmap;
		int index = 0;

		printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
			(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
		for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
			(unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
			mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
				printf(" %d: size = 0x%x, base_addr = 0x%x%08x,"
					" length = 0x%x%08x, type = 0x%x\n",
					index++,
					(unsigned) mmap->size,
					(unsigned) (mmap->addr >> 32),
					(unsigned) (mmap->addr & 0xffffffff),
					(unsigned) (mmap->len >> 32),
					(unsigned) (mmap->len & 0xffffffff),
					(unsigned) mmap->type);
			}
	}

	/* do we have drives? */
	if (CHECK_FLAG(mbi->flags, 7))
	{
		printf("drives_length = 0x%x, drives_addr = 0x%08x\n", mbi->drives_length, mbi->drives_addr);
	}

	if (mbi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
	{
		printf("boot loader name = \"%s\"\n", mbi->boot_loader_name);
	}
}

