

#include "main.h"
#include "inavr.h"
#include "Keys.h"
#include "Time.h"
#include "Work.h"
#include "lcd_futaba.h"
#include "Menu_std.h"
#include "MyMenu.h"
#include "Card_html.h"






/********************************************************************************/
/*	Меню	настройки калибровочных коэффициентов и управления распределителями	*/
/********************************************************************************/
__flash unsigned char mymenu_install_LCD [][20]=
	{
	{" Упр. распределител."},
	{" h стола сжатия 10кг"},
	{" h стола сжат. 100кг"},
	{" h стола растяж 10кг"},
	{" h стола раст. 100кг"},
	{" Пароль пользователя"},
	{"/0"},
	};
__flash unsigned char mymenu_admin_password_LCD[]=	{"Введите системный                       пароль                                  "};
__flash unsigned char mymenu_upr_raspred_LCD[]=		{"Режим управления                        пневмораспределит.                      "};
__flash unsigned char mymenu_h_stol_10kg_szat_LCD[]=	{"Высота от поршня до                     стола сжатия на 10кг                    "};
__flash unsigned char mymenu_h_stol_100kg_szat_LCD[]=	{"Высота от поршня до 100кг               стола сжатия на                         "};
__flash unsigned char mymenu_h_stol_10kg_rast_LCD[]=	{"Расстояние между    на 10кг             крючками растяжения                     "};
__flash unsigned char mymenu_h_stol_100kg_rast_LCD[]=	{"Расстояние между    на 100кг            крючками растяжения                     "};
__flash unsigned char mymenu_new_user_pass_LCD[]=		{"Введите новый       пароль              пользовательский                        "};

void MyMenu_install (void)
{
unsigned char menu;

send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_admin_password_LCD[0]);
if (input_int(0, 0, 0xffff, 0x54)!=1256) return;

menu=Menu_Str_Flash(0, &mymenu_install_LCD[0][0]);


if (menu==KEY_ENT);
if (menu==KEY_ESC);

if (menu==1)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_upr_raspred_LCD[0]);
	while (get_keys()!=KEY_ESC)
		{
		if (get_keys()==KEY_0) STOP;
		if (get_keys()==KEY_1) Set_kl_out(V1,0);
		if (get_keys()==KEY_2) Set_kl_out(V2,0);
		if (get_keys()==KEY_3) Set_kl_out(V3,0);
		if (get_keys()==KEY_4) Set_kl_out(V4,0);
		if (get_keys()==KEY_5) Set_kl_out(15,0);
		
		if (get_keys()==KEY_U) UP;
		if (get_keys()==KEY_R) UP_L;
		if (get_keys()==KEY_D) DOWN;
		if (get_keys()==KEY_L) DOWN_L;
		
		if (get_keys()==KEY_DOT) while (get_keys()!=KEY_ESC)
			{
			UP;
			__delay_cycles(PROCESSOR_CLOCK*15);
			UP_L;
			__delay_cycles(PROCESSOR_CLOCK*5);
			DOWN;
			__delay_cycles(PROCESSOR_CLOCK*15);
			DOWN_L;
			__delay_cycles(PROCESSOR_CLOCK*5);
			}
		
		
		};
	__delay_cycles(PROCESSOR_CLOCK/3);
	}

if (menu==2)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_stol_10kg_szat_LCD[0]);
	EE_settings.H_stol_10kg_szat=input_int(EE_settings.H_stol_10kg_szat, 0, 30000, 0x54);
	}
if (menu==3)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_stol_100kg_szat_LCD[0]);
	EE_settings.H_stol_100kg_szat=input_int(EE_settings.H_stol_100kg_szat, 0, 30000, 0x54);
	}
if (menu==4)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_stol_10kg_rast_LCD[0]);
	EE_settings.H_stol_10kg_rast=input_int(EE_settings.H_stol_10kg_rast, 0, 30000, 0x54);
	}
if (menu==5)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_stol_100kg_rast_LCD[0]);
	EE_settings.H_stol_100kg_rast=input_int(EE_settings.H_stol_100kg_rast, 0, 25000, 0x54);
	}
