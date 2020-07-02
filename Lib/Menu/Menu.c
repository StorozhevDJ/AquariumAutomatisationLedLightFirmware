

#include <string.h>
#include <inavr.h>
#include "LCD\LCD_futaba.h"

#include "Keys.h"
#include "Menu.h"





unsigned char menu_time;



/********************************************************************************/
/*							Бегущая строка							*/
/*	Выводит на дисплей бегущую строку, сдвиг символов происхлдит при вызове	*/
/* функции														*/
/*	nom_str - номер строки на которой будет отображаться строка (1-4)		*/
/*  Старший бит (0х80) переключает режим перехода от конца к началу строки		*/
/*	str - текстовая строка для вывода									*/
/*	nom_start_char - символ с которого начинается вывод строки на дисплей,	*/
/*если выводим новый текст то необходимо сбросить в 0						*/
/********************************************************************************/
unsigned char nom_start_char;
  signed char kol_space=LCD_COLUMN;
unsigned char Menu_Marquee (unsigned char __flash *str, unsigned char nom_str)
{
unsigned char temp;
unsigned char st=0;
if  (nom_str&0x80) st=kol_space;
if ((nom_str&0x07)==1) LCD_SendCmd(LCD_DRAM_LINE1+st);
if ((nom_str&0x07)==2) LCD_SendCmd(LCD_DRAM_LINE2+st);
if ((nom_str&0x07)==3) LCD_SendCmd(LCD_DRAM_LINE3+st);
if ((nom_str&0x07)==4) LCD_SendCmd(LCD_DRAM_LINE4+st);

str+=nom_start_char;
for (temp=(LCD_COLUMN-kol_space);temp>0;temp--)
	{
	if (*str == 0)
		{
		if (temp==LCD_COLUMN) kol_space=LCD_COLUMN;
		break;
		}
	LCD_SendUChar(*str++);
	};

if (kol_space>0)
	{
	nom_start_char=0;
	kol_space--;
	}
else nom_start_char++;
if (kol_space==19) return 0;
return 1;
}



/********************************************************************************/
/*							Меню									*/
/********************************************************************************/
unsigned char Menu_Menu1 (unsigned char __flash ** menu_LCD, unsigned char max_val, unsigned char nom_menu)
{
unsigned char k;
unsigned char n=1;
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrUCF80(&menu_LCD[nom_menu][0]);
__delay_cycles(PROCESSOR_CLOCK/3);
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_SendUChar('>');
while (1)
	{
	k=get_keys();
	if ((k==KEY_U)||(k==KEY_D)||(k==KEY_ESC)||(k==KEY_ENT))
		{
		if ((n<max_val)&&(k==KEY_D)) n++;
		if ((n>1)&&(k==KEY_U)) n--;
		LCD_SendCmd(LCD_DRAM);
		LCD_WriteStrUCF80(&menu_LCD[nom_menu][0]);
		if (n==1) LCD_SendCmd(LCD_DRAM_LINE1);
		if (n==2) LCD_SendCmd(LCD_DRAM_LINE2);
		if (n==3) LCD_SendCmd(LCD_DRAM_LINE3);
		if (n==4) LCD_SendCmd(LCD_DRAM_LINE4);
		LCD_SendUChar('>');
		if (k==KEY_ESC)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return 0;
			};
		if (k==KEY_ENT)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return n;
			};
		__delay_cycles(PROCESSOR_CLOCK/3);
		};
	};
}



/*******************************************************************************
*							Меню									*
*	Производит постраничное листание пунктов меню						*
*	menu_LCD - массив страниц текста меню								*
*	min_val - номер верхней строки, которую можно выбрать					*
*	max_val - номер последней строки, которую можно выбрать				*
*	nom_menu - номер страници, с которой начинается отоброжение меню			*
*	default_val - номер строки на которой по умолчанию оказывается указатель	*
********************************************************************************/
unsigned char Menu_Page_flash (unsigned char __flash ** menu_LCD, unsigned char min_val, unsigned char max_val, unsigned char nom_menu, char default_val)
{
unsigned char k;
//unsigned char n=min_val-1;
unsigned char n;
if (default_val>max_val) n=min_val-1; else
if (default_val<min_val) n=min_val-1; else
n=default_val;
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrUCF80(&menu_LCD[nom_menu+(n/LCD_ROW)][0]);
__delay_cycles(PROCESSOR_CLOCK/3);
if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM_LINE1);
if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM_LINE2);
if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM_LINE3);
if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM_LINE4);
LCD_SendUChar('>');
while (1)
	{
	k=get_keys();
	if ((k==KEY_U)||(k==KEY_D)||(k==KEY_ESC)||(k==KEY_ENT))
		{
		if ((n<max_val-1)&&(k==KEY_D)) n++;
		if ((n>min_val-1)&&(k==KEY_U)) n--;
		LCD_SendCmd(LCD_DRAM);
		LCD_WriteStrUCF80(&menu_LCD[nom_menu+(n/LCD_ROW)][0]);
		if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM_LINE1);
		if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM_LINE2);
		if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM_LINE3);
		if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM_LINE4);
		LCD_SendUChar('>');
		if (k==KEY_ESC)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return 0xff;
			};
		if (k==KEY_ENT)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return n;
			};
		__delay_cycles(PROCESSOR_CLOCK/3);
		};
	};
}



