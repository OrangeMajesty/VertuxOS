#pragma once

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN 	0x02
#define ATTR_SYSTEM 	0x04
#define ATTR_VOLUME_ID 	0x08
#define ATTR_DIRECTORY	0x10
#define ATTR_ARCHIVE  	0x20

struct FAT32_{
  char BS_jmpBoot[3];       // 0x00 jmp command to boot code
  char BS_OEMName[8];       // 0x03 OEM Name
  unsigned short BPB_BytsPerSec;    // 0x0b Bytes per sector 512,1024,2048 шыш 4096
  unsigned char BPB_SecPerClus;     // 0x0d Sectors per Cluster
  unsigned short BPB_RsvdSecCnt;    // 0x0e Reserved sectors count
  unsigned char BPB_NumFATs;        // 0x10 FATs number
  unsigned short BPB_RootEntCnt;    // 0x11 (not used for FAT32)
  unsigned short BPB_TotSec16;      // 0x13 (not used for FAT32)
  unsigned char BPB_Media;          // 0x15 (not used)
  unsigned short BPB_FATSz16;       // 0x16 (not used for FAT32)
  unsigned short BPB_SecPerTrk;     // 0x18 (SPT geometry for CHS)
  unsigned short BPB_NumHeads;      // 0x1a (HPC geometry for CHS)
  unsigned int BPB_HiddSec;       // 0x1c (Sectors before this partition)
  unsigned int BPB_TotSec32;      // 0x20 (Total sectors must be !=0 for FAT32)
  unsigned int BPB_FATSz32;       // 0x24 Sectors count in one FAT
  unsigned short BPB_ExtFlags;      // 0x28 See Microsoft document
  unsigned short BPB_FSVer;         // 0x2a Version of FAT32 filesystem
  unsigned int BPB_RootClus;      // 0x2c Root Directory cluster number
  unsigned short BPB_FSInfo;        // 0x30 Reserved = 1 usially see doc...
  unsigned short BPB_BkBootSec;     // 0x32 Sector in reserve area with Boot Sect Copy
  unsigned char BPB_Reserved[12];   // 0x34 
  unsigned char BS_DrvNum;          // 0x40 Drive number for int 0x13
  unsigned char BS_Reserved1;       // 0x41
  unsigned char BS_BootSig;         // 0x42 0x29 means that next fields present
  unsigned int BS_VolID;          // 0x43 Volume ID 32 bit value
  char BS_VolLab[11];       // 0x47
  char BS_FilSysType[8];    // 0x52 "FAT32   "
  unsigned long KlusterRootDir;		// 0x5a
} __attribute__((packed)) *StructFat32;	// 0x5e

struct FAT32_DIR{
	char filename[11];
	unsigned char 	DirAttr;
	unsigned char 	Dir_NTresrv[7];
	unsigned char 	Dir_CtrTimeTenth;
	unsigned short 	Dir_FstClusHI;
	unsigned short 	Dir_WrtTime;
	unsigned short 	Dir_wrtDate;
	unsigned short 	Dir_FstClusLO;
	unsigned int 	Dir_Filesize;
}  __attribute__((packed)) *StructFat32Dir;

//RootDirSectors (0) = ((RDE * 32) + (fat_sector_to_klaster  – 1)) / fat_sector_to_klaster ;
//FirstDataSector = fat_reserv_sectors + (fat_copys * fat32_bigsektorpearlfat) + RootDirSectors; Корневой каталог

//FirstSectorofCluster = ((N – 2) * fat_sector_to_klaster) + FirstDataSector;	содержимое файла по fat таблице

//DataSec = fat_bigtotalsektors – (fat_reserv_sectors + (fat_copys * fat32_bigsektorpearlfat) + RootDirSectors);
//CountolClusters = DataSec / fat_sector_to_klaster;
//ThisFATSecNum = fat_reserv_sectors + (FATOffset / fat_sector_to_klaster );
//ThisFATEntOffset = REM(FATOffset / fat_sector_to_klaster );
/*
virtual at 0xd12 ;Size: 32
	Dir_name		db 11 dup (?)
	Dir_attr		db 0
	Dir_NTresrv		db 0
	Dir_CtrTimeTenth	db ?
	Dir_FstClusHI		dw ?;99 91
	;db 6 dup(?)
	Dir_WrtTime		dw ?
	Dir_wrtDate		dw ?
	Dir_FstClusLO		dw ?
	Dir_Filesize		dd ?
end virtual

	
	
;0000	Неиспользуемый кластер
;от 1 до количества кластеров	Номер кластера со следующей частью файла
;FFFF0h - FFFF6	Зарезервированный кластер
;FFF7	Испорченный кластер
;FFF8h - FFFFh	Конец файла (эта позиция соответствует последнему кластеру файла )
	
			;File attributes:
*/

//cluster_begin_lba = Partition_LBA_Begin + BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32);
//lba_addr = cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;
// unsigned long FAT32_SearchDir(void* Addr) {
// 	void * last_StructFat32 = StructFat32;
	
// 	StructFat32 = Addr;
// 	StructFat32->KlusterRootDir = StructFat32->BPB_RsvdSecCnt + (StructFat32->BPB_NumFATs * StructFat32->BPB_FATSz32);
// 	clrscr();

// 	HexInAsciiSz((int)last_StructFat32,8);
// PANIC;
// return StructFat32 -> KlusterRootDir;
// }