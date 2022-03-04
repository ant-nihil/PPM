#include "output.h"
#include "pwm.h"

void output_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 	//
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);	//
	
}

void output_disable(void)
{
	TIM_SetCompare1(TIM1,SERVO_STOP);
	TIM_SetCompare2(TIM1,SERVO_STOP);
}

void output_one(u8 ch,u16 pwm)
{
	if(ch%2 == 0)//偶数
	{
		set_channel_even(ch/2);
		TIM_SetCompare2(TIM1,pwm);
		
	}
	else//奇数
	{
		set_channel_odd((ch+1)/2);
		TIM_SetCompare1(TIM1,pwm);
	}
	
}

u8 output_two(u8 ch1,u8 ch2)
{
	u8 res = 0;
	if(ch1%2 == 0)//偶数
	{
		set_channel_even(ch1/2);
		TIM_SetCompare2(TIM1,SERVO_OPEN);
		
		if(ch2==0)
			TIM_SetCompare1(TIM1,SERVO_STOP);
		else if(ch2%2!=0)
		{
			set_channel_odd((ch2+1)/2);
			TIM_SetCompare1(TIM1,SERVO_CLOSE);
		}
		else
		{
			res = 1;//ch2未执行
			
		}
		
	}
	else//奇数
	{
		set_channel_odd((ch1+1)/2);
		TIM_SetCompare1(TIM1,SERVO_OPEN);
		
		if(ch2==0)
			TIM_SetCompare2(TIM1,SERVO_STOP);
		else if(ch2%2==0)
		{
			set_channel_even(ch2/2);
			TIM_SetCompare2(TIM1,SERVO_CLOSE);
		}
		else
		{
			res = 1;//ch2未执行
				
		}
	}
	return res;
	
}
