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
	SysTick->LOAD = (uint32_t)170000;                //The value which will be decrementing, 24bit value
	SysTick->VAL = 0;                              //(undefined on reset)
 	SysTick->CTRL  =  (SysTick_CTRL_CLKSOURCE_Msk) //Processor clock (AHB)
 				   |  (SysTick_CTRL_ENABLE_Msk)    //Enables the counter
 				   |  (SysTick_CTRL_TICKINT_Msk);  //Exception request
}

uint64_t ms;
void SysTick_Handler()
{
	ms++;
}

