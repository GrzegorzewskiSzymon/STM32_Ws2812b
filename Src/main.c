#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/Ws2812b/ws2812b.h"
#include "../Inc/Libraries/Ws2812b/effects.h"
#include "../Inc/Libraries/Sysclk/sysclk.h"
#include "../Inc/Libraries/Utils/utils.h"

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
    uint64_t time = ms;
	while(1)
	{
		Systick_Check();//Need to put this in IRQ
		if(ms - time > 50)
		{
			Rainbow(led);
			time = ms;
		}
	}

}