/*******************************************************************************
*							Меню									*
*	Производит постраничное листание пунктов меню из строки в ОЗУ			*
*	min_val - номер верхней строки, которую можно выбрать					*
*	max_val - номер последней строки, которую можно выбрать				*
*	nom_menu - номер страници, с которой начинается отоброжение меню			*
*	default_val - номер строки на которой по умолчанию оказывается указатель	*
********************************************************************************/
unsigned char Menu_Page (unsigned char str_menu[LCD_COLUMN], unsigned char min_val, unsigned char max_val, char default_val)
{
unsigned char k;
unsigned char n;

if (default_val>max_val) n=min_val-1; else
if (default_val<min_val) n=min_val-1; else
n=default_val;
LCD_SendCmd(LCD_DRAM);
LCD_WriteStrUCCount(&str_menu[(n/LCD_ROW)], 80);
__delay_cycles(PROCESSOR_CLOCK/3);
if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM_LINE1);
if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM_LINE2);
if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM_LINE3);
if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM_LINE4);
LCD_SendUChar('>');
while (1)
	{
	k=get_keys();
	if ((k==KEY_U)||(k==KEY_D)||(k==KEY_ESC)||(k==KEY_ENT))
		{
		if ((n<max_val-1)&&(k==KEY_D)) n++;
		if ((n>min_val-1)&&(k==KEY_U)) n--;
		LCD_SendCmd(LCD_DRAM);
		LCD_WriteStrUCCount(&str_menu[80*(n/LCD_ROW)], 80);
		if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM_LINE1);
		if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM_LINE2);
		if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM_LINE3);
		if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM_LINE4);
		LCD_SendUChar('>');
		if (k==KEY_ESC)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return 0xff;
			};
		if (k==KEY_ENT)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return n;
			};
		__delay_cycles(PROCESSOR_CLOCK/3);
		};
	};
}



/********************************************************************************/
/*							Меню	(НЕ ДОДЕЛАНО!)						*/
/*	Производит постраничное листание пунктов меню из структуры в EEPROM		*/
/*	min_val - номер верхней строки, которую можно выбрать					*/
/*	max_val - номер последней строки, которую можно выбрать				*/
/*	nom_menu - номер страници, с которой начинается отоброжение меню			*/
/*	default_val - номер строки на которой по умолчанию оказывается указатель	*/
/********************************************************************************/
/*extern __eeprom TEEspring_tabl	EE_spring_tabl[];
unsigned char Menu_Page_EEPROM_struct (char default_val)
{
unsigned char k;
unsigned char n;

if (default_val>100-4) n=100-4; else n=default_val;

LCD_SendCmd(LCD_DRAM);
write_str_lcd_PPPc((unsigned char *)(&EE_spring_tabl[n-1].spring_name[0]), LCD_COLUMN);
__delay_cycles(PROCESSOR_CLOCK/3);
if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM);
if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM|0x40);
if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM|0x14);
if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM|0x54);
LCD_SendUChar('>');
while (1)
	{
	k=get_keys();
	if ((k==KEY_U)||(k==KEY_D)||(k==KEY_ESC)||(k==KEY_ENT))
		{
		if ((n<100)&&(k==KEY_D)) n++;
		if ((n>  1)&&(k==KEY_U)) n--;
		LCD_SendCmd(LCD_DRAM);
		write_str_lcd_PPPc((unsigned char *)(&EE_spring_tabl[n-1].spring_name[0]), LCD_COLUMN);
		if (n%LCD_ROW==0) LCD_SendCmd(LCD_DRAM);
		if (n%LCD_ROW==1) LCD_SendCmd(LCD_DRAM|0x40);
		if (n%LCD_ROW==2) LCD_SendCmd(LCD_DRAM|0x14);
		if (n%LCD_ROW==3) LCD_SendCmd(LCD_DRAM|0x54);
		LCD_SendUChar('>');
		if (k==KEY_ESC)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return 0xff;
			};
		if (k==KEY_ENT)
			{
			__delay_cycles(PROCESSOR_CLOCK/3);
			return n;
			};
		__delay_cycles(PROCESSOR_CLOCK/3);
		};
	};
}*/



