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

#define  LED_CNT 100

typedef struct
{
	uint8_t r;//red
	uint8_t g;//green
	uint8_t b;//blue
}Ws2812b_Pixel;

extern Ws2812b_Pixel led[LED_CNT];
extern Ws2812b_Pixel led_reversed[LED_CNT];
extern Ws2812b_Pixel colors[8];


typedef enum{
	TIMER_READY= 0,
	TIMER_BUSY,
	TIMER_IDLE_BETWEEN_CYCLES, //after cycle need to wait (50us for timer2) to start another

}Ws2812b_setLed_flag;


typedef struct
{
	Ws2812b_setLed_flag flag;
	uint64_t ms_end_of_cycle;
}Timer;

extern Timer timer2;

uint8_t Ws2812b_setLed(Ws2812b_Pixel *pixel, uint16_t cnt);
void BitReversalGRB(Ws2812b_Pixel *pix, uint16_t cnt);
void Interrupt_Setup();
void GPIOA_Setup();
void TIM1_Setup();
void TIM2_Setup();

#endif /* WS2812B_WS2812B_H_ */
