/*
 * ws2812b.h
 *
 *  Created on: 11 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef WS2812B_WS2812B_H_
#define WS2812B_WS2812B_H_

#define ZERO 0b11000000
#define ONE 0b11111000

#define LED_CNT 100

typedef struct
{
	uint8_t r;//red
	uint8_t g;//green
	uint8_t b;//blue
}Ws2812b_Pixel;

extern Ws2812b_Pixel led[LED_CNT];
extern Ws2812b_Pixel colors[8];

void Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt);

#endif /* WS2812B_WS2812B_H_ */