/********************************************************************************
*							Меню									*
*	Построчное листание меню											*
*	menu3_LCD - массив строк меню										*
********************************************************************************/
/*unsigned char Menu_Line_flash (const char __flash menu3_LCD[][LCD_COLUMN], unsigned char min_val, unsigned char max_val)
{
unsigned char k;
unsigned char s=1;
unsigned char n=min_val-1;
while (1)
	{
	LCD_SendCmd(LCD_DRAM_LINE1);
	LCD_WriteStrUCF20(&menu3_LCD[n][0]);
	LCD_SendCmd(LCD_DRAM_LINE2);
	LCD_WriteStrUCF20(&menu3_LCD[n+1][0]);
	LCD_SendCmd(LCD_DRAM_LINE3);
	LCD_WriteStrUCF20(&menu3_LCD[n+2][0]);
	LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_WriteStrUCF20(&menu3_LCD[n+3][0]);
	if      (s==1) LCD_SendCmd(LCD_DRAM_LINE1);
	else if (s==2) LCD_SendCmd(LCD_DRAM_LINE2);
	else if (s==3) LCD_SendCmd(LCD_DRAM_LINE3);
	else if (s==4) LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_SendUChar('>');//(0x10);
	__delay_cycles(PROCESSOR_CLOCK/4);
	k=get_keys();
	while ((k!=KEY_U)&&(k!=KEY_D)&(k!=KEY_ESC)&&(k!=KEY_ENT)) k=get_keys();
	if (k==KEY_D)
		{
		if (s<LCD_ROW) s++;
		else if ((n+LCD_ROW)<max_val) n++;
		};
	if (k==KEY_U)
		{
		if (s>1) s--;
		else if (n>min_val-1) n--;
		};
	if (k==KEY_ENT) return n+s;
	if (k==KEY_ESC) return 0xff;
	};
}*/

unsigned char Menu_Lineflash (const char __flash menu_string[][LCD_COLUMN], unsigned char min_val, unsigned char max_val)
{
unsigned char key;
unsigned char cursor_line=min_val;	//строка курсора на дисплее (может меняться от 1 до 4)
unsigned char menu_line=0;		//указатель на выбранный пункт меню (меняется от min до max-LCD_COLUMN)
while (1)
	{
	LCD_SendCmd(LCD_DRAM_LINE1);
	LCD_WriteStrUCF20(&menu_string[menu_line+0][0]);
	LCD_SendCmd(LCD_DRAM_LINE2);
	LCD_WriteStrUCF20(&menu_string[menu_line+1][0]);
	LCD_SendCmd(LCD_DRAM_LINE3);
	LCD_WriteStrUCF20(&menu_string[menu_line+2][0]);
	LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_WriteStrUCF20(&menu_string[menu_line+3][0]);
	if      (cursor_line==1) LCD_SendCmd(LCD_DRAM_LINE1);
	else if (cursor_line==2) LCD_SendCmd(LCD_DRAM_LINE2);
	else if (cursor_line==3) LCD_SendCmd(LCD_DRAM_LINE3);
	else if (cursor_line==4) LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_SendUChar(0x10);
	__delay_cycles(PROCESSOR_CLOCK/4);
	key=get_keys();
	while ((key!=KEY_U)&&(key!=KEY_D)&(key!=KEY_ESC)&&(key!=KEY_ENT)) key=get_keys();
	if (key==KEY_D)
		{
		if (cursor_line<LCD_ROW) cursor_line++;//мах 4
		else if (/*(menu_line+LCD_ROW)<max_val*/menu_string[menu_line+4][1]) menu_line++;
		};
	if (key==KEY_U)
		{
		//курсор и меню прокручены в низ, двигаем курсор
		if ((cursor_line>1)&&(menu_line+cursor_line>min_val)) cursor_line--;
		//курсор в верху экрана, меню прокручено вниз, двигаем меню
		else if((cursor_line==1)&&(menu_line>min_val)) menu_line--;
		//курсор поднят до первого пункта меню, меню еще не поднято до верха, поднимим меню
		else if((menu_line<min_val)&&(menu_line>0))
			{
			cursor_line++;
			menu_line--;
			}
		//Меню поднято до верха, курсор ниже первого пункта меню, поднимим курсор
		else if((menu_line==0)&&(cursor_line>min_val)) cursor_line--;
		};
	if (key==KEY_ENT) return menu_line+cursor_line-min_val;
	if (key==KEY_ESC) return 0xff;
	};
}



