#include "CPU.h"
#ifndef _PCI_Device_h
#define _PCI_Device_h

#define PCI_Address 0x0CF8 //W
#define PCI_Data	0x0cfc //RW

unsigned char HDDinit(unsigned char TypeConnect);
void OutEax(unsigned int adr, unsigned long data);
unsigned int InEax(unsigned int adr);
void HexInAsciiSz(const long hex_,unsigned char size);

struct PCI_Dev
{
	//0x0
	unsigned short VendorID;
	unsigned short DeviceID;
	//0x4
	unsigned short Command;
	unsigned short Status;
	//0x8
	unsigned char RevisionID;
	unsigned char ProgIF;
	unsigned char SubClass;
	unsigned char ClassCode;
	//0xc
	unsigned char CacheLineSize;
	unsigned char LatencyTimer;
	unsigned char HeaderType;
	unsigned char BIST;

	unsigned int BAR0;	//0x10
	unsigned int BAR1;	//0x14
	unsigned int BAR2;	//0x18
	unsigned int BAR3;	//0x1c
	unsigned int BAR4;	//0x20
	unsigned int BAR5;	//0x24
	//0x28
	unsigned int CardbusCisPointer;
	//0x2c
	unsigned short SystemVendorID;
	unsigned short SubSystemID;
	//0x30
	unsigned int ExoansionRomBaseAddress;
	//0x34
	unsigned char CapabilitiesPointer;
	unsigned char reserv;
	unsigned short reserv1;
	//0x38
	unsigned int reserv2;
	//0x3c
	unsigned char InterruptLine;
	unsigned char InterruptPIN;
	unsigned char MinGrant;
	unsigned char MaxLatency;

	//--- Info ---
	unsigned int Address_BusDevFunc;
} __attribute__((packed));

// 0CF8h -W порт адреса Address
// 0CFCh RW порт данных Data
// Оба порта являются 32 битными.
// +---+---------+-------+----------+--------+---------------+-+-+
// |31 |30     24|23   16|15      11|10     8|7             2|1|0|
// +---+---------+-------+----------+--------+---------------+-+-+
// | с | резерв  |шина   |устройство| функция|Индекс регистра|0|0|
// +---+---------+-------+----------+--------+---------------+-+-+
struct PCI_Dev IDE_Controller;

char MSC_init(unsigned long device){

	unsigned long value = 0;
	device = device & 0xffffff00;
	IDE_Controller.Address_BusDevFunc = device;


	OutEax(PCI_Address,device);
	value = InEax(PCI_Data);
	IDE_Controller.VendorID = value;
	IDE_Controller.DeviceID = value >> 16;

	OutEax(PCI_Address,device+0x4);
	value = InEax(PCI_Data);
	IDE_Controller.Command = value;
	IDE_Controller.Status = value >> 16;

	OutEax(PCI_Address,device+0x8);
	value = InEax(PCI_Data);
	IDE_Controller.RevisionID = value & 0xff;
	IDE_Controller.ProgIF = value >> 8;
	IDE_Controller.SubClass = value >> 16;
	IDE_Controller.ClassCode = value >> 24;

	OutEax(PCI_Address,device+0xc);
	value = InEax(PCI_Data);
	IDE_Controller.CacheLineSize = value & 0xff;
	IDE_Controller.LatencyTimer = value >> 8;
	IDE_Controller.HeaderType = value >> 16;
	IDE_Controller.BIST = value >> 24;

	OutEax(PCI_Address,device+0x10);
	IDE_Controller.BAR0 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x14);
	IDE_Controller.BAR1 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x18);
	IDE_Controller.BAR2 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x1c);
	IDE_Controller.BAR3 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x20);
	IDE_Controller.BAR4 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x24);
	// OutEax(PCI_Data,0xffffffff);
	IDE_Controller.BAR5 = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x28);
	IDE_Controller.CardbusCisPointer = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x2c);
	value = InEax(PCI_Data);
	IDE_Controller.SystemVendorID = value;
	IDE_Controller.SubSystemID = value >> 16;	

	OutEax(PCI_Address,device+0x30);
	IDE_Controller.ExoansionRomBaseAddress = InEax(PCI_Data);

	OutEax(PCI_Address,device+0x34);
	IDE_Controller.CapabilitiesPointer = InEax(PCI_Data);
	//0x38 reserv

	OutEax(PCI_Address,device+0x3c);
	value = InEax(PCI_Data);
	IDE_Controller.InterruptLine = value & 0xff;
	IDE_Controller.InterruptPIN = value >> 8;
	IDE_Controller.MinGrant = value >> 16;
	IDE_Controller.MaxLatency = value >> 24;


	if(value == 0xffffffff){
		printf("\nHDD FATAL ERROR!\n");
		return 0x1; // Device not found
	}

	device += 0x8;
	OutEax(PCI_Address,device);
	value = InEax(PCI_Data);

	switch(((value>>16)&0xff)){
		case 0x00: printf(" | SCSI"); HDDinit(0x0);break;
		case 0x01: printf(" | IDE"); HDDinit(0x1); break;
		case 0x02: printf(" | Floppy disk"); HDDinit(0x2); break;
		case 0x03: printf(" | IPI bus"); HDDinit(0x3); break;
		case 0x04: printf(" | RAID bus"); HDDinit(0x4); break;
		case 0x05: printf(" | ATA"); HDDinit(0x5); break;
		case 0x06: printf(" | SATA"); HDDinit(0x6); break;
		case 0x07: printf(" | Serial Attached SCSI"); HDDinit(0x7); break;
		case 0x08: printf(" | Non-Volatile memory"); HDDinit(0x8); break;
		case 0x80: printf(" | Mass storage"); HDDinit(0x80); break;
		default:   printf(" | This HDD not Supported!"); break;
	}
	
	printf("\n");
