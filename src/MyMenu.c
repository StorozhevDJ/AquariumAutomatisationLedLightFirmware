
#include <inavr.h>

#include "Time.h"
#include "PWM.h"

#include "Lib\LCD\lcd_futaba.h"
#include "src\LCD_text.h"
#include "Menu\Menu.h"
#include "MyMenu.h"

#include "main.h"



extern TEwork_mode work_mode;



/*******************************************************************************
* Отображение главного меню
*******************************************************************************/
void MyMenu_main(void)
{
unsigned char menu_ptr;

while(1)
	{
	menu_ptr=Menu_Lineflash(menu_main, 2, 6);
	switch(menu_ptr)
		{
		case 0: __delay_cycles(PROCESSOR_CLOCK/3); return;
		case 1: WorkModeSetup(); break;
		case 2: TimeSetup(); break;
		case 3: PWMSetup(); break;
		case 4: PWMMaxSetup(); break;
		default: break;
		}
	}
}



/*******************************************************************************
* Отображение главного экрана
*******************************************************************************/
void LCD_Home(volatile TStime *time, unsigned char pwm_val, TEpwm_dir pwm_dir, unsigned char temperature)
{
//Work mode
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrUCF20(LCD_home_work_mode[work_mode]);
//Write temperature
LCD_SendCmd(LCD_DRAM_LINE2);
LCD_WriteStrCCF(LCD_home_water);
LCD_SendCmd(LCD_DRAM_LINE2+6);
LCD_WriteByteDec(temperature);
LCD_SendUChar('°');
LCD_SendUChar('C');
//Write light
LCD_SendCmd(LCD_DRAM_LINE3);
LCD_WriteStrCCF(LCD_home_light);
LCD_SendCmd(LCD_DRAM_LINE3+6);
if (pwm_val>0)
	{
	LCD_WriteByteDec((unsigned short)((unsigned short)((pwm_val+1)*100)>>8));
	LCD_SendUChar('%');
	if (pwm_dir==pwm_dir_up)   LCD_SendUChar(0x18);
	if (pwm_dir==pwm_dir_dn)   LCD_SendUChar(0x19);
	if (pwm_dir==pwm_dir_const)LCD_SendUChar('-');
	if (pwm_dir==pwm_dir_none) LCD_SendUChar(' ');
	}
else LCD_WriteStrCCF(LCD_pwm_off);
//Time
LCD_SendCmd(LCD_DRAM_LINE4);
LCD_WriteStrCCF(LCD_home_time);
TimeLCD(time);

//LCD_SendCmd(LCD_DRAM_LINE2+15);
//LCD_WriteByteDec(get_keys());
}



/*******************************************************************************
* Отображение часов
*******************************************************************************/
void TimeLCD(volatile TStime *time)
{
//Write hour
LCD_SendCmd(LCD_DRAM_LINE4);
if (time->hour<10) LCD_SendUChar(' ');
LCD_WriteByteDec(time->hour);
//Write minute
LCD_SendCmd(LCD_DRAM_LINE4+3);
if (time->min<10) LCD_SendUChar('0');
LCD_WriteByteDec(time->min);
//Write seconds
LCD_SendCmd(LCD_DRAM_LINE4+6);
if (time->sec<10) LCD_SendUChar('0');
LCD_WriteByteDec(time->sec);
//Write day
LCD_SendCmd(LCD_DRAM_LINE4+10);
if (time->day<10) LCD_SendUChar(' ');
LCD_WriteByteDec(time->day);
//Write mon
LCD_SendCmd(LCD_DRAM_LINE4+13);
if (time->mon<10) LCD_SendUChar('0');
LCD_WriteByteDec(time->mon);
//Write year
LCD_SendCmd(LCD_DRAM_LINE4+18);
LCD_WriteByteDec(time->year);
}



