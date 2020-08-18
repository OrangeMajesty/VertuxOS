#pragma once
// ver 0.1

// Type 
#include "technologies/PATA.h"
// filesystem
#include "technologies/FAT32.h"

// struct StructFat32;

unsigned char HddPoint;
unsigned char buffer_[0x400];

typedef struct StructHDDPartition 
{
	unsigned char id;
	unsigned long maxLBA;
	unsigned char filesystem;
	unsigned long* StructFilesystem;
} HDD_Partition;
// 
struct StructHDD
{
	unsigned char MasterSlave;
	unsigned char NumPartition;
	unsigned char Type;
	/*
	0x0 - undefined
	0x1 - PATA
	*/
	unsigned char LBA28_48_CHS_Mode;
	HDD_Partition HDDPartition[MaxHDDPartition];
	void *pci;
} HDD[MaxHDD];
void WaitTick(unsigned long Tick);
unsigned long malloc(unsigned long long NBlocks, char type);
void free(unsigned long virtual, unsigned int block, unsigned char type);
char PATA_ReadLBA28(char device,unsigned char size ,unsigned long Sector, void *Buffer);

struct f_struct_ {
	char filename[255];
	char exp;
	char attribute;
	unsigned long EIP;
	unsigned long CSbase, CSlong;
	unsigned long DSbase, DSlong;
	unsigned long FSbase, FSlong;
	unsigned long ESbase, ESlong;	// может использоваться как временная переменая при загрузке
	unsigned long GSbase, GSlong; 	// может использоваться как временная переменая при загрузке
	unsigned long LBA;
	unsigned long SearchLBA;
	unsigned long long size;
} file_preloader;

// header file
#include "technologies/format/FUse.h"


