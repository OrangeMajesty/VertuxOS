/*
;==================================
;В начале обработки прерывания посылайте в контроллер 8259A команду конца прерывания (EOI). 
;Контроллер состоит из двух контроллеров master и slave. Master обслуживает первые 8 IRQ, 
;slave - вторые и для них посылка EOI будет выглядеть так:
;
;для master (IRQ 0..7)
;	mov	al,20h
;	out	20h,al
;для slave (IRQ 8..15)
;	mov	al,20h
;	out	0a0h,al
;--------------------------------
;Значения в портах 21h и A1h содержат флаги маскировки прерываний для master- и slave-контроллера соответственно. 
;Для того, чтобы разрешить какое-либо прерывание, нужно сбросить соответствующий бит, а для запрещения - установить.
;
;Прерывания master:
;Бит	IRQ	Устройство
;0	0	Таймер
;1	1	Клавиатура
;2	2	Каскад (подключён ко второму контроллеру)
;3	3	COM 2/4
;4	4	COM 1/3
;5	5	LPT 2
;6	6	Контроллер дисковода FDC (Floppy Drive Controller)
;7	7	LPT 1
;Прерывания slave:
;Бит	IRQ	Устройство
;0	8	Часы реального времени RTC (Real Time Clock)
;1	9	Редирект с IRQ 2
;2	10	Резерв (т.е. не имеет устройства по умолчанию)
;3	11	Резерв (т.е. не имеет устройства по умолчанию)
;4	12	Резерв (т.е. не имеет устройства по умолчанию)
;5	13	Исключение сопроцессора
;6	14	Контроллер винчестера HDC (Hard Drive Controller)
;7	15	Резерв (т.е. не имеет устройства по умолчанию)
;------------------- Пример ------------------------
;in al,21h      ; Читаем маску master-а
;and    al,0feh ; FEh = 11111110b - сбрасываем 0-й бит.
;out    21h,al      ; Записываем маску в контроллер. Таймер разрешён.
;===========================================================
Контроллер прерываний — устройство, которое получает запросы на аппаратные прерывания от всех внешних устройств. Он определяет, какие запросы следует обслужить, какие должны ждать своей очереди, а какие не будут обслуживаться вообще. Контроллеров прерываний, так же как и контроллеров DMA, два. Первый контроллер, обслуживающий запросы на прерывания от IRQ0 до IRQ7, управляется через порты 20h и 21h, а второй (IRQ8 – IRQ15) — через порты A0h и A1h.
Команды контроллеру делят на команды управления (OCW) и инициализации (ICW):
порт 20h/A0h для записи: OCW2, OCW3, ICW1
порт 20h/A0h для чтения — см. команду OCW3
порт 21h/A1h для чтения и записи: OCW1 — маскирование прерываний
порт 21h/A1h для записи — ICW2, ICW3, ICW4 сразу после ICW1

*/


#include "Type.h"
#include "CPU.h"
//#include "APIC.h"

#ifndef IRQ_h
#define IRQ_h

#define IQR_Master          0x20
#define IQR_Master1         0x21

#define IQR_Slave           0xA0
#define IQR_Slave1          0xA1

#define IQR_k_Base          0x20

#define IQR_k_Timer         IQR_k_Base + 0x0
#define IQR_k_Keybord       IQR_k_Base + 0x1
#define IQR_k_IQR2          IQR_k_Base + 0x2
#define IQR_k_COM2_4        IQR_k_Base + 0x3
#define IQR_k_COM1_3        IQR_k_Base + 0x4
#define IQR_k_LTP2          IQR_k_Base + 0x5
#define IQR_k_FloppyDC      IQR_k_Base + 0x6
#define IQR_k_LTP1          IQR_k_Base + 0x7
#define IQR_k_RTC           IQR_k_Base + 0x8
#define IQR_k_RedirectIQR2  IQR_k_Base + 0x9
#define IQR_k_Reserv1       IQR_k_Base + 0x10
#define IQR_k_Reserv2       IQR_k_Base + 0x11
#define IQR_k_Reserv3       IQR_k_Base + 0x12
#define IQR_k_SoProcessor   IQR_k_Base + 0x13
#define IQR_k_HardDC        IQR_k_Base + 0x14
#define IQR_k_Reserv4       IQR_k_Base + 0x15

//#include "IO.h"



void idt_grade(signed int num, unsigned short selector,uint32_t base, unsigned char flag);


//Настройка Контроллера Прерываний.
 // ; сброс заявки в контроллере прерываний: посылка End-Of-Interrupt (EOI) ...
 //    push ax
 //    mov  al, 20h
 //    out  020h, al   ; ... в ведущий (Master) контроллер ...
 //    out  0A0h, al   ; ... и в ведомый (Slave) контроллер.
 //    pop  ax




// Interrupt Mask Register (IMR) (Регистр маскирования запросов на прерывания) 
// — описывает, прерывания каких уровней сейчас заблокированы (замаскированы).

// Interrupt Request Register (IRR) (Регистр запросов на прерывания) — 
// описывает, какие прерывания сейчас ожидают подтверждения.

