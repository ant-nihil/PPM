#ifndef __EXIT_H
#define __EXIT_H

#include "stm32f10x.h"

typedef enum {
	PWM_STATUS_NONE=0,
	PWM_STATUS_RAISE,    
	PWM_STATUS_FALLING,  
}pwm_status;

extern uint16_t Pulse_Width[9];

void TIM3_Init(u16 arr,u16 psc);
void EXTILine_Config(void);
void exit_handler(void);

uint16_t Pwm_Duty_Cycle(void);
uint16_t Pwm_Period(void);
uint32_t Pwm_Fre(void);
uint16_t Pwm_Pulse_Width(void);

void PPM_Check(void);

void TIM2_Pwm_Init(u16 arr,u16 psc);


#endif
