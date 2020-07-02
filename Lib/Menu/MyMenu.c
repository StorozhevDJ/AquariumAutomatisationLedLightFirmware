

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
/*	����	��������� ������������� ������������� � ���������� ����������������	*/
/********************************************************************************/
__flash unsigned char mymenu_install_LCD [][20]=
	{
	{" ���. �������������."},
	{" h ����� ������ 10��"},
	{" h ����� ����. 100��"},
	{" h ����� ������ 10��"},
	{" h ����� ����. 100��"},
	{" ������ ������������"},
	{"/0"},
	};
__flash unsigned char mymenu_admin_password_LCD[]=	{"������� ���������                       ������                                  "};
__flash unsigned char mymenu_upr_raspred_LCD[]=		{"����� ����������                        �����������������.                      "};
__flash unsigned char mymenu_h_stol_10kg_szat_LCD[]=	{"������ �� ������ ��                     ����� ������ �� 10��                    "};
__flash unsigned char mymenu_h_stol_100kg_szat_LCD[]=	{"������ �� ������ �� 100��               ����� ������ ��                         "};
__flash unsigned char mymenu_h_stol_10kg_rast_LCD[]=	{"���������� �����    �� 10��             �������� ����������                     "};
__flash unsigned char mymenu_h_stol_100kg_rast_LCD[]=	{"���������� �����    �� 100��            �������� ����������                     "};
__flash unsigned char mymenu_new_user_pass_LCD[]=		{"������� �����       ������              ����������������                        "};

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
/*				���� �������� �� ������� ������						*/
/*	�������� ���������� ������										*/
/*	������ ���������� ������ �� �����									*/
/*	�������������� ���������� ������ (����� ������)						*/
/*	�������� ������ (����� ������)									*/
/********************************************************************************/
__flash unsigned char mymenu_springlist_LCD [][20]=
	{
	{" �������� ����������"},
	{" ������ �� �����    "},
	{" ��������������     "},
	{" �������� �������   "},
	{"/0"},
	};

void Spring_List (void)
{
unsigned char menu=0;

while (get_keys()!=KEY_ESC)
	{
	//���� �������� �� ������� ������
	menu=Menu_Str_Flash(menu, &mymenu_springlist_LCD[0][0]);
	
	//������ �������� ����������
	if (menu==1)
		{
		MyMenu_Spring_Tabl_View();
		}
	//������ �������� ����������
	if (menu==2)
		{
		MyMenu_Spring_Tabl_Print();
		}
	//������ �������� ����������
	if (menu==3)
		{
		MyMenu_Spring_Tabl_Edit();
		}
	//������ �������� ����������
	if (menu==4)
		{
		MyMenu_Spring_Tabl_Delete();
		}
	//� ���� ������ ESC
	if (menu==0xff)
		{
		return;
		}
	__delay_cycles(PROCESSOR_CLOCK/3);
	}
}



/********************************************************************************/
/*				����������� ��������� ���������� ������					*/
/********************************************************************************/
__flash unsigned char mymenu_spring_tabl_h0_LCD[]={"                    ���������:          ������ � ���������        -       ��  "};
__flash unsigned char mymenu_spring_tabl_f1_LCD[]={"������ ��������:                        h1=   .  ��         F1=      -      H "};
__flash unsigned char mymenu_spring_tabl_f2_LCD[]={"������ ��������:                        h2=   .  ��         F2=      -      H "};