/*******************************************************************************
* Установка времени и даты
*******************************************************************************/
void TimeSetup(void)
{
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrCCF(LCD_time_setup);

LCD_SendCmd(LCD_DRAM_LINE3+15);
LCD_WriteByteSDec(time_correct);

TimeLCD(&time);
//Input Date
time.year=Menu_InputInt(time.year, 15, 99, LCD_DRAM_LINE4+18);
time.mon =Menu_InputInt(time.mon,  1, 12, LCD_DRAM_LINE4+13);
time.day =Menu_InputInt(time.day,  1, day_in_mon[time.mon], LCD_DRAM_LINE4+10);
//Input Time
time.hour=Menu_InputInt(time.hour, 0, 23, LCD_DRAM_LINE4+0);
time.min =Menu_InputInt(time.min,  0, 59, LCD_DRAM_LINE4+3);
time.sec =Menu_InputInt(time.sec,  0, 59, LCD_DRAM_LINE4+6);

time_correct =Menu_InputSInt(time_correct,  -120, 120, LCD_DRAM_LINE3+15);
}



/*******************************************************************************
* Настройка ШИМ выхода
*******************************************************************************/
void PWMSetup(void)
{
unsigned char menu_ptr;
for (char n=0; n<PWM_DOTS_MAX; n++)
	{
	menu_ptr=Menu_Lineflash(menu_pwm_add_point, 3, 4);
	//Выбрали пункт "Следующее значение"
	if (menu_ptr==0)
		{
		unsigned char tmp_hour, tmp_min, tmp_val;
		LCD_SendCmd(LCD_DRAM_LINE1);
		LCD_WriteStrCCF(LCD_pwm_edit);
		//Номер значения
		LCD_SendCmd(LCD_DRAM_LINE1+11);
		LCD_WriteByteDec(n+1);
		//Часы
		LCD_SendCmd(LCD_DRAM_LINE2+7);
		tmp_hour=setting_ee.pwm_settings[n].hour>23?0:setting_ee.pwm_settings[n].hour;
		LCD_WriteByteDec(tmp_hour);
		//Минуты
		LCD_SendCmd(LCD_DRAM_LINE2+10);
		tmp_min=setting_ee.pwm_settings[n].min>59?0:setting_ee.pwm_settings[n].min;
		LCD_WriteByteDec(tmp_min);
		//Значение яркости
		LCD_SendCmd(LCD_DRAM_LINE3+9);
		tmp_val=((unsigned short)setting_ee.pwm_settings[n].val*100+50)/255;
		LCD_WriteByteDec(tmp_val);
		//Ввод значений
		setting_ee.pwm_settings[n].hour=Menu_InputInt(tmp_hour,0, 23, LCD_DRAM_LINE2+ 7);//часы
		setting_ee.pwm_settings[n].min =Menu_InputInt(tmp_min, 0, 59, LCD_DRAM_LINE2+10);//минуты
		setting_ee.pwm_settings[n].val=(Menu_InputInt(tmp_val, 0,100, LCD_DRAM_LINE3+ 9)*255)/100;//значение яркости
		}
	else	//Выбрали пункт "Завершить редактир."
		{
		//Очистим оставшиеся ячейки памяти
		for (; n<20; n++)
			{
			setting_ee.pwm_settings[n].hour=0xff;
			setting_ee.pwm_settings[n].min =0xff;
			setting_ee.pwm_settings[n].val =0xff;
			}
		break;
		}
	}
}



/*******************************************************************************
* Выбор режима работы (автоматический/ручной)
*******************************************************************************/
void WorkModeSetup(void)
{
work_mode=(TEwork_mode)Menu_Lineflash(menu_work_mode_select, 3, 4);
}



/*******************************************************************************
* Настройка максимального значения ШИМ выхода
*******************************************************************************/
void PWMMaxSetup(void)
{
LCD_SendCmd(LCD_DRAM_LINE1);
LCD_WriteStrCCF(LCD_pwm_max);
setting_ee.max_pwm_ee=Menu_InputInt(setting_ee.max_pwm_ee,  0, 255, LCD_DRAM_LINE2+12);
max_pwm=setting_ee.max_pwm_ee;
}
