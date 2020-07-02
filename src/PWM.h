
#ifndef PWM_H
#define PWM_H



#define PWM_COUNT		0xFF
#define PWM_DOTS_MAX	20



typedef struct
	{
	unsigned char hour;
	unsigned char min;
	unsigned char val;
	} Tpwm_setting;

typedef struct
	{
	Tpwm_setting pwm_settings[PWM_DOTS_MAX];
	unsigned char max_pwm_ee;
	}Tsetting;

typedef enum
	{
	pwm_dir_none=0,
	pwm_dir_up,
	pwm_dir_dn,
	pwm_dir_const
	}TEpwm_dir;


extern __eeprom Tsetting setting_ee;

extern unsigned char max_pwm;



void PWM_Init (void);

long LinearApproximation(long xn, int x1, long x2, long y1, long y2);
unsigned char PWM_Calc(Tpwm_setting last, Tpwm_setting next, TStime *time);
unsigned int PWM_CalcTimeChange(Tpwm_setting last, Tpwm_setting next);

Tpwm_setting PWM_GetNextVal(TStime *time);
void PWM_SetOut(unsigned char pwm_val);


#endif //PWM_H