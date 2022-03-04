#include "digitalinput.h"

u16 digital_input;
u8 input_on_num;

void digitalInput_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JTAG 释放IO
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_15;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 	//
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_15);	//上拉输入 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 	//
	GPIO_SetBits(GPIOC,GPIO_Pin_13);	//上拉输入 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 	//
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//上拉输入 
	
	digital_input = get_all_input();
	input_on_num = get_input_on_num( digital_input);
	
}

u16 get_all_input(void)
{
	u16 input = 0;
	input |= !DI1;
	input |= (!DI2)<<1;
	input |= (!DI3)<<2;
	input |= (!DI4)<<3;
	input |= (!DI5)<<4;
	input |= (!DI6)<<5;
	input |= (!DI7)<<6;
	input |= (!DI8)<<7;
	input |= (!DI9)<<8;
	input |= (!DI10)<<9;
	input |= (!DI11)<<10;
	input |= (!DI12)<<11;
	input |= (!DI13)<<12;
	input |= (!DI14)<<13;
	input |= (!DI15)<<14;
	input |= (!DI16)<<15; 
	return input;
	
}

u8 get_input_on_num(u16 dinput)
{
	u8 num = 0;
	if((dinput&0x01) == 1)
		num++;
	if((dinput>>1&0x01) == 1)
		num++;
	if((dinput>>2&0x01) == 1)
		num++;
	if((dinput>>3&0x01) == 1)
		num++;
	if((dinput>>4&0x01) == 1)
		num++;
	if((dinput>>5&0x01) == 1)
		num++;
	if((dinput>>6&0x01) == 1)
		num++;
	if((dinput>>7&0x01) == 1)
		num++;
	if((dinput>>8&0x01) == 1)
		num++;
	if((dinput>>9&0x01) == 1)
		num++;
	if((dinput>>10&0x01) == 1)
		num++;
	if((dinput>>11&0x01) == 1)
		num++;
	if((dinput>>12&0x01) == 1)
		num++;
	if((dinput>>13&0x01) == 1)
		num++;
	if((dinput>>14&0x01) == 1)
		num++;
	if((dinput>>15&0x01) == 1)
		num++;
	
	return num;
}

u8 get_input_on_ch(u8 num)
{
	u8 i;
	u8 ch = 0;//pwm通道
	u8 index = 0;//使能序号
	for(i =0;i<16;i++)
	{
		if((digital_input>>i&0x01) == 1)
			index++;
		ch++;
		if(index==num)
			return ch;
		
	}
	return 0;
	
		
}




