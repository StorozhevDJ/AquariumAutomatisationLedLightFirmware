
#ifndef LCD_FUTABA_DRIVER_H
#define LCD_FUTABA_DRIVER_H


#define LCD_CLEAR     (0x01)
#define LCD_HOME      (0x02)
#define LCD_MODE      (0x04)
	#define INC      (0x02)
	#define ASHIFT   (0x01)
#define LCD_CONTROL   (0x08)
	#define DISPLAY  (0x04)
	#define CURSOR   (0x02)
	#define BLINK    (0x01)
#define LCD_CURSOR    (0x10)
	#define DSHIFT   (0x08)
	#define TO_RIGHT (0x04)
#define LCD_FUNCTION  (0x20)
	#define DL_8BIT   (0x10)
	#define LINE     (0x08)
	#define FONT     (0x04)
#define LCD_CGRAM     (0x40)
#define LCD_DRAM      (0x80)

#define LUMINANCE_LEVEL_25	3
#define LUMINANCE_LEVEL_50	2
#define LUMINANCE_LEVEL_75	1
#define LUMINANCE_LEVEL_100	0

#define LCD_STR_LEN		(20)



#define DDRSI		DDRB_Bit3
#define DDRSTB		DDRB_Bit4
#define DDRSCK		DDRB_Bit5

#define PORTSI		PORTB_Bit3
#define PORTSTB	PORTB_Bit4
#define PORTSCK	PORTB_Bit5

#define LCD_SPI_STB_ON	PORTSTB=0//(PORTB&=~(1<<PB4))
#define LCD_SPI_STB_OFF	PORTSTB=1//(PORTB|= (1<<PB4))


//#define LCD_SPI_MODE
#define LCD_SOFT_SPI_MODE
//#define MOTOROLA_M68	
//#define INTEL_I80



void LCD_Init(void);
void LCD_SendCmd(unsigned char x);
void LCD_SendData(unsigned char x);

void LCD_LuminanceLevel(unsigned char val);
void LCD_Cursor(unsigned char display_enable, unsigned char mode);



#endif //LCD_FUTABA_DRIVER_H