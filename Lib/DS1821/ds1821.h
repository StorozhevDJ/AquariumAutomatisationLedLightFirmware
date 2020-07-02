



#define START_CONVERT_T (0xEE)
#define READ_TEMPERATURE (0xAA)
#define ERROR_TERMO (0x7F)  

typedef unsigned char volatile __tiny *Io_Reg;

#define MAX_TERMO_ERROR 10


#define MASKBIT(x, y) (x &= (y))
#define ISBITS(x, y) ((x & (y)) == (y))

#define TESTBIT(x, y) (x & (y))
#define TESTBIT(x, y) (x & (y))
#define CLEARBIT(x, y) (x &= (~(y)))
#define SETBIT(x, y) (x |= (y))
#define TOGGLEBIT(x, y) (x ^= (y))
#define _SIZE(x) (sizeof((x))/sizeof((x)##[0]))


#define __delay_500mcs() __delay_cycles(500*(PROCESSOR_CLOCK / 1000000));
#define __delay_80mcs()  __delay_cycles( 80*(PROCESSOR_CLOCK / 1000000));
#define __delay_10mcs()  __delay_cycles( 10*(PROCESSOR_CLOCK / 1000000));
#define __delay_2mcs()   __delay_cycles(  2*(PROCESSOR_CLOCK / 1000000));



char __get_termo_1wire(Io_Reg PORTx, Io_Reg DDRx, Io_Reg PINx, unsigned char bit);