#include "pwm.h"


/****************************************************************************************
*函 数 名:TIM1_Init(u16 arr,u16 psc)
*函数功能:初始化定时器1相关IO
*形    参:无
*返 回 值:无
*****************************************************************************************/
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure; 
//	NVIC_InitTypeDef isr;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//打开端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_AFIO,ENABLE);  //打开定时器时钟 
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//ABC
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	//强制输出低电平
	
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_11;//ABC
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP; //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_11);	//强制输出低电平
	
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);	//强制输出低电平
	
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	//强制输出低电平
	
	TIM_TimeBaseInitStructure.TIM_Period 				= arr-1; //设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler 		= psc-1;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM2;//模式1
  TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Disable;//主通道未配置：与串口1发送相冲突
  TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Enable;//使能互补通道
  //TIM_OCInitStructure.TIM_Pulse 				= 0x7FF;										
  TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_Low;//默认输出高电平,装载值越大
  TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCNPolarity_Low;//默认输出低电平
  TIM_OCInitStructure.TIM_OCIdleState 	= TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState 	= TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM2;//模式1
  TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Disable;//主通道未配置：与串口1发送相冲突
  TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Enable;//使能互补通道
  //TIM_OCInitStructure.TIM_Pulse 				= 0x7FF;										
  TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_Low;//默认输出高电平,装载值越大
  TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCNPolarity_Low;//默认输出低电平
  TIM_OCInitStructure.TIM_OCIdleState 	= TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState 	= TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	

	//下面这句是高级定时器才有，输出PWM必须打开
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                             //pwm输出
	TIM_Cmd(TIM1,ENABLE);//
}

void set_channel_odd(u8 ch)
{
	switch(ch)
	{
		case 1:PAout(6)=0;PAout(5)=0;PAout(4)=0;
			break;
		case 2:PAout(6)=0;PAout(5)=0;PAout(4)=1;
			break;
		case 3:PAout(6)=0;PAout(5)=1;PAout(4)=0;
			break;
		case 4:PAout(6)=0;PAout(5)=1;PAout(4)=1;
			break;
		case 5:PAout(6)=1;PAout(5)=0;PAout(4)=0;
			break;
		case 6:PAout(6)=1;PAout(5)=0;PAout(4)=1;
			break;
		case 7:PAout(6)=1;PAout(5)=1;PAout(4)=0;
			break;
		case 8:PAout(6)=1;PAout(5)=1;PAout(4)=1;
			break;
		default:PAout(6)=0;PAout(5)=0;PAout(4)=0;
			break;
	}
}

void set_channel_even(u8 ch)
{
	switch(ch)
	{
		case 1:PBout(11)=0;PBout(2)=0;PBout(1)=0;
			break;
		case 2:PBout(11)=0;PBout(2)=0;PBout(1)=1;
			break;
		case 3:PBout(11)=0;PBout(2)=1;PBout(1)=0;
			break;
		case 4:PBout(11)=0;PBout(2)=1;PBout(1)=1;
			break;
		case 5:PBout(11)=1;PBout(2)=0;PBout(1)=0;
			break;
		case 6:PBout(11)=1;PBout(2)=0;PBout(1)=1;
			break;
		case 7:PBout(11)=1;PBout(2)=1;PBout(1)=0;
			break;
		case 8:PBout(11)=1;PBout(2)=1;PBout(1)=1;
			break;
		default:PBout(11)=0;PBout(2)=0;PBout(1)=0;
			break;
	}
}
