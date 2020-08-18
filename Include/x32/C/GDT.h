#ifndef GDT_h
#define GDT_h


#include "Display.h"

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[Max_Table_GDT];
struct gdt_ptr gdtr32;


/* This is in start.asm. We use this to properly reload
*  the new segment registers */

extern unsigned int _Stack_down;

void gdt_flush(void){
     __asm volatile(
        "lgdt %0\n\t"
        //"movl %1, %%eax  \n\t"
       // "movl %%eax, %%esp\n\t"
        "movw $0x10, %%ax\n\t"
        "movw %%ax, %%gs\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%ss\n\t"
        "movw %%ax, %%ds\n\t"
        "ljmp $0x8, $fin\n\t"
        "fin:\n\t"
        :
        : "m"(gdtr32), "m"(_Stack_down)
        :
        );
}
void tss_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access) 
{
    //TSS
    // биты доступа
    //  0: = 1
    //  1: Бит B (Busy) - занятость задачи (см. следующую главу)
    //  2: = 1
    //  3: = 0
    //  4: = 0
    // 5,6: = DPL - Уровень привилегий сегмента TSS (см. следующую главу)
    //  7: P - бит присутствия сегмента, обычно установлен в 1.

    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16);

    /* Finally, set up the granularity and access flags */
    gdt[num].access = access;
}
 // void ldt_set_gate(unsigned long* AdrLDT, int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran){
 //    /* Setup the descriptor base address */

 // }
/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(unsigned int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call _gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gdtr32.limit = (sizeof(struct gdt_entry) * Max_Table_GDT) - 1;
    gdtr32.base = (uint16_t)&gdt;
    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    /* Flush out the old GDT and install the new changes! */

    //gdt_set_gate(3, 0, 0xFFFFFFFF, 0x92, 0xCF); // LDT Alias

    //tss_set_gate(3, 0 , 0xffffffff, 0xD);
    
    gdt_flush();

}
#endif //End
