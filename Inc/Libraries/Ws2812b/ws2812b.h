/*
 * ws2812b.h
 *
 *  Created on: 11 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef WS2812B_WS2812B_H_
#define WS2812B_WS2812B_H_

#define STOP_TIM2 TIM2->CR1 &=~ (1<<TIM_CR1_CEN_Pos);
#define START_TIM2 TIM2->CR1 |= (1<<TIM_CR1_CEN_Pos);
#define SEND1 TIM1->CCR1 = 8; //output pin is inverted
#define SEND0 TIM1->CCR1 = 14;

typedef struct
{
	uint8_t r;//red
	uint8_t g;//green
	uint8_t b;//blue
}Ws2812b_Pixel;
extern Ws2812b_Pixel led[100];

typedef enum{
	READY= 0,
	TIMER_BUSY,

}Ws2812b_setLed_flag;

uint8_t Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt);
void BitReversalGRB(Ws2812b_Pixel *pix, uint16_t cnt);
void Interrupt_Setup();
void GPIOA_Setup();
void TIM1_Setup();
void TIM2_Setup();

#endif /* WS2812B_WS2812B_H_ */
