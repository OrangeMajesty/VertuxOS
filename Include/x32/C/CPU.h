//************************ 
//Platform Setup

//#define AVR 
//#define X64
#pragma once
#ifndef CPU_h
#define CPU_h

void printf(const char *Msg,...);
int strcmp(char a[], char h[]);

//Для того, чтобы получить информацию о дополнительных функциях, имеющихся в процессорах, 
//в регистре EAX перед вызовом CPUID должен быть установлен бит 31.
struct CPUinfo {
	char 			Vendor[13];
	unsigned int 	Apogee;
	char 			verAPIC;
	bool			CPUID;

}__attribute__((packed));
struct CPUinfo CPU;

void check_CPUID(){
	bool CPUID_ = 0;
	__asm(
		"pushfl					\n\t"
		"pop %%EAX 				\n\t"
		"movl %%ebx, %%EAX  	\n\t"
		"xor $0x200000, %%EAX 	\n\t"
		"push %%eax 			\n\t"
		"popfl					\n\t"
		"pushfl 				\n\t"
		"pop %%eax 				\n\t"
		"xor %%ebx, %%eax 		\n\t"
		"je no_cpuid			\n\t"
		"mov $0x1, %0			\n\t"
		"no_cpuid:				\n\t"
		:"=r"(CPUID_)::
		);
		if(CPUID_){
			CPU.CPUID = 1;
		}
}
/*
ASCII-строка	HEX-значения EBX:EDX:ECX	Производитель
GenuineIntel	756E6547:49656E69:6C65746E	Intel
AuthenticAMD	68747541:69746E65:444D4163	AMD
CyrixInstead	69727943:736E4978:64616574	Cyrix
CentaurHauls	746E6543:48727561:736C7561	Centaur
SiS SiS SiS		20536953:20536953:20536953	SiS
NexGenDriven	4778654E:72446E65:6E657669	NexGen
GenuineTMx86	756E6547:54656E69:3638784D	Transmeta
RiseRiseRise	65736952:65736952:65736952	Rise
UMC UMC UMC		20434D55:20434D55:20434D55	UMC
Geode by NSC	646F6547:79622065:43534E20	National Semiconductor
Vortex86 SoC	74726F56:36387865:436F5320	DM&P Electronics

"AMDisbetter!" – early engineering samples of AMD K5 processor
"TransmetaCPU" – Transmeta
"VIA VIA VIA " – VIA
"Vortex86 SoC" – Vortex
The following are known ID strings from virtual machines:

"KVMKVMKVM" – KVM
"Microsoft Hv" – Microsoft Hyper-V or Windows Virtual PC
" lrpepyh vr" – Parallels (it possibly should be "prl hyperv ", but it is encoded as " lrpepyh vr")
"VMwareVMware" – VMware
"XenVMMXenVMM" – Xen HVM
*/
void init_CPU(){
	char Microsoft_HV[] 			= "Microsoft Hv";
	char AMDK5[] 					= "AMDisbetter!";
	char Transmeta1[] 				= "TransmetaCPU";
	char VIA[] 						= "VIA VIA VIA ";
	char Vortex[] 					= "CentaurHauls";
	char KVM[] 						= "KVMKVMKVM";
	char Parallels[] 				= "prl hyperv ";
	char VMware[] 					= "VMwareVMware";
	char Xen_HVM[] 					= "XenVMMXenVMM";
	
	char Intel[] 					= "GenuineIntel";
	char Cyrix[] 					= "CyrixInstead";
	char Centaur[] 					= "CentaurHauls";
	char AMD[] 						= "AuthenticAMD";
	char SiS[] 						= "SiS SiS SiS ";
	char NexGen[] 					= "NexGenDriven";
	char Transmeta[] 				= "GenuineTMx86";
	char Rise[] 					= "RiseRiseRise";
	char UMC[] 						= "UMC UMC UMC ";
	char National_Semiconductor[] 	= "Geode by NSC";
	char DM_P_Electronics[] 		= "Vortex86 SoC";


	check_CPUID();
if(CPU.CPUID){
	//get vendor
	__asm(
		"movl $0x0, %%EAX 	\n\t"
		"cpuid 				\n\t"
		"movl %%ebx, %0		\n\t"
		"movl %%edx, (%0)+4	\n\t"
		"movl %%ecx, (%0)+8	\n\t"
		:"=m"(CPU.Vendor),"=a"(CPU.Apogee)
		:
		:
		);
	if(strcmp(CPU.Vendor, AMD)){
	__asm(
		"movl $0x80000001, %%EAX 	\n\t"
		"cpuid 						\n\t"
		"bt $3, %%ecx 				\n\t"
		"jnc __endInpelapic2		\n\t"
		"movb $0x1, %0				\n\t"
		"__endInpelapic2:			\n\t"
		:"=r"(CPU.verAPIC)
		:
		:
		);
	}
	if(strcmp(CPU.Vendor, Microsoft_HV)){
	}
	if(strcmp(CPU.Vendor, AMDK5)){
	}
	if(strcmp(CPU.Vendor, Transmeta)){
	}
	if(strcmp(CPU.Vendor, Transmeta1)){
	}
	if(strcmp(CPU.Vendor, VIA)){
	}
	if(strcmp(CPU.Vendor, Vortex)){
	}
	if(strcmp(CPU.Vendor, KVM)){
	}
	if(strcmp(CPU.Vendor, Parallels)){
	}
	if(strcmp(CPU.Vendor, VMware)){
	}
	if(strcmp(CPU.Vendor, Xen_HVM)){
	}
	if(strcmp(CPU.Vendor, Intel)){
	__asm(
		"movl $0x1, %%EAX 	\n\t"
		"cpuid 				\n\t"
		"bt $9, %%EDX 		\n\t"
		"jnc __endInpelapic	\n\t"
		"movb $0x1, %0		\n\t"
		"__endInpelapic:	\n\t"
		"bt $21, %%ecx 		\n\t"
		"jnc __endInpelapic1\n\t"
		"incb %0			\n\t"
		"__endInpelapic1:	\n\t"
		:"=r"(CPU.verAPIC)
		:
		:
		);
	}
	if(strcmp(CPU.Vendor, Cyrix)){
	}
	if(strcmp(CPU.Vendor, Centaur)){
	}
	if(strcmp(CPU.Vendor, SiS)){
	}
	if(strcmp(CPU.Vendor, NexGen)){
	}
	if(strcmp(CPU.Vendor, Rise)){
	}
	if(strcmp(CPU.Vendor, UMC)){
	}
	if(strcmp(CPU.Vendor, National_Semiconductor)){
	}
	if(strcmp(CPU.Vendor, DM_P_Electronics)){
	}
}
}
int strcmp(char a[], char h[]) { // Собери библиотеку для строк
int i = 0;
while(1){
	if(a[i] == h[i]){
			if((h[i] == '\0') || (a[i] == '\0')){
				return 1;
				break;
			}}
	else 
		break;
	i++;
}
return 0;
}

#endif