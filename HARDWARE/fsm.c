#include "fsm.h"
#include "output.h"
#include "timer.h"
#include "digitalinput.h"
#include "pwm.h"


u8 output_num = 1;
u8 output_enable;
u8 pwm_read_state;
u8 out_put_cnt;

Fsm_state_TypeDef fsm_state;

u8 pwm_read(void)
{
	
	if(pwm_read_state!=0 && pwm_cnt_H==0)
	{
		if(pwm_cnt_L < 10)
		{
			return 0;
		}
		else if(pwm_read_state==1)
		{
			pwm_read_state = 0;
			return 1;
		}
		else if(pwm_read_state==2)
		{
			pwm_read_state = 0;
			return 2;
		}
	}
	
	if(pwm_cnt_H < 10)
	{
		pwm_read_state = 0;//无效
		return 0;
	}
	else if (pwm_cnt_H <150)
	{
		pwm_read_state = 1;//短按
		return 0;
	}
	else 
	{
		pwm_read_state = 2;//长按
		return 0;
		
	}

	
}

void fsm_event_disable(u8 read)
{
	output_disable();

	
	if(output_enable)
		fsm_state = Idle;
}

void fsm_event_idle(u8 read)
{
	output_disable();
	if(read==1)
		fsm_state = Output;

	if(!output_enable)
		fsm_state = Disable;
}

void fsm_event_output(u8 read)
{
	u8 result = 0;

	if(input_on_num==0)
	{
		output_enable = 0;//未使能任何通道，直接关闭
	}
	else 
	{
		if(output_num==0)
			output_one(get_input_on_ch(input_on_num),SERVO_CLOSE);//最后一次触发关闭所有
		else if(output_num==1)
			result = output_two(get_input_on_ch(output_num),get_input_on_ch(input_on_num));
		else
			result = output_two(get_input_on_ch(output_num),get_input_on_ch(output_num-1));
		
		if(++out_put_cnt>25)
		{
			out_put_cnt = 0;
			if(result == 0)
			{
				if(++output_num>input_on_num)
					output_num = 0;
				fsm_state = Idle;
			}
			else
			{
				fsm_state = Output_continue;//ch2无法同时执行，下一秒执行
			}
			

		}
	}

	if(!output_enable)
		fsm_state = Disable;
	
}

void fsm_event_output_continue(u8 read)
{
	if(input_on_num==0)
	{
		output_enable = 0;//未使能任何通道，直接关闭
	}
	else 
	{
		if(output_num==1)
			output_one(get_input_on_ch(input_on_num),SERVO_CLOSE);
		else
			output_one(get_input_on_ch(output_num-1),SERVO_CLOSE);
		
		if(++out_put_cnt>25)
		{
			out_put_cnt = 0;
			if(++output_num>input_on_num)
				output_num = 0;
			fsm_state = Idle;
		}
	}

	if(!output_enable)
		fsm_state = Disable;
	
}

void fsm_handler(void)
{
	switch(fsm_state)
	{
		case Disable:fsm_event_disable(pwm_read());break;
		case Idle:	 fsm_event_idle(pwm_read());	 break;
		case Output: fsm_event_output(pwm_read()); break;
		case Output_continue:fsm_event_output_continue(pwm_read());break;
		
	}
	
}
