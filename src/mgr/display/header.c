/*
 	Описание служебной информации Менеджера экрана.
	По Шаблону "DriverHeader"

*/
#define NumberPorts 0x2 	// Количество запрашиваемых портов

struct {
 	unsigned int signature;
 	unsigned short purpose;
 	unsigned int version;

 	unsigned long user;
 	unsigned short NeedPort[NumberPorts];

 } __attribute__((packed)) 

DisplayManager = {
	0x64756C65,				// "module" & 0xffffffff 
 	0x00000300, 			// ushort (type) | uchar (class) | uchar (subclass) 
 	0x0,  					// version
 	0x0,
 	{0x1f34,0x1f42}
 };
	

