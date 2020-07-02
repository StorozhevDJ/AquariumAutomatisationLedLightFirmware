
#ifndef TIME_H
#define TIME_H

//Timer 10ms (100Hz)
typedef struct
	{
	unsigned char x10ms;
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char mon;
	unsigned char year;
	  signed char time_correct;
	} TStime;

extern TStime time;


extern __flash const char day_in_mon[12];
extern __eeprom signed char time_correct;


void Timer_Init (void);


#endif //TIME_H