/********************************************************************************
*							Меню									*
*	Построчное листание меню из переданной строки во флеше					*
*	str - массив строк меню
*	default_val - номер строки на которой по умолчанию оказывается указатель	*
*	str - передаваяемая строка с текстом меню							*
********************************************************************************/
unsigned char Menu_LineFlash (const char __flash *str, unsigned char default_val)
{
unsigned char max_val;
unsigned char k;
unsigned char s=1;
unsigned char n=(default_val);

for (unsigned int i=0; i<100*MENU_STR_LEN; i++)
	{
	if (str[i]==0)
		{
		max_val=i/MENU_STR_LEN;
		break;
		}
	}

if (n>max_val-LCD_ROW)
	{
	s=LCD_ROW-(max_val-n);
	n=max_val-LCD_ROW;
	}

while (1)
	{
	LCD_SendCmd(LCD_DRAM_LINE1);
	LCD_WriteStrUCF20(&str[MENU_STR_LEN*(n+0)]);
	LCD_SendCmd(LCD_DRAM_LINE2);
	LCD_WriteStrUCF20(&str[MENU_STR_LEN*(n+1)]);
	LCD_SendCmd(LCD_DRAM_LINE3);
	LCD_WriteStrUCF20(&str[MENU_STR_LEN*(n+2)]);
	LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_WriteStrUCF20(&str[MENU_STR_LEN*(n+3)]);
	
	if (s==1) LCD_SendCmd(LCD_DRAM_LINE1);
	if (s==2) LCD_SendCmd(LCD_DRAM_LINE2);
	if (s==3) LCD_SendCmd(LCD_DRAM_LINE3);
	if (s==4) LCD_SendCmd(LCD_DRAM_LINE4);
	LCD_SendUChar(0x10);//('>');
	
	__delay_cycles(PROCESSOR_CLOCK/4);
	k=get_keys();
	while (k==0xff) k=get_keys();
	if (k==KEY_D)
		{
		if (s<LCD_ROW) s++;
		else if ((n+LCD_ROW)<max_val) n++;
		};
	if (k==KEY_U)
		{
		if (s>1) s--;
		else if (n>0) n--;
		};
	if (k<10)
		{
		s=1;
		if (k*10<=max_val-LCD_ROW) n=k*10;
		};
	if (k==KEY_ENT) return n+s;
	if (k==KEY_ESC) return 0xff;
	};
}



