/*
 * RegistersConfig.c
 *
 *  Created on: 24 Mar 2023
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "../Ws2812b/ws2812b.h"

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

void Interrupt_Setup()
{
	NVIC_EnableIRQ(TIM2_IRQn);    //Enable interrupt from TIM2
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick
}

void GPIOA_Setup()
{
	//Enable peripheries AHB2
	RCC->AHB2ENR |= 1 ;//bit0 is responsible for GPIOA

	//PA5
	GPIOA->MODER =  0xABFFF7FF;// MODE5[0;1] -> General purpose output mode

	//PA8 as TIM1_CH1
	GPIOA->MODER &= ~(1<<16); //  MODE8 [10]   -> Alternate function mode
	GPIOA->AFR[1] |= (6<<0);  //  AFSEL1[0001] -> AF1
	GPIOA->OSPEEDR |= (1<<16);

}

void TIM1_Setup()
{
	 RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;//Enable clock

	 TIM1->ARR = 20; /* Set the Autoreload value */
	 TIM1->CCR1 = 13; /* Set the Pulse value */
	 TIM1->PSC = 9; /* Set the Prescaler value */
//	 TIM1->RCR = 10 - 1; /* Set the Repetition counter value */
	 TIM1->CR1 |= TIM_CR1_OPM; /* Select the OPM Mode */
	 TIM1->CCMR1 |= (6<<TIM_CCMR1_OC1M_Pos);//PWM Mode 1
	 TIM1->CCER = TIM_CCER_CC1E; /* Enable the Compare output channel 1 */
	 TIM1->CCER |= (1<<TIM_CCER_CC1P_Pos);//inverting pin output
	 TIM1->BDTR |= TIM_BDTR_MOE; //Main output enable
	 TIM1->EGR |= (1<<TIM_EGR_UG_Pos);

	 //Slave mode selection [SMS]
//	 TIM1->SMCR |= (6<<0); /* Trigger Mode - The counter starts at a rising edge of the trigger tim_trgi
//	 (but it is not reset). Only the start of the counter is controlled.*/

	 //Trigger selection
//	 TIM1->SMCR |= (1<<4);// Internal Trigger 1 (tim_itr1)

}

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


void TIM2_IRQHandler()
{
	TIM2_IRQHandler_RGB();
}

void TIM2_Setup()
{
	RCC->APB1ENR1 |= (1<<0); //TIM2 clock enable

	//TIM2_CH1 is PA0 in AF1
	TIM2->CCMR1 |= (6<<4);  //OC1M[0;1;1;0] -> PWM mode 1

	/*Frq = [CLK/(PSC+1)]/[ARR]
	 * ~708kHz
	 * */
	TIM2->PSC = 9;
	TIM2->CCR1 = 9;
	TIM2->CNT =  0;
	TIM2->ARR =  24;

	TIM2->CCER |= (1<<0);//Capture/Compare 1 output enable

	//Master mode selection
	TIM2->CR2 |= (4<<4); //Compare - tim_oc1refc signal is used as trigger output (tim_trgo)

	TIM2->DIER |= (1<<TIM_DIER_UIE_Pos);//Update interrupt enable
}



