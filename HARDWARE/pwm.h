#ifndef __PWM_H__
#define __PWM_H__
#include "sys.h"

#define SERVO_OPEN 1970
#define SERVO_CLOSE 1370
#define SERVO_STOP 0

void TIM1_PWM_Init(u16 arr,u16 psc);

void set_channel_odd(u8 ch);

void set_channel_even(u8 ch);





#endif 

