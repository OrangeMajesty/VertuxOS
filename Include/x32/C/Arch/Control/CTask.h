#pragma once

// int PriorityNum = 0;
// char PriorityKey[] = {High,Normal,High,Low,Normal,High};

unsigned int NextCallTask = 0;

void idt_gradeTask(signed int num, unsigned short selector,unsigned char flag);

void TaskManagerGUI(){
	clrscr();
	while(1){
			setposition(0);
	    	printf("\n\t\t\t  TASK MANAGER\t  |  Max Task: ");
	    	HexInAsciiSz(Max_Task,8);
	    	printf("\n--------------------------------------------------------------------------------");
	    	printf("  PID  | Status |     EIP     |   EFLAGS   |  LINK  |   NCycles   | Priority |");
	    	printf("\n--------------------------------------------------------------------------------  ");
			for (int i = 0; i < Max_Task; i++)
			{if(QueueTask[i]){
	    	HexInAsciiSz((SProccess[i].processid)<<16,4);
	    	 switch((SProccess[i].status)){
	    		case 0x1: printf("   Frozen   0x"); break;
	    		case 0x2: printf("    Work    0x"); break;
	    		case 0x3: printf("   Close    0x"); break;
	    	}
	    	HexInAsciiSz(TSSTask[i*2].EIP,8);
	    	printf("     ");
	    	HexInAsciiSz(TSSTask[i*2].EFLAGS,8);
	    	printf("   ");
	    	HexInAsciiSz((TSSTask[i*2].LINK),8);
	    	printf("    ");
	    	HexInAsciiSz((SProccess[i].Cycle),8);
	    		printf("     ");
	    		 switch(SProccess[i].priority){
	    			case High: printf(" High"); break;
	    			case Low:  printf(" Low"); break;
	    			case Normal: printf("Normal"); break;
	    			default: printf("Unreal"); break;
	    		}
	    	printf("\n  ");
	    	// WaitTick(0xffff);
	    }
	}}
}
void init(){
	WaitTick(0xffffff);
	
	// LoadProcces();
}
void init2(){
		// PANIC;
	while(1){
		WaitTick(0xffff);
	}
	// LoadProcces();
}
// * Команда JMP или CALL может ссылаться на другой сегмент любым из 4-х способов:
// Операнд команды содержит селектор целевого сегмента.
// Операнд указывает на дескриптор шлюза вызова, который содержит селектор целевого сегмента.
// Операнд указывает на TSS, в котором содержится целевой сегмент кода.
// Операнд указывает на шлюз задачи, который указывает на TSS, в котором содержится целевой сегмент кода.
// #define AD (unsigned short)0x13
void TaskControl(){
	IRQ_Open();
	idt_open();
    while(1){
    	NextCallTask++;
	    if(NextCallTask == Max_Task) NextCallTask = 1;
	    if(QueueTask[NextCallTask]){ //Если Задача присутствует
			if(NextCallTask){
			tss_access(GDT_FirstSelector_TSS+(NextCallTask*2), 0x89);
	    	// TssChange(2,0x89,0,0,0);
	    	// printf("L");
	    	// WaitTick(0xffffffff);
	    	// HexInAsciiSz(QueueTask[NextCallTask],8);
	    	// if(NextCallTask == 3) PANIC;
	    	// PANIC;
	    	SProccess[NextCallTask].Cycle++;
	    	EOI;
	    	switch(QueueTask[NextCallTask]){
			   	case 0x90: __asm("ljmp $0x90, $0x0"); break;
			   	case 0xa0: __asm("ljmp $0xa0, $0x0"); break;
			   	case 0xb0: __asm("ljmp $0xb0, $0x0"); break;
			   	case 0xc0: __asm("ljmp $0xc0, $0x0"); break;
			   	case 0xd0: __asm("ljmp $0xd0, $0x0"); break;
			   	case 0xe0: __asm("ljmp $0xe0, $0x0"); break;
			   	case 0xf0: __asm("ljmp $0xf0, $0x0"); break;
			   	case 0x100: __asm("ljmp $0x100, $0x0"); break;
			   	case 0x110: __asm("ljmp $0x110, $0x0"); break;
			   	default:  break;
			   }
			 }	
	    }
    }
}

void TaskInit(){
	NewProccess((unsigned int)&TaskControl+0x24);
	NewProccess((unsigned int)&TaskManagerGUI+0x24);
	NewProccess((unsigned int)&init2+0x24);
	// printf("Call fopen\n");
	LoadProccess("/mgr/display/init.mgr");
	// printf("NIX\n");
	PANIC;
	// LoadDriver("/scontrol/display/init");
// PANIC;
	idt_gradeTask(IQR_k_Timer, QueueTask[0], FlagsTask);

	__asm("ljmp $0x80, $0x0");

}