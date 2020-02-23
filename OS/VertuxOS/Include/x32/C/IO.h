#ifndef IO_h
#define IO_h
//#define mbarrier() asm volatile ("":::"memory")

// /* Input a byte from a port */
// inline unsigned char inportb(unsigned int port)
// {
//    unsigned char ret;
//    asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
//    return ret;
// }

//  Output a byte to a port 
// /* July 6, 2001 added space between :: to make code compatible with gpp */

// inline void outportb(unsigned int port,unsigned char value)
// {
//    asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
// }

// --------- IN

unsigned int InEax(unsigned int adr){
	unsigned int ret = 0x0;
	__asm("inl %%dx, %%eax"
		:"=a"(ret)
		:"d"(adr)
		:
		);
	return ret;
}
unsigned InAx(unsigned int adr){
	unsigned ret = 0x0;
	__asm("inw %%dx, %%ax"
		:"=a"(ret)
		:"d"(adr)
		:
		);
	return ret;
}
unsigned char InAl(unsigned int adr){
	unsigned char ret = 0x0;
	__asm("inb %%dx, %%al"
		:"=a"(ret)
		:"d"(adr)
		:
		);
	return ret;
}

// -------- OUT
void OutEax(unsigned int adr, unsigned long data){
	__asm("outl %%eax, %%dx"
		:
		: "d"(adr), "a"(data)
		:
		);
}
void OutAx(unsigned int adr, unsigned short data){
	__asm("outw %%ax, %%dx"
		:
		: "d"(adr), "a"(data)
		:
		);
}
void OutAl(unsigned int adr, unsigned char data){
	__asm("outb %%al, %%dx"
		:
		: "d"(adr), "a"(data)
		:
		);
}




#endif //End

