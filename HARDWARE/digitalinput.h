#ifndef __DIGITALINPUT_H__
#define __DIGITALINPUT_H__
#include "sys.h"


#define DI1 PBin(12)
#define DI2 PBin(13)
#define DI3 PBin(14)
#define DI4 PBin(15)
#define DI5 PAin(8)
#define DI6 PAin(9)
#define DI7 PAin(10)
#define DI8 PAin(11)

#define DI9  PAin(15)
#define DI10 PBin(3)
#define DI11 PBin(4)
#define DI12 PBin(5)
#define DI13 PBin(6)
#define DI14 PBin(7)
#define DI15 PBin(8)
#define DI16 PCin(13)

extern u16 digital_input;
extern u8 input_on_num;;
void digitalInput_init(void);
u8 get_input_on_num(u16 dinput);
u8 get_input_on_ch(u8 num);

u16 get_all_input(void);
#endif

//PA4.PA6.PC4.PB0.PA3.PA5.PA7.PC5
