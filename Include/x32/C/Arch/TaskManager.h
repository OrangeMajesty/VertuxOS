#pragma once

#define FlagsTask 		0x85
#define FlagsTss 		0x89

// Info Priority
#define Normal 			0x1
#define High 			0x2
#define Low	 			0x3

// Info Status
#define Frozen 			0x1
#define Work 			0x2
#define Close			0x3

// Info Type
#define Resident		0x1
#define Paged			0x2

#define Max_LDTselector 0x6

#define CoreSpaces		0x1
#define UserSpaces		0x0

#define None 				0x0
#define _CS_ 				0x8
#define _DS_ 				0x10
#define _SS0_				0x18
#define _SS1_				0x20
#define _FS_				0x28

unsigned long next_pid = 0;
unsigned long next_selector = 0;

unsigned int QueueTask[Max_Task] = {0};

struct TSSStruct
{
	unsigned int	LINK;
	unsigned int	ESP0, SS0;
	unsigned int	ESP1, SS1;
	unsigned int	ESP2, SS2;
	unsigned int	CR3;
	unsigned int	EIP;
	unsigned int	EFLAGS;
	unsigned int	EAX,ECX,EDX,EBX;
	unsigned int	ESP,EBP;
	unsigned int	ESI,EDI;
	unsigned int	ES,CS,SS,DS,FS,GS;
	unsigned int	LDTR;
	unsigned long long	ioMap;
} __attribute__((packed));

struct StructProccess {
	unsigned long 	processid;
			 char   name[25];
	unsigned long 	TSS;
	unsigned char 	status;
	unsigned char 	priority;
	unsigned char 	type;
	unsigned int	Cycle;
	unsigned char 	Stack0[Max_TStackSize];
	unsigned char 	Stack1[Max_TStackSize];
} __attribute__((packed));

struct TSSStruct TSSTask[Max_Task];
struct StructProccess SProccess[Max_Task];
struct gdt_entry LDT_Task[Max_Task*Max_LDTselector];

