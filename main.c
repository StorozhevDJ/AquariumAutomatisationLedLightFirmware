

#include <inavr.h>

#include "main.h"

#include "Time.h"
#include "PWM.h"
#include "PWM.h"
#include "ds1821\ds1821.h"

#include "Lib\LCD\lcd_futaba.h"
#include "src\LCD_text.h"
#include "Menu\Menu.h"
#include "Lib\Menu\Keys.h"
#include "MyMenu.h"








__flash const unsigned int tabl_Uout[8]={10,20,40,80,160,320,640,760};



unsigned char brightness;	//Значение в таблице яркостей
unsigned char brightness_tmp;

unsigned char key_mode;		//Режим управления кнопкой

struct
	{
	unsigned char sec:1;
	unsigned char min:1;
	unsigned char hour:1;
	unsigned char day:1;
	} time_flag;
unsigned int  new_pwm;//Current value of PWM

char temperature;


TEwork_mode work_mode;
TEpwm_dir pwm_dir;



//Tpwm_setting LED_PWM_setting_current[3];	//Last, current and next LED PWM settings


char test;
#pragma vector=TIMER2_COMP_vect
__interrupt void Timer2(void) 
{
if (++time.x10ms>=100)
	{
	time_flag.sec=1;
	time.x10ms=0;
	if (++time.sec>=60)
		{
		time.sec=0;
		time_flag.min=1;
		if (++time.min>=60)
			{
			time.min=0;
			time_flag.hour=1;
			if (++time.hour>=24)
				{
				time.hour=0;
				time.time_correct=time_correct;	//обновим 
				if ((++time.day>day_in_mon[time.mon-1])||	//Последний день в месяце?
				    ((time.day==29)&&(time.mon==2)&&(time.year&0x03)))//Учет високосного года
					{
					time.day=1;
					if (++time.mon>12)
						{
						time.mon=1;
						time.year++;
						}
					}
				}
			}
		}
	//Корректировка ухода часов (в 24:00)
	if ((time.hour==0)&&(time.time_correct!=0))
		{
		time.sec+=(time.time_correct>0)?1:-1;
		if (time.time_correct>0) time.time_correct--; else time.time_correct++;
		}
	}
}




// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input|0xC0;
// Start the AD conversion
ADCSR|=0x40;
// Wait for the AD conversion to complete
while ((ADCSR & 0x10)==0);
ADCSR|=0x10;
return ADC;
}



/*
    Uout*T			 tabl_Uout[brightness]*PWM_COUNT
Ton=------	=>	OCR1B=-------------------------------;
     Uin						read_adc(1)

T=PWM_COUNT;	//Задаем сами, определяет частоту ШИМ и как следствие индуктивность L
0<=Ton<=T;	//Определяет выходную скважность и как следствие яркость

3<=Uin<=4.5;	=>	586<=read_adc()<=976
3<=Uout<=3.8;	=>	586<=tabl_Uout[]<=742
*/
void Set_Uout (void)
{
unsigned int u_in;
unsigned int temp;
//Смотрим входное напряжение
u_in=read_adc(0);
//u_in=600;
//Если входное напряжение меньше чем требуемое выходное, сделаем его равным
if (tabl_Uout[brightness]>u_in) u_in=tabl_Uout[brightness];
//Рассчитаем значение ШИМ таймера
temp=(unsigned long)(((unsigned long)(tabl_Uout[brightness]*PWM_COUNT))/u_in);
if (OCR1B!=temp)
	{
	for (unsigned char i=0; i<PWM_COUNT; i++)
		{
		if (OCR1B<temp) OCR1B++;
		if (OCR1B>temp) OCR1B--;
		__delay_cycles((PROCESSOR_CLOCK/PWM_COUNT)/4);
		}
	}
}