/*if (menu==6)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_left_rast_LCD[0]);
	EE_settings.H_stol_rastjazen_left=input_int(EE_settings.H_stol_rastjazen_left, 0, 20000, 0x54);
	}
if (menu==7)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_h_right_rast_LCD[0]);
	EE_settings.H_stol_rastjazen_right=input_int(EE_settings.H_stol_rastjazen_right, 0, 20000, 0x54);
	}*/
if (menu==6)
	{
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_new_user_pass_LCD[0]);
	EE_settings.user_pass=input_int(EE_settings.user_pass, 0, 0xffff, 0x54);
	}
}



/********************************************************************************/
/*				Меню действий со списком пружин						*/
/*	Просмотр параметров пружин										*/
/*	Запись параметров пружин на карту									*/
/*	Редактирование параметров пружин (через пароль)						*/
/*	Удаление пружин (через пароль)									*/
/********************************************************************************/
__flash unsigned char mymenu_springlist_LCD [][20]=
	{
	{" Просмотр параметров"},
	{" Запись на карту    "},
	{" Редактирование     "},
	{" Удаление пружины   "},
	{"/0"},
	};

void Spring_List (void)
{
unsigned char menu=0;

while (get_keys()!=KEY_ESC)
	{
	//Меню действий со списком пружин
	menu=Menu_Str_Flash(menu, &mymenu_springlist_LCD[0][0]);
	
	//Выбран просмотр параметров
	if (menu==1)
		{
		MyMenu_Spring_Tabl_View();
		}
	//Выбран просмотр параметров
	if (menu==2)
		{
		MyMenu_Spring_Tabl_Print();
		}
	//Выбран просмотр параметров
	if (menu==3)
		{
		MyMenu_Spring_Tabl_Edit();
		}
	//Выбран просмотр параметров
	if (menu==4)
		{
		MyMenu_Spring_Tabl_Delete();
		}
	//В меню нажали ESC
	if (menu==0xff)
		{
		return;
		}
	__delay_cycles(PROCESSOR_CLOCK/3);
	}
}



/********************************************************************************/
/*				Отображение табличных параметров пружин					*/
/********************************************************************************/
__flash unsigned char mymenu_spring_tabl_h0_LCD[]={"                    состоянии:          Высота в свободном        -       мм  "};
__flash unsigned char mymenu_spring_tabl_f1_LCD[]={"Первая нагрузка:                        h1=   .  мм         F1=      -      H "};
__flash unsigned char mymenu_spring_tabl_f2_LCD[]={"Вторая нагрузка:                        h2=   .  мм         F2=      -      H "};

void MyMenu_Spring_Tabl_View (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

while (get_keys()!=KEY_ESC)
	{
	//Выбираем пружину
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	while (get_keys()!=KEY_ESC)
		{
		//Показываем название пружины и высоты в свободном состоянии
		send_cmd_lcd(LCD_DRAM);
		write_str_lcd_PP(&mymenu_spring_tabl_h0_LCD[0]);
		send_cmd_lcd(LCD_DRAM);
		write_str_lcd_PPPc(&spring_data.spring_name[0], 20);
		send_cmd_lcd(LCD_DRAM|(0x54+0));
		write_int_dec_lcd_d(spring_data.h0_min, 2);
		send_cmd_lcd(LCD_DRAM|(0x54+7));
		write_int_dec_lcd_d(spring_data.h0_max, 2);
		__delay_cycles(PROCESSOR_CLOCK/3);
		while (get_keys()==0xff);
		if (get_keys()==KEY_ESC) break;
		//Показываем назрузку для первой высоты
		send_cmd_lcd(LCD_DRAM);
		write_str_lcd_PP(&mymenu_spring_tabl_f1_LCD[0]);
		send_cmd_lcd(LCD_DRAM|(0x40+3));
		write_int_dec_lcd_d(spring_data.h1_min, 2);
		send_cmd_lcd(LCD_DRAM|(0x54+3));
		write_long_dec_lcd_d((unsigned long)spring_data.f1_min*2, 2);
		send_cmd_lcd(LCD_DRAM|(0x54+10));
		write_long_dec_lcd_d((unsigned long)spring_data.f1_max*2, 2);
		__delay_cycles(PROCESSOR_CLOCK/3);
		while (get_keys()==0xff);
		if (get_keys()==KEY_ESC) break;
		//Показываем назрузку для второй высоты
		send_cmd_lcd(LCD_DRAM);
		write_str_lcd_PP(&mymenu_spring_tabl_f2_LCD[0]);
		send_cmd_lcd(LCD_DRAM|(0x40+3));
		write_int_dec_lcd_d(spring_data.h2_min, 2);
		send_cmd_lcd(LCD_DRAM|(0x54+3));
		write_long_dec_lcd_d((unsigned long)spring_data.f2_min*2, 2);
		send_cmd_lcd(LCD_DRAM|(0x54+10));
		write_long_dec_lcd_d((unsigned long)spring_data.f2_max*2, 2);
		__delay_cycles(PROCESSOR_CLOCK/3);
		while (get_keys()==0xff);
		if (get_keys()==KEY_ESC) break;
		}
	__delay_cycles(PROCESSOR_CLOCK/3);
	}
}



