/*
 * effects.c
 *
 *  Created on: 18 Dec 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "ws2812b.h"
#include "effects.h"
#include "../RegistersConfig/RegistersConfig.h"

void Rainbow_SetupColors(Ws2812b_Pixel *color)
{
    color[0].r=150; color[0].g=150; color[0].b=150;
    color[1].r=255; color[1].g=000; color[1].b=000;//red
    color[2].r=255; color[2].g=100; color[2].b=000;//orange
    color[3].r=100; color[3].g=255; color[3].b=000;//yellow
    color[4].r=000; color[4].g=255; color[4].b=000;//green
    color[5].r=000; color[5].g=100; color[5].b=255;//light blue
    color[6].r=000; color[6].g=000; color[6].b=255;//blue
    color[7].r=100; color[7].g=000; color[7].b=255;//violet
}

void Rainbow(Ws2812b_Pixel *pixel)
{
	static uint8_t j = 0;
	static uint8_t k = 1;
    //shift all values by one led
    uint8_t i=0;
    for(i=LED_CNT; i>1; i--)
        pixel[i-1]=pixel[i-2];
    //change color when colorlength is reached
    if(k>COLORLENGTH)
    {
        j++;
        if(j>7)
        {
          j=0;
        }

        k=0;
    }
    k++;
    //loop colors

    //fade red
    if(pixel[0].r<(colors[j].r-FADE))  pixel[0].r+=FADE;
    if(pixel[0].r>(colors[j].r+FADE))  pixel[0].r-=FADE;
    if(pixel[0].g<(colors[j].g-FADE))  pixel[0].g+=FADE;
    if(pixel[0].g>(colors[j].g+FADE))  pixel[0].g-=FADE;
    if(pixel[0].b<(colors[j].b-FADE))  pixel[0].b+=FADE;
    if(pixel[0].b>(colors[j].b+FADE))  pixel[0].b-=FADE;

    Ws2812b_setLed(pixel,LED_CNT);
}
