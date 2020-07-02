

#include "Time.h"


__flash const char day_in_mon[12]={31,29,31,30,31,30,31,31,30,31,30,31};
__eeprom __no_init signed char time_correct;


TStime time;



void Timer_Init (void)
{
// Clock value: PROCESSOR_CLOCK/1024
// Mode: CTC top=OCR2
// OC1B output: Non-Inv.
TCCR2=(1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
TIMSK=(1<<OCIE2);
OCR2 =180-1;
//OCR2B=0x0000;
}