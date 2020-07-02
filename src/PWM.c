
#include <inavr.h>

#include "Time.h"
#include "PWM.h"



__eeprom Tsetting setting_ee;
//__eeprom unsigned char max_pwm_ee;

unsigned char max_pwm;



void PWM_Init (void)
{
// Clock value: PROCESSOR_CLOCK
// Mode: Fast PWM top=ICR1
// OC1B output: Non-Inv.
TCCR1A=(1<<WGM11)/*|(1<<WGM10)*/|(1<<COM1B1)/*|(1<<COM1A1)*/;//0x23;
TCCR1B=(1<<WGM13)|(1<<WGM12)|(1<<CS10);//0x19;
ICR1  =PWM_COUNT;
OCR1A =0;
OCR1B =0;

//Инициализируем порты
PORTB|=(1<<PB1);	//Выход ШИМ
DDRB |=(1<<PB2)|(1<<PB1);

max_pwm=setting_ee.max_pwm_ee;
}



signed long LinearApproximation(long xn, int x1, long x2, long y1, long y2)
{
return y1+((y2*xn)-(y1*xn))/(x2-x1);
}



/*******************************************************************************
* Calculate PWM value for time "time"
*******************************************************************************/
unsigned char PWM_Calc(Tpwm_setting last, Tpwm_setting next, TStime *time)
{
int new_val_pwm;
long tn=(long)(time->hour*3600)+(int)(time->min*60)+time->sec;
long t1=(long)(last.hour*3600)+(int)(last.min*60);
long t2=(long)(next.hour*3600)+(int)(next.min*60);
if (t2<t1) t2+=3600*24L;
if (t1==t2) new_val_pwm=next.val;
else	new_val_pwm=LinearApproximation(tn-t1, 0, t2-t1, last.val, next.val);
return new_val_pwm>255?255:new_val_pwm;
}



//function not work!!!
unsigned int PWM_CalcTimeChange(Tpwm_setting last, Tpwm_setting next)
{
unsigned int time_change;
unsigned int t1=(int)(last.hour*3600)+(int)(last.min*60);
unsigned int t2=(int)(next.hour*3600)+(int)(next.min*60);
time_change=LinearApproximation(next.val>last.val?last.val+1:last.val-1, last.val, next.val, t1, t2);
return time_change;
}



Tpwm_setting PWM_GetNextVal(TStime *time)
{
Tpwm_setting next_val;
unsigned short last_time, next_time, current_time;
current_time=(unsigned short)(time->hour*1800)+(unsigned short)(time->min*30)+(time->sec>>1)+1;
last_time=(unsigned short)(setting_ee.pwm_settings[0].hour*1800)+(unsigned short)(setting_ee.pwm_settings[0].min*30);
for (char i=0; i<PWM_DOTS_MAX; i++)
	{
	if ((i<PWM_DOTS_MAX-1)&&(setting_ee.pwm_settings[i+1].hour!=0xff))//Еще не достигли конца
		{
		next_time=(unsigned short)(setting_ee.pwm_settings[i+1].hour*1800)+(unsigned short)(setting_ee.pwm_settings[i+1].min*30);
		if ((current_time>last_time)&&(current_time<=next_time))
			{
			next_val=setting_ee.pwm_settings[i+1];
			break;
			}
		last_time=next_time;
		}
	else next_val=setting_ee.pwm_settings[0];
	}
return next_val;
}



/*******************************************************************************
* Setting the PWM output
*******************************************************************************/
void PWM_SetOut(unsigned char pwm_val)
{
if (pwm_val)
	{
	OCR1B=(unsigned short)(pwm_val*max_pwm)/255;
	//If last state is Off, then PWM output turning on and wait ending transient process in PWM filter
	if (PORTB_Bit1==1) __delay_cycles(PROCESSOR_CLOCK/30);
	PORTB_Bit1=0;
	}
else//if PWM is switching off, then turn off PowerSupply
	{
	PORTB_Bit1=1;
	OCR1B=0;
	}
}




