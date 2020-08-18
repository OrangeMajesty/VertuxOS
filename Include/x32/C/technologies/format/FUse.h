#include "ELF86.h"

#pragma once

char ReadHDD(unsigned short NumHDD, void* Buffer, unsigned char size, unsigned long readsector);
unsigned long malloc(const unsigned long long NBlocks, char type);

char FUseHeaderExtract(char FileFormat, void* AddressBlock) {
	char* addr = AddressBlock;
	// char* Tlink;

	switch(FileFormat){
		case 0x1: // ELF
			if(addr[4] == 0x2) { // (64 or 32) bit
				printf("\nELF x64 not supported");
				Elf64_Ehdr = AddressBlock;

			} else { // 32
				Elf32_Ehdr = AddressBlock;
				file_preloader.EIP = Elf32_Ehdr->e_entry;
				Elf32_Phdr = (void*)(((int)(Elf32_Ehdr->e_phentsize)*0) + (int)(Elf32_Ehdr->e_phoff) + (int)(Elf32_Ehdr));
				// Elf32_Shdr = (void*)(((int)(Elf32_Ehdr->e_shentsize)*0) + (int)(Elf32_Ehdr->e_shoff) + (int)(Elf32_Ehdr));
				clrscr();
				printf("Format: ");
				printf((char*)Elf32_Ehdr->e_ident-35);
				printf(" x32");
				printf("\nVersion: 0x");
				HexInAsciiSz(addr[6]<<24, 2);
				printf(".");
				HexInAsciiSz(Elf32_Ehdr->e_version, 8);
				printf("\nEntry point: 0x");
				HexInAsciiSz(Elf32_Ehdr->e_entry,8);
				printf("\n\nTable base: 0x");
				HexInAsciiSz((int)AddressBlock,8);
				printf("\nTable Loffset: 0x");
				HexInAsciiSz((int)Elf32_Ehdr->e_shoff,8);
				printf("\nTable Goffset: 0x");
				HexInAsciiSz((int)Elf32_Shdr,8);
				printf("\nshtrndx: 0x");
				HexInAsciiSz((int)Elf32_Ehdr->e_shtrndx,8);
				printf("\n-------------------\n");


// PANIC;
				HexInAsciiSz((int)Elf32_Phdr,8);
				printf("\n");
				for (int i = 0; i != Elf32_Ehdr->e_phnum; i++)
				{
					Elf32_Phdr = (void*)(((int)(Elf32_Ehdr->e_phentsize)*i) + (int)(Elf32_Ehdr->e_phoff) + (int)(Elf32_Ehdr));
					if(Elf32_Phdr->p_type == 0x1) { // LOAD
						file_preloader.GSbase = Elf32_Phdr->p_offset;
						file_preloader.GSlong = Elf32_Phdr->p_filesz;

					}
					// HexInAsciiSz(i<<24,2);
					// printf(". ");
					// switch((char)Elf32_Phdr->p_type){
					// 	case 0x0: printf("Null"); break;
					// 	case 0x1: printf("Load"); break;
					// 	// case 0x2: printf("Dynamic"); break;
					// 	// case 0x3: printf("Interp"); break;
					// 	// case 0x4: printf("Note"); break;
					// 	// case 0x6: printf("PHDR"); break;
					// 	// case 0x7: printf("TLS"); break;
					// 	default:  printf("OFF"); break;
					// }
					// printf("  offset 0x");
					// HexInAsciiSz(Elf32_Phdr->p_offset,8);
					// printf("  vaddr 0x");
					// HexInAsciiSz(Elf32_Phdr->p_vaddr,8);
					// printf("  paddr 0x");
					// HexInAsciiSz(Elf32_Phdr->p_paddr,8);
					// printf("\n    memsz 0x");
					// HexInAsciiSz(Elf32_Phdr->p_memsz,8);
					// printf("  filesz 0x");
					// HexInAsciiSz(Elf32_Phdr->p_filesz,8);
					// printf("  flags 0x");
					// HexInAsciiSz(Elf32_Phdr->p_flags,8);
					// printf("  allign 0x");
					// HexInAsciiSz(Elf32_Phdr->p_align,8);
					// printf("\n");

				}
				// printf("------------\n");
				// Tlink = (void*)malloc(file_preloader.size,1);

				// ReadHDD(0, Tlink, file_preloader.size/0x200, file_preloader.LBA);
				// Tlink += file_preloader.EIP;
				// clrscr();

			}
		break;

		case 0x0:
		default: printf("File format not supported"); break;
	}

	// file_preloader.DSbase = ;
	// file_preloader.DSlong = ;

	// file_preloader.CSbase = ;
	// file_preloader.CSlong = ;

	// file_preloader.ESbase = ;
	// file_preloader.ESlong = ;

	// file_preloader.FSbase = ;
	// file_preloader.FSlong = ;

	// file_preloader.GSbase = ;
	// file_preloader.GSlong = ;

	return 0;
}
char FUseGetFileFormat(void* AddressBlock){ // Блок с заголовками
	void* FileFormat[] = {&ELF_IDENT,0}; // Поддерживаемые Форматы. Добавлять в конец списка до '0'.
	char* addr = AddressBlock;
	char* format ;
	for(int i = 0, Num = 0; FileFormat[Num] != 0x0; Num++){
		format = (void*)FileFormat[Num];
		format+=36;
		i = 0;
		while(format[i] == addr[i]){
			if(format[i+1] == 0x0){
				return Num+1;
			}
		i++;
		}
	}
	return 0;
}
	// unsigned char Num = 0;

		// HexInAsciiSz((long)format,8); 
		// printf(" \nELF - ");
		// HexInAsciiSz((long)ELF_IDENT,8);
		// printf(" \nAddress - ");
		// HexInAsciiSz((long)AddressBlock,8);
		// printf(" \nformat - ");
		// format = (void*)((FileFormat[i]) + 36);
		// format = ELF_IDENT + 36;
		// format = (void*)FileFormat[Num];
		// format+=36;
		// HexInAsciiSz((long)format,8);
		// printf(" \n");

		// for (int i = 0; i < 0xf; i++)
		// {	
		// 		printf("\n");
		// 		HexInAsciiSz((long)addr[i],8);
		// 		printf("\t");
		// 		HexInAsciiSz((long)format[i],8);
		// 		printf("\t");
		// 		HexInAsciiSz((long)FileFormat[i+Num],8);
		// }