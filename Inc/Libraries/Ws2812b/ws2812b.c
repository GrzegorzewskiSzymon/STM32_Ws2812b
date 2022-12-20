/*
 * ws2812b.c
 *
 *  Created on: 11 Dec 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "ws2812b.h"
#include "../Utils/utils.h"

void Interrupt_Setup()
{
	NVIC_EnableIRQ(TIM2_IRQn);    //Enable interrupt from TIM2
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick
}

uint8_t nrOfactualBitToSend;
uint16_t nrOfLedsToUpdate = LED_CNT;
uint16_t nrOfSendingLed = 0;
uint32_t pixel_G_R_B;

uint8_t Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt)
{
	//Check if timer is still running
	if(TIM2->CR1 & TIM_CR1_CEN)
	{
		return TIMER_BUSY;
	}
	else if( (timer2.flag == TIMER_IDLE_BETWEEN_CYCLES) && (ms - timer2.ms_end_of_cycle < 1))
	{
		return TIMER_IDLE_BETWEEN_CYCLES;
	}

	timer2.flag = TIMER_READY;


	BitReversalGRB(pixel, cnt);

	//Start timer
	nrOfLedsToUpdate = cnt;
	START_TIM2;

	return TIMER_READY;
}

Ws2812b_Pixel led_reversed[LED_CNT];

void BitReversalGRB(Ws2812b_Pixel *pix, uint16_t cnt)
{
	uint16_t pixNr =0;
	uint8_t tmp_r = 0, tmp_g = 0, tmp_b = 0;
	uint8_t	i = 0;

	while(pixNr < cnt)
	{
		while(i<8)
		{
			if(pix[pixNr].r & (1<<i))
				tmp_r += (1<<(7-i));
			if(pix[pixNr].g & (1<<i))
				tmp_g += (1<<(7-i));
			if(pix[pixNr].b & (1<<i))
				tmp_b += (1<<(7-i));
			i++;
		}
		led_reversed[pixNr].r = tmp_r;
		tmp_r = 0;
		led_reversed[pixNr].g = tmp_g;
		tmp_g = 0;
		led_reversed[pixNr].b = tmp_b;
		tmp_b = 0;

		pixNr++;
		i=0;
	}
}

void TIM2_IRQHandler()
{
	TIM2->SR &= ~TIM_SR_UIF;//Reset flag

	pixel_G_R_B = led_reversed[nrOfSendingLed].g | (led_reversed[nrOfSendingLed].r << 8) | (led_reversed[nrOfSendingLed].b << 16);

	if (nrOfSendingLed < nrOfLedsToUpdate)
	{

		if (pixel_G_R_B & (1<<nrOfactualBitToSend))
		{
			SEND1;
			TIM1->CR1 |= (1 << TIM_CR1_CEN_Pos);
		}
		else
		{
			SEND0;
			TIM1->CR1 |= (1 << TIM_CR1_CEN_Pos);
		}

		nrOfactualBitToSend++;
		if (nrOfactualBitToSend == 24)
		{
			nrOfactualBitToSend = 0;
			nrOfSendingLed++;
		}

	}

	if(nrOfSendingLed >= nrOfLedsToUpdate)
	{
		nrOfSendingLed = 0;
		STOP_TIM2;
		timer2.flag = TIMER_IDLE_BETWEEN_CYCLES;
		timer2.ms_end_of_cycle = ms;
		return;
	}
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

Timer timer2;

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
