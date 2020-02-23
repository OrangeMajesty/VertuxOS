#ifndef APIC_h
#define APIC_h
#include "CPU.h"

// Если вы собираетесь использовать процессор локальный APIC и IOAPIC, необходимо сначала отключить ПОС. Это делается с помощью:
// мов  Al ,  0xFF 
// из  0xa1 ,  аль 
// из  0x21 ,  ал

char ifAPIC() {
_cpuid();
return _cpuid_valid;
}
void init_ioapic(){

}
#endif