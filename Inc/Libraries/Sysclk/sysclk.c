/*
 * sysclk.c
 *
 *  Created on: 12 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "sysclk.h"

void ClockFrequency_Setup()
{

	RCC->CR |= (1<<8); //HSION

	while(!(RCC->CR & (1<<10))); //HSIRDY

	RCC->CR = 0x00000063;//reset

	//Enable HSE and wait till its ready
	RCC->CR |= (1<<16); //HSEON
	while (!(RCC->CR & (1<<17)));//HSERDY


	//Select the Range 1 boost mode by clearing the R1MODE bit is in the PWR_CR5 register
	PWR->CR5 &= ~(1<<8);

	//Voltage scaling range selection
	PWR->CR1 |= (1<<9);//VOS[0;1] -> Range 1


	FLASH->ACR |= (1<<8);//Enable prefetch to mitigate the disparity in speed at higher clock rates
	FLASH->ACR |= (5<<0);//Period to the Flash access time


	                        //PLLR = 2 by default
	RCC->PLLCFGR |= (85<<8);//PLLN = 85
	RCC->PLLCFGR |= (5<<4); //PLLM = 6
	RCC->PLLCFGR |= (3<<0); //PLL SRC
	RCC->PLLCFGR |= (1<<24);//Main PLL clock selected

	//Enable the PLL again by setting PLLON
	RCC->CR |= (1<<24);

	//Wait until PLLRDY is set
	while(!(RCC->CR & 1<<25))
	{
	}

	//The system clock must be divided by 2 using the AHB prescaler before switching
	// to a higher system frequency
	RCC->CFGR |= (1<<7);

	// Configure and switch to PLL for a new system frequency
	RCC->CFGR |= (3<<0);

	//Wait for at least 1us and then reconfigure the AHB prescaler to get the needed
	// HCLK clock frequency.
	while(!(RCC->CFGR & (2<<2)) && !(RCC->CFGR & (1<<2)))
	{
	}

	RCC->CFGR &= ~(1<<7);
}
