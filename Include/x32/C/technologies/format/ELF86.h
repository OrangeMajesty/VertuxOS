#pragma once

char ELF_IDENT[] = {0x7f,'E','L','F',0x0};

#define EI_NIDENT 16

struct {
        unsigned char   e_ident[EI_NIDENT];
        unsigned short      e_type;
        unsigned short      e_machine;
        unsigned int      	e_version;
        unsigned int      	e_entry;
        unsigned int      	e_phoff;
        unsigned int      	e_shoff;
        unsigned int      	e_flags;
        unsigned short      e_ehsize;
        unsigned short      e_phentsize;
        unsigned short      e_phnum;
        unsigned short      e_shentsize;
        unsigned short      e_shnum;
        unsigned short      e_shtrndx;
}  __attribute__((packed))  *Elf32_Ehdr;

struct {
    	uint32_t   sh_name;
    	uint32_t   sh_type;
    	uint32_t   sh_flags;
    	uint32_t   sh_addr;
    	uint32_t   sh_offset;
    	uint32_t   sh_size;
    	uint32_t   sh_link;
    	uint32_t   sh_info;
    	uint32_t   sh_addralign;
    	uint32_t   sh_entsize;
}   __attribute__((packed)) *Elf32_Shdr;

struct {
    	uint32_t   p_type;
    	uint32_t   p_offset;
    	uint32_t   p_vaddr;
    	uint32_t   p_paddr;
    	uint32_t   p_filesz;
    	uint32_t   p_memsz;
    	uint32_t   p_flags;
    	uint32_t   p_align;
} __attribute__((packed)) *Elf32_Phdr;

struct {
        unsigned char   e_ident[EI_NIDENT];
//         Elf64_Half      e_type;
//         Elf64_Half      e_machine;
//         Elf64_Word      e_version;
//         Elf64_Addr      e_entry;
//         Elf64_Off       e_phoff;
//         Elf64_Off       e_shoff;
//         Elf64_Word      e_flags;
//         Elf64_Half      e_ehsize;
//         Elf64_Half      e_phentsize;
//         Elf64_Half      e_phnum;
//         Elf64_Half      e_shentsize;
//         Elf64_Half      e_shnum;
//         Elf64_Half      e_shtrndx;
}  __attribute__((packed)) *Elf64_Ehdr;

// typedef struct {
    // uint32_t   p_type;
    // uint32_t   p_flags;
    // Elf64_Off  p_offset;
    // Elf64_Addr p_vaddr;
    // Elf64_Addr p_paddr;
    // uint64_t   p_filesz;
    // uint64_t   p_memsz;
    // uint64_t   p_align;
// } Elf64_Phdr;

// ELFMAG0 0x7f e_ident[EI_MAG0]
// ELFMAG1 ’E’ e_ident[EI_MAG1]
// ELFMAG2 ’L’ e_ident[EI_MAG2]
// ELFMAG3 ’F’ e_ident[EI_MAG3]
// _ ______________________________________ 
// http://www.sco.com/developers/gabi/1998-04-29/ch4.eheader.html

void _ELF86EchoEMachine(char target) {
	switch(target) {
		case 0: printf("No Machine\n"); break;
		case 1: printf("AT&T WE 32100\n"); break;
		case 2: printf("SPARC\n"); break;
		case 3: printf("Intel 80386\n"); break;
		case 4: printf("Motorola 68000\n"); break;
		case 5: printf("Motorola 88000\n"); break;
		case 7: printf("Intel 80860\n"); break;
		case 8: printf("MIPS RS3000\n"); break;
		default: printf("Undefined\n");
	}
}