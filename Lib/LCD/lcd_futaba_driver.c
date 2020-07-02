
#include <inavr.h>
#include <ioavr.h>

#include "lcd_futaba_driver.h"



void LCD_SPI (unsigned char byte);



void LCD_SendCmd(unsigned char cmd)
{
#if defined LCD_SPI_MODE || defined LCD_SOFT_SPI_MODE
	LCD_SPI_STB_ON;
	LCD_SPI(0xF8);
	LCD_SPI(cmd);
	LCD_SPI_STB_OFF;
#else
	__delay_cycles(200);
	PORTG|=(1<<PG2);   // E _/``
	__delay_cycles(200);
	// конфигурируем порт на вывод
	PORTC=cmd;
	__delay_cycles(200);
	DDRC=0xFF;
	__delay_cycles(200);
	PORTG&=~(1<<PG2);   // E ``\_
	__delay_cycles(200);
#endif
}



void LCD_SendData(unsigned char data)
{
#if defined LCD_SPI_MODE || defined LCD_SOFT_SPI_MODE
	LCD_SPI_STB_ON;
	LCD_SPI(0xF8|0x02);
	LCD_SPI(data);
	LCD_SPI_STB_OFF;
#else
	__delay_cycles(200);
	PORTG|=(1<<PG0);
	__delay_cycles(200);
	send_cmd_lcd(data);
	__delay_cycles(200);
	PORTG&=~(1<<PG0);
	__delay_cycles(200);
	space_cntr++;
	__delay_cycles(200);
#endif
}



void LCD_SPI (unsigned char byte)
{
#ifdef LCD_SPI_MODE
SPDR = byte;				//Data Byte
while(!(SPSR & (1<<SPIF)));

#elif defined LCD_SOFT_SPI_MODE
for (unsigned char i=0; i<8; i++)
	{
	PORTSCK=0;
	__delay_cycles(2);
	PORTSI=(byte&(0x80>>i))?1:0;
	__delay_cycles(2);
	PORTSCK=1;
	__delay_cycles(2);
	};
#else

#endif
}



void LCD_LuminanceLevel(unsigned char val)
{
LCD_SendCmd(LCD_FUNCTION|DL_8BIT|LINE|(val&0x03));
}



void LCD_Cursor(unsigned char display_enable, unsigned char mode)
{
LCD_SendCmd(LCD_CONTROL|((display_enable&0x01)<<2)|(mode&0x03));
}



void LCD_Init(void)
{
#ifdef LCD_SPI_MODE
	LCD_SPI_STB_OFF;
	DDRB |= (1<<PB3)|(1<<PB5)|(1<<PB4)|(1<<PB2);// Set MOSI, SCK and MISO(STB), SS as output
	//DDRB &=~(1<<PB4);				// Set MISO as input
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR0);// Enable SPI, Master, set clock rate fck/16
#elif defined LCD_SOFT_SPI_MODE
	LCD_SPI_STB_OFF;
	DDRSTB=1;
	PORTSCK=1;
	DDRSCK=1;
	DDRSI=1;
#else
	__delay_cycles(100);
	PORTG&=~(1<<PG1)|(1<<PG0)|(1<<PG2);  // RS, RW, E
	__delay_cycles(100);
	DDRG|=(1<<PG1)|(1<<PG0)|(1<<PG2);
	__delay_cycles(100);
	PORTC=0x00;
	__delay_cycles(100);
	DDRC=0xFF;
#endif

__delay_cycles(100);
/*LCD_SendCmd(LCD_FUNCTION|DL_8BIT|LINE);
LCD_SendCmd(0x38);
LCD_SendCmd(0x38);
LCD_SendCmd(0x38);
LCD_SendCmd(0x38);
LCD_SendCmd(0x38);
LCD_SendCmd(0x38);*/
LCD_SendCmd(LCD_CLEAR);
LCD_SendCmd(LCD_HOME);
LCD_SendCmd(LCD_CONTROL|DISPLAY);//Display On
LCD_LuminanceLevel(3);
}
