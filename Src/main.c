#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/Ws2812b/ws2812b.h"
#include "../Inc/Libraries/Ws2812b/effects.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"
void Systick_Setup();
void Systick_Check();

extern uint64_t ms;//reference time counted from the beginning#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"

Ws2812b_Pixel led[100];
Ws2812b_Pixel colors[8];

int main(void)
{
	ClockFrequency_Setup();
	Systick_Setup();
	TIM1_Setup();
	TIM2_Setup();
	Interrupt_Setup();
	GPIOA_Setup();

	Rainbow_SetupColors(colors);

    for(uint16_t i=99; i>0; i--)
    {
        led[i-1].r=0;led[i-1].g=0;led[i-1].b=0;
    }

	Ws2812b_setLed(led, 8);//reset led strip
	while(1)
	{
		Rainbow(led);
	}

}