char GetThisFileSys(unsigned char NumHDD, unsigned char partitionn) {
	char *vbuf = "FAT32?FAT16?!"; // EB 58 90 – безусловный переход и сигнатура;

	unsigned char * To = 0;
	unsigned char * addr = (void*)buffer_;

	unsigned int PointVbuf = 0;
	unsigned int trueVbuf = 0;
	unsigned int trueAddr = 0;

	vbuf+=36;
	addr+=0x52;

	while(*vbuf != '!'){

		if(*vbuf == '?') {
			PointVbuf++;

			if(trueAddr == trueVbuf) {
				break;
			}

			trueVbuf = 0;
			trueAddr = 0;

			addr = (void*)buffer_;
			addr += 0x52; 			// 0x2e + 0x24(offset to functoion)

			vbuf++;
		}
		if(*vbuf == *addr)
			trueVbuf++;
		
		vbuf++;
		addr++;

		trueAddr++;
	}

	switch(PointVbuf){
		case 0x1: 
			HDD[NumHDD].HDDPartition[partitionn].filesystem = PointVbuf;
			HDD[NumHDD].HDDPartition[partitionn].StructFilesystem = (void*)malloc(1,0);// Выделяем память для структуры
			// struct StructFat32 *FAT32_0x1 = (void*)HDD[NumHDD].HDDPartition[partitionn].StructFilesystem;
			addr = (void*)buffer_;
			To = (void*)HDD[NumHDD].HDDPartition[partitionn].StructFilesystem;
			trueVbuf = 0;
			while(trueVbuf != 0x5a){
				*To = *addr;
				To++;
				addr++;
				trueVbuf++;
			}
		break; //FAT32
		case 0x2: break; // FAT16
	}
	return PointVbuf;
}
unsigned char HDDinit(unsigned char TypeConnect) {
	unsigned int partitionn = 0;
	if (HddPoint == MaxHDD) {
		return 0x9; // Превышение возможный подключенных носителей.
	}
	switch(TypeConnect){
		case 0x00:  break;
		case 0x01: 
			// Проверить поддерживаемые режимы. LBA CHS
			if(PATA_ReadLBA28(1, 1, 0, &buffer_[0]) == 0x0) {
				// Разбор файловой системы в MBR.
				HDD[HddPoint].LBA28_48_CHS_Mode += 0x1 + 0x8;
				HDD[HddPoint].MasterSlave = 0x1;
				HDD[HddPoint].Type = 0x1; // PATA

				// Далее настройки получаем от MBR
				
				HDD[HddPoint].HDDPartition[partitionn].id = 0;
				HDD[HddPoint].HDDPartition[partitionn].maxLBA = 0;
				HDD[HddPoint].HDDPartition[partitionn].filesystem = 0;
				// GetThisFileSys(HddPoint, partitionn);
			}
			if(PATA_ReadLBA28(0, 1, 0, (void*)&buffer_[0]) == 0x0) {
				// Разбор файловой системы в MBR.
				HDD[HddPoint].LBA28_48_CHS_Mode += 0x1 + 0x8;
				HDD[HddPoint].MasterSlave = 0x0;
				HDD[HddPoint].Type = 0x1; // PATA

				// Далее настройки получаем от MBR
				HDD[HddPoint].HDDPartition[partitionn].id = 0;
				HDD[HddPoint].HDDPartition[partitionn].maxLBA = 0;
				HDD[HddPoint].HDDPartition[partitionn].filesystem = 0;
				// GetThisFileSys(HddPoint, partitionn);
			}
		break; // PATA
		case 0x02: break;
		case 0x03: break;
		case 0x04: break;
		case 0x05: break;
		case 0x06: break;
		case 0x07: break;
		case 0x08: break;
		case 0x80: break;
		default: break;
	}
	HddPoint++;
	return 0;
}
char ReadHDD(unsigned short NumHDD, void* Buffer, unsigned char size, unsigned long readsector){
	unsigned char Hdd = NumHDD >> 8;
	unsigned char partition = NumHDD & 0xff;

	switch(HDD[Hdd].Type){
		case 0x0: break;
		case 0x1:
			if(HDD[Hdd].LBA28_48_CHS_Mode & 0x4){	//	pata48
				PATA_ReadLBA48();
			} else if(HDD[Hdd].LBA28_48_CHS_Mode & 0x8){ // pata28
				if(!PATA_ReadLBA28(HDD[Hdd].MasterSlave, size, readsector, Buffer))
					return 0x1; // Read Error
			} else {
				PATA_ReadCHS();
			}
		break;
	}
	// if (No LBA support)
//    Use CHS.
// else if (the LBA Sector Address > 0x0FFFFFFF)
//    Use LBA48.
// else
//    Use LBA28.
	return 0;
}
// int strcmp(char a[], char h[]);
char fopen(char *Patch, unsigned short NumHDD) {
	char* patch = (void*)Patch;
	patch+=36;

	unsigned char Hdd = NumHDD >> 8;
	unsigned char partition = NumHDD & 0xff;

	char* OLDpatch;

	char* TLink;
	char* SDirFs;

	if(!HDD[Hdd].HDDPartition[partition].filesystem){
		GetThisFileSys(Hdd, partition);
	}

	switch(HDD[Hdd].HDDPartition[partition].filesystem) {
		case 0x1: 
			StructFat32 = (void*)HDD[Hdd].HDDPartition[partition].StructFilesystem;
			if(!StructFat32->KlusterRootDir){
				StructFat32->KlusterRootDir = StructFat32->BPB_RsvdSecCnt + (StructFat32->BPB_NumFATs * StructFat32->BPB_FATSz32);
			}

			TLink = (void*)malloc((0x200*StructFat32->BPB_SecPerClus),1);
			SDirFs = (void*)HDD[Hdd].HDDPartition[partition].StructFilesystem;
			SDirFs += (sizeof(struct FAT32_));
			StructFat32Dir = (void*)SDirFs;
				
			for(unsigned int kluster = StructFat32->KlusterRootDir; ; kluster++){
				if(ReadHDD(NumHDD, TLink, 1, kluster) != 0x0){
					// printf("\nFileManager: HDD ERROR");
				}
				// printf("\n------------\n");
				// HexInAsciiSz(kluster,8);
				// printf(" Kluster\n");
				for (int i = 0; i != 0x200; i+=0x20) {
					for (int y = 0; y != 0x20; y++) {
						SDirFs[y] = TLink[y+i];
					}
					if(StructFat32Dir->filename[0] != 0x0){ // Eсли файл пресyтствует
						if(*patch == '/'){
							patch++;	
						}
						// HexInAsciiSz((int)StructFat32Dir->filename[0]<<24,2);
						// printf(" "); 
						OLDpatch = (void*)patch;
						for(int o = 0; *patch != '/';){
							if(*patch == '.'){ // Подстройка строки под стандарты ФС
								patch++;
								while(StructFat32Dir->filename[o] == 0x20){
								// printf("\nOOPS\n");
									if(o > 12){
										// PANIC;
										break;
									}
									o++;
								}
							}
							// printf("|to 0x");
							// HexInAsciiSz(*patch<<24,2);
							if(*patch >= 'a' && *patch <= 'z'){
								*patch-=0x20;
								if(*patch != (char)StructFat32Dir->filename[o]){ // продолжение подстройки строки под стандарты ФС
									// clrscr();
									*patch+=0x20;
									patch = (void*)OLDpatch;
									break;
								}
								// printf(" do 0x");
								// HexInAsciiSz(*patch<<24,2);
								*patch+=0x20;
							} else {
								if(*patch != (char)StructFat32Dir->filename[o]){
									patch = (void*)OLDpatch;
									break;
								}
							}
							o++;
							patch++;
							
							if(*patch == 0x0){
								if(StructFat32Dir->DirAttr == ATTR_ARCHIVE){
									// printf("\nFileFound!");
									// Заполняем структуру
									// HexInAsciiSz(kluster,8);

									free((unsigned long)TLink,(0x200*StructFat32->BPB_SecPerClus),1);// освобождаем занятую память

									for (int tmp = 0; tmp != 11; tmp++) {
										file_preloader.filename[tmp] = StructFat32Dir->filename[tmp];
									}
									file_preloader.size = StructFat32Dir->Dir_Filesize;
									file_preloader.attribute = StructFat32Dir->DirAttr;
									file_preloader.LBA = ((StructFat32Dir->Dir_FstClusHI+StructFat32Dir->Dir_FstClusLO - 2)*8)+StructFat32->KlusterRootDir;
									file_preloader.SearchLBA = kluster;

									TLink = (void*)malloc(file_preloader.size,1);

									// SDirFs = (void*)malloc(file_preloader.size,0);
									for(unsigned long Dec = 0; Dec >= file_preloader.size; Dec += 0x1000) {
										ReadHDD(NumHDD, TLink, 1*StructFat32->BPB_SecPerClus, file_preloader.LBA+(Dec/0x1000)); // Load Full file
										
									}
									// clrscr();
									// HexInAsciiSz((int)SDirFs,8);
									FUseHeaderExtract(FUseGetFileFormat(TLink),TLink); // return GS

									SDirFs = (void*)malloc((file_preloader.GSbase+file_preloader.GSlong),1);

									HexInAsciiSz((int)TLink,8);
									printf("\nBase ");
									HexInAsciiSz((int)file_preloader.GSbase,8);
									printf("\nlong ");
									HexInAsciiSz((int)file_preloader.GSlong,8);

									PANIC;
									while(1){}
									
									
									
									return 0;
								}
							}
							if(*patch == '/'){
								if(StructFat32Dir->DirAttr == ATTR_DIRECTORY){
									// printf("\nDIRFOUND\n");
									// printf("\nPATCH = '");
									// printf((char*)(patch-36));
									// printf("'\n");
									i = 0x200-0x20;
									kluster = ((StructFat32Dir->Dir_FstClusHI+StructFat32Dir->Dir_FstClusLO - 2)*8)+StructFat32->KlusterRootDir-1;
									break;
								}
							}
						}
					} else {
						free((unsigned long)TLink,(0x200*StructFat32->BPB_SecPerClus),1);// освобождаем занятую память
						printf("\nfile not found");
						return 0x2; //file not found 
					}
				}

			}
		break; // FAT32
		case 0x2: printf("\nUnsupported FileSystem"); return 0x3; break; // FAT16
		default: return 0x1; break; // Filesystem undefined
	}
	// free((unsigned long)TLink, 2 ,0);// освобождаем занятую память
	return 0;
}