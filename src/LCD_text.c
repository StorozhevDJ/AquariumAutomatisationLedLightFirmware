
#include "LCD\LCD_futaba.h"

__flash const char LCD_hello[]=
	{"\
 ������������������ \
     ����������     \
 ������� ���������� \
  \"����� ��������\"  "};

__flash const char LCD_home_work_mode[][LCD_COLUMN]={"������ ����� ������ ", "�������������� �����"};
__flash const char LCD_home_water[]={"����:               "};
__flash const char LCD_home_light[]={"����:               "};
__flash const char LCD_home_time[] ={"  :  :      /  /20  "};


__flash const char LCD_time_setup[]=
	{"\
��������� ���� �    \
�������������:      \
�������             \
 h:mm:ss   d/MM/20yy"};

__flash const char menu_pwm_add_point[][LCD_COLUMN]=
	{
	" ��������� PWM(���) ",
	"                    ",
	" ��������. �������� ",
	" ��������� ��������.",
	"\0"
	};
__flash const char LCD_pwm_edit[]=
	{"\
�������� #          \
�������:    %       \
�����:   :          \
                    "};
__flash const char LCD_pwm_max[]=
	{"\
������������ ������.\
                    \
PWM ������:         \
                    "};
__flash const char LCD_pwm_off[] ={"��������"};

__flash const char menu_work_mode_select[][LCD_COLUMN]=
	{
	" ����� ������       ",
	"                    ",
	" ������             ",
	" ��������������     ",
	"\0"
	};


__flash const char menu_main[][LCD_COLUMN]=
	{
	"   ���� ��������    ",
	" �����...           ",
	" ����� ������       ",
	" ��������� �������  ",
	" ��������� ���������",
	" ��������� PWM      ",
	"\0"
	};
