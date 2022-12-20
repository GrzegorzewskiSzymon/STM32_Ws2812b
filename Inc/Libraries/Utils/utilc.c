/*
 * utilc.c
 *
 *  Created on: 17 Dec 2022
 *      Author: Szymon Grzegorzewski
 */


#include <stdint.h>
#include "stm32g431xx.h"
#include  "utils.h"
void Systick_Setup()
{
	SysTick->LOAD = (uint32_t)170000; //The value which will be decrementing, 24bit value
 	SysTick->CTRL  =  (1<<2) |         //CLKSOURCE bit2,  Clock: AHB
 					  (1<<0);          //Enables the counter
}

uint64_t ms;
void Systick_Check()
{
	if((SysTick->CTRL & 0x10000)) //if COUNTFLAG on bit16 is set
	{
		ms++;
		SysTick->CTRL -= 0x10000; //reset flag
	}
}
