#ifndef __OUTPUT_H
#define __OUTPUT_H
#include "sys.h"

#endif

#define out7 PBout(9)

void output_init(void);
void output_one(u8 ch,u16 pwm);
u8 output_two(u8 ch1,u8 ch2);
void output_disable(void);

