// Разработка Ядра для ОС: vortex
// Версия: 0.1 - Alpha
//------- Описание ----------
// Файловая система: 		FAT32
// Конфигурационный файл: 	?????
// Разрядность: 			x32
//---- Стадии Разработки ----
/*
. Менеджер памяти
. Менеджер процессов
. IPC
*/
//-------- Память -----------
//   00000-005FF        используется BIOS ( 1536 )
//   00600-07BFF        свободно ( 30208 )
//   07C00-07DFF        программа начальной загрузки ( 512 )
//   07E00-9FFFF        свободно ( 623104 )
//   A0000-FFFFF        используется BIOS ( 393216 )
//   B8000-(5*80)*2
//----- Служебная памать ----
//   01600-0....        Буфер 
//---------------------------
//#define mbarrier() asm volatile ("":::"memory")

//Define
#include "Include/x32/C/Header.h"
//Other
#include "Include/x32/C/Type.h"
#include "Include/x32/C/GnuAssembl.h"
#include "Include/x32/C/IO.h"
#include "Include/x32/C/Display.h"
#include "Include/x32/C/IDT.h"
#include "Include/x32/C/GDT.h"
#include "Include/x32/C/PCI_Device.h"
#include "Include/x32/C/filemanager.h"
//Arch
#include "Include/x32/C/Arch/IPS.h"
#include "Include/x32/C/Arch/MMU.h"
#include "Include/x32/C/Arch/TaskManager.h"
//Control
#include "Include/x32/C/Arch/Control/CTask.h"
#include "Include/x32/C/Arch/Control/CIntereptionDevice.h"


void kernel_main(unsigned int CoreSize) {
	setcolor(0x2);
	clrscr();

	printf("Loaded System\n");
	gdt_install();
	idt_install();
	init_device();

	init_memory(CoreSize);

	// load loadscreen
	// fopen("/var/screen/lscr.png",0)
	// fload()

	TaskInit();
	printf("FATAL ERROR\n");
	
} 

