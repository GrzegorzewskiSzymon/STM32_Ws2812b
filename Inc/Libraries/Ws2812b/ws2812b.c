/*
 * ws2812b.c
 *
 *  Created on: 11 Dec 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "ws2812b.h"
#include "../RegistersConfig/RegistersConfig.h"

uint8_t nrOfactualBitToSend;
uint16_t nrOfLedsToUpdate = LED_CNT;
uint16_t nrOfSendingLed = 0;
uint32_t pixel_G_R_B;

uint8_t Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt)
{

	BitReversalGRB(pixel, cnt);

	//Start timer
	nrOfLedsToUpdate = cnt;
	START_TIM2;

	return TIMER_READY;
}

uint32_t reversed_GRB[LED_CNT];

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
		reversed_GRB[pixNr] = tmp_g | (tmp_r << 8) | (tmp_b << 16);
		tmp_r = 0;
		tmp_g = 0;
		tmp_b = 0;

		pixNr++;
		i=0;
	}
}
Timer timer2;

void TIM2_IRQHandler_RGB()
{
	TIM2->SR &= ~TIM_SR_UIF;//Reset flag

	if (nrOfSendingLed < nrOfLedsToUpdate)
	{

		if (reversed_GRB[nrOfSendingLed] & (1<<nrOfactualBitToSend))
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