/********************************************************************************/
/*			Редактирование табличных параметров пружин					*/
/********************************************************************************/
__flash unsigned char mymenu_user_pass_edit_LCD[]=		{"Для редактирования  введите пароль:     параметров пружин                       "};

__flash unsigned char mymenu_spring_tabl_name_edit_LCD[]=	{"Введите название                        пружины:                                "};
__flash unsigned char mymenu_spring_tabl_h0min_edit_LCD[]=	{"Минимальная         свободном состоянии:допустимая высота в      мм*100         "};
__flash unsigned char mymenu_spring_tabl_h0max_edit_LCD[]=	{"Максимальная        свободном состоянии:допустимая высота в      мм*100         "};
__flash unsigned char mymenu_spring_tabl_h1_edit_LCD[]=	{"Высота пружины для  нагрузки:           измерения первой         мм*100         "};
__flash unsigned char mymenu_spring_tabl_h2_edit_LCD[]=	{"Высота пружины для  нагрузки:           измерения второй         мм*100         "};
__flash unsigned char mymenu_spring_tabl_f1min_edit_LCD[]=	{"Минимальное         создаваемое усилие: допустимое первое         Н*100         "};
__flash unsigned char mymenu_spring_tabl_f1max_edit_LCD[]=	{"Максимальное        создаваемое усилие: допустимое первое         Н*100         "};
__flash unsigned char mymenu_spring_tabl_f2min_edit_LCD[]=	{"Минимальное         создаваемое усилие: допустимое второе         Н*100         "};
__flash unsigned char mymenu_spring_tabl_f2max_edit_LCD[]=	{"Максимальное        создаваемое усилие: допустимое второе         Н*100         "};

__flash unsigned char mymenu_spring_tabl_save_edit_LCD[]=	{"Сохранить введенные                     параметры?          ESC              ENT"};

void MyMenu_Spring_Tabl_Edit (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_user_pass_edit_LCD[0]);
if (input_int(0, 0, 0xffff, 0x54)!=EE_settings.user_pass) return;

