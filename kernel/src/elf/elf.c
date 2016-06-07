#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	
	/* Load each program segment */
	int i;
	for(i=0;i<elf->e_phnum;i++ ) {
		/* Scan the program header table, load each segment into memory */
		ph=(void *) elf->e_phoff+i*elf->e_phentsize;
		if(ph->p_type == PT_LOAD) {
 
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
     		if(brk < new_brk) { brk = new_brk; }
			uint32_t pa=mm_malloc(ph->p_vaddr,ph->p_memsz);
#endif  
#ifndef HAS_DEVICE
			ramdisk_read((void*)pa + KOFFSET, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz); 
#else
			ide_read((void*)pa + KOFFSET, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
			Log("%x\n", i);
#endif
			memset((void*)(pa + ph->p_filesz + KOFFSET), 0, ph->p_memsz - ph->p_filesz);
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}

