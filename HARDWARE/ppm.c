#include "ppm.h"
pwm_status Def_PWM_Status;
__IO uint16_t pwm_duty=0;
__IO uint16_t pwm_period=0;

uint8_t P_W_count=0;

uint16_t Pulse_Width[9];
//用TIM3的计时器来计数
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = arr;
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;		//2分频，系统时钟36MHz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, DISABLE);
}
//IO口的中断配置
void EXTI10_Config(void)
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
//IO的中断服务函数
void EXTI15_10_IRQHandler(void)
{
	exit_handler();
	EXTI_ClearITPendingBit(EXTI_Line10);  //清除EXIT10线路挂起
}
//进入中断后根据状态来进行各自的处理
void exit_handler(void)
{
	switch(Def_PWM_Status)
	{
		case PWM_STATUS_NONE:		TIM_Cmd(TIM3, ENABLE);
														TIM3->CNT=0;
														Def_PWM_Status=PWM_STATUS_RAISE;
														EXTI->FTSR|=(0x0001<<10);//配置为下降沿触发中断
														break;
		case PWM_STATUS_RAISE:	pwm_duty=TIM3->CNT;
														Def_PWM_Status=PWM_STATUS_FALLING;
														EXTI->RTSR|=(0x0001<<10);//配置为上升沿触发中断
														break;
		case PWM_STATUS_FALLING:pwm_period=TIM3->CNT;
								Pulse_Width[P_W_count]=pwm_duty;//Pwm_Duty_Cycle();
								P_W_count++;//脉冲计数加一，该变量代表第几个脉冲
								if((P_W_count==9)&&(pwm_duty>2700)) P_W_count=0;
														Def_PWM_Status=PWM_STATUS_RAISE;
														TIM3->CNT=0;
														EXTI->FTSR|=(0x0001<<10);//配置为下降沿触发中断

														break;
		
		default:								break;
	}
}
//得到占空比，一般来说是高电平的占空比（当IO接口默认处于低电平状态时）
uint16_t Pwm_Duty_Cycle(void)
{
	uint16_t tmp;
	
	tmp=pwm_duty*100/(20000);
	
	return (tmp);
}
//得到PWM的周期值，单位为us
uint16_t Pwm_Period(void)
{
	if(pwm_period==0)
	{
		return 0;
	}
	return (pwm_period);
}

//得到PWM的频率值，单位为Hz
uint32_t Pwm_Fre(void)
{
	uint32_t tmp=0;
	if(pwm_period==0)
	{
		return tmp;
	}
	tmp=1000000/(pwm_period);
	return tmp;
}

//得到PWM波的高脉冲宽度，单位为us
uint16_t Pwm_Pulse_Width(void)
{
	if(pwm_duty!=0)
	{
		return pwm_duty;
	}
	return 0;
}

//用于测试的PWM输出函数
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

