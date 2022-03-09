#include "ppm.h"
#include "exti.h"
pwm_status_t pwm_status;

__IO uint16_t i_count1=0;	//这是高电平的计数时间，单位为us
__IO uint16_t i_count2=0;	//这是一个高电平和一个低电平的周期时间，单位为us
uint8_t  i_number_c=0;		//记录当前处于哪一通道
uint8_t  i_number_p=0;
uint16_t channel_count[9];	//保存8个通道的高电平时间
uint16_t channel_period[9];

//IO的中断入口
void EXTI15_10_IRQHandler(void)
{
	exti_handler();//中断处理函数
	EXTI_ClearITPendingBit(EXTI_Line10);  //清除EXIT10线路挂起
}

//进入中断后根据状态来进行各自的处理
void exti_handler(void)
{
	switch(pwm_status)
	{
		case PWM_STATUS_RAISE:	
                                i_count2=ppm_read_cnt();
								if(i_count2>MAX_PULSE_WIDTH){ //如果计数值超出舵机的输出脉宽最大值
									channel_period[i_number_p]=0;//数组最后一位填充为0
									i_number_p=0;
								}else{
									channel_period[i_number_p++]=i_count2;
								}
                                ppm_reset_cnt();//计数器清0
                                pwm_status=PWM_STATUS_FALLING;
								ppm_set_fall_trigger_interrupt();//配置为下降沿触发中断
								break;
		case PWM_STATUS_FALLING:
                                i_count1=ppm_read_cnt();
								if(i_count1>MAX_PULSE_WIDTH){ //如果计数值超出舵机的输出脉宽最大值
									channel_count[i_number_c]=0;//数组最后一位填充为0
									i_number_c=0;
								}else{
									channel_count[i_number_c++]=i_count1;
								}
								pwm_status=PWM_STATUS_RAISE;
								ppm_set_raise_trigger_interrupt();//配置为上升沿触发中断
								break;
		
		default:				break;
	}
}

//得到高电平脉宽，单位为us
uint16_t* get_pulse_width(void)
{
    if(i_count1!=0){
        return channel_count;
    }
    return 0;
}

//得到占空比，数值约在3.25(650)-7.75(1550)
float* get_duty_cycle(void)//此占空比是通道中的电平占一个舵机周期的占空比
{
	uint8_t i=0;
	float duty_cycle[9];
	if(channel_count[0]!=0){
		for(i=0;i<8;i++){
			duty_cycle[i]=(float)channel_count[i]*100/20000;
		}
		duty_cycle[i]=0;
		return duty_cycle;
	}
	return 0;
}

//得到一高一低电平的周期时间，单位为us
uint16_t* get_period(void)
{
    if(i_count2!=0){
        return channel_period;
    }
    return 0;
}
