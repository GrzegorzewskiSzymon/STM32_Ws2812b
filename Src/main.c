#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/Ws2812b/ws2812b.h"
#include "../Inc/Libraries/Ws2812b/effects.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"
void Systick_Setup();
void Systick_Check();

extern uint64_t ms;//reference time counted from the beginning#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"

Ws2812b_Pixel led[LED_CNT];
Ws2812b_Pixel colors[8];

int main(void)
{
	ClockFrequency_Setup();
	Systick_Setup();
	Interrupt_Setup();
	GPIOA_Setup();
	Spi1_Setup();


	Rainbow_SetupColors(colors);

    for(uint16_t i=0; i<LED_CNT; i++)
    {
        led[i].r=0;led[i].g=0;led[i].b=0;
    }

	Ws2812b_setLed(led, 8);//reset led strip

	uint64_t tmp_ms = ms;
	while(1)
	{
		if(ms - tmp_ms >= 500)
		{
			Rainbow(led);
			tmp_ms = ms;
		}

	}

}


