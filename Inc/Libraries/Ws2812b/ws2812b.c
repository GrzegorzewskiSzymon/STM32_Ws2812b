/*
 * ws2812b.c
 *
 *  Created on: 11 Dec 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "ws2812b.h"

void Interrupt_Setup()
{

	NVIC_EnableIRQ(TIM2_IRQn); //Enable interrupt from TIM2
//	NVIC_SetVector(TIM2_IRQn, (uint32_t)&TIM2_IRQHandler);

}

void TIM2_IRQHandler()
{
	TIM2->SR &= ~TIM_SR_UIF;


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

}

void TIM1_Setup()
{
	 RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;//Enable clock

	 TIM1->ARR = 20; /* Set the Autoreload value */
	 TIM1->CCR1 = 13; /* Set the Pulse value */
	 TIM1->PSC = 9; /* Set the Prescaler value */
//	 TIM1->RCR = 10 - 1; /* Set the Repetition counter value */
	 TIM1->CR1 |= TIM_CR1_OPM; /* Select the OPM Mode */
	 TIM1->CCMR1 |= (7<<TIM_CCMR1_OC1M_Pos);//PWM mode 2
	 TIM1->CCER = TIM_CCER_CC1E; /* Enable the Compare output channel 1 */
	 TIM1->BDTR |= TIM_BDTR_MOE; //Main output enable

	 //Slave mode selection [SMS]
	 TIM1->SMCR |= (6<<0); /* Trigger Mode - The counter starts at a rising edge of the trigger tim_trgi
	 (but it is not reset). Only the start of the counter is controlled.*/

	 //Trigger selection
	 TIM1->SMCR |= (1<<4);// Internal Trigger 1 (tim_itr1)

}


void TIM2_Setup()
{
	RCC->APB1ENR1 |= (1<<0); //TIM2 clock enable

	//TIM2_CH1 is PA0 in AF1
	TIM2->CCMR1 |= (6<<4);  //OC1M[0;1;1;0] -> PWM mode 1
	TIM2->PSC = 9;


	/*Frq = [CLK/(PSC+1)]/[ARR]
	 * ~806kHz
	 * */
	TIM2->CCR1 = 10;
	TIM2->CNT =  0;
	TIM2->ARR =  20;

	TIM2->CCER |= (1<<0);//Capture/Compare 1 output enable

	//Master mode selection
	TIM2->CR2 |= (4<<4); //Compare - tim_oc1refc signal is used as trigger output (tim_trgo)

	TIM2->DIER |= (1<<TIM_DIER_UIE_Pos);//Update interrupt enable
}
