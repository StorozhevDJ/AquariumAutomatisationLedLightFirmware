
#ifndef LCD_FUTABA_H
#define LCD_FUTABA_H


#include "lcd_futaba_driver.h"



#define LCD_DRAM_LINE1      LCD_DRAM|0x00
#define LCD_DRAM_LINE2      LCD_DRAM|0x40
#define LCD_DRAM_LINE3      LCD_DRAM|0x14
#define LCD_DRAM_LINE4      LCD_DRAM|0x54

#define LCD_ROW		4
#define LCD_COLUMN		20

#define mantiss 15



void LCD_Init(void);

void LCD_SendCmd(unsigned char x);

void LCD_SendChar(void *c);
void LCD_SendUChar(unsigned char x);
void LCD_WriteStr(void * str);
void LCD_WriteStrCCF(const char __flash * str);
void LCD_WriteStrUCF(unsigned char __flash * str);
void LCD_WriteStrUC(unsigned char * str);
void LCD_WriteStrUCCount(unsigned char * str,unsigned char count);
unsigned char LCD_WriteStrUCF20(const char __flash * str);
void LCD_WriteStrUCF80(unsigned char __flash * str);
void LCD_WriteStrUCFCount(unsigned char __flash * str, unsigned char count);
void LCD_WriteByteHex(unsigned char data);

void LCD_WriteByteDec(unsigned char data);
void LCD_WriteByteSDec(signed char data);
void LCD_WriteIntDec(unsigned int data);
void LCD_WriteSIntDec(signed int data);

void LCD_WriteLongDec(unsigned long data);
void LCD_WriteLongSDec(signed long data);

void LCD_WriteByteDecDot(unsigned char data, unsigned char dot);
void LCD_WriteByteSDecDot(signed char data, unsigned char dot);
void LCD_WriteIntDecDot(unsigned int data, unsigned char dot);
void LCD_WriteIntSDecDot(signed int data, unsigned char dot);

void LCD_WriteByteBin(char data);

void LCD_WriteLongDecDot(unsigned long data, unsigned char dot);
void LCD_WriteLongSDecDot(signed long data, unsigned char dot);

void LCD_WriteIntDecFn(unsigned int data);
void LCD_WriteIntDecExtFn(unsigned int data);
void LCD_WriteHexArr(unsigned char * data, unsigned char kol);



#endif //LCD_FUTABA_H