void MyMenu_Spring_Tabl_View (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

while (get_keys()!=KEY_ESC)
	{
	//�������� �������
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	while (get_keys()!=KEY_ESC)
		{
		//���������� �������� ������� � ������ � ��������� ���������
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
		//���������� �������� ��� ������ ������
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
		//���������� �������� ��� ������ ������
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
/*			�������������� ��������� ���������� ������					*/
/********************************************************************************/
__flash unsigned char mymenu_user_pass_edit_LCD[]=		{"��� ��������������  ������� ������:     ���������� ������                       "};

__flash unsigned char mymenu_spring_tabl_name_edit_LCD[]=	{"������� ��������                        �������:                                "};
__flash unsigned char mymenu_spring_tabl_h0min_edit_LCD[]=	{"�����������         ��������� ���������:���������� ������ �      ��*100         "};
__flash unsigned char mymenu_spring_tabl_h0max_edit_LCD[]=	{"������������        ��������� ���������:���������� ������ �      ��*100         "};
__flash unsigned char mymenu_spring_tabl_h1_edit_LCD[]=	{"������ ������� ���  ��������:           ��������� ������         ��*100         "};
__flash unsigned char mymenu_spring_tabl_h2_edit_LCD[]=	{"������ ������� ���  ��������:           ��������� ������         ��*100         "};
__flash unsigned char mymenu_spring_tabl_f1min_edit_LCD[]=	{"�����������         ����������� ������: ���������� ������         �*100         "};
__flash unsigned char mymenu_spring_tabl_f1max_edit_LCD[]=	{"������������        ����������� ������: ���������� ������         �*100         "};
__flash unsigned char mymenu_spring_tabl_f2min_edit_LCD[]=	{"�����������         ����������� ������: ���������� ������         �*100         "};
__flash unsigned char mymenu_spring_tabl_f2max_edit_LCD[]=	{"������������        ����������� ������: ���������� ������         �*100         "};

__flash unsigned char mymenu_spring_tabl_save_edit_LCD[]=	{"��������� ���������                     ���������?          ESC              ENT"};

void MyMenu_Spring_Tabl_Edit (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_user_pass_edit_LCD[0]);
if (input_int(0, 0, 0xffff, 0x54)!=EE_settings.user_pass) return;

while (get_keys()!=KEY_ESC)
	{
	//�������� �������
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	//������ �������� �������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_name_edit_LCD[0]);
	if (Input_string(&spring_data.spring_name[0], 20, 0x54)==0xff) return;
	
	//����������� ������ � ��������� ���������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h0min_edit_LCD[0]);
	spring_data.h0_min=input_int(spring_data.h0_min, 500, 24000, 0x54);
	//������������ ������ � ��������� ���������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h0max_edit_LCD[0]);
	spring_data.h0_max=input_int(spring_data.h0_max, 5, 24400, 0x54);
	
	//������ ��� ��������� ������ ��������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h1_edit_LCD[0]);
	spring_data.h1_min=input_int(spring_data.h1_min, 400, 21000, 0x54);
	spring_data.h1_max=spring_data.h1_min;
	//������ ��� ��������� ������ ��������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_h2_edit_LCD[0]);
	spring_data.h2_max=input_int(spring_data.h2_max, 200, 17000, 0x54);
	spring_data.h2_min=spring_data.h2_max;
	
	//����������� ���������� ������ ����������� ��� ������ ������ �������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f1min_edit_LCD[0]);
	spring_data.f1_min=input_long((unsigned long)spring_data.f1_min*2, 10, 90000, 0x54)/2;
	//������������ ���������� ������ ����������� ��� ������ ������ �������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f1max_edit_LCD[0]);
	spring_data.f1_max=input_long((unsigned long)spring_data.f1_max*2, 10, 90000, 0x54)/2;
	
	//����������� ���������� ������ ����������� ��� ������ ������ �������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f2min_edit_LCD[0]);
	spring_data.f2_min=input_long((unsigned long)spring_data.f2_min*2, 40, 100000, 0x54)/2;
	//������������ ���������� ������ ����������� ��� ������ ������ �������
	send_cmd_lcd(LCD_DRAM);
	write_str_lcd_PP(&mymenu_spring_tabl_f2max_edit_LCD[0]);
	spring_data.f2_max=input_long((unsigned long)spring_data.f2_max*2, 40, 100000, 0x54)/2;
	
	//�������� ��������� ������
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
/*				�������� ������� �� �������							*/
/********************************************************************************/
__flash unsigned char mymenu_user_pass_spring_delete_LCD[]=	{"��� �������� �������                    ������� ������:                         "};
__flash unsigned char mymenu_confirm_spring_delete_LCD[]=	{"������� �������     �� ������?                              ESC              ENT"};

void MyMenu_Spring_Tabl_Delete (void)
{
unsigned char spring_num=0;
TEEspring_tabl spring_data;

send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_user_pass_spring_delete_LCD[0]);
if (input_int(0, 0, 0xffff, 0x54)!=EE_settings.user_pass) return;

while (get_keys()!=KEY_ESC)
	{
	//�������� �������
	spring_num=Menu_Str_EEPROM_struct(spring_num);
	spring_data=EE_spring_tabl[spring_num];
	
	if (spring_num==0xff) return;
	
	//�������� ��������� ������
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
/*			�������� ����� � �������� ���������� ������					*/
/********************************************************************************/
__flash unsigned char mymenu_create_spring_table_file_LCD[]=	{"���� �������� ����� ������.             � �����������                           "};
__flash unsigned char mymenu_created_spring_table_file_LCD[]=	{"���� � �����������                      ������ ������.                          "};
__flash unsigned char mymenu_no_iserted_card_LCD[]=			{"���������� �������  �� �����������.     ����. ����� ������                      "};
void MyMenu_Spring_Tabl_Print (void)
{
TEEspring_tabl spring_data;

//���� �������� �����
send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_create_spring_table_file_LCD[0]);

//������� ���� � ��������� � ���� ���������
HTML_Spring_Tabl_New_file();

send_cmd_lcd(LCD_DRAM|0x54);

//��������� �� ����� ������ ������
for (unsigned char i=0; i<100; i++)
	{
	spring_data=EE_spring_tabl[i];	//������� ��������� �������
	if ((spring_data.spring_name[0]!=0)&&(spring_data.spring_name[0]!=0xff))	//���� ������� ��������
		{
		HTML_Spring_Tabl_parametrs(spring_data, i+1);	//������� ��������� ������� �� �����
		}
	if (i%5==0) send_dta_lcd(0xA0);
	}
//�������� ������ � �������� ����
HTML_Spring_Tabl_End_file();

//���� ������
send_cmd_lcd(LCD_DRAM);
write_str_lcd_PP(&mymenu_created_spring_table_file_LCD[0]);


while (1)
	{
	if (get_keys()==KEY_ESC) break;
	if (get_keys()==KEY_ENT)	break;
	}
__delay_cycles(PROCESSOR_CLOCK/3);
}