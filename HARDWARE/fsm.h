#ifndef __FSM_H
#define __FSM_H
#include "sys.h"

typedef enum 
{
	Disable,
	Idle,
	Output,
	Output_continue,
}Fsm_state_TypeDef;

extern u8 output_enable;
extern void fsm_handler(void);


#endif
