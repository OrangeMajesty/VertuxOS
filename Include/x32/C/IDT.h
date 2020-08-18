#ifndef IDT_h
#define IDT_h
/*
1.  Перенаправить аппаратные прерывания (IRQ)
2.  Создать дескрипторы для всех используемых векторов (исключений, аппаратных и программных прерываний).
3.  Подготовить образ IDTR и загрузить его в регистр IDTR.
4.  Разрешить прерывания
*/

//#include "PIC.c"
#include "Display.h"

void HardFrameFrTo(unsigned int physical, unsigned int virtual, unsigned long size);
void MemSet_CR3(unsigned int base, bool PWT, bool PCD);
void IRQ_Install ();
void idt_closed();
void idt_open();
void idt_flush();
/* Defines an IDT entry */
struct idt_interrupt
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit __attribute ((packed));
    unsigned int base __attribute ((packed));
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_interrupt idt[256];
struct idt_ptr idtr32;
// IDT Table -------------------

void idt_flush(){
    __asm(
        "lidt %0\n\t"
        :
        :"m"(idtr32)
        :
        );
//lidt fword[idtr32]
}

/*
Таблица 3-1. Исключения и прерывания защищённого режима.
Номер вектора   Название    Описание    Тип Error Code  Источник исключения
0   #DE Ошибка деления  Fault   Нет Команды DIV и IDIV
1   #DB Отладка Fault/Trap  Нет Любая команда или команда INT 1
2   -   Прерывание NMI  Прерывание  Нет Немаскируемое внешнее прерывание
3   #BP Breakpoint  Trap    Нет Команда INT 3
4   #OF Переполнение    Trap    Нет Команда INTO
5   #BR Превышение предела  Fault   Нет Команда BOUND
6   #UD Недопустимая команда (Invalid Opcode)   Fault   Нет Недопустимая команда или команда UD21
7   #NM Устройство не доступно (No Math Coprocessor)    Fault   Нет Команды плавающей точки или команда WAIT/FWAIT
8   #DF Двойная ошибка  Abort   Да (Нуль)   Любая команда
9   -   Превышение сегмента сопроцессора (зарезервировано)  Fault   Нет Команды плавающей точки2
0Ah #TS Недопустимый TSS    Fault   Да  Переключение задач или доступ к TSS
0Bh #NP Сегмент не присутствует Fault   Fault   Загрузка сегментных регистров или доступ к сегментам
0Ch #SS Ошибка сегмента стека   Fault   Да  Операции над стеком и загрузка в SS
0Dh #GP Общая защита    Fault   Да  Любой доступ к памяти и прочие проверки защиты
0Eh #PF Страничное нарушение    Fault   Да  Доступ к памяти
0Fh -   Зарезервировано Intel-ом. Не использовать.      Нет  
10h #MF Ошибка плавающей точки в x87 FPU (Ошибка математики)    Fault   Нет Команда x87 FPU или команда WAIT/FWAIT
11h #AC Проверка выравнивания   Fault   Да  (Нуль) Обращение к пямяти3
12h #MC Проверка оборудования   Abort   Нет Наличие кодов и их содержимое зависит от модели4
13h #XF Исключение плавающей точки в SIMD   Fault   Нет Команды SSE и SSE25
14h-1Fh -   Заререзвировано Intel-ом. Не использовать            
20h-FFh -   Прерывания определяются пользователем   Прерывание      Внешнее прерывание или команда INT n
*/


    Interrupt(INT_DE){
        printf("\nInterrupt :0\t #Div Error\n");
        PANIC;
    }
    Interrupt(INT_DB){

        printf("\nInterrupt :1\t #Debug\n");
        PANIC;
    }
    Interrupt(INT_NMI){

        printf("\nInterrupt :2\t #NMI\n");
        PANIC;
    }
    Interrupt(INT_BP){

        printf("\nInterrupt :3\t #Breakpoint\n");
       PANIC;
    }
    Interrupt(INT_OF){
        printf("\nInterrupt :4\t #Overflow\n");
        PANIC;
    }
    Interrupt(INT_BR){
        printf("\nInterrupt :5\t #BOUND Range Exceeded Exception\n");
        PANIC;
            }
    Interrupt(INT_UD){
        unsigned int eip = 0;
        unsigned short cs = 0;
        __asm("pop %%eax\n":"=a"(eip)::);
        __asm("pop %%ax\n":"=a"(cs)::);
        printf("\nInterrupt :6\t #Invalid Opcode\n->");
        HexInAsciiSz(eip,8);
        HexInAsciiSz(cs,8);
        PANIC;
    }
    Interrupt(INT_NM){
        printf("\nInterrupt :7\t #No Math Coprocessor\n");
        PANIC;
    }
    Interrupt(INT_DF){
        
        printf("\nInterrupt :8\t #Double Error /PANIC\n");
        PANIC;
    }
    Interrupt(INT_soproc){
        printf("\nInterrupt :9\t #Reserv\n");
        //PANIC;
    } //9 Превышение сегмента сопроцессора (зарезервировано)
    Interrupt(INT_TS){
        
        printf("\nInterrupt :10\t #Invalid TSS\n");
        PANIC;
    }
    Interrupt(INT_NP){
        
        printf("\nInterrupt :11\t #Segment is not present\n");        
        PANIC;
    }
    Interrupt(INT_SS){
        
        printf("\nInterrupt :12\t #Stack Segment Error\n");
        PANIC;
    }
    Interrupt(INT_GP){
        printf("\nInterrupt:13\t #Global Protected.\n");
        PANIC;
    }
    Interrupt(INT_PF){

        // при выходе за границу прерываний
        idt_closed();
        unsigned int eax_f;
        __asm("movl %%cr2, %0\n ":"=r"(eax_f)::);
        
        printf("\nInterrupt :14\t #Page Fault in 0x");
        HexInAsciiSz(eax_f,8);
        //*((char*)0xb8000) = '+';
        HardFrameFrTo(eax_f,eax_f,0x1000);
        MemSet_CR3(0x00300000, 0, 0);
        idt_open();
        PANIC;
    }
    Interrupt(INT_Intelreserv){
        printf("\nInterrupt :15\t #Intel Reserv\n");
        //PANIC;
    } //15 Зарезервировано Intel-ом. Не использовать.
    Interrupt(INT_MF){
        printf("\nInterrupt :16\t #Match Fault\n");
        PANIC;
    }
    Interrupt(INT_AC){
        
        printf("\nInterrupt :18\t #Check Offset\n");
        PANIC;
    }
    Interrupt(INT_MC){
        printf("\nInterrupt :17\t #Check device\n");
        PANIC;
    }
    Interrupt(INT_XF){
        printf("\nInterrupt :18\t #Floating point exception\n");
        PANIC;
    }
    