/********************************************************************************/
/*							Меню									*/
/*	Построчное листание меню из структуры в EEPROM						*/
/*	default_val - номер строки на которой по умолчанию оказывается указатель	*/
/********************************************************************************/
/*const char __flash empty_LCD[]={"Пусто              "};
const char __flash clear_str[]={"                    "};
unsigned char Menu_Str_EEPROM_struct (unsigned char default_val)
{
unsigned char k;
unsigned char s=0;
unsigned char n;

TEEspring_tabl string_menu[4];

if (default_val>=100) n=100-1; else n=default_val;


while (1)
	{
	string_menu[0]=EE_spring_tabl[n+0];
	string_menu[1]=EE_spring_tabl[n+1];
	string_menu[2]=EE_spring_tabl[n+2];
	string_menu[3]=EE_spring_tabl[n+3];
	
	LCD_SendCmd(LCD_DRAM|0x00);
	write_str_lcd_P(&clear_str[0]);
	LCD_SendCmd(LCD_DRAM|0x00);
	if (s==0) LCD_SendUChar('>'); else LCD_SendUChar(' ');
	if ((string_menu[0].spring_name[0]!=0xff)&&(string_menu[0].spring_name[0]!=0x00))
		write_str_lcd_PPPc(&string_menu[0].spring_name[0], 19);
	else write_str_lcd_P(&empty_LCD[0]);
	
	LCD_SendCmd(LCD_DRAM|0x40);
	write_str_lcd_P(&clear_str[0]);
	LCD_SendCmd(LCD_DRAM|0x40);
	if (s==1) LCD_SendUChar(0x10); else LCD_SendUChar(' ');
	if ((string_menu[1].spring_name[0]!=0xff)&&(string_menu[1].spring_name[0]!=0x00))
		write_str_lcd_PPPc(&string_menu[1].spring_name[0], 19);
	else write_str_lcd_P(&empty_LCD[0]);
	
	LCD_SendCmd(LCD_DRAM|0x14);
	write_str_lcd_P(&clear_str[0]);
	LCD_SendCmd(LCD_DRAM|0x14);
	if (s==2) LCD_SendData('>'); else LCD_SendUChar(' ');
	if ((string_menu[2].spring_name[0]!=0xff)&&(string_menu[2].spring_name[0]!=0x00))
		write_str_lcd_PPPc(&string_menu[2].spring_name[0], 19);
	else write_str_lcd_P(&empty_LCD[0]);
	
	LCD_SendCmd(LCD_DRAM|0x54);
	write_str_lcd_P(&clear_str[0]);
	LCD_SendCmd(LCD_DRAM|0x54);
	if (s==3) LCD_SendData('>'); else LCD_SendUChar(' ');
	if ((string_menu[3].spring_name[0]!=0xff)&&(string_menu[3].spring_name[0]!=0x00))
		write_str_lcd_PPPc(&string_menu[3].spring_name[0], 19);
	else write_str_lcd_P(&empty_LCD[0]);
	
	
	__delay_cycles(PROCESSOR_CLOCK/4);
	k=get_keys();
	while (k==0xff) k=get_keys();
	if (k==KEY_D)
		{
		if (s<3) s++;
		else if ((n+4)<100) n++;
		};
	if (k==KEY_U)
		{
		if (s>0) s--;
		else if (n>1-1) n--;
		};
	if (k<10)
		{
		s=0;
		n=k*10;
		};
	if (k==KEY_ENT) return n+s;
	if (k==KEY_ESC) return 0xff;
	};
}*/



/********************************************************************************/
/*					Ввод чисел unsigned int							*/
/*	Ввод беззнаковых числовых значений int с заданным диапазоном			*/
/*	dec - число которое отображается в качестве начального числа			*/
/*	min, max - минимальные и максимальные числа, которые можно ввести		*/
/*	addr_lcd - адрес на экране, где будет отображаться вводимое число		*/
/*	Функция возвращает значение unsigned int							*/
/********************************************************************************/
unsigned int Menu_InputInt (unsigned int dec, unsigned int min, unsigned int max, unsigned char addr_lcd)
{
unsigned char k=0xfe;
unsigned int  dec_temp;
if (dec>max) dec=max;
if (dec<min) dec=min;
dec_temp=dec;
LCD_SendCmd(LCD_DRAM|addr_lcd);
Menu_ClearSpaceForInt(max);
LCD_SendCmd(LCD_DRAM|addr_lcd);
LCD_WriteIntDec(dec);//Выводим значение по умолчанию
LCD_SendCmd(0x0D);
while (1)
	{
	while (k==0xff) k=get_keys();
	if ((k<10)&&(dec<=max/10)) dec=dec*10+k;
	if (dec>max) dec=max;
	if (k==KEY_DOT)
		{
		dec=0;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		Menu_ClearSpaceForInt(max);
		}
	LCD_SendCmd(LCD_DRAM|addr_lcd);
	if(dec != 0) LCD_WriteIntDec(dec);
	__delay_cycles(PROCESSOR_CLOCK/4);
	if (k==KEY_U)
		{
		if (dec<max) dec++;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteIntDec(dec);
		__delay_cycles(PROCESSOR_CLOCK/4);
		}
	if (k==KEY_D)
		{
		if (dec>min) dec--;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		Menu_ClearSpaceForInt(max);
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteIntDec(dec);
		__delay_cycles(PROCESSOR_CLOCK/4);
		}
	if (k==KEY_L)
		{
		dec/=10;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteIntDec(dec);
		LCD_SendUChar(' ');
		}
	if (k==KEY_ENT) break;
	if (k==KEY_ESC)
		{
		LCD_SendCmd(0x0C);
		return dec_temp;
		}
	k=0xff;
	};
if (dec<min) dec=min;
LCD_SendCmd(0x0C);
return dec;
}



