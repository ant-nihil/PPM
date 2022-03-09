#include "exti.h"

//IO中断配置代码，若切换硬件环境仅需适配ppm开头的功能函数
/*********************/
void ppm_set_counter(void)//设置计数器计数
{
	TIM3_Init();
}
void ppm_set_io_config(void)//配置io
{
	EXTILine_Config();
}
void ppm_set_test_pwm(u16 arr,u16 psc)
{
	TIM2_Pwm_Init(arr, psc);//自动重装载值，分频值
}
//
uint16_t ppm_read_cnt(void)//读取当前计数器的值
{
	return TIM3->CNT;
}
void ppm_reset_cnt(void)//计数器清0
{
	TIM3->CNT=0;
}
void ppm_set_fall_trigger_interrupt(void)//设置下降沿触发IO中断
{
	EXTI->FTSR|=(0x0001<<10);//配置为下降沿触发中断
}
void ppm_set_raise_trigger_interrupt(void)//设置上升沿触发IO中断
{
	EXTI->RTSR|=(0x0001<<10);//配置为上升沿触发中断
}
/*********************/

//配置IO口触发式中断，触发方式：上升沿触发
void EXTILine_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure PB.10 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI10 Line to PB.10 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);

	/* Configure EXTI10 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//配置定时器作为计数器来计数，每1ms计数1000
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 50000;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;		//2分频，系统时钟36MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}

//用于测试的PWM输出函数，输出PWM周期为20ms
void TIM2_Pwm_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能TIM2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIOB

	//对将要输出PWM的IO口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//因为TIM_CH4的默认输出口不在B11，所以需要开启重映射

	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装在寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIM2时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //设置向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //PWM2输出模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //设置输出比较使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置捕获/比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //设置TIM输出比较极性高
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //

	TIM_CtrlPWMOutputs(TIM2,ENABLE);		

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH4预装载使能	 

	TIM_ARRPreloadConfig(TIM2, ENABLE); 

	TIM_Cmd(TIM2, ENABLE);  //

	TIM_SetCompare4(TIM2,99);	//设置比较值
}