// In-Service Register (ISR) (Регистр обрабатываемых запросов) — 
// описывает, какие прерывания сейчас активны и требуют команды на завершение прерывания (EOI — End Of Interrupt).

// bool IRQ_DeGet(uint8_t num){
//     bool temp = 0x0;
//     //fixme
//     return temp;
// }

// void IRQ_DeSet(uint8_t num, bool ClOp){       //Вкл\Откл Прерываний отдельных устройств.
//     //fixme
// }
//http://ibzh.16mb.com/asm/irq.htm#aDSCR
void IRQ_Setup(){
    const unsigned char icw2_1 = (unsigned char)IQR_k_Base;
    const unsigned char icw2 = (unsigned char)icw2_1 + 0x8;

__asm volatile("movb $0x11, %%al\n"
              "outb %%al, $0x20\n"

              "movb %0, %%al\n"
              "outb %%al, $0x21\n"

              "movb $0x04, %%al\n"
              "outb %%al, $0x21\n"

              "movb $0x01, %%al\n"
              "outb %%al, $0x21\n"

              "movb $0x11, %%al\n"
              "outb %%al, $0xA0\n"
              
              "movb %1, %%al\n"
              "outb %%al, $0xA1\n"
              
              "movb $0x2, %%al\n"
              "outb %%al, $0xA1\n"

              "movb $0x1, %%al\n"
              "outb %%al, $0xA1\n"


                : 
                :"r"(icw2_1),"r"(icw2)
                :"%al"
                );
    // __asm("movb $0xF0, %al\n outb %al, $0x21\n");
    // __asm("movb $0xFF, %al\n outb %al, $0xA1\n");

}
void IRQ_Open(){
    __asm volatile (
        "inb $0x70,%%al       \n\t"
        "andb $0x7F, %%al      \n\t"
        "outb %%al,$0x70      \n\t"
        :
        :
        :"%al", "memory"
        );
/*
    in   al, 70h
    and  al, 7Fh
    out  70h, al
*/
}
void IRQ_Closed(){
    // __asm("movb $0xFF, %al\n outb %al, $0x21\n");
    // __asm("movb $0xFF, %al\n outb %al, $0xA1\n");
    __asm volatile (
        "inb $0x70,%%al       \n\t"
        "orb $0x80, %%al      \n\t"
        "outb %%al,$0x70      \n\t"
        :
        :
        :"%al", "memory"
        );
/*    
    in   al, 70h
    or   al, 80h
    out  70h, al
*/
}
/*
in al,60h ; AL содержит скан-код клавиатуры
*/



void IRQ_Install (){
      if((!CPU.verAPIC)){ // Сменить in !cpuid
    printf("\nPIC Found");
    //=Отключиm APIC
    IRQ_Closed();
    IRQ_Setup();
    // IRQ_DeSet(0x1,0x1);



    idt_grade(IQR_k_Timer,          0x8, (uint32_t)&IRQ_Timer+0x24,          0x8E);
    idt_grade(IQR_k_Keybord,        0x8, (uint32_t)&IRQ_Keybord+0x24,        0x8E);
    idt_grade(IQR_k_IQR2,           0x8, (uint32_t)&IQR_IQR2+0x24,           0x8E);
    idt_grade(IQR_k_COM2_4,         0x8, (uint32_t)&IQR_COM2_4+0x24,         0x8E);
    idt_grade(IQR_k_COM1_3,         0x8, (uint32_t)&IQR_COM1_3+0x24,         0x8E);
    idt_grade(IQR_k_LTP2,           0x8, (uint32_t)&IQR_LTP2+0x24,           0x8E);
    idt_grade(IQR_k_FloppyDC,       0x8, (uint32_t)&IQR_FloppyDC+0x24,       0x8E);
    idt_grade(IQR_k_LTP1,           0x8, (uint32_t)&IQR_LTP1+0x24,           0x8E);
    idt_grade(IQR_k_RTC,            0x8, (uint32_t)&IQR_RTC+0x24,            0x8E);
    idt_grade(IQR_k_RedirectIQR2,   0x8, (uint32_t)&IQR_RedirectIQR2+0x24,   0x8E);
    idt_grade(IQR_k_Reserv1,        0x8, (uint32_t)&IQR_Reserv1+0x24,        0x8E);
    idt_grade(IQR_k_Reserv2,        0x8, (uint32_t)&IQR_Reserv2+0x24,        0x8E);
    idt_grade(IQR_k_Reserv3,        0x8, (uint32_t)&IQR_Reserv3+0x24,        0x8E);
    idt_grade(IQR_k_SoProcessor,    0x8, (uint32_t)&IQR_SoProcessor+0x24,    0x8E);
    idt_grade(IQR_k_HardDC,         0x8, (uint32_t)&IQR_HardDC+0x24,         0x8E);
    idt_grade(IQR_k_Reserv4,        0x8, (uint32_t)&IQR_Reserv4+0x24,        0x8E);

    //IRQ_Open();

} else {
    if(CPU.verAPIC == 0x2)
        printf("\nAPIC x2 Found");
    else
        printf("\nAPIC Found");
    //init_ioapic();
}
    

}
#endif //End