// ---------- Search BARs ------------

// -----------------------------------
	return 0;
}
void init_buspci(){
	unsigned long vend = 0;
	unsigned long device;

	for (unsigned int bus = 0; bus < 256; bus++)
	{
		for (unsigned int dev = 0; dev != 32; dev++)
		{
			for (unsigned char func = 0; func != 8; func++)
			{
				device = ((func*0x100)+(dev*0x800)+(bus*0x10000)+0x80000000);

				OutEax(PCI_Address,device);
				vend = InEax(PCI_Data);

				if(vend != 0xffffffff){

					printf("\n");
					HexInAsciiSz((bus << 24),2);
					printf(":");
					HexInAsciiSz((dev << 24),2);
					printf(":");
					HexInAsciiSz((func << 24),2);
					printf(":");
					HexInAsciiSz((vend << 16),4);	
					printf(":");
					HexInAsciiSz(vend,4);

					// ---- Check product vendor ------
						switch((vend & 0xffff)){
							case 0x8086: printf(" (Intel Corporation) "); break;
							case 0x1013: printf(" (Cirrus Logic Crystal) "); break;
							case 0x5143: printf(" (Qualcomm Inc. USA) "); break;
							case 0x8087: printf(" (Intel) "); break;

							default: printf(" (device indefined) "); break;
						}
					// --------------------------------
					// ----- Check product class ------
						device += 0x8;
						OutEax(PCI_Address,device);
						vend = InEax(PCI_Data);

						//HexInAscii((vend >> 24),1);
						switch((vend >> 24)&0xff){
							case 0x00: printf("Unclassified device"); break;
							case 0x01: printf("Mass storage controller"); MSC_init(device); break;
							case 0x02: printf("Network controller"); break;
							case 0x03: printf("Display controller"); break;
							case 0x04: printf("Multimedia controller"); break;
							case 0x05: printf("Memory controller"); break;
							case 0x06: printf("Bridge"); break;
							case 0x07: printf("Communication controller"); break;
							case 0x08: printf("Generic system peripheral"); break;
							case 0x09: printf("Input device controller"); break;
							case 0x0a: printf("Docking station"); break;
							case 0x0b: printf("Processor"); break;
							case 0x0c: printf("Serial bus controller"); break;
							case 0x0d: printf("Wireless controller"); break;
							case 0x0e: printf("Intelligent controller"); break;
							case 0x0f: printf("Satellite communications controller"); break;
							case 0x10: printf("Encryption controller"); break;
							case 0x11: printf("Signal processing controller"); break;
							case 0x12: printf("Processing accelerators"); break;
							case 0x13: printf("Non-Essential Instrumentation"); break;
							case 0x40: printf("Coprocessor"); break;
							case 0xff: printf("Unassigned class"); break;
							default: printf("Null"); break;
						}
					// --------------------------------
				}
			}
		}
	}
	printf("\n");
}
void init_device(){
	init_CPU();
	init_buspci();
}

#endif