#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "exit.h"
#include "ppm.h"

extern uint8_t Pulse_Width[9];
void PPM_Print_Pulse_Width(void);

int main(void)
{


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	delay_init();
	uart_init(115200);
	TIM3_Init(50000,71);	//PWM捕获的计数器（每1ms计数1000）
	EXTI10_Config();
	TIM2_Pwm_Init(1000,71);  //TIM2输出PWM波的周期为20ms
  while(1)
	{
//		printf("%d\r\n",Pwm_Duty_Cycle());
//		delay_ms(100);
		PPM_Print_Pulse_Width();
	}
	
}

void PPM_Print_Pulse_Width(void)
{
	uint8_t t;
	for(t=0;t<9;t++)
	{
		printf("%d=%2d, ",t,Pulse_Width[t]);
	}
	delay_ms(1000);
	printf("\r\n");
}