unsigned short NewProccess(unsigned int EIP);
void ldt_set_gate(unsigned int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
// Создание записи дескриптора TSS в GDT
void ltr(unsigned short selector){
	__asm("ltr %%ax"::"a"(selector):);
}
void tss_access(unsigned int num, unsigned char access){
	if(access) gdt[num].access = access;
}
char LoadProccess(char *patch) {
	unsigned long Bfirst = 0;
	unsigned short PrIndef = 0;
	if(fopen(patch, 0) != 0){
		printf("\nFile not found");
		return 0x1; // error
	}
	// Bfirst = malloc(file_preloader.size,1);
	// HexInAsciiSz(Bfirst,8);
	
	
	PrIndef = NewProccess(file_preloader.EIP);
	// HexInAsciiSz(file_preloader.EIP,8);
	// PANIC;
	ldt_set_gate((PrIndef), 0,0,0,0); // Z
	ldt_set_gate((PrIndef)+1, file_preloader.CSbase,file_preloader.CSlong,0x9a,0xcf); // CS
	ldt_set_gate((PrIndef)+2, file_preloader.DSbase,file_preloader.DSlong,0x92,0xcf); // DS
	ldt_set_gate((PrIndef)+3, (unsigned int)&SProccess[next_pid].Stack0, Max_TStackSize ,0x92 ,0x4f); // _SS0_
	ldt_set_gate((PrIndef)+4, (unsigned int)&SProccess[next_pid].Stack1, Max_TStackSize, 0x92, 0x4f); // _SS1_
	ldt_set_gate((PrIndef)+5, file_preloader.FSbase,file_preloader.FSlong,0x92,0xcf); // FS

	return 0; // Успешно
}

unsigned short NewProccess(unsigned int EIP) {
	unsigned short pidret = next_pid*Max_LDTselector;

	QueueTask[next_pid] = (next_selector+GDT_FirstSelector_TSS) << 3;

	SProccess[next_pid].processid = next_pid;
	SProccess[next_pid].TSS = QueueTask[next_pid];
	SProccess[next_pid].status = Work;
	SProccess[next_pid].priority = Normal;
	SProccess[next_pid].type = Paged;

	SProccess[next_pid].Cycle = 0;

	TSSTask[next_selector].LINK     =	GDT_FirstSelector_TSS << 3;
	TSSTask[next_selector].SS0     	=	_SS0_;
	TSSTask[next_selector].SS1     	=	_SS0_;
	TSSTask[next_selector].SS2     	=	_SS1_;
	TSSTask[next_selector].CR3     	=	Base_Memory;
	TSSTask[next_selector].EIP     	=	EIP;

/*Проверка выравнивания (Alignment Mask), 18-й бит в CR0. Когда установлен, разрешает автоматическую проверку выравнивания, иначе - запрещает. Проверка выравнивания выполняется только при следующих условиях:
	флаг AM в CR0 установлен,
	флаг AC в EFLAGS установлен,
	CPL = 3,
	процессор работает в P-Mode или V-Mode. */
	// __asm__ volatile("\npushfl\npop %%eax":"=a"(TSSTask[next_selector].ESP)::);
	TSSTask[next_selector].EFLAGS   =	0x0202;
	TSSTask[next_selector].EAX     	=	0x0;
	TSSTask[next_selector].ECX     	=	0x0;
	TSSTask[next_selector].EDX     	=	0x0;
	TSSTask[next_selector].EBX     	=	0x0;
	__asm__ volatile("\nmov %%esp, %%eax":"=a"(TSSTask[next_selector].ESP)::);
	// TSSTask[next_selector].ESP     	=	(unsigned int)&Proccess[next_pid].Stack1[0];
	//__asm__ volatile("\nmov %%esp, %%eax":"=a"(TSSTask[next_selector].EBP)::);
	TSSTask[next_selector].EBP     	=	0x0;
	TSSTask[next_selector].ESI     	=	0x0;
	TSSTask[next_selector].EDI     	=	0x0;
	TSSTask[next_selector].ES     	=	0x4 + _DS_;		// LDT
	TSSTask[next_selector].CS     	=	0x4 + _CS_;		// LDT
	TSSTask[next_selector].SS     	=	0x4 + _SS0_;	// LDT
	TSSTask[next_selector].DS     	=	0x4 + _DS_;		// LDT
	TSSTask[next_selector].FS     	=	0x4 + _FS_;		// LDT
	TSSTask[next_selector].GS     	=	0x4 + _DS_;		// LDT
	TSSTask[next_selector].LDTR     =	(next_selector+GDT_FirstSelector_TSS+1) << 3;
	TSSTask[next_selector].ioMap    =	0x0;

	// gdt_set_gate(1, Base, limit, 0x9A, 0xCF);
	gdt_set_gate(next_selector+GDT_FirstSelector_TSS+1, (unsigned long)&LDT_Task[next_pid*Max_LDTselector], sizeof(struct gdt_entry)*Max_LDTselector, 0x82, 0x4f); // LDT
	// void tss_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access)
	tss_set_gate(next_selector+GDT_FirstSelector_TSS, (unsigned long)&TSSTask[next_selector], sizeof(struct TSSStruct), FlagsTss);

	ldt_set_gate((next_pid*Max_LDTselector), 0,0,0,0); // Z
	ldt_set_gate((next_pid*Max_LDTselector)+1, 0,0xFFFFffff,0x9a,0xcf); // CS
	ldt_set_gate((next_pid*Max_LDTselector)+2, 0,0xFFFFFFFF,0x92,0xcf); // DS
	ldt_set_gate((next_pid*Max_LDTselector)+3, (unsigned int)&SProccess[next_pid].Stack0, Max_TStackSize ,0x92 ,0x4f); // _SS0_
	ldt_set_gate((next_pid*Max_LDTselector)+4, (unsigned int)&SProccess[next_pid].Stack1, Max_TStackSize, 0x92, 0x4f); // _SS1_
	ldt_set_gate((next_pid*Max_LDTselector)+5, 0,0xFFFFFFFF,0x92,0xcf); // FS

	// printf("\n0x");
	// HexInAsciiSz((uint16_t)&LDT_Task[next_pid*Max_LDTselector],8);
	next_selector+=2;
	next_pid+=1;

	return pidret;
}
//	-------------------

void ldt_set_gate(unsigned int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    LDT_Task[num].base_low = (base & 0xFFFF);
    LDT_Task[num].base_middle = (base >> 16) & 0xFF;
    LDT_Task[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    LDT_Task[num].limit_low = (limit & 0xFFFF);
    LDT_Task[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    LDT_Task[num].granularity |= (gran & 0xF0);
    LDT_Task[num].access = access;
}