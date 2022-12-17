#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/Ws2812b/ws2812b.h"
#include "../Inc/Libraries/Sysclk/sysclk.h"

Ws2812b_Pixel led[100];
int main(void)
{
	ClockFrequency_Setup();
	TIM1_Setup();
	TIM2_Setup();
	Interrupt_Setup();
	GPIOA_Setup();

	led[0].r = 1;  led[0].g = 1; led[0].b = 1;
	led[1].r = 0;  led[1].g = 0; led[1].b = 0;
	led[2].r = 5;  led[2].g = 2; led[2].b = 0;
	Ws2812b_setLed(led, 3);

	while(1)
	{

	}

}