/********************************************************************************/
/*					Ввод чисел signed Int							*/
/*	Ввод числовых значений int со знаком с заданным диапазоном				*/
/*	dec - число которое отображается в качестве начального числа			*/
/*	min, max - минимальные и максимальные числа, которые можно ввести		*/
/*	addr_lcd - адрес на экране, где будет отображаться вводимое число		*/
/*	Функция возвращает значение signed int									*/
/********************************************************************************/
signed int Menu_InputSInt (signed int dec, signed int min, signed int max, unsigned char addr_lcd)
{
unsigned char k=0xfe;
unsigned int  dec_temp;
if (dec>max) dec=max;
if (dec<min) dec=min;
dec_temp=dec;
LCD_SendCmd(LCD_DRAM|addr_lcd);
if (min<0) LCD_SendUChar(' ');
Menu_ClearSpaceForInt(max);
LCD_SendCmd(LCD_DRAM|addr_lcd);
LCD_WriteSIntDec(dec);//Выводим значение по умолчанию
LCD_SendCmd(0x0D);
while (1)
	{
	while (k==0xff) k=get_keys();
	if ((k<10)&&(dec<=max/10)) dec=dec*10+k;
	if (dec>max) dec=max;
	if (k==KEY_DOT)
		{
		dec=0;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		if (min<0) LCD_SendUChar(' ');
		Menu_ClearSpaceForInt(max);
		}
	LCD_SendCmd(LCD_DRAM|addr_lcd);
	//if(dec != 0) LCD_WriteSIntDec(dec);
	__delay_cycles(PROCESSOR_CLOCK/3);
	if (k==KEY_U)
		{
		if (dec<0)
			{
			LCD_SendCmd(LCD_DRAM|addr_lcd);
			LCD_SendUChar(' ');
			Menu_ClearSpaceForInt(max);
			}
		if (dec<max) dec++;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteSIntDec(dec);
		__delay_cycles(PROCESSOR_CLOCK/3);
		}
	if (k==KEY_D)
		{
		if (dec>min) dec--;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		if (min<0) LCD_SendUChar(' ');
		Menu_ClearSpaceForInt(max);
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteSIntDec(dec);
		__delay_cycles(PROCESSOR_CLOCK/3);
		}
	if (k==KEY_L)
		{
		dec/=10;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteSIntDec(dec);
		LCD_SendUChar(' ');
		}
	if (k==KEY_R)
		{
		dec=-dec;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteSIntDec(dec);
		}
	if (k==KEY_ENT) break;
	if (k==KEY_ESC)
		{
		LCD_SendCmd(0x0C);
		return dec_temp;
		}
	k=0xff;
	};
if (dec<min) dec=min;
LCD_SendCmd(0x0C);
return dec;
}



