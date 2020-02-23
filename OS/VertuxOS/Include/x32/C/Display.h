/**************************************
// Драйвер для текстового видеорежима
// Разрешение: 25*80
// База: 0xb8000
// ------------ Примечания ------------
// Добиться модульности для выгрузки
// из памяти. Заменить "Стандартным"
// драйвером для графич. режима.
// No Support PCI Video Device
*//////////////////////////////////////
//	Supported functions
// '\n' - New Line
// '\t' - Tabular
///////////////////////////////////////
#ifndef _Display_h
#define _Display_h

//#include "Type.h"

#define BASE_A 0xA0000
#define BASE 0xB8000

#define Display_V 50	//Size
#define Display_H 160	//Size

//uint8_t status = 0;	// 1 - Graphe mode | 0 - Text mode

void printf(const char *Msg,...);
void clrscr(void);
uint32_t getposition(void);
void setcolor (char color);
void setposition(uint32_t Offset); 	// Для корректноого отображения,
									//использовать четные числа.


uint32_t Display_position = (uint32_t)0x0;
char *Display_color = '\0';



//Display_H = (uint16_t)25;
//Display_V = (uint16_t)80;

//Color list



//-- end
void showCopyrating(void) {
	clrscr();
printf("\n\n\n\t========================= Copyrating ========================\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t|\t\t\t\t\t\t|\n");
	printf("\t=============================================================\n");
	
	printf("\tPress any key to Continue\t\t\t(c) 2020");

}
//см Функции с переменным числом параметров
// void out_char(unsigned char word){

// }
void HexInAsciiSz(const long hex_,unsigned char size){
	unsigned char list[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
//----------- Echo
	char *Adr = (void*)BASE;
	uint32_t i = Display_position;
	char *color = (char*)Display_color;
//----------- 

	for(char item = -4;i != (Display_position+(0x1*size*2)); i+=2, item-=4){
	Adr[i] = list[(hex_ >> item) & 0xf];
	Adr[i+1] = *color;
	}

	Display_position = i;
	
}
void printf(const char* Msg,...) {
	//char *Msg = "Hello I'm Stalk OS!";
	char *Adr = (void*)BASE;
	uint32_t i = Display_position;
	char *color = (char*)Display_color;
	char *err = "NULL";
	Msg+=36; // fixme 
	unsigned char *Arg = (void*)Msg;
//	unsigned char *Arg_str;
	int number = 0;
	err+=36;
	while(*Msg)
	{
		if(*Msg == '\0') break;
		switch(*Msg)
		{
		case '%' :
		// Start % switch
				number++;
				Msg++;
				switch(*Msg)
				{
					case 's':
					Msg++;
					break;
					case 'h':
					Msg++;
					// Arg = (void*)&Msg+(number*4);
					//Arg_str = Arg;
					while(1){
						Adr[i] = *Arg;
						Adr[i+1] = *color;
						i+=2;
						Arg++;
					}
					break;
					case '%': 
					number--;
					Msg++;
						Adr[i] = '%';
						Adr[i+1] = *color;
						i+=2;
					break;
					default:
					Msg++;
						while(*err){
							Adr[i] = *err;
							Adr[i+1] = *color;
							i+=2;
							err++;

						}
						err-=4;
						break;
				}
				break;
		// End % switch
				
		case '\n' : 	
				Msg++;
				setposition(i+(Display_H-(i%Display_H)));
				i = Display_position; break;
		case '\t' : 	
				//b = i;
				Msg++;
				setposition(i+(20-(i%20)));
				i = Display_position; break;
		default: 
			Adr[i] = *Msg;
			Adr[i+1] = *color;
			i+=2;
			Msg++;
			break;
		}
	
	}
	if(i < 0xfff){ 
		Display_position = i;
	}
	else {
		Display_position = 0x0;
	}
	
}
void setposition(uint32_t Offset) {
	Display_position = Offset;
}
void setcolor (char color) {
	*Display_color = color;
}
void clrscr(void) {
	//setcolor('\0');
	char *Adr = (void*)BASE;
	char *color = (char*)Display_color;
	//*color += 36;
	
	for(uint32_t i = 0x0; i != (Display_H * Display_V);i+=2)
	{
		Adr[i] = 0x0;	
		Adr[i+1] = *color; // background
	}
	setposition(0x0);
}

#endif //End

