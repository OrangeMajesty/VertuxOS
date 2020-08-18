#ifndef _Memory_h
#define _Memory_h

//#define DEBUG 
unsigned long long MemorySize;
unsigned int LastMemoryBlock = 0;

/*
0:	P (Present - присутствие). Если 0, то страница не отображена на физическую память. Это значит, что она либо не определена, либо её содержимое было записано на диск операционной системой в процессе свопинга. Если происходит обращение к неприсутствующей странице (у которой бит P = 0), то процессор генерирует исключение страничного нарушения (#PF).
1:	R / W (Read / Write - Чтение / Запись). Если 0, то для этой страницы разрешено только чтение, 1 - чтение и запись.
2:	U / S (User / Supervisor - Пользователь / Система). Если 0, то доступ к странице разрешён только с нулевого уровня привилегий, если 1 - то со всех.
3:	PWT (Write-Through - Сквозная запись). Когда этот флаг установлен, разрешено кэширование сквозной записи (write-through) для данной страницы, когда сброшен - кэширование обратной записи (write-back).
4:	PCD (Cache Disabled - Кэширование запрещено). Когда установлен, кэширование данной страницы запрещено. Кэширование страниц запрещают для портов ввода/вывода, отображённых на память либо в случаях, когда кэширование не даёт выигрыша в производительности. Также, кэширование запрещается при обработке исключений и отладке в ситуациях, связанных с программированием кэшей. 
      Более подробно работа с флагами PWT и PCD описана в разделе "Управление кэшами".
5:	A (Accessed - Доступ). Устанавливается процессором каждый раз, когда он производит обращение к данной странице. Процессор не сбрасывает этот флаг - его может может сбросить программа, чтобы потом, через некоторое время определить, был ли доступ к этой странице, или нет. 
      Например, страницу с флагом A=0 можно не сохранять на диск в процессе свопинга, т.к. она не использовалась.
6:	D (Dirty - Грязный). Устанавливается каждый раз, когда процессор производит запись в данную страницу. Этот флаг также не сбрасывается процессором и может использоваться программой, чтобы определить, была ли запись в страницу или нет. 
      Например, страницу с флагом D=0 можно не сохранять на диск в процессе свопинга, т.к. её содержимое не изменялось.
7:	PAT (Page Table Attribute Index - Индекс атрибута таблицы страниц). Для процессоров, которые используют таблицу атрибутов страниц (PAT - page attribute table), этот флаг используется совместно с флагами PCD и PWT для выбора элемента в PAT, который выбирает тип памяти для страницы. Этот бит введён в процессоре Pentium III, а для процессоров, не использующих PAT, бит PAT должен быть равен 0.
8:	G (Global Page - Глобальная страница). Когда установлен, определяет глобальную страницу. Такая страница остаётся достоверной в кэшах TLB при перезагрузке регистра CR3 или переключении задач. Этот бит введён в Pentium Pro и работа с ним подробно описана в "Управлении кэшами". Для процессоров, младше Pentium Pro, этот бит зарезервирован и должен быть равен 0.
9..11:	Доступно для использования программой. Процессор не использует эти биты.
12..31:	Базовый адрес страницы - это адрес, с которого начинается страница, другими словами - это физический адрес, на который отображена данная страница.
*/
void MemSet_Frame(unsigned int PTE, bool Present, unsigned int flags, unsigned int freebits, unsigned int Base_adr){

	unsigned int a    = 0x0;
	unsigned int *Adr = (void*)PTE;

	a = Base_adr;
	a += ((freebits & 0x7) << 9);
	a += ((flags & 0xFF) << 1);
	if(Present)
		a++;

	*Adr = a;
	
}
/*
0:	P (Present - присутствие). Если 0, то таблица страниц не отображена на физическую память. Это значит, что она либо не определена, либо её содержимое было записано на диск операционной системой в процессе свопинга. Если происходит обращение к неприсутствующей таблице страниц (у которой бит P = 0), то процессор генерирует исключение страничного нарушения (#PF).
1:	R / W (Read / Write - Чтение / Запись). Если 0, то для всех страниц этой таблицы разрешено только чтение, 1 - чтение и запись.
2:	U / S (User / Supervisor - Пользователь / Система). Если 0, то доступ ко всем страницам данной таблицы разрешён только с нулевого уровня привилегий, если 1 - то со всех.
3:	PWT (Write-Through - Сквозная запись). Когда этот флаг установлен, разрешено кэширование сквозной записи (write-through) для всех страниц данной таблицы страниц, когда сброшен - разрешено кэширование обратной записи (write-back).
4:	PCD (Cache Disabled - Кэширование запрещено). Когда установлен, кэширование всех страниц данной таблицы страниц запрещено. Кэширование страниц запрещают для портов ввода/вывода, отображённых на память либо в случаях, когда кэширование не даёт выигрыша в производительности. Также, кэширование запрещается при обработке исключений и отладке в ситуациях, связанных с программированием кэшей. 
      Более подробно работа с флагами PWT и PCD описана в разделе "Управление кэшами".
5:	A (Accessed - Доступ). Устанавливается процессором каждый раз, когда он производит обращение к страницам данной таблицы страниц. Процессор не сбрасывает этот флаг - его может может сбросить программа, чтобы потом, через некоторое время определить, был ли доступ к этой таблице страниц, или нет.
6:	0 (зарезервировано).
7:	PS (Page size - Размер страницы). Если 0, то используются страницы размером 4Кб и данный элемент PDE указывает на таблицу страниц. Если флаг PS установлен в 1, то данный элемент PDE указывает на страницу размером 4Мб (для 32-разрядной адресации), либо размером 2Мб (при расширенной физической адресации).
8:	G (Global Page - Глобальная страница). В PDE этот бит процессором игнорируется.
9..11:	Доступно для использования программой. Процессор не использует эти биты.
12..31:	Базовый адрес таблицы страниц - это адрес, с которого начинается таблица страниц, другими словами - это физический адрес, на который она отображена.
*/
void MemSet_PDE(unsigned int PDE, bool Present, unsigned int flags, unsigned int freebits, unsigned int Base_adr){
	
	unsigned int a 	  = 0x0;
	unsigned int *PTE = (void*)Base_adr;
	unsigned int *Adr = (void*)PDE;
	unsigned int fori = 0x0;

	a = Base_adr;
	a += ((freebits & 0x7) << 9);
	a += ((flags & 0xDF) << 1);
	if(Present)
		a++;
	
		*Adr = a;
	while(1) {
		if(fori != 0x1000)
			break;
		PTE[fori] = 0x00000000;
		fori ++;
	}

}
/*
бит 3 - PWT (Write-Thorough) - сквозная запись,
бит 4 - PCD (Cache Control) - контроль кэширования
*/
void MemSet_CR3(unsigned int base, bool PWT, bool PCD){
	if(PWT)
		base += 0x4;
	if(PCD)
		base += 0x8;

	__asm volatile (
		"movl %0, %%eax		\n\t"
		"movl %%eax, %%cr3 	\n\t"
		:
		:"m"(base)
		:
		);
}
void MemSet_PageTrONOFF () {
	__asm volatile(
		"movl %%cr4, %%eax			\n\t"
		//"orb $0x80, %%eax			\n\t"
		"movl %%eax, %%cr4			\n\t"
		"movl %%cr0, %%eax			\n\t"
		"or $0x80000000, %%eax		\n\t"
		"movl %%eax, %%cr0			\n\t"
		:::
		);
}
extern unsigned int BGMM;	// Before Get Memory Map
unsigned int get_memory_size(){
	unsigned int* after_adr;

	if(BGMM == 0x12345678)	{
		printf("true\n");
	} else	{
		after_adr = (void*)0x4000000;
		while(1) {
			*after_adr = (unsigned int)0x98128723;//Рандомное число
			if(*after_adr != (unsigned int)0x98128723){
				break;
			}
			after_adr += (unsigned int)0x100000;
		}
		return (unsigned int)after_adr;
	}
return 0;
}
//   Show Memory Map - ...
void SMM(char flags){ 
	unsigned int *PDE_ = (void*)Base_Memory;
	unsigned int *Table;

	unsigned int begin;
	unsigned int end;

	unsigned int old_end;

	unsigned int size_block =0;
	unsigned int size = 0;



	switch(flags){
		case 'P'://проецируемые адреса
			printf("\nMemory list");
			for(;(void*)PDE_ != (void*)(Base_Memory + 0x1000); PDE_++, size+=0x1000){
				Table = (void*)(*PDE_ & 0xfffff000);
				if(*PDE_ & 0x1){
					if(!((*Table) & 0x1)){
						while(!((*Table) & 0x1)){
							Table++;
						}
					}
						begin = *Table & 0xfffff000;
						size_block=0;
								
					while((*Table) & 0x1){
						Table++;
						size_block+=0x1000;
					}
					Table--;
					end = *Table & 0xfffff000;
					if(old_end != end){
					printf("\n0x");
					HexInAsciiSz(begin,8);
					printf(" -- 0x");
					HexInAsciiSz(end,8);
					printf(" (0x");
					HexInAsciiSz(size_block,8);
					printf(") KB\t");
					old_end = end;
					}
				}
			}
			break;
		default://Bysy //Обьем занятой памяти
		break;
	}
	printf("\n");
	//обьем занятой памяти
	//показать общий обьем памяти 
}
void set_memory_map(){
	unsigned char* Map_ = (void*)Base_Memory_map;
	unsigned int* PDE = (void*)Base_Memory;
	unsigned int* PTE = (void*)0x0;
	unsigned int i = 0x0;
	unsigned int U = 0x0;
	if(MemorySize == 0){
		printf("\nMMU -> Set Mem Map\nMemory Error\n");
		PANIC
	}
// Построение карты памяти
	while(U < 0x400){
		if(*PDE & 0x1) {
			//Проектируемый блок
			i = 0x0;
			PTE = (void*)(*PDE & 0xfffff000);
			while(i != 0x400){
					if(PTE[i] & 0x1){
						*Map_ += (0x1 << (i%8));
					} else {
						#ifdef DEBUG			
						*Map_ = 0xBA;			//Debug
						#endif
					}
					i+=1;
					if(!(i%8)){ Map_++; }
			}
		} else { 
			#ifdef DEBUG			
				*Map_ = 0x44;					// Debug
			#endif
			Map_ += 0x80;
		}
		PDE++;
		U++;
	}
}
//   Hard Frame | From To - ...
void HardFrameFrTo(unsigned int physical, unsigned int virtuall, unsigned long size) { 
	
	unsigned int PDBR = Base_Memory + (0x4 * (virtuall >> 22));
	unsigned int TDBR = Base_Memory + 0x1000 + (0x1000*(virtuall >> 22));

	uint32_t TDBRnew = TDBR;
	uint32_t i1 = physical;

	physical &= 0xfffff000;
	virtuall  &= 0xfffff000;

	if(size & 0x00000FFF){ // Выравниваем в 4к.
		size &= 0xfffff000;
		size += 0x1000;
	}

	for(unsigned long i = 0; i != size; i+= 0x1000, PDBR += 0x4, TDBR+=0x1000){
		MemSet_PDE(PDBR, 1, 1, 0, TDBR);
		// Поставить метку на карту

		for(; TDBRnew != (TDBR+0x1000);i1 += 0x1000, TDBRnew += 4){
	 	MemSet_Frame(TDBRnew, 1, 1, 0, i1);
	 	// Заполнить карту
	 	if (TDBRnew > (TDBR+0x1000)){
	 		printf("\nHFFT endless Error\n");
	 		while(1){}
	 }
	 	}

	}
}
// Block - 0x1000 byte/4k/1 page | Type: 0 - Blocks, 1 - byte
unsigned long malloc(const unsigned long long NBlocks, char type) {
	
	unsigned long long EndBlocks = NBlocks;

	unsigned long long FreeBlocks = 0; // Смещение от первого блока в блоках
	unsigned int FirstBlock = 0;
	unsigned char FirstBit = 0;

	unsigned char* Map_ = (void*)Base_Memory_map;

	unsigned int* PDBR = (void*)0x0;
	unsigned int PDE = Base_Memory;
	unsigned int PTE = 0x0;

	unsigned long byte = (0x1000000 / 0x8000); // byte
	unsigned int bit = 0x0; // bit in byte

	if(type){
		EndBlocks = NBlocks / 0x1000;
		if (NBlocks && 0xfff)
			EndBlocks++;
	}

	if(!LastMemoryBlock){ // Ищем свободный блок, для последующих поисков
		while(Map_[byte] == 0xff){
			byte++;
		}
		LastMemoryBlock = (long)byte;
	}

	byte = LastMemoryBlock;

	// Ищем Место
	while(EndBlocks != FreeBlocks){
		if(bit == 8){
			bit = 0;
			byte++;
		}
		if(byte >= MemorySize){ // Т.к. подкачки еще нет. Выводим ошибку.
			setcolor(0x3);
			printf("\nMMU -> malloc\nMemory is completed.\nFatal Error...\n");
			PANIC;
		}
		if(((Map_[byte] >> bit) & 0x1) == 0){
			if(!FreeBlocks){
				FirstBlock = byte;
				FirstBit = bit;
			}
			FreeBlocks++;
		} else { 
			FreeBlocks = 0;
		}

		bit++;
	}
	// Ставим Кадры
	FreeBlocks = 0;
	// FirstBit = 2;

	byte = FirstBlock;
	bit = FirstBit;

	while(FreeBlocks != EndBlocks){
		if(PDE != Base_Memory + ((((FirstBlock * 8) + FreeBlocks + FirstBit) * 0x1000) >> 22) * 4){
			PDE = Base_Memory + ((((FirstBlock * 8) + FreeBlocks + FirstBit) * 0x1000) >> 22) * 4;
			PTE = Base_Memory + 0x1000 + (0x1000 * ((((FirstBlock * 8) + FreeBlocks + FirstBit) * 0x1000) >> 22));
			PDBR = (void*)PDE;
			if(!(*PDBR & 0x1)){
				MemSet_PDE(PDE, 1, 1, 0, PTE);
			}
			if(FreeBlocks == 0)
				PTE += ((((((FirstBlock * 8) + FirstBit) * 0x1000) >> 11)) & 0x3ff) * 2;
		} else {
			if(bit == 8){
				byte++;
				bit = 0;
			}
			Map_[byte] += (0x1 << bit);
			MemSet_Frame(PTE, 1, 1, 0, (((FirstBlock * 8) + FreeBlocks + FirstBit) * 0x1000));
			PTE+=4;
			FreeBlocks++;
			bit++;
	}
}

// #define DEBUG
#ifdef DEBUG
			clrscr();
			set_memory_map();
			SMM('P');
			printf("\n 0x");
			HexInAsciiSz((int)&Map_[FirstBlock],8);
			printf(" -- MAP\n 0x");
			HexInAsciiSz((long)((((FirstBlock * 8) + FirstBit) * 0x1000)),8);
			printf(" -- ADDRES\n PTE -- 0x");
			HexInAsciiSz((long)PTE,8);
			printf("\n PDE -- 0x");
			HexInAsciiSz((int)PDE,8);
			printf("\n size_block -- 0x");
			HexInAsciiSz(EndBlocks,8);
			printf("\n Return(0x");
			HexInAsciiSz((((FirstBlock * 8) + FirstBit) * 0x1000),8);
			printf(")");
			PANIC;
#endif
	return (((FirstBlock * 8) + FirstBit) * 0x1000);
}
void free(unsigned long ADDRES, unsigned int block, unsigned char type){
	// int a  = ADDRES + block;
}
/*
1.	Подготовить описания всех используемых в программе страниц, таблиц страниц и каталога страниц.
2.	Загрузить адрес начала (т.е. базовый адрес) каталога страниц в CR3.
3.	Разрешить в процессоре страничное преобразование.
*/
void init_memory(unsigned int core){	//size

	idt_closed();
	MemorySize = get_memory_size();

	MemSet_CR3(Base_Memory, 0, 0);

	HardFrameFrTo(0,0,0x1000);
	HardFrameFrTo(0x1000000, 0x1000000, core);
	
	set_memory_map();

	MemSet_PageTrONOFF();
	//printf("Memory configured!\n");

	SMM('P');	 // Show Memory Map
	idt_open();
}
#endif //End