/********************************************************************************/
/*							Ввод чисел							*/
/*	Ввод числовых значений long с заданным диапазоном						*/
/*	dec - число которое отображается в качестве начального числа			*/
/*	min, max - минимальные и максимальные числа, которые можно ввести		*/
/*	addr_lcd - адрес на экране, где будет отображаться вводимое число		*/
/*	Функция возвращает значение long									*/
/********************************************************************************/
unsigned long Menu_InputLong (unsigned long dec, unsigned long min, unsigned long max, unsigned char addr_lcd)
{
unsigned char k=0xfe;
unsigned long  dec_temp;
dec_temp=dec;
LCD_SendCmd(LCD_DRAM|addr_lcd);
/*if (max>=1000000000) write_str_lcd_PP(&space_10_LCD[0]); else
if (max>=100000000) write_str_lcd_PP(&space_10_LCD[1]); else
if (max>=10000000) write_str_lcd_PP(&space_10_LCD[2]); else
if (max>=1000000) write_str_lcd_PP(&space_10_LCD[3]); else
if (max>=100000) write_str_lcd_PP(&space_10_LCD[4]); else
if (max>=10000) write_str_lcd_PP(&space_10_LCD[5]); else
if (max>=1000) write_str_lcd_PP(&space_10_LCD[6]); else
if (max>=100) write_str_lcd_PP(&space_10_LCD[7]); else
if (max>=10) write_str_lcd_PP(&space_10_LCD[8]);*/
if (max>=10) LCD_SendUChar(' ');
if (max>=100) LCD_SendUChar(' ');
if (max>=1000) LCD_SendUChar(' ');
if (max>=10000) LCD_SendUChar(' ');
if (max>=100000) LCD_SendUChar(' ');
if (max>=1000000) LCD_SendUChar(' ');
if (max>=10000000) LCD_SendUChar(' ');
if (max>=100000000) LCD_SendUChar(' ');
if (max>=1000000000) LCD_SendUChar(' ');

LCD_SendCmd(0x0D);
while (1)
	{
	while (k==0xff) k=get_keys();
	if ((k<10)&&(dec<=max/10)) dec=dec*10+k;
	if (dec>max) dec=max;
	if (k==KEY_DOT)
		{
		dec=0;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		/*if (max>=1000000000) write_str_lcd_PP(&space_10_LCD[0]); else
		if (max>=100000000) write_str_lcd_PP(&space_10_LCD[1]); else
		if (max>=10000000) write_str_lcd_PP(&space_10_LCD[2]); else
		if (max>=1000000) write_str_lcd_PP(&space_10_LCD[3]); else
		if (max>=100000) write_str_lcd_PP(&space_10_LCD[4]); else
		if (max>=10000) write_str_lcd_PP(&space_10_LCD[5]); else
		if (max>=1000) write_str_lcd_PP(&space_10_LCD[6]); else
		if (max>=100) write_str_lcd_PP(&space_10_LCD[7]); else
		if (max>=10) write_str_lcd_PP(&space_10_LCD[8]);*/
		if (max>=10) LCD_SendUChar(' ');
		if (max>=100) LCD_SendUChar(' ');
		if (max>=1000) LCD_SendUChar(' ');
		if (max>=10000) LCD_SendUChar(' ');
		if (max>=100000) LCD_SendUChar(' ');
		if (max>=1000000) LCD_SendUChar(' ');
		if (max>=10000000) LCD_SendUChar(' ');
		if (max>=100000000) LCD_SendUChar(' ');
		if (max>=1000000000) LCD_SendUChar(' ');
		}
	LCD_SendCmd(LCD_DRAM|addr_lcd);
	if(dec != 0) LCD_WriteLongDecDot(dec, 0);
	if (k==KEY_L)
		{
		dec/=10;
		LCD_SendCmd(LCD_DRAM|addr_lcd);
		LCD_WriteLongDecDot(dec, 0);
		LCD_SendUChar(' ');
		}
	__delay_cycles(PROCESSOR_CLOCK/3);
	if (k==KEY_ENT) break;
	if (k==KEY_ESC)
		{
		LCD_SendCmd(0x0C);
		return dec_temp;
		}
	k=0xff;
	};
if (dec<min) dec=min;
LCD_SendCmd(0x0C);
return dec;
}






__flash char char_table[10][5]=
	{
	{'0', '#', '/', '-', '+'},
	{'1', ' ', '_', '=', '!'},
	{'2', 'А', 'Б', 'В', 'Г'},
	{'3', 'Д', 'Е', 'Ж', 'З'},
	{'4', 'И', 'Й', 'К', 'Л'},
	{'5', 'М', 'Н', 'О', 'П'},
	{'6', 'Р', 'С', 'Т', 'У'},
	{'7', 'Ф', 'Х', 'Ц', 'Ч'},
	{'8', 'Ш', 'Щ', 'Ъ', 'Ы'},
	{'9', 'Ь', 'Э', 'Ю', 'Я'}
	};
