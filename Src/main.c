#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/Ws2812b/ws2812b.h"
#include "../Inc/Libraries/Sysclk/sysclk.h"

int main(void)
{
	ClockFrequency_Setup();
	GPIOA_Setup();
	TIM1_Setup();
	TIM2_Setup();
	InterruptSetup();
	TIM1->CR1 |= (1<<TIM_CR1_CEN_Pos);//Counter enabled
	TIM2->CR1 |= (1<<TIM_CR1_CEN_Pos);//Counter enabled

	while(1)
	{

	}

}


