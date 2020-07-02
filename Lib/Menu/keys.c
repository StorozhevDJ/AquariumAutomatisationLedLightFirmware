//******************************************************************************//
//							Keyboard								//
//******************************************************************************//
/*
  1 PF0 -----1----2----3----4----5	 \	1	2	3	4	5
             |    |    |    |    |	 |
  2 PF1 -----6----7----8----9----10 |	6	7	8	9	0
             |    |    |    |    |	  > IN
  3 PF2 -----11---12---13---14---15 |		F	^	.
             |    |    |    |    |	 |	ESC				ENT
  4 PF3 -----16---17---18---+----+  |		<-	V	->
		   |    |    |    |    |	/
             |    |    |    |    |	_
             |    |    |    |    |	 \
  A PE2 -----/    |    |    |    |	 |
                  |    |    |    |	 |
  B PE3 ----------/    |    |    |	 |
                       |    |    |	  > OUT
  C PE4 ---------------/    |    |	 |
                            |    |	 |
  D PE5 --------------------/    |	 |
				 		   |	 |
  E PE6 -------------------------/	/
*/


#include "main.h"
#include "inavr.h"
#include "keys.h"



void Init_keys(void)
{ //Настраиваем выходы (Col)
/*  DDRE |= (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2);
  PORTE |= (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2);
  //Настраиваем входы (ROW)
  DDRB &= ~(1<<5)|(1<<4)|(1<<0);
  DDRE &= ~(1<<7);
  PORTB |= (1<<5)|(1<<4)|(1<<0);
  PORTE &= ~(1<<7);*/
DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD7));
PORTD|=((1<<PD5)|(1<<PD6)|(1<<PD7));
}



unsigned char get_keys(void)
{
/*unsigned char temp_spcr;
  unsigned char key_temp, temp;

  temp_spcr = SPCR;
  SPCR &= ~(1<<SPE);

  key_temp = 0xff;
  temp = 0xff;
  __watchdog_reset();

  //Настраиваем выходы (Col)
  DDRE  |= (1<<5)|(1<<4)|(1<<3)|(1<<2);
  PORTE |= (1<<5)|(1<<4)|(1<<3)|(1<<2);
  //Настраиваем входы (ROW)
  DDRB  &=~(1<<5)|(1<<4)|(1<<0);
  DDRE  &=~(1<<7)|(1<<6);
  PORTB |= (1<<5)|(1<<4)|(1<<0);
  PORTE |= (1<<7)|(1<<6);

  DDRB_Bit0 = 0;

  for(unsigned char i = 0; i < 20; i++)
  { PORTE &= ~(1<<5);
    __delay_cycles(100);
    if ((PINE&(1<<PE6))==0x00) key_temp = 0xff;
    if ((PINB&(1<<PB5))==0x00) key_temp = KEY_ESC;
    if ((PINB&(1<<PB4))==0x00) key_temp = KEY_U;
    if ((PINB&(1<<PB0))==0x00) key_temp = KEY_ENT;
    if ((PINE&(1<<PE7))==0x00) key_temp = 0xff;
    PORTE |= (1<<5);
	
    PORTE&=~(1<<4);
    __delay_cycles(100);
    if ((PINE&(1<<PE6))==0x00) key_temp = KEY_4;
    if ((PINB&(1<<PB5))==0x00) key_temp = KEY_1;
    if ((PINB&(1<<PB4))==0x00) key_temp = KEY_2;
    if ((PINB&(1<<PB0))==0x00) key_temp = KEY_R;
    if ((PINE&(1<<PE7))==0x00) key_temp = KEY_3;
    PORTE|=(1<<4);
	
    PORTE&=~(1<<3);
    __delay_cycles(100);
    if ((PINE&(1<<PE6))==0x00) key_temp = KEY_8;
    if ((PINB&(1<<PB5))==0x00) key_temp = KEY_5; 
    if ((PINB&(1<<PB4))==0x00) key_temp = KEY_6;
    if ((PINB&(1<<PB0))==0x00) key_temp = KEY_L;
    if ((PINE&(1<<PE7))==0x00) key_temp = KEY_7;
    PORTE |= (1<<3);

    PORTE &= ~(1<<2);
    __delay_cycles(100);
    if ((PINE&(1<<PE6))==0x00) key_temp = KEY_DOT;
    if ((PINB&(1<<PB5))==0x00) key_temp = KEY_F;
    if ((PINB&(1<<PB4))==0x00) key_temp = KEY_9;
    if ((PINB&(1<<PB0))==0x00) key_temp = KEY_D;
    if ((PINE&(1<<PE7))==0x00) key_temp = KEY_0;
    PORTE|=(1<<2);
	
    if (key_temp==0xff)
    { PORTB_Bit0=1;
	 DDRB_Bit0=1;
	 SPCR=temp_spcr;
	 return 0xff;
    }
    if (temp!=0xff)
    { if (temp!=key_temp)
	 {
			PORTB_Bit0=1;
			DDRB_Bit0=1;
			SPCR=temp_spcr;
			return 0xff;
			}
		}
	temp=key_temp;
	key_temp=0xff;
	}
PORTB_Bit0=1;
DDRB_Bit0=1;
SPCR=temp_spcr;
return temp;*/

/*Сенсорная кнопка
int key_avr=0;
for (char i=0; i<10; i++)
	{
	char key_time;
	PORTD_Bit5=0;
	DDRD_Bit5=1;	//Разряжаем емкость кнопки
	while (TCNT1);//Ждем перехода таймера через 0
	DDRD_Bit5=0;	//Отпускаем выход и даем емкости зарядиться
	//PORTD_Bit5=1;		//PullUp
	while (PIND_Bit5==0);//Ждем когда емкость зарядится
	key_time=TCNT1;	//Емкость зарядилась, смотрим сколько времени прошло
	key_avr+=key_time;	//добавим это время в фильтр
	}
return key_avr/10;*/


if (PIND_Bit5==0)//UP
	{
	//Проверяем на дребезг в течении 0,1сек
	char i;
	for (i=0; i<10; i++)
		{
		__delay_cycles(PROCESSOR_CLOCK/1000);
		if (PIND_Bit5!=0) break;
		}
	//Если кнопка все это время была нажата, проверим она одна нажата или совместно с другой
	if (i>=9)
		{
		if (PIND_Bit6==0) return KEY_DOT;
		if (PIND_Bit7==0) return KEY_L;
		return KEY_U;
		}
	}
if (PIND_Bit6==0)//DOWN
	{
	//Проверяем на дребезг в течении 0,1сек
	char i;
	for (i=0; i<10; i++)
		{
		__delay_cycles(PROCESSOR_CLOCK/1000);
		if (PIND_Bit6!=0) break;
		}
	//Если кнопка все это время была нажата, проверим она одна нажата или совместно с другой
	if (i>=9)
		{
		if (PIND_Bit5==0) return KEY_DOT;
		if (PIND_Bit7==0) return KEY_R;
		return KEY_D;
		}
	}
if (PIND_Bit7==0)//ENT
	{
	//Проверяем на дребезг в течении 0,1сек
	char i;
	for (i=0; i<10; i++)
		{
		__delay_cycles(PROCESSOR_CLOCK/1000);
		if (PIND_Bit7!=0) break;
		}
	//Если кнопка все это время была нажата, проверим она одна нажата или совместно с другой
	if (i>=9)
		{
		if (PIND_Bit5==0) return KEY_L;
		if (PIND_Bit6==0) return KEY_R;
		return KEY_ENT;
		}
	}
return 0xff;
}