IRQ_Interrupt(IRQ_Timer){
    //unsigned char* a = 0x8b000;
    //EOI;
    //PANIC;
    //printf("IRQ_Timer");   
    //*Display_color+=2;
    //printf("2");
}
IRQ_Interrupt(IRQ_Keybord){
    printf("IRQ_Keybord");
    PANIC;
}
IRQ_Interrupt(IQR_IQR2){
    //EOI;
    PANIC;
    printf("IQR_IQR2");
    
}
IRQ_Interrupt(IQR_COM2_4){
    PANIC;
    printf("IQR_COM2_4");
}
IRQ_Interrupt(IQR_COM1_3){
    printf("IQR_COM1_3");
    
}
IRQ_Interrupt(IQR_LTP2){
    printf("IQR_LTP2\n");
    
}
IRQ_Interrupt(IQR_FloppyDC){
    printf("IQR_FloppyDC");
    
}
IRQ_Interrupt(IQR_LTP1){
    printf("IQR_LTP1\n");
}
IRQ_Interrupt(IQR_RTC){
    printf("IQR_RTC");
    
}
IRQ_Interrupt(IQR_RedirectIQR2){
    printf("IQR_RedirectIQR2");
    
}
IRQ_Interrupt(IQR_Reserv1){
    printf("IQR_Reserv1\n");
}
IRQ_Interrupt(IQR_Reserv2){
    printf("IQR_Reserv2\n");   
}
IRQ_Interrupt(IQR_Reserv3){
    printf("IQR_Reserv3\n");
}
IRQ_Interrupt(IQR_SoProcessor){
    printf("IQR_SoProcessor\n");
}
IRQ_Interrupt(IQR_HardDC){
    printf("IQR_HardDC\n");
    
}
IRQ_Interrupt(IQR_Reserv4){
    printf("IQR_Reserv4\n");
}
    Interrupt(notche){
        printf("\nThis Interrupt free!\n");
    }
    Interrupt(ATA){
    	
/*
        clrscr();
        printf("Interrupt ATA\n");
        //----------- Echo
    char *Adr = (void*)0xb8000;
    uint32_t i = Display_position;
    char *color = (char*)Display_color;
    unsigned short Buffer1;
    HexInAsciiSz(InAx(0x1f7),8);
//----------- 
    setposition(0x0);
    if(!(InAl(0x1f7) & 0x1)){
    for (; i < Display_position+0x500; i+=4)
    {
        //WaitTick(0x10);
        Buffer1 = InAx(0x1f0);
        Adr[i] = Buffer1;
        Adr[i+1] = *color;
        Adr[i+2] = Buffer1 >> 8;
        Adr[i+3] = *color;

    }
    setposition(i);
    }
//EOI;
*/
}