/********************************************************************************/
/*					Ввод символов в одну строку						*/
/*	Ввод числовых значений int с заданным диапазоном						*/
/*	str - передаваемая и возвращаемая строка							*/
/*	len - максимальная длинна строки отображаемая на дисплее				*/
/*	addr_lcd - адрес на экране, где будет отображаться вводимое число		*/
/*	Функция возвращает значение ENT или ESC								*/
/********************************************************************************/
unsigned char Menu_InputString (unsigned char *str, unsigned char len, unsigned char addr_lcd)
{
unsigned char k=0xfe;			//код нажатой клавиши
unsigned char char_num=0;		//Номер введеного символа в строке
unsigned char k_num=0;			//Количество нажатий клавиши
unsigned char ch=0;				//Код введеного символ

if (len>LCD_COLUMN) len=LCD_COLUMN;
str[len-1]=0;

LCD_SendCmd(LCD_DRAM|addr_lcd);		//Переводим курсор на нужную позицию
for (unsigned char i=0; i<len-2; i++) LCD_SendUChar(' ');//Очистим место под текст
LCD_SendCmd(LCD_DRAM|addr_lcd);
LCD_WriteStr(&str[0]);			//Отображаем начальный текст
//LCD_SendCmd(LCD_DRAM|addr_lcd);
LCD_SendCmd(LCD_CONTROL|DISPLAY|BLINK);//Включаем дисплей и курсор

for (char i=0; i<LCD_COLUMN; i++)
	{
	if (str[i]==0)
		{
		char_num=i;
		break;
		}
	}
//char_num=0;


while (1)
	{
	__delay_cycles(PROCESSOR_CLOCK/3);	//Задержка ввода следующего символа
	k=0xff;
	while (k==0xff) k=get_keys();		//Крутимся тут, пока не нажмут клавишу
	k_num=0;
	while (1)
		{
		k=0xff;
		menu_time=100;
		while ((k==0xff)&&(menu_time)) k=get_keys();	//ожидаем в течении 1.5 сек нажатия клавиши
		if (k<10)						//Если принятый код клавишь 0-9, выведим введенный символ
			{
			if (ch!=k)
				{
				k_num=0;
				ch=k;
				}
			
			LCD_SendCmd(LCD_DRAM|(addr_lcd+char_num));//Переводим курсор на нужную позицию с учетом введеных символов
			LCD_SendUChar(char_table[ch][k_num%5]);//выводим на дисплей символ нажатой клавиши
			LCD_SendCmd(LCD_DRAM|(addr_lcd+char_num));
			k_num++;					//Увеличиваем номер символа в наборе символов данной клавиши
			}
		else break;					//Если приняли не символ, то выйдем
		__delay_cycles(PROCESSOR_CLOCK/3);	//Задержка ввода следующего символа
		}
	if (((k==0xff)&&(ch<10))||(k==KEY_DOT))	//Если ввели символ 0-9, а-я или "."
		{
		if (k==KEY_DOT)				//Введена "."
			{
			str[char_num]='.';
			LCD_SendUChar('.');
			}
		else							//Если нет, значит ввели др. символ
			{
			if (ch<10) str[char_num]=char_table[ch][(k_num-1)%5];
			}
		if (char_num<len-2) char_num++;
		LCD_SendCmd(LCD_DRAM|(addr_lcd+char_num));	//Переводим курсор на нужную позицию с учетом введеных символов
		}
	if (k==KEY_L)					//Нажали клавишу "влево", удалим последний символ
		{
		if (char_num>0) char_num--;
		LCD_SendCmd(LCD_DRAM|(addr_lcd+char_num));	//Переводим курсор на нужную позицию с учетом введеных символов
		}
	else
	if (k==KEY_R)					//Нажали клавишу "вправо", переведем курсор
		{
		if ((char_num<len-2)&&(str[char_num]!=0)) char_num++;
		LCD_SendCmd(LCD_DRAM|(addr_lcd+char_num));//Переводим курсор на новую позицию
		}
	else
	if ((k==KEY_ENT)||(k==KEY_ESC))	//Нажали клавишу "ENT" или "ESC", выйдим
		{
		//Удалим конец строки
		for (char_num++; char_num<len-1; char_num++) str[char_num]=' ';
		str[len-1]=0;
		LCD_SendCmd(LCD_CONTROL|DISPLAY);	//Отключим курсор
		break;
		}
	}

return k;
}



void Menu_ClearSpaceForInt(int i)
{
LCD_SendUChar(' ');
if (i>=10) LCD_SendUChar(' '); else return;
if (i>=100) LCD_SendUChar(' '); else return;
if (i>=1000) LCD_SendUChar(' '); else return;
if (i>=10000) LCD_SendUChar(' '); else return;

}



void Menu_ClearSpaceForLong(long i)
{
LCD_SendUChar(' ');
if (i>=10) LCD_SendUChar(' '); else return;
if (i>=100) LCD_SendUChar(' '); else return;
if (i>=1000) LCD_SendUChar(' '); else return;
if (i>=10000) LCD_SendUChar(' '); else return;
if (i>=100000) LCD_SendUChar(' '); else return;
if (i>=1000000) LCD_SendUChar(' '); else return;
if (i>=10000000) LCD_SendUChar(' '); else return;
if (i>=100000000) LCD_SendUChar(' '); else return;
if (i>=1000000000) LCD_SendUChar(' '); else return;
}