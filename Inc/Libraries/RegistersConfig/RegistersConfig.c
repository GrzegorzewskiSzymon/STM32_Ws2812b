/*
 * RegistersConfig.c
 *
 *  Created on: 24 Mar 2023
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "../Ws2812b/ws2812b.h"
#include "RegistersConfig.h"

void ClockFrequency_Setup()
{

	RCC->CR |= (1<<8); //HSION

	while(!(RCC->CR & (1<<10))); //HSIRDY

	RCC->CR = 0x00000063;//reset

	//Enable HSE and wait till its ready
	RCC->CR |= (1<<16); //HSEON
	while (!(RCC->CR & (1<<17)));//HSERDY


	//Select the Range 1 boost mode by clearing the R1MODE bit is in the PWR_CR5 register
	PWR->CR5 &= ~(1<<8);

	//Voltage scaling range selection
	PWR->CR1 |= (1<<9);//VOS[0;1] -> Range 1


	FLASH->ACR |= (1<<8);//Enable prefetch to mitigate the disparity in speed at higher clock rates
	FLASH->ACR |= (5<<0);//Period to the Flash access time


	                        //PLLR = 2 by default
	RCC->PLLCFGR |= (17<<8);//PLLN = 17
	RCC->PLLCFGR |= (1<<4); //PLLM = 2
	RCC->PLLCFGR |= (3<<0); //PLL SRC
	RCC->PLLCFGR |= (1<<24);//Main PLL clock selected

	//Enable the PLL again by setting PLLON
	RCC->CR |= (1<<24);

	//Wait until PLLRDY is set
	while(!(RCC->CR & 1<<25))
	{
	}

	//The system clock must be divided by 2 using the AHB prescaler before switching
	// to a higher system frequency
	RCC->CFGR |= (1<<7);

	// Configure and switch to PLL for a new system frequency
	RCC->CFGR |= (3<<0);

	//Wait for at least 1us and then reconfigure the AHB prescaler to get the needed
	// HCLK clock frequency.
	while(!(RCC->CFGR & (2<<2)) && !(RCC->CFGR & (1<<2)))
	{
	}

	RCC->CFGR &= ~(1<<7);
}

void Interrupt_Setup()
{
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick
}

void GPIOA_Setup()
{
	//Enable peripheries AHB2
	RCC->AHB2ENR |= 1 ;//bit0 is responsible for GPIOA

	//
	//SPI 1 - WS2812b
	//

	//PA5 as SCK
	GPIOA->MODER &= ~(1 << 10); //Alternate function mode
	GPIOA->AFR[0] |= (5 << 20); //AFSEL5[0101] -> AF5
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0; //Medium Speed

	//PA6 as MISO
	GPIOA->MODER &= ~(GPIO_MODER_MODER6_0);
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL6_Pos);

	//PA7 as MOSI
	GPIOA->MODER &= ~(GPIO_MODER_MODER7_0);
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL7_Pos);

}

void Systick_Setup()
{
	SysTick->LOAD = (uint32_t)102000;                //The value which will be decrementing, 24bit value
	SysTick->VAL = 0;                              //(undefined on reset)
 	SysTick->CTRL  =  (SysTick_CTRL_CLKSOURCE_Msk) //Processor clock (AHB)
 				   |  (SysTick_CTRL_ENABLE_Msk)    //Enables the counter
 				   |  (SysTick_CTRL_TICKINT_Msk);  //Exception request
}

//
// SPI 1 - WS2812b
//

void Spi1_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI1->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI1->CR1 |= (3<<SPI_CR1_BR_Pos);	//fPCLK/16 = 6Mhz
	SPI1->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI1->CR2 = 0;
}

static void Spi1_ON()
{
	SPI1->CR1 |=  SPI_CR1_SPE;
}

static void Spi1_OFF()
{
	SPI1->CR1 &=  ~SPI_CR1_SPE;

	//PA7 as GPIO to force it LOW
	GPIOA->MODER |=  (GPIO_MODER_MODE7_Msk);
	GPIOA->MODER &= ~(GPIO_MODER_MODE7_1);
	GPIOA->BSRR  |=  (1<<GPIO_BSRR_BR7_Pos);

	//PA7 as MOSI
	GPIOA->MODER |=  (GPIO_MODER_MODE7_Msk);
	GPIOA->MODER &= ~(GPIO_MODER_MODER7_0);
	GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL7_Pos);
}

void Spi1_Send(uint8_t *byte, uint32_t length)
{
	Spi1_ON();
    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI1->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI1->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}
    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI1->SR)&(1<<7))) {};
    Spi1_OFF();
}

uint64_t ms;
void SysTick_Handler()
{
	ms++;
}