/*
    unsigned short base_lo;
    unsigned short sel;        
    unsigned char always0;     
    unsigned char flags;       
    unsigned short base_hi;
*/
void idt_gradeTask(signed int num, unsigned short selector,unsigned char flag){
    if (num > 254){
        printf("\nError: Exceeding the interval overful IDT\n");
        return;
    }
    idt[num].flags = flag;
    idt[num].sel = selector;
}
void idt_grade(signed int num, unsigned short selector,uint32_t base, unsigned char flag){
    if (num > 254){
        printf("\nError: Exceeding the interval overful IDT\n");
        return;
    }
    //base+=0x24;
    idt[num].base_lo = (uint32_t)base & 0x0000FFFF;
    //idt[num].base_lo += 0x26;
    idt[num].base_hi = (uint32_t)base >> 16 ;
    idt[num].flags = flag;
    idt[num].sel = selector;
}

void idt_install(){
    idt_closed();
    printf("Configure IDT\n");
    //IRQ_Closed();
    for(int i = 0; i< 0xff; i++)
        idt_grade(i, 0x8, (uint32_t)&notche+0x24, 0x8E);
    idtr32.limit = ((sizeof(struct idt_interrupt) * 255) -1 );
    idtr32.base = (uint32_t)idt;
    //IDT_GRADE...
    idt_grade(0x0, 0x8, (uint32_t)&INT_DE       +0x24, 0x8E);
    idt_grade(0x1, 0x8, (uint32_t)&INT_DB       +0x24, 0x8E);
    idt_grade(0x2, 0x8, (uint32_t)&INT_NMI      +0x24, 0x8E);
    idt_grade(0x3, 0x8, (uint32_t)&INT_BP       +0x24, 0x8E);
    idt_grade(0x4, 0x8, (uint32_t)&INT_OF       +0x24, 0x8E);
    idt_grade(0x5, 0x8, (uint32_t)&INT_BR       +0x24, 0x8E);
    idt_grade(0x6, 0x8, (uint32_t)&INT_UD       +0x24, 0x8E);
    idt_grade(0x7, 0x8, (uint32_t)&INT_NM       +0x24, 0x8E);
    idt_grade(0x8, 0x8, (uint32_t)&INT_DF       +0x24, 0x8E);
    idt_grade(0x9, 0x8, (uint32_t)&INT_soproc   +0x24, 0x8E); //9 Превышение сегмента сопроцессора (зарезервировано)
    idt_grade(0xA, 0x8, (uint32_t)&INT_TS       +0x24, 0x8E);
    idt_grade(0xB, 0x8, (uint32_t)&INT_NP       +0x24, 0x8E);
    idt_grade(0xC, 0x8, (uint32_t)&INT_SS       +0x24, 0x8E);
    idt_grade(0xD, 0x8, (uint32_t)&INT_GP       +0x24, 0x8E);
    idt_grade(0xE, 0x8, (uint32_t)&INT_PF       +0x24, 0x8E);
    //idt_grade(0xF, 0x8, (uint32_t)&INT_Intelreserv, 0x8E); //15 Зарезервировано Intel-ом. Не использовать.
    idt_grade(0x10, 0x8, (uint32_t)&INT_MF      +0x24, 0x8E);
    idt_grade(0x11, 0x8, (uint32_t)&INT_AC      +0x24, 0x8E);
    idt_grade(0x12, 0x8, (uint32_t)&INT_MC      +0x24, 0x8E);
    idt_grade(0x13, 0x8, (uint32_t)&INT_XF      +0x24, 0x8E);
    //14h-1Fh - Зарезервировано Intel-ом. Не использовать.
    // idt_grade(0x2e, 0x8, (uint32_t)&ATA+0x24, 0x8E);
  IRQ_Install();

    idt_flush();
    idt_open();


}
        

#include "IRQ.h"
#endif //End
