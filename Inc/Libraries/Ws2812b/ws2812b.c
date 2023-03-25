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

void Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt)
{
	//One byte represents one bit, its necessary for future insertion to DMA
	uint8_t buffer[cnt*24+72];

	//First 3 bytes (72bytes to send) need to be blank(LOW state) for reset ws2812b
	for(uint8_t i = 0; i<72;i++)
		buffer[i] = 0;

	//Translate every bit to byte representing 1/0 bit for ws2812b
	for(uint16_t i=0, j=72; i<cnt; i++)
	{
		//GREEN
		for(int8_t k=7; k>=0; k--)
		{
			if((pixel[i].g & (1<<k)) == 0)
				buffer[j] = ZERO;
			else
				buffer[j] = ONE;
			j++;
		}

		//RED
		for(int8_t k=7; k>=0; k--)
		{
			if((pixel[i].r & (1<<k)) == 0)
				buffer[j] = ZERO;
			else
				buffer[j] = ONE;
			j++;
		}

		//BLUE
		for(int8_t k=7; k>=0; k--)
		{
			if((pixel[i].b & (1<<k)) == 0)
				buffer[j] = ZERO;
			else
				buffer[j] = ONE;
			j++;
		}
	}

	Spi1_Send(buffer, (cnt*24) +72);
}


uint32_t reversed_GRB[LED_CNT];



