#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ppm.h"
#include "exti.h"

void PPM_Print_Pulse_Width(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	delay_init();
	uart_init(115200);

	ppm_set_counter();	//PWM捕获的计数器（每1ms计数1000）
	ppm_set_io_config();
	ppm_set_test_pwm(1000,71);  //输出PWM波的周期为20ms

	printf("\r\ncode start\r\n");
	while(1)
	{
		PPM_Print_Pulse_Width();
	}
	
}

void PPM_Print_Pulse_Width(void)
{
	uint8_t t;
	for(t=0;t<8;t++)
	{
		printf("w%d=%4d , ",t,get_pulse_width()[t]);//打印通道脉冲宽度，单位us
	}
//	printf("\r\n");
//	for(t=0;t<8;t++)
//	{
//		printf("d%d=%.3f, ",t,get_duty_cycle()[t]);//打印各个通道占舵机标准周期的占空比
//	}
//	printf("\r\n");
//	for(t=0;t<8;t++)
//	{
//		printf("p%d=%4d , ",t,get_period()[t]);//打印一高一低的PWM的小周期，单位us
//	}
	delay_ms(1000);
	printf("\r\n");
}