void main( void )
{
//Инициализируем ШИМ таймер и порт
PWM_Init();

//Инициализируем таймер для часов (псевдо RTC + timer)
Timer_Init();


Tpwm_setting pwm_last, pwm_next;


//pwm_next=PWM_GetNextVal(&time);

/*DDRC_Bit0=0;	//Вход АЦП
PORTC_Bit0=0;

//Инициализируем АЦП
// ADC Clock frequency: 62,500 kHz
// ADC Voltage Reference: Int., cap. on AREF
// ADC High Speed Mode: Off
ADMUX=0xC0;
ADCSR=0x87;
SFIOR&=0xEF;*/

__delay_cycles(PROCESSOR_CLOCK/10);

Init_keys();

LCD_Init();
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrCCF(LCD_hello);
__delay_cycles(PROCESSOR_CLOCK*2);

__enable_interrupt();

/*WDTCR=0x1F;
WDTCR=0x0F;
WDTCR |= (1<<WDP2)|(1<<WDP1)|(1<<WDP0);

asm("wdr");

//make sure watchdod will be followed by a reset (must set this one to 0 because it resets the WDE bit)
MCUSR &= ~(1 << WDRF);
//set up WDT interrupt (from that point one have 4 cycle to modify WDTCSR)
WDTCSR = (1<<WDCE)|(1<<WDE);
//Start watchdog timer with 1s prescaller and interrupt only
WDTCSR = (1<<WDIE)|(0<<WDE)|(1<<WDP2)|(1<<WDP1);*/


LCD_Home(&time, new_pwm, pwm_dir_none, __get_termo_1wire(&PORTB, &DDRB, &PINB, 0));
pwm_next=PWM_GetNextVal(&time);


while (1)
{//Timer2();
//Прошла секунда, обработаем ежесекундные события
if (time_flag.sec)
	{
	time_flag.sec=0;
	temperature=__get_termo_1wire(&PORTB, &DDRB, &PINB, 0);
	if (work_mode==automatic)
		{
		new_pwm=PWM_Calc(pwm_last, pwm_next, &time);
		PWM_SetOut(new_pwm);
		}
	LCD_Home(&time, new_pwm, pwm_dir, temperature);//Обновим данные на экране
	}
//Началась новая минута, обработаем минутные события
if (time_flag.min)
	{
	time_flag.min=0;
	//Проверим следующее значение ШИМ
	if ((time.hour>=pwm_next.hour)&&(time.min>=pwm_next.min))
		{
		pwm_last=pwm_next;
		pwm_next=PWM_GetNextVal(&time);
		if (pwm_last.val>pwm_next.val) pwm_dir=pwm_dir_dn;
		else if (pwm_last.val<pwm_next.val) pwm_dir=pwm_dir_up;
		else if (pwm_last.val==pwm_next.val) pwm_dir=pwm_dir_const;
		}
	}
//Началcя новый час
if (time_flag.hour)
	{
	time_flag.hour=0;
	if (time.hour==0)
		{
		LCD_Init();		//Переинициализируем дисплей
		LCD_Home(&time, new_pwm, pwm_dir, temperature);//Обновим данные на экране
		LCD_LuminanceLevel(LUMINANCE_LEVEL_25);
		}
	if (time.hour== 6) LCD_LuminanceLevel(LUMINANCE_LEVEL_50);
	if (time.hour== 8) LCD_LuminanceLevel(LUMINANCE_LEVEL_100);
	if (time.hour==16) LCD_LuminanceLevel(LUMINANCE_LEVEL_75);
	if (time.hour==20) LCD_LuminanceLevel(LUMINANCE_LEVEL_50);
	}

//Нажали кнопку ENT
if (get_keys()==KEY_ENT)
	{
	MyMenu_main();
	if (work_mode==automatic)
		{
		pwm_last.val=new_pwm;
		pwm_last.hour=time.hour;
		pwm_last.min=time.min;
		pwm_next=PWM_GetNextVal(&time);
		if (pwm_last.val >pwm_next.val) pwm_dir=pwm_dir_dn;
		else if (pwm_last.val <pwm_next.val) pwm_dir=pwm_dir_up;
		else if (pwm_last.val==pwm_next.val) pwm_dir=pwm_dir_const;
		}
	}

//Управление в ручном режиме
if (work_mode==manual)
	{
	if ((get_keys()==KEY_U)||(get_keys()==KEY_D))
		{
		if ((new_pwm< 0xff)&&(get_keys()==KEY_U)) new_pwm++;
		if ((new_pwm> 0x00)&&(get_keys()==KEY_D)) new_pwm--;
		PWM_SetOut(new_pwm);
		LCD_Home(&time, new_pwm, pwm_dir_none, temperature);//Обновим данные на экране
		for (char t=new_pwm; t<200; t++) __delay_cycles(PROCESSOR_CLOCK/10000);
		}
	}
}
}
