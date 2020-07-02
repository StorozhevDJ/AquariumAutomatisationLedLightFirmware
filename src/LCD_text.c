
#include "LCD\LCD_futaba.h"

__flash const char LCD_hello[]=
	{"\
 Автоматизированная \
     аквариумом     \
 система управления \
  \"Умный аквариум\"  "};

__flash const char LCD_home_work_mode[][LCD_COLUMN]={"Ручной режим работы ", "Автоматический режим"};
__flash const char LCD_home_water[]={"Вода:               "};
__flash const char LCD_home_light[]={"Свет:               "};
__flash const char LCD_home_time[] ={"  :  :      /  /20  "};


__flash const char LCD_time_setup[]=
	{"\
Установка даты и    \
Корректировка:      \
времени             \
 h:mm:ss   d/MM/20yy"};

__flash const char menu_pwm_add_point[][LCD_COLUMN]=
	{
	" Настройка PWM(ШИМ) ",
	"                    ",
	" Редактир. значение ",
	" Завершить редактир.",
	"\0"
	};
__flash const char LCD_pwm_edit[]=
	{"\
Значение #          \
Яркость:    %       \
Время:   :          \
                    "};
__flash const char LCD_pwm_max[]=
	{"\
Максимальное значен.\
                    \
PWM выхода:         \
                    "};
__flash const char LCD_pwm_off[] ={"Отключен"};

__flash const char menu_work_mode_select[][LCD_COLUMN]=
	{
	" Режим работы       ",
	"                    ",
	" Ручной             ",
	" Автоматический     ",
	"\0"
	};


__flash const char menu_main[][LCD_COLUMN]=
	{
	"   Меню настроек    ",
	" Назад...           ",
	" Режим работы       ",
	" Установка времени  ",
	" Настройка освещения",
	" Параметры PWM      ",
	"\0"
	};
