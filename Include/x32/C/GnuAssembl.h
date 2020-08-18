#pragma once

#define Int(name) __asm("int $"#name"\n")
#define PANIC __asm("cli\nhlt\n"); while(1){}
// Table INT
#define EOI __asm("movb $0x20, %%al\n outb %%al, $0x20\n outb %%al, $0xA0\n":::)
#define IRQ_Interrupt(name) void name(); \
    __asm(#name ":\n pusha\n call _"#name"\n movb $0x20, %al \n outb %al, $0x20\n outb %al, $0xA0\n popa\n iret\n"); \
    void _ ## name()
#define Interrupt(name) void name(); \
        __asm(#name": call _"#name"\n iret \n");    \
        void _ ## name()

void WaitTick(unsigned long Tick){
	while(Tick){
		__asm("nop\n");
		Tick--;
	}
}

void idt_closed(){
    __asm("cli\n\t");
}
void idt_open(){
    __asm("sti\n\t");
}
