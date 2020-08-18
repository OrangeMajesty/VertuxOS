#pragma once

#define PATA_CMD_READ_PIO          0x20
#define PATA_CMD_READ_PIO_EXT      0x24
#define PATA_CMD_READ_DMA          0xC8
#define PATA_CMD_READ_DMA_EXT      0x25
#define PATA_CMD_WRITE_PIO         0x30
#define PATA_CMD_WRITE_PIO_EXT     0x34
#define PATA_CMD_WRITE_DMA         0xCA
#define PATA_CMD_WRITE_DMA_EXT     0x35
#define PATA_CMD_CACHE_FLUSH       0xE7
#define PATA_CMD_CACHE_FLUSH_EXT   0xEA
#define PATA_CMD_PACKET            0xA0
#define PATA_CMD_IDENTIFY_PACKET   0xA1
#define PATA_CMD_IDENTIFY          0xEC

#define PATA_ER_BBK      		   0x80    // Bad sector
#define PATA_ER_UNC      		   0x40    // Uncorrectable data
#define PATA_ER_MC       		   0x20    // No media
#define PATA_ER_IDNF     		   0x10    // ID mark not found
#define PATA_ER_MCR      		   0x08    // No media
#define PATA_ER_ABRT     		   0x04    // Command aborted
#define PATA_ER_TK0NF    		   0x02    // Track 0 not found
#define PATA_ER_AMNF     		   0x01    // No address mark

#define PrimaryPATA 		       0x1f0
#define PrimaryPATAControl 	       0x3F6

//	Установите этот параметр, чтобы остановить текущее устройство от отправки прерываний.
void PATA_NoIRQ(){
	OutAl(PrimaryPATAControl, 0x2);
	//OutAl(PrimaryPATAControl, 0x0);
}
void PATA_HardReset(){
	OutAl(PrimaryPATAControl, 0x4);
	WaitTick(0xff);
	OutAl(PrimaryPATAControl, 0x0);
}
char PATA_Status(){
	unsigned char ret = 0;
	for (int i = 0; i != 5; ++i) {
		WaitTick(0xfff);
		ret = InAl(PrimaryPATA+0x7);
	}
	return ret;
}
/*
	0x0 - без ошибки
	0x1 - Сектор поврежден
	0x2 - Ошибка команды
	0x3 - Устройство Не найдено
	0x4 - 
	0x5
	0x6
*/
char PATA_Check() {
	return 0;
}


void PATA_ReadCHS(){

}
void PATA_ReadLBA48(){

}
char PATA_ReadLBA28(char device,unsigned char size ,unsigned long Sector, void* Buffer){

	unsigned char* Addr = Buffer;
	unsigned short temp = 0;
	PATA_HardReset();

	// OutAl(PrimaryPATA+0x7,PATA_CMD_CACHE_FLUSH);

	PATA_NoIRQ(); // Настроить на прерывания
	
	switch(device){
		case 0x0: OutAl(PrimaryPATA+0x6,0xE0); break;
		case 0x1: OutAl(PrimaryPATA+0x6,0xF0); break;
		default: return 0x3; break;
	}
	OutAl(PrimaryPATA+0x1,0x0); // ignore

	OutAl(PrimaryPATA+0x2,size); 

	OutAl(PrimaryPATA+0x3,Sector);
	OutAl(PrimaryPATA+0x4,Sector >> 8);
	OutAl(PrimaryPATA+0x5,Sector >> 16);	

	OutAl(PrimaryPATA+0x7,PATA_CMD_READ_PIO);
	WaitTick(0xffff);

	for (int i = 0; i != 0x10; i++) {
		if(PATA_Status() & 0x8) {
			for (int i = 0; i != size*0x200; i+=2) {
				temp = (unsigned short)InAx(PrimaryPATA);
				Addr[i] = temp;
				Addr[i+1] = temp >> 8;
			}
			break;
		} else { PATA_HardReset();

					if(i == 0xf) {
						return 0x2; // Ошибка команды
					}
				}
	}
	return 0;
}
