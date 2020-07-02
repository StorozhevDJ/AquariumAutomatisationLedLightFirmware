

//#include "..\1wire\1wire.h"

#include "ds1821.h"



// счетчик ошибок при чтении температуры
unsigned char terr_cnt = 0;
// последнее значение температуры
unsigned char tlast_val = ERROR_TERMO;






void write_byte_1wire(Io_Reg DDRx, unsigned char bit, char data)
 {
  char temp,i;
  temp = data;
  for (i = 8; i > 0; i--)
   {
    //__disable_interrupt();
    SETBIT(*DDRx, 1<<bit);
    __delay_2mcs();
    if ((temp & (1 << 0)) == (1 << 0)) {CLEARBIT(*DDRx, 1<<bit);}
    __delay_80mcs();
    CLEARBIT(*DDRx, 1<<bit);
    //__enable_interrupt();
    __delay_2mcs();
    temp >>= 1;
   }
 }



char init_1wire(Io_Reg DDRx, Io_Reg PINx, unsigned char bit, char data) 
{
char result;
//char asreg = __save_interrupt();
//__disable_interrupt();

// generate reset pulse
SETBIT(*DDRx, 1<<bit);
__delay_500mcs();
CLEARBIT(*DDRx, 1<<bit);

__delay_2mcs();
// wait for release DQ
result = 100;
while ((*PINx&(1<<bit)) ==  (1<<bit))
	{
	__delay_2mcs();
	result--;
	if (result == 0) break; 
	}

if (result != 0)
	{
	result = 40;
	while ((*PINx&(1<<bit)) !=  (1<<bit))
		{
		__delay_2mcs();
		result--;
		if (result == 0) break;
		}
	}
//__restore_interrupt(asreg);
__delay_500mcs();
if (result != 0) write_byte_1wire(DDRx, bit, data);
return (result);
}



char read_byte_1wire(Io_Reg DDRx, Io_Reg PINx, unsigned char bit) {

	char temp = 0;

	for (unsigned char i = 8; i > 0; i--)
	{
		temp >>= 1;
		//char asreg = __save_interrupt();
        //__disable_interrupt();
        // pull down DQ line
		SETBIT(*DDRx, 1<<bit);
		__delay_2mcs();
        // release DQ line
		CLEARBIT(*DDRx, 1<<bit);
		__delay_10mcs();     //10mcs
		// test DQ line
		if ((*PINx&(1<<bit)) ==  (1<<bit))
			temp |= 0x80;
		// restore DQ line
		//__restore_interrupt(asreg);
        __delay_80mcs();                      //80mcs
  	}
 	return(temp);
}

char __get_termo_1wire(Io_Reg PORTx, Io_Reg DDRx, Io_Reg PINx, unsigned char bit) {

  	// init DQ line
	CLEARBIT(*PORTx, 1<<bit);
	CLEARBIT(*DDRx, 1<<bit);

	if (init_1wire(DDRx, PINx, bit, START_CONVERT_T))

		if (init_1wire(DDRx, PINx, bit, READ_TEMPERATURE))
		{
			terr_cnt = 0;
			tlast_val = read_byte_1wire(DDRx, PINx, bit);
			return(tlast_val);
		}
 
	terr_cnt++;
	if (terr_cnt > MAX_TERMO_ERROR)
      	// error limit >>>> set error
		return (ERROR_TERMO);
    else
      	// return with last value
		return (tlast_val);
}
