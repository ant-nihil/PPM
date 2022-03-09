#ifndef __EXIT_H
#define __EXIT_H

#include "stm32f10x.h"

#define MAX_PULSE_WIDTH 2100

typedef enum {
	PWM_STATUS_RAISE,    
	PWM_STATUS_FALLING,  
}pwm_status_t; 

void exti_handler(void);//IO中断处理函数

uint16_t* get_pulse_width(void);//得到单位为us的脉冲宽度
float* get_duty_cycle(void);//得到以20ms为周期的占空比

uint16_t* get_period(void);//得到一高电平一低电平的周期，在读取PPM时用不到，用于调试

#endif