while (get_keys()!=KEY_ESC)
	{
	//Выбираем пружину
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	//Вводим название пружины
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_name_edit_LCD[0]);
	if (Input_string(&spring_data.spring_name[0], 20, 0x54)==0xff) return;
	
	//Минимальная высота в свободном состоянии
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h0min_edit_LCD[0]);
	spring_data.h0_min=input_int(spring_data.h0_min, 500, 24000, 0x54);
	//Максимальная высота в свободном состоянии
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h0max_edit_LCD[0]);
	spring_data.h0_max=input_int(spring_data.h0_max, 5, 24400, 0x54);
	
	//Высота для измерения первой нагрузки
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h1_edit_LCD[0]);
	spring_data.h1_min=input_int(spring_data.h1_min, 400, 21000, 0x54);
	spring_data.h1_max=spring_data.h1_min;
	//Высота для измерения второй нагрузки
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h2_edit_LCD[0]);
	spring_data.h2_max=input_int(spring_data.h2_max, 200, 17000, 0x54);
	spring_data.h2_min=spring_data.h2_max;
	
	//Минимальное допустимое усилие создаваемое при первой высоте пружины
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f1min_edit_LCD[0]);
	spring_data.f1_min=input_long((unsigned long)spring_data.f1_min*2, 10, 90000, 0x54)/2;
	//Максимальное допустимое усилие создаваемое при первой высоте пружины
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f1max_edit_LCD[0]);
	spring_data.f1_max=input_long((unsigned long)spring_data.f1_max*2, 10, 90000, 0x54)/2;
	
	//Минимальное допустимое усилие создаваемое при второй высоте пружины
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f2min_edit_LCD[0]);
	spring_data.f2_min=input_long((unsigned long)spring_data.f2_min*2, 40, 100000, 0x54)/2;
	//Максимальное допустимое усилие создаваемое при второй высоте пружины
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f2max_edit_LCD[0]);
	spring_data.f2_max=input_long((unsigned long)spring_data.f2_max*2, 40, 100000, 0x54)/2;
	
	//Сохраним введенные данные
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_save_edit_LCD[0]);
	__delay_cycles(PROCESSOR_CLOCK/3);
	while (1)
		{
		if (get_keys()==KEY_ESC) break;
		if (get_keys()==KEY_ENT)
			{
			EE_spring_tabl[spring_num]=spring_data;
			break;
			}
		}
		
	__delay_cycles(PROCESSOR_CLOCK/3);
	}
}



/********************************************************************************/
/*				Удаление пружины из таблици							*/
/********************************************************************************/
__flash unsigned char mymenu_user_pass_spring_delete_LCD[]=	{"Для удаления пружины                    введите пароль:                         "};
__flash unsigned char mymenu_confirm_spring_delete_LCD[]=	{"Удалить пружину     из списка?                              ESC              ENT"};

void MyMenu_Spring_Tabl_Delete (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_user_pass_spring_delete_LCD[0]);
if (input_int(0, 0, 0xffff, 0x54)!=EE_settings.user_pass) return;

while (get_keys()!=KEY_ESC)
	{
	//Выбираем пружину
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	//Сохраним введенные данные
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_confirm_spring_delete_LCD[0]);
	send_cmd_lcd(LCD_DRAM|LCD_DRAM_LINE2);
	write_str_lcd_PPPc(&spring_data.spring_name[0], 20);
	__delay_cycles(PROCESSOR_CLOCK/3);
	while (1)
		{
		if (get_keys()==KEY_ESC) break;
		if (get_keys()==KEY_ENT)
			{
			EE_spring_tabl[spring_num].spring_name[0]=0;
			break;
			}
		}
	__delay_cycles(PROCESSOR_CLOCK/3);
	}
}



/********************************************************************************/
/*			Создание файла с таблицей параметров пружин					*/
/********************************************************************************/
__flash unsigned char mymenu_create_spring_table_file_LCD[]=	{"Идет создание файла пружин.             с параметрами                           "};
__flash unsigned char mymenu_created_spring_table_file_LCD[]=	{"Файл с параметрами                      пружин создан.                          "};
__flash unsigned char mymenu_no_iserted_card_LCD[]=			{"Невозможно создать  не установлена.     файл. Карта памяти                      "};
void MyMenu_Spring_Tabl_Print (void)
{
TEEspring_tabl spring_data;

//Идет создание файла
send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_create_spring_table_file_LCD[0]);

//Создаем файл и вписываем в него заголовок
HTML_Spring_Tabl_New_file();

send_cmd_lcd(LCD_DRAM|0x54);

//Пройдемся по всему списку пружин
for (unsigned char i=0; i<100; i++)
	{
	spring_data=EE_spring_tabl[i];	//Считаем параметры пружины
	if ((spring_data.spring_name[0]!=0)&&(spring_data.spring_name[0]!=0xff))	//Если пружина записана
		{
		HTML_Spring_Tabl_parametrs(spring_data, i+1);	//Запишем параметры пружины на карту
		}
	if (i%5==0) send_dta_lcd(0xA0);
	}
//Завершим список и сохраним файл
HTML_Spring_Tabl_End_file();

//Файл создан
send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_created_spring_table_file_LCD[0]);


while (1)
	{
	if (get_keys()==KEY_ESC) break;
	if (get_keys()==KEY_ENT)	break;
	}
__delay_cycles(PROCESSOR_CLOCK/3);
}