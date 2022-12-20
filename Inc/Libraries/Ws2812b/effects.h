/*
 * effects.h
 *
 *  Created on: 18 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_WS2812B_EFFECTS_H_
#define LIBRARIES_WS2812B_EFFECTS_H_

/*******Rainbow*******/
#define COLORLENGTH (LED_CNT/2)
#define FADE 2

extern Ws2812b_Pixel colors[8];
void Rainbow_SetupColors();
void Rainbow(Ws2812b_Pixel *pixel);

#endif /* LIBRARIES_WS2812B_EFFECTS_H_ */
