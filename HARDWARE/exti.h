#ifndef __EXIT_H
#define __EXIT_H
#include "stm32f10x.h"

void ppm_set_counter(void);
void ppm_set_io_config(void);
void ppm_set_test_pwm(u16 arr,u16 psc);

uint16_t ppm_read_cnt(void);//读取当前计数器的值
void ppm_reset_cnt(void);//计数器清0
void ppm_set_fall_trigger_interrupt(void);//设置下降沿触发IO中断
void ppm_set_raise_trigger_interrupt(void);//设置上升沿触发IO中断

void EXTILine_Config(void);
void TIM3_Init(void);
void TIM2_Pwm_Init(u16 arr,u16 psc);

#endif
